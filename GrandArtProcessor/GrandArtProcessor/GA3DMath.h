#ifndef GAS3DMATH_H
#define GAS3DMATH_H

#include "Ogre.h"
#include "Common.h"

#define DeltaMin 0.00001
inline bool Delta(float x);

class GAVector2;
class GAVector3;
class GAOrientation;
class GAMatrix3x3;
class GAMatrix4x4;

class GAVector2
{
public:
	float u,v;
	GAVector2();
	GAVector2(float u_,float v_);
	float dot(GAVector2 v2);
};
typedef std::vector<GAVector2*> GAVector2List;
class GAVector3
{
public:
	float x,y,z;
public:
	GAVector3();
	GAVector3(float x_,float y_,float z_);
	GAVector3(Ogre::Vector3& vec);

	void setPosition(float x_,float y_,float z_);
	void setPosition(GAVector3& point);
	void setPosition(Ogre::Vector3& point);
	void setPosition(GAFloat3Struct* float3);
	void setTranslation(float x_,float y_,float z_);
	void setTranslation(GAVector3& translate);
	void setTranslation(Ogre::Vector3& translate);

	float getSquaredLength();
	float getLength();
	int normalise();
	bool isZeroLength();

	float dot(GAVector3 &p);
	GAVector3 muti(float ratio);
	GAVector3 cross(GAVector3 &p);
	GAVector3 add(GAVector3 &p);
	GAVector3 minus(GAVector3 &p);
	GAVector3 divide(float ratio); 

	GAVector3 getUVWVector(GAVector3 uDir,GAVector3 vDir,GAVector3 wDir);
};
typedef std::vector<GAVector3*> GAVector3List;
typedef GAVector3 GASPoint;

class GAOrientation
{
public:
	float i,j,k,w;
public:
	GAOrientation();
	GAOrientation(GAVector3 axis,float degree);
	GAOrientation(GAVector3 vec);
	GAOrientation(float w_,float i_,float j_,float k_);

	void setIdentity();
	GAOrientation gong_e_orientation();
	GAOrientation mutiply(GAOrientation ori2);
	GAVector3 tanslateVector(GAVector3 vec);

	void pitch(float degree);
	void yaw(float degree);
	void roll(float degree);
	void setRotation(GAOrientation rotate);
};

class GAMatrix3x3
{
private:
	
public:
	float matrix[3][3];
public:
	GAMatrix3x3()
	{
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				matrix[i][j]=0;
		matrix[0][0]=1;
		matrix[1][1]=1;
		matrix[2][2]=1;
	}

	void zeroMatrix()
	{
		memset(matrix,0,sizeof(matrix));
	}
	void pitch(float degree)
	{
		GAMatrix3x3 mat;
		float r=degree/180*3.1415926f;
		float c=cos(r);
		//float s=sin(r);
		float s=sqrt(1-c*c);
		mat.matrix[1][1]=c;
		mat.matrix[2][2]=c;
		mat.matrix[1][2]=s;
		mat.matrix[2][1]=-s;
		(*this)=mutiply(mat);
	}
	void yaw(float degree)
	{
		GAMatrix3x3 mat;
		float r=degree/180*3.1415926f;
		float c=cos(r);
		float s=sqrt(1-c*c);
		mat.matrix[0][0]=c;
		mat.matrix[2][2]=c;
		mat.matrix[0][2]=-s;
		mat.matrix[2][0]=s;
		(*this)=mutiply(mat);
	}
	void roll(float degree)
	{
		GAMatrix3x3 mat;
		float r=degree/180*3.1415926f;
		float c=cos(r);
		float s=sqrt(1-c*c);
		mat.matrix[0][0]=c;
		mat.matrix[1][1]=c;
		mat.matrix[0][1]=s;
		mat.matrix[1][0]=-s;
		(*this)=mutiply(mat);
	}
	void setMatrix3x3(float* num)
	{
		int k=0;
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
			{
				matrix[i][j]=num[k];
				k++;
			}
	}
	GAVector3 translateVector(GAVector3 vec)
	{
		GAVector3 ret;
		ret.x=vec.x*matrix[0][0]+vec.y*matrix[1][0]+vec.z*matrix[2][0];
		ret.y=vec.x*matrix[0][1]+vec.y*matrix[1][1]+vec.z*matrix[2][1];
		ret.z=vec.x*matrix[0][2]+vec.y*matrix[1][2]+vec.z*matrix[2][2];
		return ret;
	}
	GAMatrix3x3 mutiply(GAMatrix3x3 mat2)
	{
		GAMatrix3x3 ret;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				ret.matrix[i][j]=matrix[i][0]*mat2.matrix[0][j]+matrix[i][1]*mat2.matrix[1][j]+
					matrix[i][2]*mat2.matrix[2][j]+matrix[i][3]*mat2.matrix[3][j];
			}
		}
		return ret;
	}
};

class GAMatrix4x4
{
public:
	float matrix[4][4];
public:
	GAMatrix4x4();
};

#endif