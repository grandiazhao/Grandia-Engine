#include "GAPPhysicsManager.h"
#include "GAPMainManager.h"
#include "GABspLoader.h"
#include "GABspConverter.h"

GAPPhysicsManager::GAPPhysicsManager(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
}
void GAPPhysicsManager::init()
{

}

bool GAPPhysicsManager::setBspWorld(std::string filePath)
{
	void* memoryBuffer = 0;
	FILE* file = fopen(filePath.c_str(),"r");
	if (file)
	{
		BspLoader bspLoader;
		int size=0;
		if (fseek(file, 0, SEEK_END) || (size = ftell(file)) == EOF || fseek(file, 0, SEEK_SET)) {
			return false;
		} 
		else
		{
			memoryBuffer = malloc(size+1);
			fread(memoryBuffer,1,size,file);
			bspLoader.loadBSPFile( memoryBuffer);

			BspConverter bsp2bullet(0);
			float bspScaling = 1.f;
			bsp2bullet.convertBsp(bspLoader,bspScaling);
			UINT brushNum=bsp2bullet.vector3SetSet.size();
			GAVector3 origin;
			float halfx,halfy,halfz;
			std::vector<GAVector3>* vec;
			for(UINT i=0;i<brushNum;i++)
			{
				vec=bsp2bullet.vector3SetSet[i];
				origin=vec->at(0);
				for(UINT j=1;j<8;j++)
				{
					origin=origin.add(vec->at(j));
				}
				origin=origin.divide(8);
				halfx=vec->at(4).x-vec->at(0).x;
				halfy=vec->at(2).y-vec->at(0).y;
				halfz=vec->at(1).z-vec->at(0).z;
				halfx/=2;halfy/=2;halfz/=2;
				if(halfx<0) halfx=-halfx;
				if(halfy<0) halfy=-halfy;
				if(halfz<0) halfz=-halfz;
				GAPhysicsBaseObject* obj=new GAPhysicsBaseObject();
				if(!obj->createPhysicsWallForWorld(origin,halfx,halfy,halfz)) return false;
				g_physicsObjSet.push_back(obj);
				g_collideSimbolSet.push_back(-1);
				insertDetectCell(obj);
			}
		}
		fclose(file);
	}
	return true;
}

void GAPPhysicsManager::update()
{
	testCollide();
	dispatchCollideResult();
}
void GAPPhysicsManager::update(Ogre::SceneNode* node)
{
	if(node!=0) testAndDispatchCollideWithSingleObject(node->getName());
}
bool GAPPhysicsManager::createPhysicsObject(Ogre::Entity* entity,GAPhysicsBaseObject::ObjectType type,Ogre::Vector3 ratio)
{
	if(checkEntityName(entity->getName())) return false;
	GAPhysicsBaseObject* obj=new GAPhysicsBaseObject();
	if(!obj->createPhysicsObjectOnEntity(g_mainMgr->getCurrentSceneMgr(),entity,ratio,type)) return false;
	g_physicsObjSet.push_back(obj);
	g_collideSimbolSet.push_back(-1);
	insertDetectCell(obj);
	return true;
}

bool GAPPhysicsManager::checkEntityName(std::string name)
{
	for(unsigned int i=0;i<g_physicsObjSet.size();i++)
	{
		if(g_physicsObjSet[i]->g_objectName==name) return true;
	}
	return false;
}
GAPhysicsBaseObject* GAPPhysicsManager::getPhysicsObject(std::string name)
{
	for(unsigned int i=0;i<g_physicsObjSet.size();i++)
	{
		if(g_physicsObjSet[i]->g_objectName==name) return g_physicsObjSet[i];
	}
	return 0;
}
GAPhysicsBaseObject* GAPPhysicsManager::getPhysicsObject(std::string name,int& count)
{
	for(unsigned int i=0;i<g_physicsObjSet.size();i++)
	{
		if(g_physicsObjSet[i]->g_objectName==name)
		{
			count=i;
			return g_physicsObjSet[i];
		}
	}
	count=-1;
	return 0;
}
void GAPPhysicsManager::showPhysicsObject(std::string name,bool isShow)
{
	GAPhysicsBaseObject* obj=getPhysicsObject(name);
	if(obj!=0)
	{
		obj->showPhysicsShape(isShow);
	}
}

void GAPPhysicsManager::changeObjectVisibleType(std::string name)
{
	GAPhysicsBaseObject* obj=getPhysicsObject(name);
	if(obj!=0)
	{
		obj->changeVisibleType();
	}
}

void GAPPhysicsManager::disableAllphysicsObj()
{
	for(unsigned int i=0;i<g_physicsObjSet.size();i++)
	{
		g_physicsObjSet[i]->disableEntity();
	}
}

void GAPPhysicsManager::enableAllphysicsObj()
{
	for(unsigned int i=0;i<g_physicsObjSet.size();i++)
	{
		g_physicsObjSet[i]->enableEntity();
	}
}
void GAPPhysicsManager::testAndDispatchCollideWithSingleObject(std::string name)
{
	int num;
	GAPhysicsBaseObject* obj=getPhysicsObject(name,num);
	if(obj==0) return;
	obj->preUpdate();
	updateDetectCell(num);
	unsigned int size=g_xDistanceSet.size();
	for(unsigned int i=0;i<size;i++)
	{
		if(i!=num)
		{
			if(g_xDistanceSet[num].start<g_xDistanceSet[i].end&&g_xDistanceSet[num].end>g_xDistanceSet[i].start)
			{
				if(g_yDistanceSet[num].start<g_yDistanceSet[i].end&&g_yDistanceSet[num].end>g_yDistanceSet[i].start)
				{
					if(g_zDistanceSet[num].start<g_zDistanceSet[i].end&&g_zDistanceSet[num].end>g_zDistanceSet[i].start)
					{
						GAVector3 collidePoint;
						int k=obj->testCollideWith(g_physicsObjSet[i],collidePoint);
						if(k==1)
						{
							rePushObject(obj,collidePoint);
							dispatchCollideResult(obj,1);
							return;
						}
					}
				}
			}
		}
	}
	dispatchCollideResult(obj,0);
}
void GAPPhysicsManager::rePushObject(GAPhysicsBaseObject* obj,GAVector3 collidePoint)
{
	if(obj==0) return;
	Ogre::Vector3 ov1=obj->g_node->_getDerivedPosition();
	GAVector3 gv1(ov1.x,ov1.y,ov1.z);
	gv1=gv1.minus(collidePoint);
	gv1.z=0;
	gv1.normalise();
	if(gv1.isZeroLength()) return;
	obj->g_node->translate(gv1.x,gv1.y,gv1.z,Ogre::Node::TS_WORLD);
}
void GAPPhysicsManager::dispatchCollideResult(GAPhysicsBaseObject* obj,int type)
{
	if(obj==0) return;
	obj->update(type);
}
void GAPPhysicsManager::testCollide()
{
	unsigned int size=g_collideSimbolSet.size();
	unsigned int i;
	for(i=0;i<size;i++)
	{
		g_physicsObjSet[i]->preUpdate();
	}
	updateDetectCell();
	useDetectCellGenerateSimbol();
	for(i=0;i<size;i++)//简单算法，每一个物体都和后面所有物体测试碰撞
	{
		if(g_collideSimbolSet[i]==-1)
		{
			g_collideSimbolSet[i]=0;
			for(unsigned int j=i+1;j<size;j++)
			{
				GAVector3 collidePoint;
				int k=g_physicsObjSet[i]->testCollideWith(g_physicsObjSet[j],collidePoint);
				if(k==1)
				{
					g_collideSimbolSet[i]=1;
					g_collideSimbolSet[j]=1;
					break;
				}
			}
		}
	}
}

void GAPPhysicsManager::dispatchCollideResult()
{
	unsigned int size=g_collideSimbolSet.size();
	unsigned int i;
	for(i=0;i<size;i++)
	{
		g_physicsObjSet[i]->update(g_collideSimbolSet[i]);
	}
}

void GAPPhysicsManager::deletePhysicsObject(std::string name)
{
	PhysicsObjSet::iterator iter=g_physicsObjSet.begin();
	CollideSimbolSet::iterator siter=g_collideSimbolSet.begin();
	for(;iter!=g_physicsObjSet.end();iter++)
	{
		if((*iter)->g_objectName==name)
		{
			GAPhysicsBaseObject* obj=(*iter);
			deleteDetectCell(obj);
			g_physicsObjSet.erase(iter);
			g_collideSimbolSet.erase(siter);
			obj->destroy();
			return;
		}
		siter++;
	}
}

void GAPPhysicsManager::insertDetectCell(GAPhysicsBaseObject* obj)
{
	DetectCell *cellx=new DetectCell();
	cellx->obj=obj;
	cellx->start=obj->g_boundingBox.minx;
	cellx->end=obj->g_boundingBox.maxx;
	cellx->length=cellx->end-cellx->start;
	g_xDistanceSet.push_back(*cellx);
	DetectCell *celly=new DetectCell();
	celly->obj=obj;
	celly->start=obj->g_boundingBox.miny;
	celly->end=obj->g_boundingBox.maxy;
	celly->length=celly->end-celly->start;
	g_yDistanceSet.push_back(*celly);
	DetectCell *cellz=new DetectCell();
	cellz->obj=obj;
	cellz->start=obj->g_boundingBox.minz;
	cellz->end=obj->g_boundingBox.maxz;
	cellz->length=cellz->end-cellz->start;
	g_zDistanceSet.push_back(*cellz);
}
void GAPPhysicsManager::updateDetectCell(int cellNo)
{
	unsigned int size=g_xDistanceSet.size();
	DetectCell *cellx,*celly,*cellz;
	if(cellNo>-1&&cellNo<(int)size)
	{
		cellx=&g_xDistanceSet[cellNo];
		cellx->start=cellx->obj->g_boundingBox.minx;
		cellx->end=cellx->obj->g_boundingBox.maxx;
		cellx->length=cellx->end-cellx->start;
		celly=&g_yDistanceSet[cellNo];
		celly->start=celly->obj->g_boundingBox.miny;
		celly->end=celly->obj->g_boundingBox.maxy;
		celly->length=celly->end-celly->start;
		//g_yDistanceSet.push_back(*celly);
		cellz=&g_zDistanceSet[cellNo];
		cellz->start=cellz->obj->g_boundingBox.minz;
		cellz->end=cellz->obj->g_boundingBox.maxz;
		cellz->length=cellz->end-cellz->start;
		return;
	}
	for(unsigned int i=0;i<size;i++)
	{
		cellx=&g_xDistanceSet[i];
		cellx->start=cellx->obj->g_boundingBox.minx;
		cellx->end=cellx->obj->g_boundingBox.maxx;
		cellx->length=cellx->end-cellx->start;
		celly=&g_yDistanceSet[i];
		celly->start=celly->obj->g_boundingBox.miny;
		celly->end=celly->obj->g_boundingBox.maxy;
		celly->length=celly->end-celly->start;
		//g_yDistanceSet.push_back(*celly);
		cellz=&g_zDistanceSet[i];
		cellz->start=cellz->obj->g_boundingBox.minz;
		cellz->end=cellz->obj->g_boundingBox.maxz;
		cellz->length=cellz->end-cellz->start;
	}
}
void GAPPhysicsManager::useDetectCellGenerateSimbol()
{
	unsigned int size=g_collideSimbolSet.size();
	for(unsigned int i=0;i<size;i++)
	{
		g_collideSimbolSet[i]=0;
		for(unsigned int j=i+1;j<size;j++)
		{
			if(g_xDistanceSet[i].start<g_xDistanceSet[j].end&&g_xDistanceSet[i].end>g_xDistanceSet[j].start)
			{
				if(g_yDistanceSet[i].start<g_yDistanceSet[j].end&&g_yDistanceSet[i].end>g_yDistanceSet[j].start)
				{
					if(g_zDistanceSet[i].start<g_zDistanceSet[j].end&&g_zDistanceSet[i].end>g_zDistanceSet[j].start)
					{
						g_collideSimbolSet[i]=-1;
					}
				}
			}
		}
	}
}
void GAPPhysicsManager::deleteDetectCell(GAPhysicsBaseObject* obj)
{
	DetectCellSet::iterator iterx=g_xDistanceSet.begin();
	DetectCellSet::iterator itery=g_yDistanceSet.begin();
	DetectCellSet::iterator iterz=g_zDistanceSet.begin();
	for(;iterx!=g_xDistanceSet.end();iterx++)
	{
		if(iterx->obj==obj)
		{
			g_xDistanceSet.erase(iterx);
			g_yDistanceSet.erase(itery);
			g_zDistanceSet.erase(iterz);
			return;
		}
		itery++;
		iterz++;
	}
}