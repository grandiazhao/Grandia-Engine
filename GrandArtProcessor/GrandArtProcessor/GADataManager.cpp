#include "GADataManager.h"
#include "GAPMainManager.h"
#include "GAFileIO.h"

GADataNode::GADataNode()
{
	g_name="";
	g_type=NodeType_Undefined;
	g_save=true;
}

void GADataNode::setName(std::string name)
{
	g_name=name;
}
void GADataNode::setType(NodeType type)
{
	g_type=type;
}
void GADataNode::setSaveChild(bool save)
{
	g_save=save;
}
GADataNode::NodeType GADataNode::getType()
{
	return g_type;
}
void GADataNode::saveNodeAsString(std::string& str)
{
}

void GADataNode::loadStringData(GAStringVector::iterator& iter)
{
}

GADataMap::GADataMap(GADataMap::DataMapType type)
{
	g_type=type;
	init();
}
void GADataMap::init()
{
	g_floatmap=0;
	g_intmap=0;
	g_strmap=0;
	g_structVec=0;
	switch(g_type)
	{
	case DataMapType_Float:
		g_floatmap=new GAFloatMap();
		break;
	case DataMapType_Int:
		g_intmap=new GAIntMap();
		break;
	case DataMapType_String:
		g_strmap=new GAStringMap();
		break;
	case DataMapType_StructFloat2:
	case DataMapType_StructFloat3:
	case DataMapType_StructFloat4:
	case DataMapType_UserDefinedStruct:
		g_structVec=new std::vector<void*>;
		break;
	default:
		break;
	}
}
bool GADataMap::addData(std::string name,std::string value)
{
	if(g_type!=DataMapType_String) return false;
	GAStringMap::iterator iter=g_strmap->find(name);
	if(iter!=g_strmap->end()) iter->second=value;
	else g_strmap->insert(std::pair<std::string,std::string>(name,value));
	return true;
}

bool GADataMap::addData(std::string name,float value)
{
	if(g_type!=DataMapType_Float) return false;
	GAFloatMap::iterator iter=g_floatmap->find(name);
	if(iter!=g_floatmap->end()) iter->second=value;
	g_floatmap->insert(std::pair<std::string,float>(name,value));
	return true;
}

bool GADataMap::addData(std::string name,int value)
{
	if(g_type!=DataMapType_Int) return false;
	GAIntMap::iterator iter=g_intmap->find(name);
	if(iter!=g_intmap->end()) iter->second=value;
	g_intmap->insert(std::pair<std::string,int>(name,value));
	return true;
}

bool GADataMap::addData(GAFloat2Struct* float2Struct)
{
	if(g_type!=DataMapType_StructFloat2) return false;
	g_structVec->push_back(float2Struct);
	return true;
}

bool GADataMap::addData(GAFloat3Struct* float3Struct)
{
	if(g_type!=DataMapType_StructFloat3) return false;
	g_structVec->push_back(float3Struct);
	return true;
}

bool GADataMap::addData(GAFloat4Struct* float4Struct)
{
	if(g_type!=DataMapType_StructFloat4) return false;
	g_structVec->push_back(float4Struct);
	return true;
}

bool GADataMap::addData(GAUserDefinedStruct* userDefinedStruct)
{
	if(g_type!=DataMapType_UserDefinedStruct) return false;
	g_structVec->push_back(userDefinedStruct);
	return true;
}
float GADataMap::getFloatData(std::string name)
{
	if(g_type!=DataMapType_Float) return 0;
	return g_floatmap->at(name);
}
int GADataMap::getIntData(std::string name)
{
	if(g_type!=DataMapType_Int) return 0;
	return g_intmap->at(name);
}
std::string GADataMap::getStringData(std::string name)
{
	if(g_type!=DataMapType_String) return "";
	return g_strmap->at(name);
}
std::vector<void*>* GADataMap::getStructVector()
{
	return g_structVec;
}
GAFloat2Struct* GADataMap::getStructFloat2Data(UINT No)
{
	if(g_type!=DataMapType_StructFloat2||No>=g_structVec->size()) return 0;
	else return (GAFloat2Struct*)g_structVec->at(No);
}
GAFloat3Struct* GADataMap::getStructFloat3Data(UINT No)
{
	if(g_type!=DataMapType_StructFloat3||No>=g_structVec->size()) return 0;
	else return (GAFloat3Struct*)g_structVec->at(No);
}
GAFloat4Struct* GADataMap::getStructFloat4Data(UINT No)
{
	if(g_type!=DataMapType_StructFloat4||No>=g_structVec->size()) return 0;
	else return (GAFloat4Struct*)g_structVec->at(No);
}
void GADataMap::saveDataAsString(std::string& str)
{
	GAPMisc misc;
	if(g_type==DataMapType_Float)
	{
		str+="float\t"+misc.ConvertToString(g_floatmap->size())+"\r\n";
		for(GAFloatMap::iterator iter=g_floatmap->begin();iter!=g_floatmap->end();iter++)
		{
			str+=iter->first+"\t"+misc.ConvertToString(iter->second)+"\r\n";
		}
	}
	else if(g_type==DataMapType_Int)
	{
		str+="int\t"+misc.ConvertToString(g_intmap->size())+"\r\n";
		for(GAIntMap::iterator iter=g_intmap->begin();iter!=g_intmap->end();iter++)
		{
			str+=iter->first+"\t"+misc.ConvertToString(iter->second)+"\r\n";
		}
	}
	else if(g_type==DataMapType_String)
	{
		str+="string\t"+misc.ConvertToString(g_strmap->size())+"\r\n";
		for(GAStringMap::iterator iter=g_strmap->begin();iter!=g_strmap->end();iter++)
		{
			str+=iter->first+"\t"+iter->second+"\r\n";
		}
	}
	else if(g_type==DataMapType_StructFloat2)
	{
		UINT size=g_structVec->size();
		GAFloat2Struct* p;
		str+="struct_float2\t"+misc.ConvertToString(size)+"\r\n";
		for(UINT i=0;i<size;i++)
		{
			p=(GAFloat2Struct*)g_structVec->at(i);
			str+=misc.ConvertToString(p->x)+"\t"+misc.ConvertToString(p->y)+"\r\n";
		}
	}
	else if(g_type==DataMapType_StructFloat3)
	{
		UINT size=g_structVec->size();
		GAFloat3Struct* p;
		str+="struct_float3\t"+misc.ConvertToString(size)+"\r\n";
		for(UINT i=0;i<size;i++)
		{
			p=(GAFloat3Struct*)g_structVec->at(i);
			str+=misc.ConvertToString(p->x)+"\t"+misc.ConvertToString(p->y)+"\t"+misc.ConvertToString(p->z)+"\r\n";
		}
	}
	else if(g_type==DataMapType_StructFloat4)
	{
		UINT size=g_structVec->size();
		GAFloat4Struct* p;
		str+="struct_float4\t"+misc.ConvertToString(size)+"\r\n";
		for(UINT i=0;i<size;i++)
		{
			p=(GAFloat4Struct*)g_structVec->at(i);
			str+=misc.ConvertToString(p->x)+"\t"+misc.ConvertToString(p->y)+"\t"+misc.ConvertToString(p->z)+"\t"+misc.ConvertToString(p->w)+"\r\n";
		}
	}
	else if(g_type==DataMapType_UserDefinedStruct)
	{
		UINT size=g_structVec->size();
		GAUserDefinedStruct* p;
		str+="struct_userDefined\t"+misc.ConvertToString(size)+"\r\n";
		for(UINT i=0;i<size;i++)
		{
			p=(GAUserDefinedStruct*)g_structVec->at(i);
			str+=p->saveStructAsString()+"\r\n";
		}
	}
	else
	{
		str+="DataMapType_Undefined\t0\r\n";
	}
	str+="\r\n";
}

void GADataMap::loadStringData(GAStringVector::iterator& iter)
{
	std::string::size_type st;
	std::string line=(*iter);
	iter++;
	GAPMisc misc;
	st=line.find_first_of("\t");
	std::string type=line.substr(0,st);
	line=line.substr(st+1,line.length()-1);
	UINT num=misc.ConvertToInt(line);
	if(type=="float") g_type=DataMapType_Float;
	else if(type=="int") g_type=DataMapType_Int;
	else if(type=="string") g_type=DataMapType_String;
	else if(type=="struct_float2") g_type=DataMapType_StructFloat2;
	else if(type=="struct_float3") g_type=DataMapType_StructFloat3;
	else if(type=="struct_float4") g_type=DataMapType_StructFloat4;
	else if(type=="struct_userDefined") g_type=DataMapType_UserDefinedStruct;
	else g_type=DataMapType_Undefined;
	init();

	if(g_type==DataMapType_Float)
	{
		std::string name;
		float value;
		for(UINT i=0;i<num;i++)
		{
			line=(*iter);
			iter++;
			st=line.find_first_of("\t");
			name=line.substr(0,st);
			line=line.substr(st+1,line.length()-1);
			value=misc.ConvertToFloat(line);
			g_floatmap->insert(std::pair<std::string,float>(name,value));
		}
	}
	else if(g_type==DataMapType_Int)
	{
		std::string name;
		int value;
		for(UINT i=0;i<num;i++)
		{
			line=(*iter);
			iter++;
			st=line.find_first_of("\t");
			name=line.substr(0,st);
			line=line.substr(st+1,line.length()-1);
			value=misc.ConvertToInt(line);
			g_intmap->insert(std::pair<std::string,int>(name,value));
		}
	}
	else if(g_type==DataMapType_String)
	{
		std::string name;
		std::string value;
		for(UINT i=0;i<num;i++)
		{
			line=(*iter);
			iter++;
			st=line.find_first_of("\t");
			name=line.substr(0,st);
			value=line.substr(st+1,line.length()-1);
			g_strmap->insert(std::pair<std::string,std::string>(name,value));
		}
	}
	else if(g_type==DataMapType_StructFloat2)
	{
		float f1,f2;
		for(UINT i=0;i<num;i++)
		{
			line=(*iter);
			iter++;
			st=line.find_first_of("\t");
			f1=misc.ConvertToFloat(line.substr(0,st));
			f2=misc.ConvertToFloat(line.substr(st+1,line.length()-1));
			GAFloat2Struct* s=new GAFloat2Struct();
			s->x=f1;s->y=f2;
			g_structVec->push_back(s);
		}
	}
	else if(g_type==DataMapType_StructFloat3)
	{
		float f1,f2,f3;
		for(UINT i=0;i<num;i++)
		{
			line=(*iter);
			iter++;
			st=line.find_first_of("\t");
			f1=misc.ConvertToFloat(line.substr(0,st));
			line=line.substr(st+1,line.length()-1);
			st=line.find_first_of("\t");
			f2=misc.ConvertToFloat(line.substr(0,st));
			f3=misc.ConvertToFloat(line.substr(st+1,line.length()-1));
			GAFloat3Struct* s=new GAFloat3Struct();
			s->x=f1;s->y=f2;s->z=f3;
			g_structVec->push_back(s);
		}
	}
	else if(g_type==DataMapType_StructFloat4)
	{
		float f1,f2,f3,f4;
		for(UINT i=0;i<num;i++)
		{
			line=(*iter);
			iter++;
			st=line.find_first_of("\t");
			f1=misc.ConvertToFloat(line.substr(0,st));
			line=line.substr(st+1,line.length()-1);
			st=line.find_first_of("\t");
			f2=misc.ConvertToFloat(line.substr(0,st));
			line=line.substr(st+1,line.length()-1);
			st=line.find_first_of("\t");
			f3=misc.ConvertToFloat(line.substr(0,st));
			f4=misc.ConvertToFloat(line.substr(st+1,line.length()-1));
			GAFloat4Struct* s=new GAFloat4Struct();
			s->x=f1;s->y=f2;s->z=f3;s->w=f4;
			g_structVec->push_back(s);
		}
	}
	else
	{
	}
}

GADataLeafNode::GADataLeafNode():GADataNode()
{
	g_type=NodeType_Leaf;
}

void GADataLeafNode::initDataMap(GADataMap::DataMapType type)
{
	g_dataMap=new GADataMap(type);
}

GADataMap* GADataLeafNode::getDataMap()
{
	return g_dataMap;
}

void GADataLeafNode::saveNodeAsString(std::string& str)
{
	str+="Leaf\t"+g_name+"\r\n";
	if(g_save) g_dataMap->saveDataAsString(str);
	else str+="DataMapType_Undefined\t0\r\n";
}

void GADataLeafNode::loadStringData(GAStringVector::iterator& iter)
{
	g_dataMap->loadStringData(iter);
}
GADataContainerNode::GADataContainerNode() : GADataNode()
{
	g_type=NodeType_Container;
	g_childNodeMap.clear();
}

GADataNode* GADataContainerNode::createChildNode(std::string name,NodeType type)
{
	GADataNode* node;
	if(g_childNodeMap.find(name)!=g_childNodeMap.end()) return g_childNodeMap.find(name)->second;
	if(type==NodeType_Container)
	{
		node=new GADataContainerNode();
		node->setName(name);
		g_childNodeMap.insert(std::pair<std::string,GADataNode*>(name,node));
	}
	else if(type==NodeType_Leaf)
	{
		node=new GADataLeafNode();
		node->setName(name);
		((GADataLeafNode*)node)->initDataMap(GADataMap::DataMapType_Undefined);
		g_childNodeMap.insert(std::pair<std::string,GADataNode*>(name,node));
	}
	else
	{
		return 0;
	}
	return node;
}

GADataContainerNode* GADataContainerNode::createChildContainerNode(std::string name)
{
	return (GADataContainerNode*)createChildNode(name,NodeType_Container);
}

GADataLeafNode* GADataContainerNode::createChildLeafNode(std::string name)
{
	return (GADataLeafNode*)createChildNode(name,NodeType_Leaf);
}

GADataContainerNode* GADataContainerNode::getChildContainerNode(std::string name)
{
	DataNodeMap::iterator iter=g_childNodeMap.find(name);
	if(iter!=g_childNodeMap.end())
	{
		if(iter->second->getType()==NodeType_Container)
		{
			return (GADataContainerNode*)iter->second;
		}
	}
	return 0;
}

GADataLeafNode* GADataContainerNode::getChildLeafNode(std::string name)
{
	DataNodeMap::iterator iter=g_childNodeMap.find(name);
	if(iter!=g_childNodeMap.end())
	{
		if(iter->second->getType()==NodeType_Leaf)
		{
			return (GADataLeafNode*)iter->second;
		}
	}
	return 0;
}

GADataNode* GADataContainerNode::getChildNode(std::string name)
{
	DataNodeMap::iterator iter=g_childNodeMap.find(name);
	if(iter!=g_childNodeMap.end())
	{
		return iter->second;
	}
	return 0;
}

GADataNode* GADataContainerNode::getChildNode(UINT num)
{
	if(num>=getChildNum()) return 0;
	DataNodeMap::iterator iter=g_childNodeMap.begin();
	for(UINT i=0;i<num;i++) iter++;
	return iter->second;
}

int GADataContainerNode::getChildNum()
{
	return g_childNodeMap.size();
}

void GADataContainerNode::saveNodeAsString(std::string& str)
{
	GAPMisc misc;
	UINT size=getChildNum();
	str+="Container\t"+g_name+"\r\n";
	if(g_save) str+=misc.ConvertToString(size)+"\r\n";
	else
	{
		str+="0\r\n";
		return;
	}
	GADataNode* p;
	for(UINT i=0;i<size;i++)
	{
		getChildNode(i)->saveNodeAsString(str);
	}
}

void GADataContainerNode::loadStringData(GAStringVector::iterator& iter)
{
	std::string line=(*iter);
	iter++;
	GAPMisc misc;
	std::string::size_type st;
	std::string type="";
	std::string name="";
	UINT num=misc.ConvertToInt(line);
	for(UINT i=0;i<num;i++)
	{
		line=(*iter);
		iter++;
		st=line.find_first_of("\t");
		type=line.substr(0,st);
		name=line.substr(st+1,line.length()-1);
		if(type=="Container")
		{
			createChildContainerNode(name)->loadStringData(iter);
		}
		else if(type=="Leaf")
		{
			createChildLeafNode(name)->loadStringData(iter);
		}
		else
		{
			//error
		}
	}
}
GADataManager::GADataManager(GAPMainManager *mainMgr)
{
	g_mainMgr=mainMgr;
	g_rootDataNode.setName("root");
}

void GADataManager::init()
{
}

GADataContainerNode* GADataManager::getRootNode()
{
	return &g_rootDataNode;
}

bool GADataManager::saveDataTree(std::string filePath)
{
	GAFileIO fileo;
	if(!fileo.createNewFile(filePath)) return false;
	GAPMisc misc;
	UINT rootChildNum=g_rootDataNode.getChildNum();
	std::string line="";
	g_rootDataNode.saveNodeAsString(line);
	fileo.writeLine(line);
	fileo.endOutput();
	return true;
}

bool GADataManager::loadDataTree(std::string filePath)
{
	std::ifstream ifp;
	ifp.open(filePath.c_str(),std::ios::in | std::ios::binary);
	if(!ifp) return false;
	Ogre::DataStreamPtr dataptr(OGRE_NEW Ogre::FileStreamDataStream(filePath,&ifp,false));
	GAStringVector sv;
	while(!dataptr->eof())
	{
		std::string line=dataptr->getLine();
		if(line.length()>0&&line.at(0)!='#'&&line.at(0)!='@')
		{
			sv.push_back(line);
		}
	}
	GAStringVector::iterator iter=sv.begin();
	iter++;
	g_rootDataNode.loadStringData(iter);
	return true;
}