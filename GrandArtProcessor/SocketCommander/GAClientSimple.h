#ifndef GACLIENTSIMPLE_H
#define GACLIENTSIMPLE_H

#include <iostream>
#include <stdio.h>
#include <Winsock2.h>
#include <string>
#include <map>
#include "windows.h"
#pragma comment(lib, "ws2_32.lib")

class GAClientSimple
{
private:
	WSADATA wsaData;//WSAata用来存储系统传回的关于WinSocket的资料。
	static SOCKET sockClient;
	SOCKADDR_IN addrServer;
public:
	static std::string recvStr;
	static UINT socketNum;
public:
	GAClientSimple();
	bool connectServer(std::string address,int port);
	static DWORD WINAPI run(LPVOID lpParameter);
	static std::string recvString();
	void sendString(std::string str);
	void destroy();
};
#endif