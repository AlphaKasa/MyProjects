#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "Shop1.h"
#include "Serializer.h"

#include <iostream>

template<typename ConnectionPro>
class CTCPServer : public ConnectionPro
{
private:
    int copy_Port;
    char* copy_IP=NULL;
    int copy_Length=100;
public:
    CTCPServer(int nServerPort)  //构造函数
    {	
	    copy_Port = nServerPort;	
    }

public:
    int RunFuction()
    {
	int nListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == nListenSocket)
	{
	    cout << "socket has an error" << endl;
	    return -1;
	}

	sockaddr_in ServerAddress;
	memset(&ServerAddress, 0, sizeof(sockaddr_in));
	ServerAddress.sin_family = AF_INET;

	if(NULL == copy_IP)
	{
	    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
	    if(inet_pton(AF_INET, copy_IP, &ServerAddress.sin_addr) != 1)
	    {
		cout << "inet_pton has an error" << endl;
		close(nListenSocket);
		return -1;
	    }
	}
	ServerAddress.sin_port = htons(copy_Port);

	if(bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) == -1)
	{
	    cout << "bind has an error" << endl;
	    close(nListenSocket);
	    return -1;
	}
	if(listen(nListenSocket, copy_Length) == -1)
	{
	    cout << "listen has an error" << endl;
	    close(nListenSocket);
	    return -1;
	}
	sockaddr_in ClientAddress;
	socklen_t LengthOfCA = sizeof(sockaddr_in);
	int nConnectedSocket = accept(nListenSocket, (sockaddr *)&ClientAddress, &LengthOfCA);
	if(-1 == nConnectedSocket)
	{
	    cout << "accept has an error" << endl;
	    close(nListenSocket);
	    return -1;
	}
	ConnectionPro *pProcessor = static_cast<ConnectionPro *>(this);
	pProcessor->ServerFunction(nConnectedSocket, nListenSocket);

	close(nConnectedSocket);
	close(nListenSocket);

	return 0;
    }


};

class CMyTCPServer1
{
public:

    void ServerFunction(int nConnectedSocket, int nListenSocket)
    {
		Shop1 s1;
  
        s1.CetPluginNames();
        s1.InitializeCommodity();

        Serializer ser;
		ser.Initialization();  
		ser.Trans(1,s1.AllCommodity);
		
		s1.Show();
        int id=1;
        int num=0;
		cout<<"请输入要购买的商品编号和数目:"<<endl;
        while(id!=0){
            unsigned char* recv_buffer=new unsigned char[1024];
            recv(nConnectedSocket, recv_buffer,8, 0);
            memcpy(&id,recv_buffer,sizeof(int));
            memcpy(&num,recv_buffer + 4,sizeof(int));
            //cout<<id<<"  "<<num<<endl;
            delete(recv_buffer);

			unsigned char* send_buffer=new unsigned char[1024];       
            float money=s1.Sell(id,num);
            memcpy(send_buffer ,&money,sizeof(float));
            send(nConnectedSocket,send_buffer,sizeof(float),0);
            delete(send_buffer);
        }
		cout<<"输入1将商品全部转移至服务器2，输入2将转移的商品撤销，输入其他退出服务器:"<<endl;
		int ins=0;
		cin>>ins;
		if(ins==1){
            FILE *fp=fopen("data1","w+");
			int num=s1.AllCommodity.size();
			fwrite(&num,sizeof(int),1,fp);
			for(int i=0;i<num;i++){
			     fwrite(&s1.AllCommodity[i]->id,sizeof(int),1,fp);
			}
			fclose(fp);
		}
        else if(ins==2){
            FILE *fp=fopen("data1","w+");
			int num=0;
			fwrite(&num,sizeof(int),1,fp);
			fclose(fp);
		}
		
    }
};

int main()
{
    CTCPServer<CMyTCPServer1> server1(4000);
    server1.RunFuction();
    return 0;
}

