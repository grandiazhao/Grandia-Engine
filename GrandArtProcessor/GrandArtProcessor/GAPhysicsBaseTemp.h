#ifndef GAPHYSICSBASETEMP_H
#define GAPHYSICSBASETEMP_H

#include "GA3DMath.h"
#include "Common.h"


class GAPhysicsBase
{
public:
	enum ObjectType
	{
		ObjectType_None=0,
		ObjectType_Plane,
		ObjectType_Sphere,
		ObjectType_Cube,
		ObjectType_Cylinder,
		ObjectType_Capsule,
	};
	ObjectType g_type;
public:
	GAPhysicsBase();
	virtual int testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint);
};

class GALine;
class GASegment;
class GAPlane;
class GASphere;
class GACube;
class GACylinder;
class GACapsule;

class GALine
{
private:
	GAVector3 p1;
	GAVector3 p2;
public:
	GALine();
	GALine(GAVector3 p1_,GAVector3 p2_);
};

class GASegment
{
private:
	GAVector3 pstart,pend;
public:
	GASegment();
	GASegment(GAVector3 pstart_,GAVector3 pend_);
	int testCollideWithPlane(GAPlane *object2,GAVector3& collidePoint);
};

class GAPlane : public GAPhysicsBase
{
public:
	GAPlane();
	virtual int testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint);
private:
	int testCollideWithPlane(GAPlane* object2,GAVector3& collidePoint);
	int testCollideWithSphere(GASphere* object2,GAVector3& collidePoint);
	int testCollideWithCube(GACube* object2,GAVector3& collidePoint);
	int testCollideWithCylinder(GACylinder* object2,GAVector3& collidePoint);
	int testCollideWithCapsule(GACapsule* object2,GAVector3& collidePoint);
};

class GASphere : public GAPhysicsBase
{
public:
	GASphere();
	virtual int testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint);
private:
	int testCollideWithPlane(GAPlane* object2,GAVector3& collidePoint);
	int testCollideWithSphere(GASphere* object2,GAVector3& collidePoint);
	int testCollideWithCube(GACube* object2,GAVector3& collidePoint);
	int testCollideWithCylinder(GACylinder* object2,GAVector3& collidePoint);
	int testCollideWithCapsule(GACapsule* object2,GAVector3& collidePoint);
};

class GACube : public GAPhysicsBase
{
public:
	GACube();
	virtual int testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint);
private:
	int testCollideWithPlane(GAPlane* object2,GAVector3& collidePoint);
	int testCollideWithSphere(GASphere* object2,GAVector3& collidePoint);
	int testCollideWithCube(GACube* object2,GAVector3& collidePoint);
	int testCollideWithCylinder(GACylinder* object2,GAVector3& collidePoint);
	int testCollideWithCapsule(GACapsule* object2,GAVector3& collidePoint);
};

class GACylinder : public GAPhysicsBase
{
public:
	GACylinder();
	virtual int testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint);
private:
	int testCollideWithPlane(GAPlane* object2,GAVector3& collidePoint);
	int testCollideWithSphere(GASphere* object2,GAVector3& collidePoint);
	int testCollideWithCube(GACube* object2,GAVector3& collidePoint);
	int testCollideWithCylinder(GACylinder* object2,GAVector3& collidePoint);
	int testCollideWithCapsule(GACapsule* object2,GAVector3& collidePoint);
};

class GACapsule : public GAPhysicsBase
{
public:
	GACapsule();
	virtual int testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint);
private:
	int testCollideWithPlane(GAPlane* object2,GAVector3& collidePoint);
	int testCollideWithSphere(GASphere* object2,GAVector3& collidePoint);
	int testCollideWithCube(GACube* object2,GAVector3& collidePoint);
	int testCollideWithCylinder(GACylinder* object2,GAVector3& collidePoint);
	int testCollideWithCapsule(GACapsule* object2,GAVector3& collidePoint);
};

#endif