#include <iostream>
#include "TCPClient.h"
#include <string>
#include <sstream>
#include<fstream>
#include <WS2tcpip.h>
#include<map>
#pragma comment (lib, "ws2_32.lib")

using namespace std;
int user_details_check();
struct userdetails
{
	string user_name;
	string password;
};

int user_details_check(string username, string user_entered_password)
{

	string s, user = username, pass = user_entered_password;
	string usercheck;
	int flag=0;
	int count, user_line, pass_loc, pass_count;
	string temp;

	//

	
		fstream obj1, obj2;
		obj1.open("user_check.txt");
		obj2.open("user_check.txt");

		for (int i = 0; i < 5; i++) {
			count = 0;
			obj1 >> s;
			temp = s;
			for (int j = 0; temp[j] != '_'; j++) {

				if (temp[j] == user[j]) {
					count += 1;

				}
				pass_loc = j;
			}
			if (count == user.length()) {


				user_line = i;
				s = s + '*';
				break;
			}


		}
		cout << pass_loc<< endl;
		for (int j = 0; j < 5; j++) {
			pass_count = 0;
			obj2 >> s;
			//cout << user_line << endl;
			if (j == user_line) {
				
				int z = 0;
				for (int x = pass_loc+1 ; s[x] != '*'; x++) {
					if (pass[z++] == s[x]) {
						pass_count += 1;
					}

				}
				if (pass_count == pass.length()) {
					cout << "Login Verified" << endl;;
					
					flag = 1;
					break;
				}
				else {
					cout << "Invalid Credentials" << endl;
					flag = 0;
					break;
				

				}
			}
		}
		if (flag == 1)
		{
			return 1;
		}
		else if (flag == 0)
		{
			return 0;
		}

	}






int main() {
	//usercheck uc;
	int flag = 0;
	int a = 0;
	TCPClient* client = new TCPClient;
	string msg = "a";
	string usernameEntered;
	string userpassword;


	//std::cout << usernameEntered<<endl;

	
		while (true) {
			if (a == 0) {
				std::cout << "Enter your username." << endl;
				Sleep(1);
				std::cin >> usernameEntered;
			std:cout << "Enter password:" << endl;
				std::cin >> userpassword;
				a = user_details_check(usernameEntered, userpassword);
				cout << a << endl;
				if (a == 0)
				{
					
					flag = 0;
					continue;

				}
				if (a == 1)
				{
					flag = 1;
					break;
				}
			}
		}
		if(flag==1)
		{
		if (client->initWinsock()) {

			client->connectSock();

			client->recvThread = thread([&] {
				client->threadRecv();
				});
		}
			 

			while(1)
			{

				
					getline(std::cin, msg);
					std::string messageToSend;
					client->username = usernameEntered;
					if (client->joinChat == false) {
						std::ostringstream ss;
						ss << client->username << ": " << msg;
						messageToSend = ss.str();
					}
					else if (client->joinChat == true) {
						std::ostringstream ss;
						ss << client->username << " joined the chat!";
						messageToSend = ss.str();
						client->joinChat = false;
					}
					client->sendMsg(messageToSend);
					//a = 1;
				
			}
		}

	


	return 0;

}