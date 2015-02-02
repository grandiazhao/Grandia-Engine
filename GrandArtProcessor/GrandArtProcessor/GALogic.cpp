#include "GALogic.h"

GALogic::GALogic()
{

}

void GALogic::addLogic(string name,string defination,int currentType)
{
	if(logicCaseSet.find(name)!=logicCaseSet.end()) return ;
	LogicCell c;
	c.name=name;
	c.defination=defination;
	_generateLogicSet(c);
	c.currentType=currentType;
	logicCaseSet[name]=c;
}

int GALogic::getValue(string name)
{
	return logicCaseSet[name].currentType;
}

int GALogic::updateAndGetValue(string name)
{
	_singleUpdateLogic(name);
	return getValue(name);
}

void GALogic::_singleUpdateLogic(string name)
{
	LogicCell& cell=logicCaseSet[name];
	for(DefinationSet::iterator iter=cell.defiSet.begin();iter!=cell.defiSet.end();iter++)
	{
		bool current=false;
		if(iter->second->size()>=3)
		{
			int value=logicCaseSet[iter->second->at(0).name].currentType;
			current=_vectorHasValue(iter->second->at(0).needs,value);
			int type=iter->second->at(1).type;
			value=logicCaseSet[iter->second->at(2).name].currentType;
			bool tempBool=_vectorHasValue(iter->second->at(2).needs,value);
			if(type==1) {current=current||tempBool;}
			else if(type==2) current=current&&tempBool;
			else if(type==3) current=current^tempBool;

			for(int i=3;i<iter->second->size();i++)
			{
				if(i%2!=0)
				{
					type=iter->second->at(i).type;
					continue;
				}
				else
				{
					value=logicCaseSet[iter->second->at(i).name].currentType;
					tempBool=_vectorHasValue(iter->second->at(i).needs,value);
					if(type==1) current=current||tempBool;
					else if(type==2) current=current&&tempBool;
					else if(type==3) current=current^tempBool;
				}
			}
		}
		else
		{
			int value=logicCaseSet[iter->second->at(0).name].currentType;
			current=_vectorHasValue(iter->second->at(0).needs,value);
		}
		if(current)
		{
			cell.currentType=iter->first;
			return;
		}
	}
}

bool GALogic::_vectorHasValue(vector<int>& needs,int value)
{
	for(int i=0;i<needs.size();i++)
	{
		if(needs[i]==value) return true;
	}
	return false;
}

void GALogic::_generateLogicSet(LogicCell &c)
{
	string temp1="";
	string defination=c.defination;
	string defi;
	GAPMisc misc;
	bool run=true;
	while(run)
	{
		string::size_type st=defination.find_first_of('#');
		if(st==temp1.npos)
		{
			run=false;
			defi=defination;
		}
		else
		{
			defi=c.defination.substr(0,st);
			defination=defination.substr(st+1);
		}
		vector<CalculateCell>* ccSet=new vector<CalculateCell>;
		string temp2;
		st=defi.find_first_of(':');
		temp2=defi.substr(0,st);
		defi=defi.substr(st+1);
		c.defiSet[misc.ConvertToInt(temp2)]=ccSet;
		temp1="";
		char* p=&defi[0];
		while(*p!='\0')
		{
			if(*p=='|')
			{
				ccSet->push_back(_generateCalculateCell(temp1));
				temp1="";
				CalculateCell cc;
				cc.type=1;
				ccSet->push_back(cc);
				p++;
			}
			else if(*p=='&')
			{
				ccSet->push_back(_generateCalculateCell(temp1));
				temp1="";
				CalculateCell cc;
				cc.type=2;
				ccSet->push_back(cc);
				p++;
			}
			else if(*p=='^')
			{
				ccSet->push_back(_generateCalculateCell(temp1));
				temp1="";
				CalculateCell cc;
				cc.type=3;
				ccSet->push_back(cc);
				p++;
			}
			else
			{
				temp1+=*p;
				p++;
			}
		}
		ccSet->push_back(_generateCalculateCell(temp1));
	}
}

GALogic::CalculateCell GALogic::_generateCalculateCell(string defination)
{
	CalculateCell cc;
	GAPMisc misc;
	if(defination[0]=='!')
	{
		cc.type=-1;
		defination=defination.substr(1,defination.length()-1);
	}
	else cc.type=0;
	string::size_type st=defination.find_first_of('(');
	cc.name=defination.substr(0,st);
	defination=defination.substr(st+1);
	st=defination.find_first_of(',');
	while(st!=defination.npos)
	{
		cc.needs.push_back(misc.ConvertToInt(defination.substr(0,st)));
		defination=defination.substr(st+1);
		st=defination.find_first_of(',');
	}
	defination=defination.substr(0,defination.length()-1);
	cc.needs.push_back(misc.ConvertToInt(defination));
	return cc;
}