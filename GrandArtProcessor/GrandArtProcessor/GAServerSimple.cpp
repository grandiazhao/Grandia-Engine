#include "GAServerSimple.h"
#include "GAPMainManager.h"
#include "GADataManager.h"

GAServerSimple* GAServerSimple::serverlp=0;
int GAServerSimple::serverStatus[20];
SOCKET GAServerSimple::sockServer;
SOCKET GAServerSimple::sockConn[20];
SOCKADDR_IN GAServerSimple::addrClient;
GADataMap* GAServerSimple::stringMap=0;
HANDLE GAServerSimple::hMutex;
GAFileIO* GAServerSimple::fout=new GAFileIO();

GAServerSimple::GAServerSimple(GAPMainManager* mainMgr)
{
	for(UINT i=0;i<MaxClientNum;i++)
	{
		serverStatus[i]=-1;
	}
	g_mainMgr=mainMgr;
	g_mainMgr->g_dataMgr->getRootNode()->createChildLeafNode("recvString")->initDataMap(GADataMap::DataMapType_String);
	stringMap=g_mainMgr->g_dataMgr->getRootNode()->getChildLeafNode("recvString")->getDataMap();
	stringMap->addData("recv","Hello");
	fout->createNewFile("server.txt");
	serverlp=this;
}

bool GAServerSimple::initServer(int port)
{
	serverPort=port;
	WORD wVersionRequested;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return false;
	}
	
	if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) {
		WSACleanup( );
		return false;
	}

	sockServer=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrServer;
	addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(port);
	bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));// °ó¶¨¶Ë¿Ú
	listen(sockServer,MaxClientNum);
	createServerThread();
	return true;
}

void GAServerSimple::createServerThread()
{
	CreateThread(0,0,this->run,0,0,0);
}
DWORD WINAPI GAServerSimple::run(LPVOID lpParameter)
{
	int num=0;
	while(true)
	{
		WaitForSingleObject(hMutex,INFINITE);
		if(serverlp==0) continue;
		int i=0;
		for(;i<20;i++)
		{
			if(serverStatus[i]==-1)
			{
				num=i;
				serverStatus[i]=1;
				break;
			}
		}
		if(i==20)
		{
			ReleaseMutex(hMutex);
			Sleep(1000);
			continue;
		}
		else ReleaseMutex(hMutex);
		acceptClient(num);
		fout->writeLine("ServerConnected");
		ServerStruct *s=new ServerStruct();
		s->server=serverlp;
		s->num=num;
		HANDLE hThread = CreateThread(NULL, 0,serverThread,s, 0, NULL);
		CloseHandle(hThread);
	}
}

void GAServerSimple::destroy()
{
	WSACleanup( );
	closesocket(sockServer);
}

void GAServerSimple::sendString(std::string str,int socketNo)
{
	send(sockConn[socketNo],str.c_str(),str.length()*2+2,0);
}

std::string GAServerSimple::recvString(int socketNo)
{
	char recvBuf[400];
	recv(sockConn[socketNo],recvBuf,400,0);
	std::string str=recvBuf;
	if(str.length()>=399) return "endConnect";
	return str;
}

void GAServerSimple::disconnectClient(int socketNo)
{
	closesocket(sockConn[socketNo]);
}

DWORD WINAPI GAServerSimple::serverThread(LPVOID lpParameter)
{
	ServerStruct *s=(ServerStruct*)lpParameter;
	GAServerSimple* ss=s->server;
	int num=s->num;
	char c=num+48;
	GAPMisc misc;
	std::string str11="SocketNum-"+misc.ConvertToString(num);
	ss->sendString(str11.c_str(),num);
	while(1) {
		std::string str=ss->recvString(num);
		if(str!="")
		{
			fout->writeLine(str);
			ss->stringMap->addData("recv",str);
			std::string str2="";
			if(str.length()>=10) str2=str.substr(str.length()-10);
			if(str2=="endConnect")
			{
				ss->serverStatus[num]=-1;
				break;
			}
		}
	}
	ss->disconnectClient(num);
	ss->serverStatus[num]=-1;
	return 0;
}

void GAServerSimple::acceptClient(int socketNo)
{
	int len=sizeof(SOCKADDR);
	sockConn[socketNo]=accept(sockServer,(SOCKADDR*)&addrClient,&len);
}