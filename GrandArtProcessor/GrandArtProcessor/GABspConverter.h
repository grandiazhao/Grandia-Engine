/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BSP_CONVERTER_H
#define BSP_CONVERTER_H

class BspLoader;
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "btBulletDynamicsCommon.h"
#include "Common.h"
#include "GA3DMath.h"
#include "MyWriteToFile.h"

///BspConverter turns a loaded bsp level into convex parts (vertices)
class BspConverter
{
	public:
		typedef std::vector<GAVector3> Vector3Set;
		std::vector<Vector3Set*> vector3SetSet;
		btDynamicsWorld *world;
		int  count;
		void convertBsp(BspLoader& bspLoader,float scaling);
		MyWriteToFile* wtf;
		BspConverter(btDynamicsWorld *world_)
		{
			world=world_;
			count=0;
			wtf=new MyWriteToFile();
		}
		virtual ~BspConverter()
		{
		}

		///this callback is called for each brush that succesfully converted into vertices
		virtual void addConvexVerticesCollider(btAlignedObjectArray<btVector3>& vertices, bool isEntity, const btVector3& entityTargetLocation)
		{
			///perhaps we can do something special with entities (isEntity)
			///like adding a collision Triggering (as example)
			if (vertices.size() > 0)
			{
				Vector3Set* set=new Vector3Set();
				for(int i=0;i<vertices.size();i++)
				{
					GAVector3* vec=new GAVector3(vertices[i].x(),vertices[i].y(),vertices[i].z());
					set->push_back(*vec);
				}
				count++;
				vector3SetSet.push_back(set);
				/*float mass = 0.f;
				btTransform startTransform;
				startTransform.setIdentity();
				startTransform.setOrigin(btVector3(0,0,0.f));*/
				//btCollisionShape* shape = new btConvexHullShape(&(vertices[0].getX()),vertices.size());
				//createRigidBody(mass, startTransform,shape);
			}
		}
		void createRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape)
		{
			btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0,0,0);
			if (isDynamic)
				shape->calculateLocalInertia(mass,localInertia);
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);

			btRigidBody* body = new btRigidBody(cInfo);
			body->setContactProcessingThreshold(1e18f);
			world->addRigidBody(body);
		}
};

#endif //BSP_CONVERTER_H

