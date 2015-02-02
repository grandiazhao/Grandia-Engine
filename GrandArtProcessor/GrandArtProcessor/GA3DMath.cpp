#include "GA3DMath.h"

inline bool Delta(float x) {return ((x>DeltaMin||x<-DeltaMin)?false:true);}


//--------------------------------GAVector2----------------------------------------------
GAVector2::GAVector2()
{
	u=0;v=0;
}

GAVector2::GAVector2(float u_,float v_)
{
	u=u_;v=v_;
}

float GAVector2::dot(GAVector2 v2)
{
	return u*v2.u+v*v2.v;
}

//--------------------------------GAVector3----------------------------------------------

GAVector3::GAVector3()
{
	x=y=z=0;
}

GAVector3::GAVector3(float x_,float y_,float z_)
{
	x=x_;
	y=y_;
	z=z_;
}

GAVector3::GAVector3(Ogre::Vector3& vec)
{
	x=vec.x;
	y=vec.y;
	z=vec.z;
}

void GAVector3::setPosition(float x_,float y_,float z_)
{
	x=x_;
	y=y_;
	z=z_;
}

void GAVector3::setPosition(GAVector3& point)
{
	return setPosition(point.x,point.y,point.z);
}

void GAVector3::setPosition(Ogre::Vector3& point)
{
	return setPosition(point.x,point.y,point.z);
}

void GAVector3::setPosition(GAFloat3Struct* float3)
{
	return setPosition(float3->x,float3->y,float3->z);
}

void GAVector3::setTranslation(float x_,float y_,float z_)
{
	x+=x_;
	y+=y_;
	z+=z_;
}

void GAVector3::setTranslation(Ogre::Vector3& translate)
{
	return setPosition(translate.x,translate.y,translate.z);
}

void GAVector3::setTranslation(GAVector3& translate)
{
	return setPosition(translate.x,translate.y,translate.z);
}

float GAVector3::getSquaredLength()
{
	return x*x+y*y+z*z;
}

float GAVector3::getLength()
{
	return sqrt(getSquaredLength());
}

int GAVector3::normalise()
{
	if(!isZeroLength())
	{
		float length=getLength();
		x=x/length;
		y=y/length;
		z=z/length;
		return 0;
	}
	else
	{
		x=y=z=0;
		return -1;
	}
}
bool GAVector3::isZeroLength()
{
	return (Delta(getSquaredLength()*10000.f));
}
float GAVector3::dot(GAVector3 &p)
{
	return (x*p.x+y*p.y+z*p.z);
}
GAVector3 GAVector3::muti(float ratio)
{
	return GAVector3(x*ratio,y*ratio,z*ratio);
}
GAVector3 GAVector3::cross(GAVector3 &p)
{
	GAVector3 ret;
	ret.x=y*p.z-p.y*z;
	ret.y=z*p.x-x*p.z;
	ret.z=x*p.y-p.x*y;
	return ret;
}
GAVector3 GAVector3::add(GAVector3 &p)
{
	return GAVector3(x+p.x,y+p.y,z+p.z);
}
GAVector3 GAVector3::minus(GAVector3 &p)
{
	return GAVector3(x-p.x,y-p.y,z-p.z);
}
GAVector3 GAVector3::divide(float ratio)
{
	return GAVector3(x/ratio,y/ratio,z/ratio);
}

GAVector3 GAVector3::getUVWVector(GAVector3 uDir,GAVector3 vDir,GAVector3 wDir)
{
	return uDir.muti(x).add(vDir.muti(y).add(wDir.muti(z)));
}

//------------------------------------GAOrientation-----------------------------------------------

GAOrientation::GAOrientation()
{
	i=j=k=0;
	w=1;
}
GAOrientation::GAOrientation(GAVector3 axis,float degree)
{
	float c=cos(degree/360*3.1415926f);
	float s=sqrtf(1-c*c);
	w=c;
	axis.normalise();
	i=axis.x*s;
	j=axis.y*s;
	k=axis.z*s;
}
GAOrientation::GAOrientation(GAVector3 vec)
{
	w=0;
	i=vec.x;
	j=vec.y;
	k=vec.z;
}
GAOrientation::GAOrientation(float w_,float i_,float j_,float k_)
{
	w=w_;
	i=i_;
	j=j_;
	k=k_;
}
void GAOrientation::setIdentity()
{
	w=1;
	i=j=k=0;
}
GAOrientation GAOrientation::gong_e_orientation()
{
	return GAOrientation(w,-i,-j,-k);
}
GAOrientation GAOrientation::mutiply(GAOrientation ori2)
{
	GAOrientation ret;
	float d1,d2,d3,d4;
	d1=w*ori2.w;
	d2=-i*ori2.i;
	d3=-j*ori2.j;
	d4=-k*ori2.k;
	ret.w=d1+d2+d3+d4;

	d1=w*ori2.i;
	d2=i*ori2.w;
	d3=j*ori2.k;
	d4=-k*ori2.j;
	ret.i=d1+d2+d3+d4;

	d1=w*ori2.j;
	d2=j*ori2.w;
	d3=k*ori2.i;
	d4=-i*ori2.k;
	ret.j=d1+d2+d3+d4;

	d1=w*ori2.k;
	d2=k*ori2.w;
	d3=i*ori2.j;
	d4=-j*ori2.i;
	ret.k=d1+d2+d3+d4;
	return ret;
}

GAVector3 GAOrientation::tanslateVector(GAVector3 vec)
{
	GAOrientation gonge=gong_e_orientation();
	GAOrientation temp(vec);
	temp=mutiply(temp).mutiply(gonge);
	GAVector3 ret(temp.i,temp.j,temp.k);
	return ret;
}

void GAOrientation::pitch(float degree)
{
	GAOrientation axisx(GAVector3(1,0,0),degree);
	(*this)=axisx.mutiply(*this);
}
void GAOrientation::yaw(float degree)
{
	GAOrientation axisx(GAVector3(0,1,0),degree);
	(*this)=axisx.mutiply(*this);
}
void GAOrientation::roll(float degree)
{
	GAOrientation axisx(GAVector3(0,0,1),degree);
	(*this)=axisx.mutiply(*this);
}
void GAOrientation::setRotation(GAOrientation rotate)
{
	(*this)=rotate.mutiply(*this);
}