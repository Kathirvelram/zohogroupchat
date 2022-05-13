#include "TCPServer.h"
#include <iostream>
#include <string>
#include <sstream>
#include<fstream>
#include"msg_add.h"
#include"linkedlist.h"
const int MAX_BUFFER_SIZE = 4096;			//Constant value for the buffer size = where we will store the data received.
#include<map>
using namespace std;
int sendcheck;
string sendstatus,missinguser;
msg *c;
class Clients
{

public:
	string temp;
	map<SOCKET, string>a;
	void insertt(SOCKET Clientsocket, string aa)
	{
		a.insert({ Clientsocket, aa });

	}
	void traverse(SOCKET Clientsocket, string aa)
	{
		for (auto itr = a.begin(); itr != a.end(); ++itr) {
			cout << itr->second << '\n';
		}
	}
	void sendd(SOCKET Clientsocket, char* buf)
	{
		for (auto itr = a.begin(); itr != a.end(); ++itr) {
			if (itr->first == Clientsocket)
			{
				temp = itr->second;
				break;
			}
		}
		for(auto itr = a.begin(); itr != a.end(); ++itr)
		{
			if (itr->first != Clientsocket)
			{
				sendcheck=send(itr->first, buf, strlen(buf), 0);
				if (sendcheck < 0)
				{
					remove(temp,buf,Clientsocket);
				}
				else if (sendcheck > 0)
				{
					sendstatus = "yes";
					c->addmsg(temp,buf,itr->second,sendstatus);
				}
			}
		}
	}
	void remove(string sender,string buf, SOCKET Clientsocket)
	{
		sendstatus = "noo";
		for (auto itr = a.begin(); itr != a.end(); ++itr) {
			if (itr->first == Clientsocket)
			{
				 missinguser= itr->second;
					break;
			}
		}
		c->addmsg(sender, buf, missinguser, sendstatus);
		closesocket(Clientsocket);
	}
};
Clients clientt;
//TCPServer::TCPServer() { }


TCPServer::TCPServer(std::string ipAddress, int port)
	:listenerIPAddress(ipAddress), listenerPort(port) {
}

TCPServer::~TCPServer() {
	cleanupWinsock();			//Cleanup Winsock when the server shuts down. 
}


//Function to check whether we were able to initialize Winsock & start the server. 
bool TCPServer::initWinsock() {

	WSADATA data;
	WORD ver = MAKEWORD(2, 2);

	int wsInit = WSAStartup(ver, &data);

	if (wsInit != 0) {
		std::cout << "Error: can't initialize Winsock." << std::endl;
		return false;
	}

	return true;

}


//Function that creates a listening socket of the server. 
SOCKET TCPServer::createSocket() {

	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);	//AF_INET = IPv4. 

	if (listeningSocket != INVALID_SOCKET) {

		sockaddr_in hint;		//Structure used to bind IP address & port to specific socket. 
		hint.sin_family = AF_INET;		//Tell hint that we are IPv4 addresses. 
		hint.sin_port = htons(listenerPort);	//Tell hint what port we are using. 
		inet_pton(AF_INET, listenerIPAddress.c_str(), &hint.sin_addr); 	//Converts IP string to bytes & pass it to our hint. hint.sin_addr is the buffer. 

		int bindCheck = bind(listeningSocket, (sockaddr*)&hint, sizeof(hint));	//Bind listeningSocket to the hint structure. We're telling it what IP address family & port to use. 

		if (bindCheck != SOCKET_ERROR) {			//If bind OK:

			int listenCheck = listen(listeningSocket, SOMAXCONN);	//Tell the socket is for listening. 
			if (listenCheck == SOCKET_ERROR) {
				return -1;
			}
		}

		else {
			return -1;
		}

		return listeningSocket;

	}

}

int ii = 0;


//Function doing the main work of the server -> evaluates sockets & either accepts connections or receives data. 
void TCPServer::run() {

	char buf[MAX_BUFFER_SIZE];		//Create the buffer to receive the data from the clients. 
	SOCKET listeningSocket = createSocket();		//Create the listening socket for the server. 
	int l;
	while (true) {

		if (listeningSocket == INVALID_SOCKET) {
			break;
		}

		fd_set master;																	//File descriptor storing all the sockets.
		FD_ZERO(&master);																//Empty file file descriptor. 

		FD_SET(listeningSocket, &master);												//Add listening socket to file descriptor. 
		string word, filename,user,temp;
		int c = 0;
		while (true) {

			fd_set copy = master;	//Create new file descriptor bc the file descriptor gets destroyed every time. 
			int templength = 0;
			int socketCount = select(0, &copy, nullptr, nullptr, nullptr);				//Select() determines status of sockets & returns the sockets doing "work". 

			for (int i = 0; i < socketCount; i++) {										//Server can only accept connection & receive msg from client. 
				SOCKET sock = copy.fd_array[i];											//Loop through all the sockets in the file descriptor, identified as "active". 
				if (sock == listeningSocket) {											//Case 1: accept new connection.
					SOCKET client = accept(listeningSocket, nullptr, nullptr);	//Accept incoming connection & identify it as a new client. 
					int flag = 0;
					int aaa=recv(client, buf,strlen(buf), 0);
					//cout << aaa<<endl;

					int ll = strlen(buf);
					string bufd;
					for (int i = 0; i < aaa; i++)
					{
						bufd = bufd + buf[i];
					}
					//cout << bufd<<endl;
				
					
					if (aaa == 0)
					{

						cout << "connection closed reci" << endl;
					}
					else if (aaa < 0)
					{
						cout << "username not recevied";
					}
					//cout << bufd<< endl;
					std::string clientid ;
					fstream file;
					filename = "user_check.txt";
					file.open(filename.c_str());
		
					if (aaa > 0)
					{
						while (file >> word)
						{
							//cout << "hello" << endl;
							// displaying content
							//cout << word << endl;
							temp = word;
							user = "";
							for (int i = 0; i < temp.length(); i++)
							{

								if (temp[i] != '_')
								{
									user = user + temp[i];
								}
								else if (temp[i] == '_')
								{
									break;
								}
							}
							
							if (user == bufd)
							{
								clientt.insertt(client, user);
								flag = 1;
								break;
							}
							//templength = templength + temp.length();
						}
						
						if (flag == 1)
						{
							//clientt.traverse(client, user);
							FD_SET(client, &master);											//Add new connection to list of sockets.  
							std::string welcomeMsg = "Welcome to Zoho's Chat.\n";				//Notify client that he entered the chat. 
							send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
							std::cout << "New user joined the chat." << std::endl;
						}
						if (flag == 0)
						{
							cout << "client dropped!" << endl;
							closesocket(client);
						}
					}

						//cout << clientid<<endl;

						
					
						//Log connection on server side. 

				}
				else {																	//Case 2: receive a msg.	
					int c = 0;
					ZeroMemory(buf, MAX_BUFFER_SIZE);								//Clear the buffer before receiving data. 
					int bytesReceived = recv(sock, buf, MAX_BUFFER_SIZE, 0);//Receive data into buf & put it into bytesReceived. 
					

					//c->addmsg(user1, msg);

					//std::cout << buf << std::endl;
					if (bytesReceived <= 0) {	//No msg = drop client. 
						closesocket(sock);
						FD_CLR(sock, &master);	//Remove connection from file director.
					}
					else {	//Send msg to other clients & not listening socket. 


						for (int i = 0; i < master.fd_count; i++) {			//Loop through the sockets. 
							SOCKET outSock = master.fd_array[i];
							int flag = 0;
							if (outSock != listeningSocket) {

								if (outSock == sock) {		//If the current socket is the one that sent the message:
									std::string msgSent = "Message delivered.";
									send(outSock, msgSent.c_str(), msgSent.size() + 1, 0);	//Notify the client that the msg was delivered. 	

								}
								else {	//If the current sock is not the sender -> it should receive the msg. 
									string bufd1;
									for (int i = 0; i <bytesReceived; i++)
									{
										bufd1= bufd1 + buf[i];
									}
									if (int i = 0; i < bytesRecieved; i++)
									{
										if (bufd1[i] == '@')
										{
											flag = 1;
											break;
										}
									}
									if(flag==1)
									{ 

									send(outSock, username, bytesReceived, 0);

									}
									else
									{
										clientt.sendd(sock, buf);
									}

							}
						}

						std::cout << std::string(buf, 0, bytesReceived) << std::endl;			//Log the message on the server side. 

					}

				}
			}
		}


	}

}


//Function to send the message to a specific client. 
void TCPServer::sendMsg(int clientSocket, std::string msg) {

	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
	cout << "hello im from sendmsg class" << endl;

}


void TCPServer::cleanupWinsock() {

	WSACleanup();

}
