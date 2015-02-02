#ifndef GASERVERSIMPLE_H
#define GASERVERSIMPLE_H

#include "Common.h"
#include <iostream>
#include <stdio.h>
#include <Winsock2.h>
#include <string>
#include <map>
#include "windows.h"
#pragma comment(lib, "ws2_32.lib")
#include "GAFileIO.h"
#include "GADataManager.h"

class GAPMainManager;

class GAServerSimple
{
#define MaxClientNum 20
	static SOCKET sockServer;
	static SOCKET sockConn[20];   //服务器最大并发连接数：20
	SOCKADDR_IN addrServer;
	static SOCKADDR_IN addrClient;
	static GAServerSimple* serverlp;
	WSADATA wsaData;//WSAata用来存储系统传回的关于WinSocket的资料。
	static int serverStatus[MaxClientNum];////-1---该sockConn空闲。 1---该sockConn被占用。 0---该sockConn需要被关闭
	int serverPort;
	static HANDLE hMutex;
	static GAFileIO *fout;
	struct ServerStruct
	{
		GAServerSimple* server;
		int num;
	};
public:
	GAPMainManager* g_mainMgr;
public:
	GAServerSimple(GAPMainManager* mainMgr);
	bool initServer(int port);
	void destroy();
	void sendString(std::string str,int socketNo=0);
	std::string recvString(int socketNo);
	void disconnectClient(int socketNo);

private:
	void createServerThread();
	static DWORD WINAPI serverThread(LPVOID lpParameter);
	static DWORD WINAPI run(LPVOID lpParameter);
	static void acceptClient(int socketNo);
	static GADataMap* stringMap;
};


#endif