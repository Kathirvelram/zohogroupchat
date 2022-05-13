#pragma once
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class msg
{
public:
	void addmsg(std::string username1, std::string msg1, std::string username2, std::string sendstatus)
	{
		ofstream file;
		file.open("msg.txt", std::ios_base::app);
		file << username1 + "_" + msg1 + "_" + username2 + "_" + sendstatus + " ";
	}
		
		//file << username2 + "_";
		

	
};
