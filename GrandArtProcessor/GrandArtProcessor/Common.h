#ifndef COMMON_H
#define COMMON_H

#include "Ogre.h"
#include "GAPMisc.h"


//#define GAExport __declspec( dllexport )
//#define GAExport __declspec( dllimport )
#define GAExport

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

typedef std::map<std::string,std::string> GAStringMap;
typedef std::map<std::string,int> GAIntMap;
typedef std::map<std::string,float> GAFloatMap;
typedef std::vector<std::string> GAStringVector;

struct GAFloat2Struct
{
	float x,y;
};

struct GAFloat3Struct
{
	float x,y,z;
};

struct GAFloat4Struct
{
	float x,y,z,w;
};
#endif