#ifndef GASFILEIO_H
#define GASFILEIO_H

#include <fstream>
#include <iostream>
#include <string>

#define UINT unsigned int

class GAFileIO
{
	std::ifstream fin;
	std::ofstream fout;
	UINT totalSize;
	UINT sizeCount;
public:
	GAFileIO()
	{
	}
	~GAFileIO()
	{
		endOutput();
		endInput();
	}
	bool openFile(std::string fileName)
	{
		fin.open(fileName);
		if(!fin) return false;
		return true;
	}
	std::string getNextLine(int maxLineSize=1024)
	{
		char str[1024];
		fin.getline(str,maxLineSize);
		std::string ret(str);
		//delete str;
		return ret;
	}
	bool createNewFile(std::string fileName)
	{
		fout.open(fileName);
		if(fout.bad()) return false;
		return true;
	}
	void writeLine(std::string line)
	{
		fout<<line<<std::endl;
	}
	void endOutput()
	{
		fout.close();
	}
	void endInput()
	{
		fin.close();
	}
};
#endif