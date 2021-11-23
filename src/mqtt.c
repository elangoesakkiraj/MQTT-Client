#include "mqtt_utils.h"

int main(int argc, char* argv[])
{
    struct config_struct config;
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

}

