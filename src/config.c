#include "mqtt_utils.h"

void read_str_from_config_line(char* config_line, char* val) {   
    //printf("param %s %s\n",config_line,val); 
    char prm_name[MAX_CONFIG_VARIABLE_LEN];
    sscanf(config_line, "%s %s\n", prm_name, val);
}

int read_int_from_config_line(char* config_line) {    
    char prm_name[MAX_CONFIG_VARIABLE_LEN];
    int val;
    sscanf(config_line, "%s %d\n", prm_name, &val);
    return val;
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
        if (strstr(buf, "log_level")) {
            log_level = read_int_from_config_line(buf);
            //printf("log level %d\n",log_level);
        }

    }
    fclose(fp);
    LOG_INFO("MQTT Client Version 3.1.1 protocol support client...\n");
    
    mqtt_connection(config);
}