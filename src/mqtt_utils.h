#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MQTTClient.h"  // paho-mqtt library
#include <json-c/json.h> // json library

#define TIMEOUT     10000L
#define MAX_LEN 256
#define CONFIG_LINE_BUFFER_SIZE 100
#define MAX_CONFIG_VARIABLE_LEN 20

volatile MQTTClient_deliveryToken deliveredtoken;

time_t t;

// config file params

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

void read_str_from_config_line(char* config_line, char* val);


// mqtt connection start here.


void mqtt_connection(struct config_struct config);

// Random payload generator

const char* payload_generator();
