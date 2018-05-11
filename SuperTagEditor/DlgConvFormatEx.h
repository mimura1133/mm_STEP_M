#if !defined(AFX_DLGCONVFORMATEX_H__6A322040_212A_11D4_9459_00402641B29B__INCLUDED_)
#define AFX_DLGCONVFORMATEX_H__6A322040_212A_11D4_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConvFormatEx.h : �w�b�_�[ �t�@�C��
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgConvFormatEx �_�C�A���O

class CDlgConvFormatEx : public COptionPage
{
	DECLARE_DYNCREATE(CDlgConvFormatEx)

// �R���X�g���N�V����
public:
	CDlgConvFormatEx();
	~CDlgConvFormatEx();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgConvFormatEx)
	enum { IDD = IDD_DLG_CONV_FORMAT_EX };
	CButton	m_btSpaceInitNumber;
	CComboBox	m_listFormatType;
	CEdit	m_editFormat;
	int		m_nAddNumber;
	int		m_nInitNumber;
	UINT	m_nMinColumn;
	int		m_nFormatType;
	//}}AFX_DATA

	USER_CONV_FORMAT_EX	m_userFormatEx[USER_CONV_FORMAT_EX_MAX];

	int		m_nOutputFormatType;
	void	OutputFormatString(LPCTSTR);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDlgConvFormatEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgConvFormatEx)
	afx_msg void OnBtAlbumName();
	afx_msg void OnBtArtistName();
	afx_msg void OnBtComment();
	afx_msg void OnBtFileName();
	afx_msg void OnBtFixString();
	afx_msg void OnBtNumber();
	afx_msg void OnBtReleaseYear();
	afx_msg void OnBtTrackName();
	afx_msg void OnBtTrackNumber();
	afx_msg void OnBtTrackNumber2();
	afx_msg void OnBtNameChange();
	afx_msg void OnSelChangeListFormatType();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtTrackNumber3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGCONVFORMATEX_H__6A322040_212A_11D4_9459_00402641B29B__INCLUDED_)
