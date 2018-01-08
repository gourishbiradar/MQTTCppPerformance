#pragma warning(disable : 4996)

#include <iostream>
#include <mosquitto.h>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>

#define HOST "localhost"
#define PORT 1883
#define TOPIC "testing"
#define KEEP_ALIVE 60
#define LOG_LIMIT 1024
int OUT_NUM = 0;
int msg_count = 0;

void on_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	std::ostringstream ss;
	ss << "LOG_" << OUT_NUM << ".txt";
	std::string file_name = ss.str();
	std::ofstream file;
	file.open(file_name.c_str(),std::ios::app);
	std::time_t val = std::time(nullptr);
	struct tm* now = localtime(&val);
	std::ostringstream rec;
	rec << now->tm_hour << now->tm_min << now->tm_sec;
	std::string TimeStamp = rec.str(); 	
	//Disconnect mosq if it recives a stop signal
	if (message->payloadlen)
	{
		std::ostringstream ss;
		ss << message->payload;
		std::string msg = ss.str();
		if (!strcmp(msg.c_str(), "Finished sending"))
		{
			mosquitto_disconnect(mosq);
			return;
		}
	}
	
	if (message->payloadlen)
	{
		//std::cout << "Reached Here" << std::endl;
		std::cout << "Read a message:" << message->mid <<" "<< message->payload << std::endl;
		file << TimeStamp << " " << message->mid << " " << message->payloadlen << "\n";
		msg_count++;
		file.close();
	}
	if (msg_count == LOG_LIMIT)
	{
		OUT_NUM = OUT_NUM + 1;
		msg_count = 0;
	}
}
int main()
{
	// create a mosq
	struct mosquitto* mosq = NULL;
	bool clean_session = false;
	mosquitto_lib_init();
	mosq = mosquitto_new("server", clean_session, NULL);
	if (!mosq)
	{
		std::cerr << "Error: out of memory\t Error#: "<< errno << std::endl;
		system("PAUSE");
		return -1;
	}

	// connect mosq to a host and port
	int rc = mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE);
	if (rc)
	{
		std::cout << mosquitto_strerror(rc) << std::endl;
		//std::cerr << "Can't connect to host:" << HOST << " at port:" << PORT << "Error#:" << errno << std::endl;
		system("PAUSE");
		return -1;
	}
	// subscribe to the topic
	int sub_status = mosquitto_subscribe(mosq, NULL, TOPIC, 0);
	if (sub_status)
	{
		std::cerr << "can't subscribe to topic:" << TOPIC << "Error:"<< mosquitto_strerror(sub_status) << std::endl;
		system("PAUSE");
	}
	// read the message from the topic
	mosquitto_message_callback_set(mosq,on_message_callback);

	mosquitto_loop_forever(mosq, -1, 1);
	
	// at close signal destroy mosq
	mosquitto_destroy(mosq);

	// cleanup
	mosquitto_lib_cleanup();
	std::cout << "Finished reading\n";
	system("PAUSE");
	return 0;
}