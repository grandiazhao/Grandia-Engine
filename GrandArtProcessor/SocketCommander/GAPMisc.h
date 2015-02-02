#ifndef GAPMISC_H
#define GAPMISC_H

#include <string>
#include <tchar.h>
#include <WinNls.h>
#include <WinNT.h>
#include <sstream>

class GAPMisc
{
public:
	GAPMisc(){}
	std::string WChar2Ansi(LPCWSTR pwszSrc)
	{
		int nLen=WideCharToMultiByte(CP_ACP,0,pwszSrc,-1,NULL,0,NULL,NULL);
		if(nLen<=0) return std::string("");
		char* pszDst=new char[nLen];
		if(NULL==pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP,0,pwszSrc,-1,pszDst,nLen,NULL,NULL);
		pszDst[nLen-1]=0;
		std::string strTemp(pszDst);
		delete[] pszDst;
		return strTemp;
	}
	wchar_t* Ansi2WChar(const char* ansiString)
	{
		int nLen=MultiByteToWideChar(CP_ACP,0,ansiString, -1, NULL, 0);
		if(nLen<0)
		{
			return 0;
		}
		wchar_t* wc=new wchar_t[nLen];
		MultiByteToWideChar (CP_ACP,0,ansiString,-1,wc,nLen);  
		wc[nLen-1]=0;
		return wc;
	}
	
	template <class T> 
	std::string ConvertToString(T value)
	{
		std::stringstream ss;
	  ss << value;
	  return ss.str();
	}

	int ConvertToInt(std::string str)
	{
		return atoi(str.c_str());
	}

	float ConvertToFloat(std::string str)
	{
		return (float)atof(str.c_str());
	}

	std::string stringTrip(std::string str)
	{
		while((str.find_first_of(" ")==0||str.find_first_of("\t")==0)&&str.length()!=0)
		{
			str=str.substr(1,str.length()-1);
		}
		while((str.find_last_of(" ")==str.length()-1||str.find_last_of("\t")==str.length()-1)&&str.length()!=0)
		{
			str=str.substr(0,str.length()-2);
		}
		return str;
	}

	bool checkTwoLineSegmentCross(float p1x,float p1y,float p2x,float p2y,float p3x,float p3y,float p4x,float p4y,float& ratio)
	{
		float bx=p4x-p3x;
		float by=p4y-p3y;
		float cx=p1x-p3x;
		float cy=p1y-p3y;

		float dx=p1y-p2y;
		float dy=p2x-p1x;
		float x=bx*dx+by*dy;
		if(x>-0.0001&&x<0.0001) return false;
		cy=cx*dx+cy*dy;
		cy=cy/x;
		if(cy>0&&cy<1)
		{
			ratio=cy;
			x=p3x+cy*bx;
			float y=(p1x-x)*(p2x-x);
			if(y<0) return true;
			else if(y>0) return false;
			else
			{
				y=p3y+cy*by;
				x=(p1y-y)*(p2y-y);
				if(x<0) return true;
				else return false;
			}
		}
		else return false;
	}
};
#endif