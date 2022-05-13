#include <iostream>
#include "TCPClient.h"
#include <string>
#include <sstream>
#include<fstream>
#include <WS2tcpip.h>
#include<map>
#include <exception>
#include <filesystem>
#pragma comment (lib, "ws2_32.lib")
namespace fs = std::filesystem;
using namespace std;
int user_details_check();
void filetransfer();
struct userdetails
{
	string user_name;
	string password;
};
void filetransfer()
{
	fs::path sourceFile = "C:\\Users\\karthik\\SOCKET\\AUTH_GROUP_CHAT\\AUTHENICATION CLIENT\\user_check.txt";
	fs::path targetParent = "C:\\Users\\karthik\\SOCKET\\AUTH_GROUP_CHAT\\AUTHENTICATION GROUP CHAT";
	auto target = targetParent / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".

	try // If you want to avoid exception handling, then use the error code overload of the following functions.
	{
		fs::create_directories(targetParent); // Recursively create target directory if not existing.
		fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
	}
	catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
	{
		std::cout << e.what();
	}
}

int user_details_check(string username,string userpassword)
{
	// filestream variable file
	fstream file;
	string word, t, q, filename;

	// filename of the file
	filename = "user_check.txt";
	file.open(filename.c_str());
	// opening file

	
	string user=username;
	string pass=userpassword;
	string temp;
	int tempi;
	int flag = 0;
	// extracting words from the file
	
	while (file >> word)
	{
		//cout << "hello" << endl;
		// displaying content
		//cout << word << endl;
		temp = word;
		user = "";
		pass = "";
		//cout << temp<<endl;
		for (int i = 0; i < temp.length(); i++)
		{

			if (temp[i] != '_')
			{
				user = user + temp[i];
			}
			else if (temp[i] == '_')
			{
				tempi = i + 1;
				while (tempi < temp.length())
				{
					pass = pass + temp[tempi];
					tempi++;
				}
				break;
			}
		}
		//cout << username<<endl;
		//cout << user <<endl;
		//cout << pass << endl;
		int usercheck = user.compare(username);
		int passcheck = pass.compare(userpassword);
		//cout << usercheck << " " << passcheck << endl;
		if (usercheck ==0&& passcheck==0)
		{
			
			flag=1;
			break;
		}
		
	}
	if (flag == 1)
	{
		return 1;
	}
	else if(flag==0)
	{
		
		return 0;
	}				
}
void signup()
{
	ofstream file;
	string username, password;
	file.open("user_check.txt", std::ios_base::app);
	s:
		cout << "enter username:" << endl;

		cin >> username;
		cout << "enter password:" << endl;
		cin >> password;
		
		int a = user_details_check(username,password);
		if (a == 0)
		{
			file << username + "_";
			file << password + ' ';
		}
		else
		{
			cout << "username already exist" << endl;
			goto s;
		}
		cout << "successfully signed up" << endl;
		
	
}

void login()
{
	int a = 0;
	TCPClient* client = new TCPClient;
	string msg = "a";
	string username, password;
	int flag = 0;
	int c=0;
	//cout << "loo" << endl;
	while (true) {
		cout << "enter username: " << endl;
		cin >> username;
		cout << "enter password: " << endl;
		cin >> password;
		a = user_details_check(username, password);
		//cout << a << endl;
		if (a == 0)
		{
			cout << "wrong credientials" << endl;
			c++;
			if (c == 2)
			{
				break;
			}
			else
			{
				continue;
			}
		}
		if (a == 1)
		{
			cout << "successfully logged in" << endl;
			flag = 1;
			break;
		}
	}
	if (flag == 1)
	{
		if (client->initWinsock()) {

			client->connectSock(username);

			client->recvThread = thread([&] {
				client->threadRecv();
				});
		}


		while (1)
		{
			getline(std::cin, msg);
			std::string messageToSend;
			client->username = username;
			if (client->joinChat == false) {
				std::ostringstream ss;
				ss << msg;
				messageToSend = ss.str();
			}
			else if (client->joinChat == true) {
				//std::ostringstream ss;
				//ss << client->username << " joined the chat!";
				//messageToSend = ss.str();
				client->joinChat = false;
			}
			client->sendMsg(messageToSend);
			//a = 1;

		}
	}
}



int main() {
	char user_choice,user_choice1;
		
		cout << "enter the choice : s (for signup), l (for login)" << endl;
		cin >> user_choice;
		switch (user_choice)
		{
		case 's':
			signup();
			filetransfer();
			cout << "do u want to login (y/n):" << endl;
			cin >> user_choice1;
			if (user_choice1 == 'y')
			{
				login();
				break;
			}
			else if (user_choice1== 'n')
			{
				break;
			}
		case 'l':
			login();
			break;
		default:
			cout << "enter the correct choice" << endl;
			break;

		}

return 0;

}