#include "GAClientSimple.h"
#include "GAFileIO.h"
#include "GAPMisc.h"


void main()
{
	GAClientSimple c;
	char s[200];
	GAPMisc misc;
	GAFileIO file;
	file.openFile("serverport.txt");
	int port=misc.ConvertToInt(file.getNextLine());
	c.connectServer("127.0.0.1",port);
	std::string str;
	bool connected=false;
	while(true)
	{
		if(!connected)
		{
			Sleep(30);
			if(c.recvStr!="")
			{
				std::cout<<c.recvStr<<"\t"<<c.socketNum<<std::endl;
				c.recvStr="";
				connected=true;
			}
		}
		else
		{
			std::cin>>s;
			str=s;
			c.sendString(str);
			Sleep(30);
			if(c.recvStr!="")
			{
				std::cout<<c.recvStr<<std::endl;
				c.recvStr="";
			}
		}
		
		//std::string str=c.recvString();
		//if(str!="") std::cout<<str<<std::endl;
		//c.disconnectServer();
	}

}