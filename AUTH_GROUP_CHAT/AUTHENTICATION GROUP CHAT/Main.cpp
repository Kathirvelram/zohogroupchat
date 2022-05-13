#include <iostream>
#include <string>
#include "TCPServer.h"
#include "linkedlist.h"
#include"msg_add.h"
#include<fstream>
using namespace std;
string serverIP = "127.0.0.1";
class t
{
public:
	void addmsg(std::string username, std::string msg)
	{
		ofstream file;
		file.open("user_check.txt", std::ios_base::app);
		file << username + "_";
		file << msg + ' ';
	}
};
int main() {
	//ofstream file;
	TCPServer server(serverIP, 54010);

	if (server.initWinsock()) {

		server.run();

	}


}