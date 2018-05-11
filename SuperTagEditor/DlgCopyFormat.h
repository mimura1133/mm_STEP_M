#if !defined(AFX_DLGCOPYFORMAT_H__263B867A_1D1D_4E7B_B72B_1D07BDCE144B__INCLUDED_)
#define AFX_DLGCOPYFORMAT_H__263B867A_1D1D_4E7B_B72B_1D07BDCE144B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCopyFormat.h : �w�b�_�[ �t�@�C��
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCopyFormat �_�C�A���O

class CDlgCopyFormat : public COptionPage
{
// �R���X�g���N�V����
public:
	void OnOK();
	CDlgCopyFormat(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgCopyFormat)
	enum { IDD = IDD_DLG_COPY_FORMAT };
	CComboBox	m_listFormatType;
	CEdit	m_editFormat;
	int		m_nFormatType;
	//}}AFX_DATA

	USER_COPY_FORMAT_FORMAT	m_userFormatEx[USER_COPY_FORMAT_FORMAT_MAX];

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgCopyFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgCopyFormat)
	afx_msg void OnSelChangeListFormatType();
	afx_msg void OnBtAlbumName();
	afx_msg void OnBtArtistName();
	afx_msg void OnBtComment();
	afx_msg void OnBtFileName();
	afx_msg void OnBtFixString();
	afx_msg void OnBtNameChange();
	afx_msg void OnBtNumber();
	afx_msg void OnBtReleaseYear();
	afx_msg void OnBtTrackName();
	afx_msg void OnBtTrackNumber();
	afx_msg void OnBtTrackNumber2();
	afx_msg void OnBtTrackNumber3();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OutputFormatString(LPCTSTR sText);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGCOPYFORMAT_H__263B867A_1D1D_4E7B_B72B_1D07BDCE144B__INCLUDED_)
