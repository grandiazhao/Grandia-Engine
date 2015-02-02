#ifndef GAFILEIO_H
#define GAFILEIO_H

#include <string>
#include <fstream>

class GAFileIO
{
	std::ifstream fin;
	std::ofstream fout;
	int totalSize;
	int sizeCount;
public:
	GAFileIO()
	{
	}
	bool openFile(std::string fileName)
	{
		fin.open(fileName.c_str());
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
		fout.open(fileName.c_str());
		if(fout.bad()) return false;
		return true;
	}
	void writeLine(std::string& line)
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