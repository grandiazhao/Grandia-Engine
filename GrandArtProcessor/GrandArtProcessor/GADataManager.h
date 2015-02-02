#ifndef GADATAMANAGER_H
#define GADATAMANAGER_H

#include "Common.h"

class GAExport GADataNode
{
public:
	enum NodeType
	{
		NodeType_Container=0,
		NodeType_Leaf,
		NodeType_Undefined
	};
protected:
	std::string g_name;
	NodeType g_type;
	bool g_save;
public:
	GADataNode();

	void setName(std::string name);
	void setType(NodeType type);
	void setSaveChild(bool save);
	NodeType getType();
	virtual void saveNodeAsString(std::string& str);
	virtual void loadStringData(GAStringVector::iterator& iter);
};

class GAExport GAUserDefinedStruct
{
public:
	GAUserDefinedStruct(){};
	virtual std::string saveStructAsString()=0;
	virtual void loadStringToStruct(std::string line)=0;
};


class GAExport GADataMap
{
	friend class GADataLeafNode;
private:
	GAStringMap* g_strmap;
	GAIntMap* g_intmap;
	GAFloatMap* g_floatmap;
	std::vector<void*> *g_structVec;
public:
	enum DataMapType
	{
		DataMapType_String=0,
		DataMapType_Int,
		DataMapType_Float,
		DataMapType_StructFloat2,
		DataMapType_StructFloat3,
		DataMapType_StructFloat4,
		DataMapType_UserDefinedStruct,
		DataMapType_Undefined
	};
	DataMapType g_type;
public:
	GADataMap(DataMapType type);

	bool addData(std::string name,std::string value);
	bool addData(std::string name,float value);
	bool addData(std::string name,int value);
	bool addData(GAFloat2Struct* float2Struct);
	bool addData(GAFloat3Struct* float3Struct);
	bool addData(GAFloat4Struct* float4Struct);
	bool addData(GAUserDefinedStruct* userDefinedStruct);

	float getFloatData(std::string name);
	int getIntData(std::string name);
	std::string getStringData(std::string name);
	std::vector<void*>* getStructVector();
	GAFloat2Struct* getStructFloat2Data(UINT No);
	GAFloat3Struct* getStructFloat3Data(UINT No);
	GAFloat4Struct* getStructFloat4Data(UINT No);
	void saveDataAsString(std::string& str);
	void loadStringData(GAStringVector::iterator& iter);
private:
	void init();
};

class GAExport GADataLeafNode : public GADataNode
{
private:
	GADataMap* g_dataMap;
public:
	GADataLeafNode();

	void initDataMap(GADataMap::DataMapType type);
	GADataMap* getDataMap();
	void saveNodeAsString(std::string &str);
	void loadStringData(GAStringVector::iterator& iter);
};

class GAExport GADataContainerNode : public GADataNode
{
private:
	typedef std::map<std::string,GADataNode*> DataNodeMap;
	DataNodeMap g_childNodeMap;
public:
	GADataContainerNode();

	GADataNode* createChildNode(std::string nodeName,NodeType type);
	GADataContainerNode* createChildContainerNode(std::string nodeName);
	GADataLeafNode* createChildLeafNode(std::string nodeName);

	GADataContainerNode* getChildContainerNode(std::string name);
	GADataLeafNode* getChildLeafNode(std::string name);
	GADataNode* getChildNode(UINT number);
	GADataNode* getChildNode(std::string name);
	int getChildNum();
	void saveNodeAsString(std::string &str);
	void loadStringData(GAStringVector::iterator& iter);
};


class GAPMainManager;

class GAExport GADataManager
{
private:
	GAPMainManager* g_mainMgr;
	GADataContainerNode g_rootDataNode;
public:
	GADataManager(GAPMainManager* mainMgr);
	void init();

	GADataContainerNode* getRootNode();
	bool saveDataTree(std::string filePath);
	bool loadDataTree(std::string filePath);
};
#endif