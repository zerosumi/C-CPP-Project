// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__182CA98A_44DD_4B53_AA6D_143DBFE3D6E4__INCLUDED_)
#define AFX_LINE_H__182CA98A_44DD_4B53_AA6D_143DBFE3D6E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicShape.h"

class CLine : public CBasicShape   
{
protected:
	DECLARE_SERIAL(CLine)
public:
	CLine();
	virtual ~CLine();
	void SetTag(int mid_tag);
	void SetPoint(CPoint m_point1,CPoint m_point2);
	void Draw(CDC* pDC);
    void Serialize(CArchive &ar);
	bool CheckRect(CPoint c_point);
	CString OnSavesvg();
	void DrawTag(CClientDC* pDC);
	void In_Draw_Point(CClientDC* pDC);
	bool in_hot_point(CPoint m_point);
	 bool Check_Out_Rect(CPoint c_point1,CPoint c_point2);
public:
	int width1;
	int width2;
	CPoint mpoint1;
	CPoint mpoint2;
	CPoint mpoint3;
	CPoint mpoint4;

};

#endif // !defined(AFX_LINE_H__182CA98A_44DD_4B53_AA6D_143DBFE3D6E4__INCLUDED_)
