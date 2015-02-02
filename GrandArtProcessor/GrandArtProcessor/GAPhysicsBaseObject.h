#ifndef GASPHYSICSBASEOBJECT_H
#define GASPHYSICSBASEOBJECT_H

#include "Common.h"
#include "Ogre.h"
#include "GAPMisc.h"
#include "GA3DMath.h"

class GAExport GASimpleObject
{
public:
	GAVector3 origin;
	GAVector3 uDir;
	GAVector3 vDir;
	GAVector3 wDir;
	GAVector3 worldUDir,worldVDir,worldWDir;
	GAOrientation orientation,worldOrientation;
	//GAMatrix3x3 oriMatrix;
public:
	GASimpleObject();
	virtual void update(int style=0)=0;
	void setTranslation(GAVector3 translate);
	void setOrientation(GAOrientation rotate);
	GASPoint averagePoint(GAVector3List* pList);
};

class GAExport GACurve : public GASimpleObject
{
public:
	enum CurveType
	{
		CurveType_Line=0,
		CurveType_undefined
	};
	GAVector3List *pointList;
	CurveType curveType;
	GAVector3 startPoint;
	GAVector3 endPoint;
public:
	GACurve();
	void setLineCurve(GAVector3 startPoint,GAVector3 endPoint);
	float getLength();
	GAVector3 getLineDirection(bool normalise=false);
	void update(int style=0);
};

class GAExport GALineCurve : public GACurve
{
public:
	GALineCurve()
	{
		curveType=CurveType_Line;
		startPoint=endPoint=GAVector3(0,0,0);
	}
	GALineCurve(GAVector3 startPoint_,GAVector3 endPoint_)
	{
		startPoint=startPoint_;
		endPoint=endPoint_;
		curveType=CurveType_Line;
	}
};
class GAExport GAIndexLineCurve
{
public:
	GAVector3* origin;
	GAVector3* endPoint;
	unsigned int oIndex;
	unsigned int eIndex;
	GALineCurve* lineCurve;
public:
	GAIndexLineCurve()
	{
		origin=0;
		endPoint=0;
		oIndex=eIndex=0;
		lineCurve=new GALineCurve();
	}
	GAIndexLineCurve(int oIndex_,int eIndex_)
	{
		origin=0;
		endPoint=0;
		setIndex(oIndex_,eIndex_);
		lineCurve=new GALineCurve();
	}
	void setIndex(int oIndex_,int eIndex_)
	{
		oIndex=oIndex_;
		eIndex=eIndex_;
	}
	void setPoint(GAVector3 *origin_,GAVector3* endPoint_)
	{
		origin=origin_;
		endPoint=endPoint_;
	}
	GALineCurve* getLineCurve(GAVector3List* vertexList)
	{
		if(oIndex==eIndex) return 0;
		if(oIndex<vertexList->size()&&eIndex<vertexList->size())
		{
			lineCurve->setLineCurve(*vertexList->at(oIndex),*vertexList->at(eIndex));
			return lineCurve;
		}
		return 0;
	}
};
typedef std::vector<GAIndexLineCurve*> GAIndexLineCurveList;

class GAExport GARay : public GASimpleObject
{
public:
	GARay(){};
	void update(int style=0){};
};

typedef std::vector<GACurve*> GACurveList;

class GAExport GASimplePlane : public GASimpleObject
{
public:
	enum PlaneType
	{
		PlaneType_Triangle=0,
		PlaneType_Quadrilateral,
		PlaneType_N_Sided
	};
	GACurveList* edgeList;
	GAIndexLineCurveList* edgeIndexList;
	GAVector3List* vertexList;
	GAVector3List* uvwVertexList;
	PlaneType type;
public:
	GASimplePlane();
	bool createPlane(GAVector3List* pointList);//以Point的平均点为origin
	bool createPlane(GASPoint p1,GASPoint p2,GASPoint p3,GASPoint p4);
	bool createPlane(GAVector3List* pointList,GAVector3 uDirection);
	bool createPlane(GAVector3List* pointList,unsigned int originPointPos);
	bool createPlane(GASPoint origin,GAVector3 uDir,GAVector3 vDir,GACurveList* curveList);
	bool reCreatePlane(GAVector3List* pointList,GASPoint origin,GAVector3 uDir,GAVector3 vDir);
	bool createRPlane(GAVector3 outerDir,GASPoint origin,GAVector3 uDir,GAVector2 minUV,GAVector2 maxUV);
	int isPointInPlane(GASPoint p); //in=1,on=2,out=0,error=-1;
	int calculateLineIntersectWithThisPlane(GALineCurve curve,GASPoint &intersectPoint);
	int calculateLineIntersectWithThisPlane(GASPoint lineStart,GASPoint lineEnd,GASPoint &intersectPoint);//intersect=1;no=0;error=-1
	int calculateLineIntersectWithThisPlaneInBorder(GALineCurve curve,GASPoint &intersectPoint);
	int calculateLineIntersectWithThisPlaneInBorder(GASPoint lineStart,GASPoint lineEnd,GASPoint &intersectPoint);
	float getULength(GASPoint p);
	float getVLength(GASPoint p);
	void getUVLength(GASPoint p,float &u,float &v);
	void update(int style=0);
};
typedef std::vector<GASimplePlane*> GASimplePlaneList;

class GAExport GASimpleCube : public GASimpleObject
{
public:
	GAIndexLineCurveList* edgeIndexList;
	GAVector3List *uvwVertexList;
	GAVector3List *vertexList;
	GASimplePlaneList* faceList;
	struct CubePlane
	{
		GAVector3 uDir;
		GAVector3 vDir;
		GAVector3 wDir;
		GASPoint origin;
		GAVector3List* uvwVertexList;
	};
public:
	GASimpleCube();
	bool createCube(float halfx,float halfy,float halfz);
	bool createCube(GASimplePlane bottom,float height);

	int calculateLineIntersectWithThisCube(GALineCurve lineCurve,GAVector3List* intersectList,bool sortByDistance=true);
	int calculateLineIntersectWithThisCube(GASPoint pstart,GASPoint pend,GAVector3List* intersectList,bool sortByDistance=true);
	int calculatePlaneIntersectWithThisCube(GASimplePlane plane,GAVector3List* intersectList);
	int calculateCubeIntersectWithThisCube(GASimpleCube* cube,GAVector3List* intersectList);
	void update(int style=0);

private:
	void updateFaceList();
};
class GAExport GASimpleSphere : public GASimpleObject
{
public:
	GASimpleSphere(){};
	void update(int style=0){};
};
class GAExport GASimpleCylinder : public GASimpleObject
{
public:
	GASimpleCylinder(){};
	void update(int style=0){};
};

class GAExport GAPhysicsBaseObject
{
public:
	enum ObjectType
	{
		ObjectType_Cube=0,
		ObjectType_Sphere,
		ObjectType_Cylinder,
		ObjectType_Plane
	};
	struct BoundingBox
	{
		float maxx,maxy,maxz;
		float minx,miny,minz;
	};
	Ogre::Entity* g_entity;
	Ogre::SceneNode *g_node,*g_moveNode,*g_rotateNode;
	GASimplePlane* g_plane;
	GASimpleCube* g_cube;
	GASimpleCylinder* g_cylinder;
	GASimpleSphere* g_sphere;
	bool g_showPhysicsShape;
	bool g_moving;
	std::string g_objectName;
	ObjectType g_type;
	Ogre::Vector3 g_originTransfer;
	int g_collideMask;
	BoundingBox g_boundingBox;
	Ogre::Vector3 formerPos;
	Ogre::Quaternion formerOri;
public:
	GAPhysicsBaseObject();
	bool createPhysicsObjectOnEntity(Ogre::SceneManager *sm,Ogre::Entity* entity,Ogre::Vector3 scale=Ogre::Vector3(1,1,1),int collideMask=0,ObjectType type_=ObjectType_Cube);
	bool createPhysicsWallForWorld(GAVector3 origin,float halfx,float halfy,float halfz,GAOrientation ori=GAOrientation(1,0,0,0));
	void setCollideMask(int mask);

	int testCollideWith(GAPhysicsBaseObject* obj,GAVector3 &collidePoint);
	void updateCollisionShape(GAVector3 pos,GAOrientation qua);
	
	void showPhysicsShape(bool show);
	void changeVisibleType();

	void disableEntity();
	void enableEntity();
	
	void preUpdate();
	void update(int type=0);
	
	void destroy();
private:
	void updateBoundingBox();
};

#endif