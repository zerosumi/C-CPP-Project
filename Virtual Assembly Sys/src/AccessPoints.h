// AccessPoints.h: interface for the AccessPoints class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCESSPOINTS_H__9AD6B161_FB34_46AF_BC1B_8DE7C240851F__INCLUDED_)
#define AFX_ACCESSPOINTS_H__9AD6B161_FB34_46AF_BC1B_8DE7C240851F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "point.h"
class AccessPoints  
{
public:
	bool isPure; 
	point ap;
	AccessPoints* next; 
	AccessPoints* front;
	AccessPoints();
	virtual ~AccessPoints();

};

#endif // !defined(AFX_ACCESSPOINTS_H__9AD6B161_FB34_46AF_BC1B_8DE7C240851F__INCLUDED_)
