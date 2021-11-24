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
    LOG_INFO("Message arrived\n");
    printf("topic: %s\n", topicName);
    LOG_INFO("topic: %s\n", topicName);
    //printf("message:%d %s\n", message->payloadlen, message.payload);
    printf("message: ");
    char* payloadptr = message->payload;
    LOG_INFO("message: %s",payloadptr);
    for(int i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    LOG_TRACE("Message FREE\n");
    MQTTClient_freeMessage(&message);
    LOG_TRACE("topic FREE\n");
    MQTTClient_free(topicName);
    LOG_TRACE("messageArrived completed\n");
    return 1;
}

void connlost(void *context, char *cause)
{
    LOG_INFO("Connection Lost");
    printf("\nConnection lost\n");
    LOG_ERROR("cause: %s\n", cause);
    printf("cause: %s\n", cause);
}

void deliveryComplete(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    LOG_INFO("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int mqtt_subscribe(MQTTClient client)
{
    char topic[30] = "mqtt-topic";
    int qos = 0;
    LOG_INFO("enter the subscriber -- < topic > < qos (0/1/2)> : ");
    printf("enter the subscriber -- < topic > < qos (0/1/2)> : ");
    scanf("%s",topic);
    scanf("%d",&qos);
    LOG_DEBUG("subscriber topic %s qos %d",topic,qos);
    int rc = MQTTClient_subscribe(client, topic, qos);
    return rc;
}


void mqtt_connection(struct config_struct config) {
    LOG_TRACE("mqtt_connection entry");
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
    LOG_DEBUG("connect clientid %s,clean sess %d,keepalive %d",CLIENTID,clean_session,keepalive);
    MQTTClient_create(&client,config.ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    //printf("%d %d\n",clean_session,keepalive);
    conn_opts.keepAliveInterval = keepalive;
    conn_opts.cleansession = clean_session;

    if((strcmp(config.security,"enable")==0)||(strcmp(config.security,"ENABLE")==0))
    {
        conn_opts.username = config.username;
        conn_opts.password = config.password;
        LOG_DEBUG("connect username %s,pwd %s",config.username,config.password);
    }

    if((strcmp(config.will_opts,"enable")==0)||(strcmp(config.will_opts,"ENABLE")==0))
    {
        conn_opts.will = &will_opts;
        will_opts.topicName = config.will_topic;
        will_opts.message = config.will_message;
        LOG_DEBUG("will topic %s",config.will_topic);
    }


    MQTTClient_setCallbacks(client, NULL, connlost, messageArrived, deliveryComplete);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        LOG_ERROR("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    
    LOG_INFO("do you want to subscribe topic ? (y/n)  ");
    printf("do you want to subscribe topic ? (y/n)  ");
    scanf("%s",get_sub);
    LOG_TRACE("subscribe confirm %s",get_sub);
    if((strcmp(get_sub,"y")==0)||(strcmp(get_sub,"Y")==0))
    {
        mqtt_subscribe(client);
        LOG_TRACE("subscribed");
    }
    
    printf("Enter -- < publish_topic > < publish_qos (0/1/2)> < publish_retain (0/1)> : ");
    scanf("%s",publish_topic);
    scanf("%d",&publish_qos);
    scanf("%d",&publish_retain);
    int flag=0;
    LOG_DEBUG("pub topic %s qos %d retain %d",publish_topic,publish_qos,publish_retain);
    while(1)
    {   
        printf("Enter <d> to Disconnect the client or <p> to publish the message\n");
        LOG_TRACE("Enter <d> to Disconnect the client or <p> to publish the message\n");
        ch = getchar();
        //printf("################### %c\n",ch);
        LOG_TRACE("switch ch - %d",ch);
        switch (ch)
        {
            case 'd':
                LOG_TRACE("switch ch - d");
                MQTTClient_disconnect(client, 100);
                MQTTClient_destroy(&client);
                flag=1;
                LOG_INFO("Client Disconnect Successfully...\n");
                printf("Client Disconnect Successfully...\n");
                break;
            case 'p':
                LOG_TRACE("switch ch - p");
                PAYLOAD = payload_generator();
                pubmsg.payload = PAYLOAD;
                pubmsg.payloadlen = strlen(PAYLOAD);
                pubmsg.qos = publish_qos;
                pubmsg.retained = publish_retain;
                LOG_DEBUG("pub payload- %s",PAYLOAD);
                MQTTClient_publishMessage(client, publish_topic, &pubmsg, &token);

                //printf("Waiting for up to %d seconds for publication of %s\n""on topic %s for client with ClientID: %s\n",(int)(TIMEOUT/1000), PAYLOAD, publish_topic, CLIENTID);

                rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
                LOG_INFO("Message delivered\n");
                printf("Message delivered\n");
                break;

        }
        sleep(2);
        if(flag==1)
            break;

    }
    //fclose( log);

}

