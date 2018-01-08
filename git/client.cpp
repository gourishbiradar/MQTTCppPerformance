#pragma warning(disable : 4996)

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <mosquitto.h>
#include "FileGen.h"

#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define TOPIC "testing"
#define LOG_LIMIT 1024
int OUT_NUM = 0;
int msg_count = 0;

int main(int argc, char *argv[])
{
	//int *mid = 0;
	std::vector<std::string> files(NUMBER_OF_FILES);
	bool clean_session = false;
	struct mosquitto *mosq = NULL;

	mosquitto_lib_init();
	mosq = mosquitto_new("client", clean_session, NULL);
	if (!mosq) {
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}

	int rc = mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE);
	if (rc) {
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}
	if (FileGenerator(files)==0)
	{
		int i = 0;
		while (i < NUMBER_OF_FILES)
		{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			//TODO: improve the way of reading the files
			std::ostringstream ss;
			ss << "File_" << i << ".txt";
			std::string file_name = ss.str();

			std::string message;
			std::ifstream file;
			//TODO: File error handling
			file.open(file_name.c_str());
			if (file)
			{
				while (std::getline(file, message))
				{					
					std::cout << message << std::endl;
					if (mosquitto_publish(mosq, NULL, TOPIC, message.size(), message.c_str(), 0, true))
					{
						std::cerr << "Can't publish the message: " << message << std::endl;
					}
					else
					{
						//Logging the results only meant for performance analysis
						std::ostringstream ss;
						ss << "LOG_" << OUT_NUM << ".txt";
						std::string file_name = ss.str();
						std::ofstream file;
						file.open(file_name.c_str(), std::ios::app);
						std::time_t val = std::time(nullptr);
						struct tm* now = localtime(&val);
						std::ostringstream rec;
						rec << now->tm_hour << now->tm_min << now->tm_sec;
						std::string TimeStamp = rec.str();
						file << TimeStamp << " " << message.size() << std::endl;
						msg_count++;
						file.close();
					}
					if (msg_count == LOG_LIMIT)
					{
						OUT_NUM = OUT_NUM + 1;
						msg_count = 0;
					}
				}
				i++;
				file.close();
			}
			else
			{
				std::cerr << "Can't open file:" << file_name << std::endl;
				system("PAUSE");
				return -1;
			}
		}
	}

	std::string message = "Finished sending";
	mosquitto_publish(mosq, NULL, TOPIC, message.size(), message.c_str(), 0, true);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return 0;
}