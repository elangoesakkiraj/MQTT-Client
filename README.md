# MQTT-Client
 MQTT-Client in C

MQTT Client Using Eclipse Paho Library

# Linux/Mac

	# for Eclipse paho-mqtt lib dependency

	The C client can be built for Linux/Unix/Mac with make and gcc. To build:

	git clone https://github.com/eclipse/paho.mqtt.c.git
	cd org.eclipse.paho.mqtt.c.git
	make
	To install:

	sudo make install


	# for json lib dependency

	sudo apt install libjson-c-dev 			# for Linux
	
	brew install json-c			        # for mac

# Build 

	# c files compile with CMake & build the folder structure using (CMAKE & MAKE FILE).

	cd <path>/src/  # staged to this folder 

	mkdir Build     # create directory into the src folder name as build 

	cd build 		# now switch into build folder

	cmake ..		# now type command cmake .. in terminal before check cmake v3.5 is available or not 

	make 			# now cmake generate the more file & dir in build dir. type the command make to compile c lib

	cd ..           # then back to src folder 

	make from_cmake2build # then run this command for create folder structure.


	# c file compile & build the folder struct using (GCC cmd & MAKE FILE).

	cd <path>/src   # staged to this folder

	make  			# type make  command to build 
