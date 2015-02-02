#ifndef __MYWRITETOFILE_H__
#define __MYWRITETOFILE_H__
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;
class MyWriteToFile
{
public:
	MyWriteToFile(const std::string str="d:\\test.data")
	{
		f1.open(str.c_str());
	}
	bool write(std::string str)
	{
		if(!f1) return false;
		f1<<str<<endl;
		return true;
		
	}
	~MyWriteToFile()
	{
		f1.close();
	}
private:
	ofstream f1;
};
#endif
