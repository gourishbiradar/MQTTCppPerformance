# MQTTCppPerformance
The performance of MQTT in c++ is tested on both server and client side

# How to install the mosquitto broker?
Go to https://mosquitto.org/download/ to download the version of mqtt broker you wish to use
Go the the directory where the broker is installed and run the mosquitto.exe to start the broker

# How to check if the broker is running?
# On Windows 
1. Open command prompt (cmd in run)
2. Type netstat -a 
3. In the list displayed look for a local IP (127.0.0.1 or 0.0.0.0) and the port number *1883* which is the default port of Mosquitto broker
4. If there is no such socket present in the list restart the computer or the service through services.msc

# How to use the client and broker side programs?
Run the server.exe file to start the server 
Run the client.exe file to send required messages
The logs created on both the client and server folder can be used to test the performance of cpp implementation of Mosquitto

# Further reading
https://github.com/redboltz/mqtt_cpp is a good place to learn a more modern approach of mqtt
