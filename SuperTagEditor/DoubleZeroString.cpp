// DoubleZeroString.cpp: implementation of the CDoubleZeroString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoubleZeroString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoubleZeroString::CDoubleZeroString()
{
	m_ar.SetSize(0, 16);
	Empty();
}

CDoubleZeroString::~CDoubleZeroString()
{
	Empty();
}

void CDoubleZeroString::Empty()
{
	m_ar.RemoveAll();
	m_ar.FreeExtra();

	m_buf.clear();
}

bool CDoubleZeroString::Add(LPCTSTR str)
{
	ASSERT(str!=NULL);
	try
	{
		m_ar.Add(str);
	}
	catch(CMemoryException* ex)
	{
		ex->Delete();
		return	false;
	}
	return	true;
}

bool CDoubleZeroString::AddDoubleZero(LPCTSTR dzstring)
{
	ASSERT(dzstring!=NULL);
	auto p =	dzstring;

	while(*p)
	{
		if(Add(p)==false)
			return	false;
		p	+=	(lstrlen(p) + 1);
	}
	return	true;
}

int CDoubleZeroString::GetCount() const
{
	return	m_ar.GetSize();
}

CDoubleZeroString::operator LPCTSTR()
{
	m_buf.clear();
		//	calculate the required buffer length
	size_t	tlen(0);
	if(m_ar.GetSize()>0)
	{
		int i; for(i=0;i<m_ar.GetSize();i++)
			tlen+=	(m_ar[i].GetLength() + 1); // add 1 for the normal zeroterm.
	}
	else
		tlen=1;
	tlen++;	//	for the double zeroterminator

	m_buf.resize(tlen, TEXT('0'));
	auto p = m_buf.data();
	int i; for(i=0;i<m_ar.GetSize();i++)
	{
		lstrcpy(p, m_ar[i]);
		p+= (m_ar[i].GetLength() + 1);
	}

	return	m_buf.data();
}

LPCTSTR CDoubleZeroString::Get(int index)
{
		// assert on debug builds when index is out of bounds
	ASSERT(index>=0&&index<=m_ar.GetSize());
	if(index<0||index>=m_ar.GetSize())
		return	NULL;
	else
		return	m_ar[index];
}
