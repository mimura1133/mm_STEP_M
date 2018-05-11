#if !defined(AFX_DLGUSERCONVFORMARTTAG2TAG_H__7642E6AE_07ED_405A_A76C_041446689D4C__INCLUDED_)
#define AFX_DLGUSERCONVFORMARTTAG2TAG_H__7642E6AE_07ED_405A_A76C_041446689D4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUserConvFormartTag2Tag.h : �w�b�_�[ �t�@�C��
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUserConvFormartTag2Tag �_�C�A���O

class CDlgUserConvFormartTag2Tag : public COptionPage
{
	DECLARE_DYNCREATE(CDlgUserConvFormartTag2Tag)

// �R���X�g���N�V����
public:
	CDlgUserConvFormartTag2Tag();
	~CDlgUserConvFormartTag2Tag();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgUserConvFormartTag2Tag)
	enum { IDD = IDD_DLG_USER_CONV_FORMAT_TAG2TAG };
	CComboBox	m_listFormatType;
	CEdit	m_editFormat;
	int		m_nFormatType;
	//}}AFX_DATA

	USER_CONV_FORMAT_TAG2TAG	m_userFormatTag2Tag[USER_CONV_FORMAT_TAG2TAG_MAX];

	int		m_nOutputFormatType;
	void	OutputFormatString(LPCTSTR);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgUserConvFormartTag2Tag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgUserConvFormartTag2Tag)
	afx_msg void OnBtAlbumName();
	afx_msg void OnBtArtistName();
	afx_msg void OnBtComment();
	afx_msg void OnBtNameChange();
	afx_msg void OnBtReleaseYear();
	afx_msg void OnBtTrackName();
	afx_msg void OnBtTrackNumber();
	afx_msg void OnBtTrackNumber2();
	afx_msg void OnBtTrackNumber3();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeListFormatType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGUSERCONVFORMARTTAG2TAG_H__7642E6AE_07ED_405A_A76C_041446689D4C__INCLUDED_)
