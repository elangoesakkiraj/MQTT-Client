########### CMake VERSION 3.5 ##################


cmake_minimum_required(VERSION 3.5)

########### For C Compile Include ##############

project(client)
add_executable(${PROJECT_NAME} 
			   config.c
			   json_payload.c
			   log_utils.c
			   mqtt.c)

############ Library include ##################

target_link_libraries(${PROJECT_NAME} json-c)

target_link_libraries(${PROJECT_NAME} paho-mqtt3c)


############# Testing #########################

enable_testing()