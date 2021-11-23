#include "mqtt_utils.h"

const char* payload_generator()
{
    json_object * jobj = json_object_new_object(); 

    //json_object *jstring = json_object_new_string("payload contain random and timestamp");

    time(&t);

    json_object *jstring = json_object_new_string(ctime(&t)); // string value

    json_object *jint = json_object_new_int((int)time(NULL)); // int value

    json_object *jint1 = json_object_new_int(rand());         // int value

    //json_object_object_add(jobj,"Init", jstring);

    json_object_object_add(jobj,"Current Local Time", jstring); // json string key (current local time)

    json_object_object_add(jobj,"timestamp", jint);             // json int key (timestamp)

    json_object_object_add(jobj,"Random Number", jint1);        // json int key (random number)

    return json_object_to_json_string(jobj);                    // return the json generator 
}