#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<vector>
#include <fcntl.h>
#include <memory>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "Serializer.h"
using namespace std;
float money=10000;


template<typename ConnectionPor>
class CTCPClient : public ConnectionPor
{
private:
    int copy_Port;
    char *copy_IP;
public:
    CTCPClient(int nServerPort, const char *strServerIP)
    {
	copy_Port = nServerPort;

	int nlength = strlen(strServerIP);
	copy_IP = new char [nlength + 1];
	strcpy(copy_IP, strServerIP);
    }
public:
    int RunFuction()
    {
	int nClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == nClientSocket)
	{
	    cout << "socket has an error" << endl;
	    return -1;
	}

	sockaddr_in ServerAddress;
	memset(&ServerAddress, 0, sizeof(sockaddr_in));
	ServerAddress.sin_family = AF_INET;
	if(inet_pton(AF_INET, copy_IP, &ServerAddress.sin_addr) != 1)
	{
	    cout << "inet_pton has an error" << endl;
	    close(nClientSocket);
	    return -1;
	}

	ServerAddress.sin_port = htons(copy_Port);

	if(connect(nClientSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
	{
	    cout << "connect has an error" << endl;
	    close(nClientSocket);
	    return -1;
	}

	ConnectionPor *pProcess = static_cast<ConnectionPor *>(this);
	pProcess->ClientFunction(nClientSocket);

	close(nClientSocket);

	return 0;
    }

};

class CMyTCPClient
{
public:


    void ClientFunction(int nConnectedSocket)
    {
		int id=1;
        int num=0;
        while(id!=0){
            unsigned char* send_buffer=new unsigned char[1024];
            cin>>id;
            cin>>num;
            memcpy(send_buffer,&id,sizeof(int));
            memcpy(send_buffer + 4,&num,sizeof(int));
            send(nConnectedSocket,send_buffer,8,0);
            delete(send_buffer);

            unsigned char* recv_buffer=new unsigned char[1024];
            recv(nConnectedSocket, recv_buffer,sizeof(float), 0);
            float cost;
            memcpy(&cost,recv_buffer,sizeof(float));
            money-=cost;
            cout<<"当前账户余额:"<<money<<endl;
            delete(recv_buffer);
        }
		

    }
};

int main()
{
    CTCPClient<CMyTCPClient> client(4000, "127.0.0.1");	
    client.RunFuction(); 
    return 0;
}