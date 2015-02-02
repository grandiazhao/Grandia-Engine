#include "GAClientSimple.h"

GAClientSimple::GAClientSimple()
{

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
	return true;
}

void GAClientSimple::run()
{

}

void GAClientSimple::sendString(std::string str)
{
	send(sockClient,str.c_str(),str.length()+1,0);//发送数据
}

std::string GAClientSimple::recvString()
{
	char recvBuf[200];
	recv(sockClient,recvBuf,200,0);
	std::string str=recvBuf;
	return str;
}