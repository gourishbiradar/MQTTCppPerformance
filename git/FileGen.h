#pragma once
#include <windows.h>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#define SIZE 1024
#define NUMBER_OF_FILES 10
struct DataWord
{
	double info;
	double time;
	int qos;
};

struct DataWord RandomMessage()
{
	srand(time(NULL));
	struct DataWord message;
	message.info = rand();
	message.time = rand();
	message.qos = rand() % 3;
	return message;
}

int FileGenerator(std::vector<std::string> names)
{
	std::cout << "Generating file" << std::endl;
	for (int j = 0; j < NUMBER_OF_FILES; j++)
	{
		std::ofstream file;
		std::ostringstream ss;
		ss << "File_" << j << ".txt";
		std::string name = ss.str();
		std::cout << "Writing file: " << name << std::endl;
		//TODO: Test for failures in opening file
			file.open(name.c_str());
			names.push_back(name);
		for (int k = 0; k < SIZE; k++)
		{
			struct DataWord message = RandomMessage();
			file << message.qos << message.time << message.info << std::endl;
		}
		file.flush();
		file.close();
	}
	return 0;
}