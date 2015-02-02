#ifndef GALOGIC_H
#define GALOGIC_H

#include "Common.h"
#include <vector>
#include <map>

using namespace std;
class GALogic
{
public:
	enum LogicType
	{
		LogicType_Bool=0,
		LogicType_Int,
	};
	struct CalculateCell
	{
		string name;
		int type;
		std::vector<int> needs;
	};
	typedef std::map<int,std::vector<CalculateCell>*> DefinationSet;
	struct LogicCell
	{
		string defination;
		string name;
		int currentType;
		DefinationSet defiSet;
	};
private:
	//defination -- list all cases that makes the logic returns true.
	//1:name1(1,2,3,5-8)&name2(2,3,5)#4:name1(1,2,3,5-8)|name2(2,3,5) name1(1,2,3,5-8)^name2(2,3,5) !name1(1,2,3)
	std::map<string,LogicCell> logicCaseSet;  //name and defination
public:
	GALogic();
	void addLogic(string name,string defination,int currentType);
	void updateLogic();
	int getValue(string name);
	int updateAndGetValue(string name);
	void setLogic(string name);
private:
	bool _vectorHasValue(std::vector<int>& needs,int value);
	void _singleUpdateLogic(string name);
	void _generateLogicSet(LogicCell& c);
	CalculateCell _generateCalculateCell(string defination);
};




#endif