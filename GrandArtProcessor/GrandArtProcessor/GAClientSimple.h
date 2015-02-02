#ifndef GACLIENTSIMPLE_H
#define GACLIENTSIMPLE_H

#include "Common.h"
#include <iostream>
#include <stdio.h>
#include <Winsock2.h>
#include <string>
#include <map>
#include "windows.h"
#pragma comment(lib, "ws2_32.lib")
#include "GAFileIO.h"

class GAClientSimple
{
private:
	WSADATA wsaData;//WSAata�����洢ϵͳ���صĹ���WinSocket�����ϡ�
	SOCKET sockClient;
	SOCKADDR_IN addrServer;
public:
	GAClientSimple();
	bool connectServer(std::string address,int port);
	void run();
	std::string recvString();
	void sendString(std::string str);
	void destroy();
};
#endif