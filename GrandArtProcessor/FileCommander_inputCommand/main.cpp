#include "GAFileIO.h"
#include <iostream>

int main()
{
	char c='q';
	GAFileIO fileio;
	std::string str="";
	while(c!='x')
	{
		std::cin>>c;
		fileio.createNewFile("../SceneChaBei/inputCommand.txt");
		if(c=='l')
		{
			str="Left";
		}
		else if(c=='r')
		{
			str="Right";
		}
		fileio.writeLine(str);
		fileio.endOutput();
	}
}