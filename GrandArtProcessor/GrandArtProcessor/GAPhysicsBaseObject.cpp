#include "GAPhysicsBaseObject.h"

//----------------------------------------------------------------------------
GASimpleObject::GASimpleObject()
{
}

void GASimpleObject::setTranslation(GAVector3 translate)
{
	origin.setTranslation(translate);
}

void GASimpleObject::setOrientation(GAOrientation rotate)
{
	orientation.setRotation(rotate);
}
GASPoint GASimpleObject::averagePoint(GAVector3List* plist)
{
	GASPoint point(0,0,0);
	for(unsigned int i=0;i<plist->size();i++)
	{
		point=point.add(*plist->at(i));
	}
	point=point.divide((float)plist->size());
	return point;
}
//----------------------------------------------------------------------------
GACurve::GACurve()
{
	pointList=new GAVector3List();
	curveType=CurveType_undefined;
}

void GACurve::setLineCurve(GAVector3 pstart,GAVector3 pend)
{
	curveType=CurveType_Line;
	startPoint=pstart;
	endPoint=pend;
}

float GACurve::getLength()
{
	if(curveType=CurveType_Line)
	{
		GAVector3 p=endPoint.minus(startPoint);
		return p.getLength();
	}
	return 0;
}

GAVector3 GACurve::getLineDirection(bool normalise)
{
	if(!normalise) return endPoint.minus(startPoint);
	else
	{
		GAVector3 p=endPoint.minus(startPoint);
		p.normalise();
		return p;
	}
}

void GACurve::update(int style)
{

}
//----------------------------------------------------------------------------

GASimplePlane::GASimplePlane()
{
	edgeList=new GACurveList();
	vertexList=new GAVector3List();
	uvwVertexList=new GAVector3List();
	edgeIndexList=new GAIndexLineCurveList();
	origin.setPosition(0,0,0);
	orientation=GAOrientation(1,0,0,0);
}

bool GASimplePlane::createPlane(GAVector3List* pointList)
{
	unsigned int k=pointList->size();
	if(k==3) type=PlaneType_Triangle;
	else if(k==4) type=PlaneType_Quadrilateral;
	else if(k>4) type=PlaneType_N_Sided;
	else return false;
	origin.setPosition(0,0,0);
	unsigned int i;
	for(i=0;i<k-1;i++)
	{
		GASPoint* p=new GASPoint(*pointList->at(i));
		origin=origin.add(*p);
		vertexList->push_back(p);
		GAIndexLineCurve *c=new GAIndexLineCurve(i,i+1);
		edgeIndexList->push_back(c);
	}
	{
		GASPoint* p=new GASPoint(*pointList->at(i));
		origin=origin.add(*p);
		vertexList->push_back(p);
		GAIndexLineCurve *c=new GAIndexLineCurve(i,0);
		edgeIndexList->push_back(c);
	}

	origin=origin.divide((float)(i+1));
	for(i=0;i<vertexList->size();i++)
	{
		uDir=vertexList->at(i)->minus(origin);
		if(!Delta(uDir.getSquaredLength())) break;
	}
	uDir.normalise();
	GAVector3 tempDir;
	for(;i<vertexList->size();i++)
	{
		tempDir=edgeIndexList->at(i)->getLineCurve(vertexList)->getLineDirection(true);
		if(!Delta(1-tempDir.dot(uDir))) break;
	}
	if(i==vertexList->size()) return false;
	tempDir=tempDir.cross(uDir);
	vDir=uDir.cross(tempDir);
	vDir.normalise();
	wDir=uDir.cross(vDir);
	wDir.normalise();
	for(i=0;i<k;i++)
	for(i=0;i<k;i++)
	{
		GAVector3 *v=new GAVector3();
		GASPoint p=vertexList->at(i)->minus(origin);
		v->x=p.dot(uDir);
		v->y=p.dot(vDir);
		v->z=0;
		uvwVertexList->push_back(v);
	}
	return true;
}

bool GASimplePlane::createPlane(GAVector3List* pointList,unsigned int originPointPos)
{
	unsigned int k=pointList->size();
	if(k==3) type=PlaneType_Triangle;
	else if(k==4) type=PlaneType_Quadrilateral;
	else if(k>4) type=PlaneType_N_Sided;
	else return false;
	if(originPointPos>=k) return false;
	
	unsigned int i;
	for(i=0;i<k-1;i++)
	{
		GASPoint* p=new GASPoint(*pointList->at(i));
		vertexList->push_back(p);
		GASPoint* p2=new GASPoint(*pointList->at(i+1));
		GACurve *c=new GACurve();
		c->setLineCurve(*p,*p2);
		edgeList->push_back(c);
	}
	{
		GASPoint* p=new GASPoint(*pointList->at(i));
		vertexList->push_back(p);
		GASPoint* p2=new GASPoint(*pointList->at(0));
		GACurve *c=new GACurve();
		c->setLineCurve(*p,*p2);
		edgeList->push_back(c);
	}

	origin=*pointList->at(originPointPos);
	uDir=edgeList->at(originPointPos)->getLineDirection(true);
	originPointPos++;
	if(originPointPos>=k) originPointPos-=k;
	GAVector3 tempDir=edgeList->at(originPointPos)->getLineDirection(true);
	if(Delta(tempDir.dot(uDir))) return false;
	tempDir=tempDir.cross(uDir);
	vDir=uDir.cross(tempDir);
	vDir.normalise();
	for(i=0;i<k;i++)
	{
		GAVector3 *v=new GAVector3();
		GASPoint p=vertexList->at(i)->minus(origin);
		v->x=p.dot(uDir);
		v->y=p.dot(vDir);
		v->z=0;
		uvwVertexList->push_back(v);
	}
	return true;
}
bool GASimplePlane::reCreatePlane(GAVector3List* pointList,GASPoint origin_,GAVector3 uDir_,GAVector3 vDir_)
{
	uDir=uDir_;
	vDir=vDir_;
	wDir=uDir.cross(vDir);
	wDir.normalise();
	origin=origin_;
	unsigned int size=uvwVertexList->size();
	for(unsigned int i=0;i<size;i++)
	{
		GAVector3 vec=pointList->at(i)->minus(origin);
		uvwVertexList->at(i)->x=uDir.dot(vec);
		uvwVertexList->at(i)->y=vDir.dot(vec);
		uvwVertexList->at(i)->z=wDir.dot(vec);
		vertexList->at(i)->setPosition(*pointList->at(i));
	}
	return true;
}
int GASimplePlane::isPointInPlane(GASPoint p)
{
	GAVector3 pdir=p.minus(origin);
	GAVector2 puv(pdir.dot(uDir),pdir.dot(vDir));
	unsigned int size_1=uvwVertexList->size()-1;
	unsigned int i;
	int count=0;
	GAVector3 uvw1,uvw2;
	float u1,u2,v1,v2,k,cv,u0,v0;
	for(i=0;i<size_1;i++)
	{
		uvw1=*uvwVertexList->at(i);
		uvw2=*uvwVertexList->at(i+1);
		u1=uvw1.x;
		u2=uvw2.x;
		v1=uvw1.y;
		v2=uvw2.y;
		u0=puv.u;
		v0=puv.v;
		if(Delta(u2-u1))
		{
			if(Delta(u0-u1)||Delta(u0-u2))
			{
				if((v0-v1)*(v0-v2)<0) return 2;
			}
			continue;
		}
		k=(u1-u0)*(u2-u0);
		if(Delta(k))
		{
			if(abs(u1-u0)<abs(u2-u0))
			{
				if(v2>v0) continue;
				cv=(v2-v1)/(u2-u1)*(u0-u1)+v1;
				if(Delta(cv-v0)) return 2;
				if(cv>v0) count++;
			}
			else
			{
				if(v1>v0) continue;
				cv=(v2-v1)/(u2-u1)*(u0-u1)+v1;
				if(Delta(cv-v0)) return 2;
				if(cv>v0) count++;
			}
		}
		else if(k<0)
		{
			cv=(v2-v1)/(u2-u1)*(u0-u1)+v1;
			if(Delta(cv-v0)) return 2;
			if(cv>v0) count++;
		}
		else
		{
			continue;
		}
	}
	{
		uvw1=*uvwVertexList->at(i);
		uvw2=*uvwVertexList->at(0);
		u1=uvw1.x;
		u2=uvw2.x;
		v1=uvw1.y;
		v2=uvw2.y;
		u0=puv.u;
		v0=puv.v;
		if(Delta(u2-u1))
		{
			if(Delta(u0-u1)||Delta(u0-u2))
			{
				if((v0-v1)*(v0-v2)<0) return 2;
			}
			if(count%2==0) return 0;
			else return 1;
		}
		k=(u1-u0)*(u2-u0);
		if(Delta(k))
		{
			if(abs(u1-u0)<abs(u2-u0))
			{
				if(v2>v0) ;
				else
				{	cv=(v2-v1)/(u2-u1)*(u0-u1)+v1;
					if(Delta(cv-v0)) return 2;
					if(cv>v0) count++;
				}
			}
			else
			{
				if(v1>v0) ;
				else
				{
					cv=(v2-v1)/(u2-u1)*(u0-u1)+v1;
					if(Delta(cv-v0)) return 2;
					if(cv>v0) count++;
				}
			}
		}
		else if(k<0)
		{
			cv=(u0-u1)/(u2-u1)*(v2-v1)+v1;
			if(Delta(cv-v0)) return 2;
			if(cv>v0) count++;
		}
	}
	if(count%2==0) return 0;
	else return 1;
}

int GASimplePlane::calculateLineIntersectWithThisPlane(GASPoint lineStart,GAVector3 lineEnd,GAVector3& intersectPoint)
{
	GAVector3 lineDir=lineEnd.minus(lineStart);
	float ll=lineDir.getLength();
	if(lineDir.normalise()==-1) return -1;
	GAVector3 oo=origin.minus(lineStart);
	//(o+au+bv)w=(o1+kd)w   ow=o1w+kdw; kdw=ow-o1w;
	float t1=lineDir.dot(wDir);
	if(Delta(t1)) return 0; //平行,没有交点（共面有无穷多交点，也认为没有交点）
	float k=oo.dot(wDir)/t1;
	if(k>=0-DeltaMin&&k<=ll+DeltaMin)
	{
		intersectPoint=lineStart.add(lineDir.muti(k));
		return 1;
	}
	return 0;
}
int GASimplePlane::calculateLineIntersectWithThisPlane(GALineCurve lineCurve,GAVector3& intersectPoint)
{
	return calculateLineIntersectWithThisPlane(lineCurve.startPoint,lineCurve.endPoint,intersectPoint);
}
int GASimplePlane::calculateLineIntersectWithThisPlaneInBorder(GALineCurve curve,GASPoint &intersectPoint)
{
	int k=calculateLineIntersectWithThisPlane(curve,intersectPoint);
	if(k!=1) return k;
	else
	{
		int j=isPointInPlane(intersectPoint);
		if(j==1||j==2) return 1;
	}
	return 0;
}
int GASimplePlane::calculateLineIntersectWithThisPlaneInBorder(GASPoint lineStart,GASPoint lineEnd,GASPoint &intersectPoint)
{
	int k=calculateLineIntersectWithThisPlane(lineStart,lineEnd,intersectPoint);
	if(k!=1) return k;
	else
	{
		int j=isPointInPlane(intersectPoint);
		if(j==1||j==2) return 1;
	}
	return 0;
}
float GASimplePlane::getULength(GASPoint p)
{
	GAVector3 vec=p.minus(origin);
	return vec.dot(uDir);
}
float GASimplePlane::getVLength(GASPoint p)
{
	GAVector3 vec=p.minus(origin);
	return vec.dot(vDir);
}
void GASimplePlane::getUVLength(GASPoint p,float &u,float &v)
{
	GAVector3 vec=p.minus(origin);
	u=vec.dot(uDir);
	v=vec.dot(vDir);
}

void GASimplePlane::update(int style)
{
	if(style==2) return;
	uDir=orientation.tanslateVector(uDir);
	uDir.normalise();
	vDir=orientation.tanslateVector(vDir);
	vDir.normalise();
	wDir=orientation.tanslateVector(wDir);
	wDir.normalise();
	orientation.setIdentity();
	if(style==1) return;
	unsigned int total=vertexList->size();
	for(unsigned int i=0;i<total;i++)
	{
		vertexList->at(i)->setPosition(uvwVertexList->at(i)->getUVWVector(uDir,vDir,wDir).add(origin));	
	}
}

GASimpleCube::GASimpleCube()
{
	uvwVertexList=new GAVector3List();
	vertexList=new GAVector3List(); 
	edgeIndexList=new GAIndexLineCurveList();
	faceList=new GASimplePlaneList();
}
bool GASimpleCube::createCube(float halfx,float halfy,float halfz)
{
	//从负x，负y，负z开始,逆时针方向
	GAVector3 *v1=new GAVector3(-halfx,-halfy,-halfz);
	GAVector3 *v2=new GAVector3(-halfx,-halfy,halfz);
	GAVector3 *v3=new GAVector3(halfx,-halfy,halfz);
	GAVector3 *v4=new GAVector3(halfx,-halfy,-halfz);
	GAVector3 *v5=new GAVector3(-halfx,halfy,-halfz);
	GAVector3 *v6=new GAVector3(-halfx,halfy,halfz);
	GAVector3 *v7=new GAVector3(halfx,halfy,halfz);
	GAVector3 *v8=new GAVector3(halfx,halfy,-halfz);
	vertexList->push_back(v1);
	vertexList->push_back(v2);
	vertexList->push_back(v3);
	vertexList->push_back(v4);
	vertexList->push_back(v5);
	vertexList->push_back(v6);
	vertexList->push_back(v7);
	vertexList->push_back(v8);

	//curve 从负x,负y,负z开始，按x，y，z，逆时针方向
	GAIndexLineCurve *c1=new GAIndexLineCurve(0,3);
	GAIndexLineCurve *c2=new GAIndexLineCurve(4,7);
	GAIndexLineCurve *c3=new GAIndexLineCurve(5,6);
	GAIndexLineCurve *c4=new GAIndexLineCurve(1,2);
	GAIndexLineCurve *c5=new GAIndexLineCurve(0,1);
	GAIndexLineCurve *c6=new GAIndexLineCurve(3,2);
	GAIndexLineCurve *c7=new GAIndexLineCurve(7,6);
	GAIndexLineCurve *c8=new GAIndexLineCurve(4,5);
	GAIndexLineCurve *c9=new GAIndexLineCurve(0,4);
	GAIndexLineCurve *c10=new GAIndexLineCurve(1,5);
	GAIndexLineCurve *c11=new GAIndexLineCurve(2,6);
	GAIndexLineCurve *c12=new GAIndexLineCurve(3,7);
	edgeIndexList->push_back(c1);
	edgeIndexList->push_back(c2);
	edgeIndexList->push_back(c3);
	edgeIndexList->push_back(c4);
	edgeIndexList->push_back(c5);
	edgeIndexList->push_back(c6);
	edgeIndexList->push_back(c7);
	edgeIndexList->push_back(c8);
	edgeIndexList->push_back(c9);
	edgeIndexList->push_back(c10);
	edgeIndexList->push_back(c11);
	edgeIndexList->push_back(c12);

	origin.setPosition(0,0,0);
	orientation.setIdentity();
	uDir=c1->getLineCurve(vertexList)->getLineDirection(true);
	wDir=c5->getLineCurve(vertexList)->getLineDirection(true);
	vDir=c9->getLineCurve(vertexList)->getLineDirection(true);
	worldUDir=uDir;
	worldVDir=vDir;
	worldWDir=wDir;

	for(unsigned int i=0;i<8;i++)
	{
		GAVector3* uvw=new GAVector3();
		GAVector3* point=vertexList->at(i);
		uvw->x=point->dot(uDir);
		uvw->y=point->dot(vDir);
		uvw->z=point->dot(wDir);
		uvwVertexList->push_back(uvw);
	}

	GAVector3List* plist=new GAVector3List();
	plist->push_back(vertexList->at(0));
	plist->push_back(vertexList->at(1));
	plist->push_back(vertexList->at(2));
	plist->push_back(vertexList->at(3));
	GASimplePlane *p1=new GASimplePlane();
	p1->createPlane(plist);
	faceList->push_back(p1);
	plist->clear();
	plist->push_back(vertexList->at(4));
	plist->push_back(vertexList->at(5));
	plist->push_back(vertexList->at(6));
	plist->push_back(vertexList->at(7));
	GASimplePlane *p2=new GASimplePlane();
	p2->createPlane(plist);
	faceList->push_back(p2);
	plist->clear();
	plist->push_back(vertexList->at(0));
	plist->push_back(vertexList->at(1));
	plist->push_back(vertexList->at(5));
	plist->push_back(vertexList->at(4));
	GASimplePlane *p3=new GASimplePlane();
	p3->createPlane(plist);
	faceList->push_back(p3);
	plist->clear();
	plist->push_back(vertexList->at(1));
	plist->push_back(vertexList->at(2));
	plist->push_back(vertexList->at(6));
	plist->push_back(vertexList->at(5));
	GASimplePlane *p4=new GASimplePlane();
	p4->createPlane(plist);
	faceList->push_back(p4);
	plist->clear();
	plist->push_back(vertexList->at(2));
	plist->push_back(vertexList->at(3));
	plist->push_back(vertexList->at(7));
	plist->push_back(vertexList->at(6));
	GASimplePlane *p5=new GASimplePlane();
	p5->createPlane(plist);
	faceList->push_back(p5);
	plist->clear();
	plist->push_back(vertexList->at(3));
	plist->push_back(vertexList->at(0));
	plist->push_back(vertexList->at(4));
	plist->push_back(vertexList->at(7));
	GASimplePlane *p6=new GASimplePlane();
	p6->createPlane(plist);
	faceList->push_back(p6);
	updateFaceList();
	delete plist;
	return true;
}

void GASimpleCube::updateFaceList()
{
	GAVector3List plist;
	plist.push_back(vertexList->at(0));
	plist.push_back(vertexList->at(1));
	plist.push_back(vertexList->at(2));
	plist.push_back(vertexList->at(3));
	GASPoint origin=averagePoint(&plist);
	faceList->at(0)->reCreatePlane(&plist,origin,uDir,wDir);
	plist.clear();
	plist.push_back(vertexList->at(4));
	plist.push_back(vertexList->at(5));
	plist.push_back(vertexList->at(6));
	plist.push_back(vertexList->at(7));
	origin=averagePoint(&plist);
	faceList->at(1)->reCreatePlane(&plist,origin,wDir,uDir);
	plist.clear();
	plist.push_back(vertexList->at(0));
	plist.push_back(vertexList->at(1));
	plist.push_back(vertexList->at(5));
	plist.push_back(vertexList->at(4));
	origin=averagePoint(&plist);
	faceList->at(2)->reCreatePlane(&plist,origin,wDir,vDir);
	plist.clear();
	plist.push_back(vertexList->at(1));
	plist.push_back(vertexList->at(2));
	plist.push_back(vertexList->at(6));
	plist.push_back(vertexList->at(5));
	origin=averagePoint(&plist);
	faceList->at(3)->reCreatePlane(&plist,origin,uDir,vDir);
	plist.clear();
	plist.push_back(vertexList->at(2));
	plist.push_back(vertexList->at(3));
	plist.push_back(vertexList->at(7));
	plist.push_back(vertexList->at(6));
	origin=averagePoint(&plist);
	faceList->at(4)->reCreatePlane(&plist,origin,vDir,wDir);
	plist.clear();
	plist.push_back(vertexList->at(3));
	plist.push_back(vertexList->at(0));
	plist.push_back(vertexList->at(4));
	plist.push_back(vertexList->at(7));
	origin=averagePoint(&plist);
	faceList->at(5)->reCreatePlane(&plist,origin,vDir,uDir);
	plist.clear();
}

int GASimpleCube::calculateLineIntersectWithThisCube(GASPoint pstart,GASPoint pend,GAVector3List* intersectList,bool sortByDistance)
{
	int count=0;
	for(int i=0;i<6;i++)
	{
		GASimplePlane *p=faceList->at(i);
		GAVector3* pinter=new GAVector3();
		int k=p->calculateLineIntersectWithThisPlaneInBorder(pstart,pend,*pinter);
		if(k==1) 
		{
			intersectList->push_back(pinter);
			count++;
		}
		else
		{
			delete pinter;
			if(k==-1) return k;
		}
	}
	if(sortByDistance)
	{
		//sort
	}
	if(count==0) return 0;
	return 1;
}
int GASimpleCube::calculateLineIntersectWithThisCube(GALineCurve lineCurve,GAVector3List* plist,bool sortByDistance)
{
	return calculateLineIntersectWithThisCube(lineCurve.startPoint,lineCurve.endPoint,plist,sortByDistance);
}
int GASimpleCube::calculatePlaneIntersectWithThisCube(GASimplePlane plane,GAVector3List* intersectList)
{
	int count=0;
	//先检测cube上的线是否碰了plane
	for(int i=0;i<12;i++)
	{
		GALineCurve *c=edgeIndexList->at(i)->getLineCurve(vertexList);
		GASPoint *interPoint=new GASPoint();
		int k=plane.calculateLineIntersectWithThisPlaneInBorder(*c,*interPoint);
		if(k==-1) return -1;
		else if(k==1)
		{
			intersectList->push_back(interPoint);
			count++;
		}
	}
	//检测plane上的线段是否碰到了cube的面
	unsigned int edgeNum=plane.edgeIndexList->size();
	for(unsigned int i=0;i<edgeNum;i++)
	{
		GALineCurve *c=plane.edgeIndexList->at(i)->getLineCurve(plane.vertexList);
		int k=calculateLineIntersectWithThisCube(*c,intersectList);
		if(k==-1) return -1;
	}
	if(count==0) return 0;
	return 1;
}
int GASimpleCube::calculateCubeIntersectWithThisCube(GASimpleCube* cube,GAVector3List* intersectList)
{
	int count=0;
	for(int i=0;i<12;i++)
	{
		GALineCurve *c=edgeIndexList->at(i)->getLineCurve(vertexList);
		int k=cube->calculateLineIntersectWithThisCube(*c,intersectList);
		if(k==-1) return -1;
		else if(k==1) 
		{
			count++;
		}
	}
	for(int i=0;i<12;i++)
	{
		GALineCurve *c=cube->edgeIndexList->at(i)->getLineCurve(cube->vertexList);
		int k=calculateLineIntersectWithThisCube(*c,intersectList);
		if(k==-1) return -1;
		else if(k==1)
		{
			count++;
		}
	}
	if(count==0) return 0;
	return 1;
}
void GASimpleCube::update(int style)
{
	if(style==3) return;
	if(style==2)
	{
		uDir=orientation.tanslateVector(uDir);
		uDir.normalise();
		vDir=orientation.tanslateVector(vDir);
		vDir.normalise();
		wDir=orientation.tanslateVector(wDir);
		wDir.normalise();
	}
	else
	{
		uDir=orientation.tanslateVector(worldUDir);
		uDir.normalise();
		vDir=orientation.tanslateVector(worldVDir);
		vDir.normalise();
		wDir=orientation.tanslateVector(worldWDir);
		wDir.normalise();
	}
	orientation.setIdentity();
	if(style==1) return;
	unsigned int total=vertexList->size();
	//vertexList->clear(); //内存要不足，这句不对
	for(unsigned int i=0;i<total;i++)
	{
		vertexList->at(i)->setPosition(uvwVertexList->at(i)->getUVWVector(uDir,vDir,wDir).add(origin));
	}
	updateFaceList();
}


GAPhysicsBaseObject::GAPhysicsBaseObject()
{
	g_node=0;
	g_entity=0;
	g_objectName="";
	g_plane=0;
	g_sphere=0;
	g_cylinder=0;
	g_cube=0;
	g_collideMask=0;
	g_moving=false;
	g_showPhysicsShape=true;
}

bool GAPhysicsBaseObject::createPhysicsObjectOnEntity(Ogre::SceneManager* sm,Ogre::Entity* entity,Ogre::Vector3 scaleRatio,int collideMask,ObjectType type_)
{
	GAPMisc misc;
	if(g_node!=0) sm->destroySceneNode(g_node);
	g_node=0;
	g_type=type_;
	g_collideMask=collideMask;
	
	g_node=sm->getRootSceneNode()->createChildSceneNode();
	Ogre::AxisAlignedBox box=entity->getWorldBoundingBox(true);
	Ogre::Vector3 size=box.getSize();
	Ogre::Vector3 pos=box.getCenter();
	g_moveNode=entity->getParentSceneNode();
	g_rotateNode=entity->getParentSceneNode();
	Ogre::Vector3 opos=g_moveNode->_getDerivedPosition();
	Ogre::Quaternion qua=g_rotateNode->_getDerivedOrientation();
	g_originTransfer=pos-opos;
	g_node->setPosition(pos);
	//g_node->setOrientation(qua);
	formerOri=g_rotateNode->getOrientation();
	g_objectName=entity->getName();
	if(g_type==ObjectType_Cube)
	{
		g_entity=sm->createEntity("cube.mesh");
		g_entity->setMaterialName("Examples/transparent/pink");
		g_cube=new GASimpleCube();
		g_cube->createCube(size.x/2*scaleRatio.x,size.y/2*scaleRatio.y,size.z/2*scaleRatio.z);
		g_cube->origin=GAVector3(pos.x,pos.y,pos.z);
		g_cube->orientation=GAOrientation(qua.w,qua.x,qua.y,qua.z);
		preUpdate();
	}
	else return false;
	g_node->attachObject(g_entity);
	misc.setEntityScaleByWorldLengthXYZ(g_entity,size.x,size.y,size.z,scaleRatio.x,scaleRatio.y,scaleRatio.z);
	return true;
}

bool GAPhysicsBaseObject::createPhysicsWallForWorld(GAVector3 origin,float halfx,float halfy,float halfz,GAOrientation ori)
{
	static int count=0;
	GAPMisc misc;
	g_objectName="wall_"+misc.ConvertToString(count);
	count++;
	g_node=0;
	g_originTransfer=Ogre::Vector3(0,0,0);
	g_type=ObjectType_Cube;
	g_cube=new GASimpleCube();
	g_cube->createCube(halfx,halfy,halfz);
	g_cube->origin=origin;
	g_cube->orientation=ori;
	updateCollisionShape(origin,ori);
	return true;
}

void GAPhysicsBaseObject::setCollideMask(int mask)
{
	g_collideMask=mask;
}
void GAPhysicsBaseObject::preUpdate()
{
	Ogre::Vector3 opos=g_moveNode->_getDerivedPosition()+g_originTransfer;
	Ogre::Quaternion oqua=g_rotateNode->_getDerivedOrientation();
	GAVector3 pos(opos.x,opos.y,opos.z);
	//GAOrientation qua(oqua.w,oqua.x,oqua.y,oqua.z);
	GAOrientation qua(1,0,0,0);
	updateCollisionShape(pos,qua);
};
void GAPhysicsBaseObject::update(int type)
{
	if(type==0)
	{
		g_node->setPosition(g_moveNode->_getDerivedPosition()+g_originTransfer);
		//g_node->setOrientation(g_rotateNode->_getDerivedOrientation());
		formerOri=g_rotateNode->getOrientation();
	}
	else if(type==1)
	{
		g_moveNode->setPosition(g_node->getPosition()-g_originTransfer);
		g_rotateNode->setOrientation(formerOri);
		Ogre::Vector3 opos=g_node->getPosition();
		Ogre::Quaternion oqua=g_node->getOrientation();
		GAVector3 pos(opos.x,opos.y,opos.z);
		GAOrientation qua(oqua.w,oqua.x,oqua.y,oqua.z);
		updateCollisionShape(pos,qua);
	}	
}
void GAPhysicsBaseObject::updateCollisionShape(GAVector3 pos,GAOrientation qua)
{
	if(g_type==ObjectType_Cube)
	{
		g_cube->origin=pos;
		g_cube->orientation=qua;
		g_cube->update();
	}
	updateBoundingBox();
}
void GAPhysicsBaseObject::showPhysicsShape(bool show)
{
	if(g_node==0) return;
	if(show)
	{
		g_showPhysicsShape=true;
		g_node->setVisible(true);
	}
	else
	{
		g_showPhysicsShape=false;
		g_node->setVisible(false);
	}
}

void GAPhysicsBaseObject::changeVisibleType()
{
	if(g_node==0) return;
	if(g_showPhysicsShape)
	{
		g_showPhysicsShape=false;
		g_node->setVisible(false);
	}
	else
	{
		g_showPhysicsShape=true;
		g_node->setVisible(true);
	}
}

void GAPhysicsBaseObject::disableEntity()
{
	if(g_node!=0)
	{
		g_node->detachAllObjects();
	}
}

void GAPhysicsBaseObject::enableEntity()
{
	if(g_node!=0)
	{
		g_node->attachObject(g_entity);
	}
}

int GAPhysicsBaseObject::testCollideWith(GAPhysicsBaseObject* obj,GAVector3 &collidePoint)
{
	if(obj->g_collideMask==-1||g_collideMask==-1) return 0;
	bool test=false;
	if(obj->g_collideMask==0||g_collideMask==0) test=true;
	else if(obj->g_collideMask==g_collideMask) test=true;
	if(!test) return 0;

	if(g_type==ObjectType_Cube)
	{
		if(obj->g_type==ObjectType_Cube)
		{
			GAVector3List *plist=new GAVector3List();
			int k=g_cube->calculateCubeIntersectWithThisCube(obj->g_cube,plist);
			if(k==1)
			{
				k++;
				k--;
				collidePoint=GAVector3(0,0,0);
			}
			UINT size=plist->size();
			for(unsigned int i=0;i<size;i++)
			{
				GAVector3 *vecptr=plist->at(i);
				collidePoint=collidePoint.add(*vecptr);
				delete vecptr;
			}
			collidePoint=collidePoint.divide((float)size);
			delete plist;
			return k;
		}
	}
	return 0;
}

void GAPhysicsBaseObject::destroy()
{
	if(g_cube!=0) delete g_cube;
	g_node->detachAllObjects();
	g_node->getParentSceneNode()->removeAndDestroyChild(g_node->getName());
}

void GAPhysicsBaseObject::updateBoundingBox()
{
	if(g_type==ObjectType_Cube)
	{
		GAVector3List* list=g_cube->vertexList;
		if(list==0) return;
		unsigned int size=list->size();
		if(size==0) return;
		GAVector3 vec=*list->at(0);
		g_boundingBox.maxx=vec.x;
		g_boundingBox.maxy=vec.y;
		g_boundingBox.maxz=vec.z;
		g_boundingBox.minx=vec.x;
		g_boundingBox.miny=vec.y;
		g_boundingBox.minz=vec.z;
		for(unsigned int i=1;i<size;i++)
		{
			vec=*list->at(i);
			if(vec.x>g_boundingBox.maxx) g_boundingBox.maxx=vec.x;
			else if(vec.x<g_boundingBox.minx) g_boundingBox.minx=vec.x;
			if(vec.y>g_boundingBox.maxy) g_boundingBox.maxy=vec.y;
			else if(vec.y<g_boundingBox.miny) g_boundingBox.miny=vec.y;
			if(vec.z>g_boundingBox.maxz) g_boundingBox.maxz=vec.z;
			else if(vec.z<g_boundingBox.minz) g_boundingBox.minz=vec.z;
		}
	}
}