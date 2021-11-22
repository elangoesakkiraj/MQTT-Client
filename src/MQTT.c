// gcc -Wall MQTT_C.c -o client -lpaho-mqtt3c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <json-c/json.h>
#include "MQTTClient.h"

#define TIMEOUT     10000L
#define MAX_LEN 256
#define CONFIG_LINE_BUFFER_SIZE 100
#define MAX_CONFIG_VARIABLE_LEN 20

volatile MQTTClient_deliveryToken deliveredtoken;

time_t t;

struct config_struct
{
  char ADDRESS[MAX_LEN];  
  char security[MAX_CONFIG_VARIABLE_LEN];
  char username[MAX_CONFIG_VARIABLE_LEN];
  char password[MAX_CONFIG_VARIABLE_LEN];
  char will_opts[MAX_CONFIG_VARIABLE_LEN];
  char will_topic[MAX_CONFIG_VARIABLE_LEN];
  char will_message[MAX_LEN];
}config;


void read_config_file(char* config_filename, struct config_struct config);


void read_str_from_config_line(char* config_line, char* val) {   
    //printf("param %s %s\n",config_line,val); 
    char prm_name[MAX_CONFIG_VARIABLE_LEN];
    sscanf(config_line, "%s %s\n", prm_name, val);
}


int main(int argc, char* argv[])
{
    printf("MQTT Client Version 3.1.1 protocol support client...\n");
    read_config_file("../conf/client.conf", config);
    return EXIT_SUCCESS;
}

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("topic: %s\n", topicName);
    //printf("message:%d %s\n", message->payloadlen, message.payload);
    printf("message: ");
    char* payloadptr = message->payload;
    for(int i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("cause: %s\n", cause);
}

void deliveryComplete(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int mqtt_subscribe(MQTTClient client)
{
    char topic[30] = "mqtt-topic";
    int qos = 0;
    printf("enter the subscriber -- < topic > < qos (0/1/2)> : ");
    scanf("%s",topic);
    scanf("%d",&qos);
    int rc = MQTTClient_subscribe(client, topic, qos);
    return rc;
}

const char* payload_generator()
{
    json_object * jobj = json_object_new_object();

    //json_object *jstring = json_object_new_string("payload contain random and timestamp");

    time(&t);

    json_object *jstring1 = json_object_new_string(ctime(&t));

    json_object *jint = json_object_new_int((int)time(NULL));

    json_object *jint1 = json_object_new_int(rand());

    //json_object_object_add(jobj,"Init", jstring);

    json_object_object_add(jobj,"Current Local Time", jstring1);

    json_object_object_add(jobj,"timestamp", jint);

    json_object_object_add(jobj,"Random Number", jint1);

    return json_object_to_json_string(jobj);
}

void mqtt_connection(struct config_struct config) {

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc,ch;
    int clean_session,keepalive,publish_qos,publish_retain;
    char CLIENTID[MAX_LEN],publish_topic[MAX_LEN];
    char get_sub[20];
    const char *PAYLOAD;

    //printf("ADD %s\n",config.ADDRESS);
    printf("Enter -- < clientid > < cleansession (0/1)> < keepalive >: ");
    scanf("%s",CLIENTID);
    scanf("%d",&clean_session);
    scanf("%d",&keepalive);

    MQTTClient_create(&client,config.ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    //printf("%d %d\n",clean_session,keepalive);
    conn_opts.keepAliveInterval = keepalive;
    conn_opts.cleansession = clean_session;

    if((strcmp(config.security,"enable")==0)||(strcmp(config.security,"ENABLE")==0))
    {
        conn_opts.username = config.username;
        conn_opts.password = config.password;
    }

    if((strcmp(config.will_opts,"enable")==0)||(strcmp(config.will_opts,"ENABLE")==0))
    {
        conn_opts.will = &will_opts;
        will_opts.topicName = config.will_topic;
        will_opts.message = config.will_message;
    }


    MQTTClient_setCallbacks(client, NULL, connlost, messageArrived, deliveryComplete);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    
    printf("do you want to subscribe topic ? (y/n)  ");
    scanf("%s",get_sub);

    if((strcmp(get_sub,"y")==0)||(strcmp(get_sub,"Y")==0))
    {
        mqtt_subscribe(client);
        // char topic[30];
        // int qos;
        // scanf("%s",topic);
        // scanf("%d",&qos);
        // MQTTClient_subscribe(client, topic, qos);
    }
    
    printf("Enter -- < publish_topic > < publish_qos (0/1/2)> < publish_retain (0/1)> : ");
    scanf("%s",publish_topic);
    scanf("%d",&publish_qos);
    scanf("%d",&publish_retain);
    int flag=0;
    while(1)
    {   
        printf("Enter <d> to Disconnect the client or <p> to publish the message\n");
        ch = getchar();
        //printf("################### %c\n",ch);
        switch (ch)
        {
            case 'd':
                MQTTClient_disconnect(client, 100);
                MQTTClient_destroy(&client);
                flag=1;
                break;
            case 'p':
                PAYLOAD = payload_generator();

                pubmsg.payload = PAYLOAD;
                pubmsg.payloadlen = strlen(PAYLOAD);
                pubmsg.qos = publish_qos;
                pubmsg.retained = publish_retain;

                MQTTClient_publishMessage(client, publish_topic, &pubmsg, &token);

                //printf("Waiting for up to %d seconds for publication of %s\n""on topic %s for client with ClientID: %s\n",(int)(TIMEOUT/1000), PAYLOAD, publish_topic, CLIENTID);

                rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

                printf("Message with delivery token %d delivered\n", token);
                break;

        }
        sleep(2);
        if(flag==1)
            break;

    }
    // PAYLOAD = payload_generator();

    // pubmsg.payload = PAYLOAD;
    // pubmsg.payloadlen = strlen(PAYLOAD);
    // pubmsg.qos = publish_qos;
    // pubmsg.retained = 0;

    // MQTTClient_publishMessage(client, publish_topic, &pubmsg, &token);

    // //printf("Waiting for up to %d seconds for publication of %s\n""on topic %s for client with ClientID: %s\n",(int)(TIMEOUT/1000), PAYLOAD, publish_topic, CLIENTID);

    // rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

    // printf("Message with delivery token %d delivered\n", token);
    // //sleep(30);
    // printf("press Q or q to disconnect the client\n");
    // do
    // {
    //     ch = getchar();
    // } while(ch!='Q' && ch != 'q');
    // MQTTClient_disconnect(client, 10000);
    // MQTTClient_destroy(&client);
    //return rc;

}

void read_config_file(char* config_filename, struct config_struct config) {
    FILE *fp;
    char buf[CONFIG_LINE_BUFFER_SIZE];

    if ((fp=fopen(config_filename, "r")) == NULL) {
        fprintf(stderr, "Failed to open config file %s", config_filename);
        exit(EXIT_FAILURE);
    }
    while(! feof(fp)) {
        fgets(buf, CONFIG_LINE_BUFFER_SIZE, fp);
        //printf("buffer %s\n",buf);
        if (buf[0] == '#' || strlen(buf) < 4) {
            continue;
        }
        if (strstr(buf, "host")) {
            read_str_from_config_line(buf, config.ADDRESS);
            //printf("config.add %s\n",config.ADDRESS);
        }
        if (strstr(buf, "security")) {
            read_str_from_config_line(buf, config.security);
            //printf("config.security %s\n",config.security);
        }
        if (strstr(buf, "username")) {
            read_str_from_config_line(buf, config.username);
            //printf("config.username %s\n",config.username);
        }
        if (strstr(buf, "password")) {
            read_str_from_config_line(buf, config.password);
            //printf("config.username %s\n",config.password);
        }
        if (strstr(buf, "will_option")) {
            read_str_from_config_line(buf, config.will_opts);
            //printf("config.will_opts %s\n",config.will_opts);
        }
        if (strstr(buf, "will_topic")) {
            read_str_from_config_line(buf, config.will_topic);
            //printf("config.will_topic %s\n",config.will_topic);
        }
        if (strstr(buf, "will_payload")) {
            read_str_from_config_line(buf, config.will_message);
            //printf("config.will_msg %s\n",config.will_message);
        }

    }
    fclose(fp);

    mqtt_connection(config);
}