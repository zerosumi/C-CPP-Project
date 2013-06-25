// BasicShape.h: interface for the CBasicShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICSHAPE_H__325AE13C_402A_4977_B3DF_D85BCE06387B__INCLUDED_)
#define AFX_BASICSHAPE_H__325AE13C_402A_4977_B3DF_D85BCE06387B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define round(a) ( int ) ( a + .5 )
class CBasicShape :public CObject
{
protected:
	LOGFONT logFont;
	CFont m_font;
	COLORREF m_clr;
	int tag;
	
	CString m_title;                    //�Ĺ�����
	                 
public:
	CBasicShape* LineFront;
	CBasicShape* LineBehind;
	int tag1;
	int tag2;
	int R_tag1;   //������ε��������߳��ڵ�
	int	R_tag2;
	int R_tag3; 
	int k;    //���ڱ�����εĳ����
	int intag1;   //�����ڵ�
	int intag2;
	int intag3;
	int outtag1;
	int outtag2;
	int outtag3;
	CPoint point1;
	CPoint point2;
public:
	void SetlogFont(LOGFONT *lf);
	void SetClr(COLORREF clr);
	LOGFONT* GetlogFont();
	CFont* GetFont();
	COLORREF GetClr();
	void SetTitle( CString title );
	CString GetTitle() const;
	CBasicShape();
	virtual ~CBasicShape();
	virtual void SetTag(int mid_tag);
	virtual int GetTag();
	virtual void SetPoint(CPoint m_point1,CPoint m_point2);
	virtual void Draw(CDC* pDC);
	virtual void Serialize(CArchive& ar);
	virtual CString OnSavesvg();
	virtual bool CheckRect(CPoint c_point);//����������Ƿ�����ͼԪ�ڣ���ѡ��
	virtual bool Check_Out_Rect(CPoint c_point1,CPoint c_point2);//�������Ƿ��ѡͼԪ�������ѡ��
	virtual void DrawTag(CClientDC* pDC);//�����ȵ�С����
	virtual void MovePoint(CPoint m_point1,CPoint m_point2);//�ı�ͼԪ����
	virtual bool in_hot_point(CPoint m_point);//����Ƿ����ȵ㷽����
	virtual void Zoom(CPoint m_point1,CPoint m_point2);//��������
	virtual void In_Draw(CClientDC* pDC);//�ػ�ͼԪ
	virtual void In_Draw_Point(CClientDC* pDC);//��ȥ�ȵ�
    void SetCharacter(CString mid_title);       //���ڴ洢���ֵĺ����������ĵ���serializa����
	CPoint GetPoint11();
	CPoint GetPoint12();
	CPoint GetPoint13();
	CPoint GetPoint2();
	CBasicShape* GetLine_ptr1();
	CBasicShape* GetLine_ptr2();
	void AddTag();
	int Get_k();
	virtual void paste_change();
	void Reduce_k();
	int GetWidth();
	bool EntryCheck();    //�����б���ڵ��Ƿ��ѱ�ѡ��.
	void Set_in_out_tag(int i_tag1,int i_tag2,int i_tag3,int o_tag1,int o_tag2,int o_tag3);
	void SetRTag(int tag11,int tag22,int R_tag11,int R_tag22,int R_tag33);
	void Setintag(int i_tag);
private:

};

#endif // !defined(AFX_BASICSHAPE_H__325AE13C_402A_4977_B3DF_D85BCE06387B__INCLUDED_)
