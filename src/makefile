CC=gcc

create : client_binary
	@echo --------------------------------------------------
	@echo " 	Successfully Client Created... "
	@echo --------------------------------------------------

client_binary:json_binary

	$(CC) -Wall mqtt.c -o client  json_payload.o config.o -lpaho-mqtt3c  -ljson-c

json_binary:config_binary

	$(CC) json_payload.c -c -ljson-c

config_binary:

	$(CC) config.c -c 