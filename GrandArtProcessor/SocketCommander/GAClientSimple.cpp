#include "GAClientSimple.h"
#include "GAPMisc.h"

SOCKET GAClientSimple::sockClient;
std::string GAClientSimple::recvStr;
UINT GAClientSimple::socketNum=(UINT)-1;

GAPMisc misc;

GAClientSimple::GAClientSimple()
{
	recvStr="";
}

bool GAClientSimple::connectServer(std::string address,int port)
{
	std::string ipAddr=address;
	WORD wVersionRequested;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return false;
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) 
	{
		WSACleanup( );
		return false;
	}
	sockClient=socket(AF_INET,SOCK_STREAM,0);// AF_INET ..tcp连接
	addrServer.sin_addr.S_un.S_addr=inet_addr(ipAddr.c_str());//设置网路地址
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(port);// 设置端口号
	err=connect(sockClient,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));//连接服务器
	if ( err != 0 ) {
		return false;
	}
	CreateThread(0,0,this->run,0,0,0);
	return true;
}

DWORD WINAPI GAClientSimple::run(LPVOID lpParameter)
{
	while(true)
	{
		recvString();
	}
}

void GAClientSimple::sendString(std::string str)
{
	str=misc.ConvertToString(socketNum)+"-"+str;
	send(sockClient,str.c_str(),str.length()+2,0);//发送数据
}

std::string GAClientSimple::recvString()
{
	char recvBuf[400];
	recv(sockClient,recvBuf,400,0);
	std::string str=recvBuf;
	std::string::size_type st=str.find_first_of("-");
	if(st!=str.npos)
	{
		std::string str1=str.substr(st+1);
		socketNum=atoi(str1.c_str());
		recvStr=str;
		return str;
	}
	if(str.length()>=399) return "";
	recvStr=str;
	return str;
}