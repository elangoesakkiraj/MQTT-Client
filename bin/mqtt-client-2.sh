echo "SELECT THE TEST PROCESS"

echo "Test 1) will connect"
echo "Test 2) duplicate connect"
echo "Test 3) SYS/#  topic"
echo "Test 4) # topic subscribe"
echo "Test 5) pub and sub"

read a

echo "you selected the test case $a"

case $a in 
	1 ) 
		echo "########################################################################################################################"
		echo "will connect..."
		echo "* ensure in conf file will topic is enable & log level must be 1.give will topic name and will message.then connect the <path/bin/mqtt-client-1.sh client"
		echo "* connect the another client from <path/bin/mqtt-client-2.sh with subscribe to will topic name."
		echo "* now disconnect the <path/bin/mqtt-client-1.sh.mqtt-client-1.sh publish will message and mqtt-client-2.sh receive the will message from mqtt-client-1.sh";;
	2 ) 
		echo "########################################################################################################################"
		echo "duplicate connect"
		echo "* ensure log level must be 3 in conf file.connect the <path/bin/mqtt-client-1.sh client with the clientid <eg: mqtt-client> and subscribe to any topic"
		echo "* once again connect the <path/bin/mqtt-client-2.sh client with the same clientid <eg: mqtt-client>."
		echo "* Here,same client id( mqtt-client-2.sh ) doesn't allow to connect with mqtt server at the same time";;
	3 ) 
		echo "########################################################################################################################"
		echo "$SYS/# topic"
		echo "* log level must be 1 & connect the <path/bin/mqtt-client-1.sh client and subscribe to $SYS/# topic."
		echo "* it will receive broker performance details and broker metric message rate & network perf from group $SYS/ topic."
		echo "* we can moniter and build client application.";;
	4 ) 
		echo "########################################################################################################################"
		echo "# topic subscribe"
		echo "* log level must be 1 & connect the <path/bin/mqtt-client-1.sh client and subscribe to # topic."
		echo "* it will receives the every messages from broker .whatever message receives broker from all client, include $SYS topic."
		echo "* it will sents to # subscriber client";;
	5 ) 
		echo "########################################################################################################################"
		echo "pub and sub"
		echo "* connect the <path/bin/mqtt-client-1.sh client and subscribe to (test-topic) topic."
		echo "* same client publish with subscribed topic (test-topic)."
		echo "* the client mqtt-client-1 published message recevied by itself.";;
	*) 
		echo "default"
esac

control_c() {
	echo "########################################################################################################################"
    echo "################################################   TEST RESULT   #######################################################"
    echo "########################################################################################################################"
    echo "$a"
    case $a in
		1)
			echo "test case 1:"
			filecheck=$(cat ../log/client.log | grep -c "topic: will-topic")
			if [ $filecheck -gt 0 ]; then
				echo "################################################"
				echo "will connect sucessfully test pass"
			else
				echo "################################################"
				echo "will connect Test fail"
			fi
			echo "Test completed...";;
    	2)
			echo "test case 2:"
			filecheck=$(cat ../log/client.log | grep -c "Failed to connect, return code -1")
			if [ $filecheck -gt 0 ]; then
				echo "################################################"
				echo "duplicate works sucessfully test pass"
			else
				echo "################################################ $SYS/broker/version"
				echo "duplicate Test fail"
			fi
			echo "Test completed...";;
		3)
			echo "test case 3:"
			filecheck=$(cat ../log/client.log | grep -c "topic: $SYS/broker/version")
			if [ $filecheck -gt 0 ]; then
				echo "################################################"
				echo "$SYS topic sucessfully test pass"
			else
				echo "################################################"
				echo "$SYS topic Test fail"
			fi
			echo "Test completed...";;
		4)
			echo "test case 4:"
			filecheck=$(cat ../log/client.log | grep -c "Message arrived")
			if [ $filecheck -gt 0 ]; then
				echo "################################################"
				echo "# topic subscribe sucessfully test pass"
			else
				echo "################################################"
				echo "# topic subscribe Test fail"
			fi
			echo "Test completed...";;
		5) 
			echo "test case 5:"
			filecheck=$(cat ../log/client.log | grep -c "topic: test-topic")
			if [ $filecheck -gt 0 ]; then
				echo "################################################"
				echo "pub and sub sucessfully test pass"
			else
				echo "################################################"
				echo "pub and sub Test fail"
			fi
			echo "Test completed...";;

	esac

}

trap control_c INT

../lib/client 

sleep 1

control_c