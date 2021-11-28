CC=gcc

create : build_creation
	@echo --------------------------------------------------
	@echo " 	Successfully Client Created... "
	@echo --------------------------------------------------

build_creation:client_binary
	@echo --------------------------------------------------
	@echo " 	folder structure Created... "
	@echo --------------------------------------------------
	mkdir ../mqttclient
	mkdir ../mqttclient/conf
	mkdir ../mqttclient/log
	mkdir ../mqttclient/lib	
	mkdir ../mqttclient/bin
	cp ../bin/* ../mqttclient/bin/
	cp ../conf/* ../mqttclient/conf/
	cp ./build/client ../mqttclient/lib/
	rm -rf client json_payload.o config.o log_utils.o
	@echo --------------------------------------------------
	@echo " 	zip compressed... "
	@echo --------------------------------------------------
	zip -ry ../mqttclient.zip ../mqttclient
	
from_cmake2build:clean
	@echo --------------------------------------------------
	@echo " 	folder structure Created... "
	@echo --------------------------------------------------
	mkdir ../mqttclient
	mkdir ../mqttclient/conf
	mkdir ../mqttclient/log
	mkdir ../mqttclient/lib	
	mkdir ../mqttclient/bin
	cp ../bin/* ../mqttclient/bin/
	cp ../conf/* ../mqttclient/conf/
	cp ./build/client ../mqttclient/lib/
	rm -rf client json_payload.o config.o log_utils.o
	@echo --------------------------------------------------
	@echo " 	zip compressed... "
	@echo --------------------------------------------------
	zip -ry ../mqttclient.zip ../mqttclient

client_binary:json_binary
	@echo --------------------------------------------------
	mkdir ./build
	$(CC) -Wall mqtt.c -o ./build/client  json_payload.o config.o log_utils.c -lpaho-mqtt3c  -ljson-c
	@echo --------------------------------------------------
	@echo " 	C files Compiled... "
	@echo --------------------------------------------------

json_binary:config_binary

	$(CC) json_payload.c -c -ljson-c
	

config_binary:clean
	@echo --------------------------------------------------
	@echo " 	C files Compiling... "
	@echo --------------------------------------------------

	$(CC) config.c -c 

clean:
	@echo --------------------------------------------------
	@echo " 	cleaning binary files... "
	@echo --------------------------------------------------
	rm -rf client json_payload.o config.o log_utils.o
	rm -rf ../mqttclient
	rm -rf ../mqttclient.zip
	@echo --------------------------------------------------
	@echo " 	cleaning completed... "
	@echo --------------------------------------------------