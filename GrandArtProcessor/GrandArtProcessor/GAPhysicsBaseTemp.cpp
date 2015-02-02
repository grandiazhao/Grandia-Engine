#include "GAPhysicsBaseTemp.h"


//------------------------------GAPhysicsBase------------------------------------------
GAPhysicsBase::GAPhysicsBase()
{

}

int GAPhysicsBase::testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint)
{
	return 0;
}


//--------------------------------GALine-----------------------------------------------
GALine::GALine()
{
	p1=GAVector3(0,0,0);
	p2=GAVector3(1,1,1);
}

GALine::GALine(GAVector3 p1_,GAVector3 p2_)
{
	p1=p1_;
	p2=p2_;
}


//-------------------------------GASegment---------------------------------------------
GASegment::GASegment()
{
	pstart=GAVector3(0,0,0);
	pend=GAVector3(1,1,1);
}
GASegment::GASegment(GAVector3 pstart_,GAVector3 pend_)
{
	pstart=pstart_;
	pend=pend_;
}


//-------------------------------GAPlane-----------------------------------------------
GAPlane::GAPlane()
{

}


int GAPlane::testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint)
{
	return 0;
}

//-----------------------------------GACube--------------------------------------------

GACube::GACube()
{

}

int GACube::testCollideWith(GAPhysicsBase* object2,GAVector3& collidePoint)
{
	return 0;
}

int GACube::testCollideWithCube(GACube* object2,GAVector3& collidePoint)
{
	return 0;
}

//--------------------------------GACylinder-------------------------------------------



//--------------------------------GASphere---------------------------------------------



//--------------------------------GACapsule--------------------------------------------