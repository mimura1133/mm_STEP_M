#if !defined(AFX_CDlgMoveFolder_H__812CB3F9_1690_46AC_9DA9_FFE17C564166__INCLUDED_)
#define AFX_CDlgMoveFolder_H__812CB3F9_1690_46AC_9DA9_FFE17C564166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgMoveFolder.h : �w�b�_�[ �t�@�C��
//
#include "OptionPage/OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMoveFolder �_�C�A���O

class CDlgMoveFolder : public COptionPage
{
// �R���X�g���N�V����
public:
	void OnOK();
	CDlgMoveFolder(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgMoveFolder)
	enum { IDD = IDD_DLG_MOVE_FOLDER };
	CEdit	m_editInitPath;
	CButton	m_chkCopy;
	CComboBox	m_listFormatType;
	CEdit	m_editFormat;
	int		m_nFormatType;
	//}}AFX_DATA

	USER_MOVE_FODLER_FORMAT	m_userFormatEx[USER_MOVE_FODLER_FORMAT_MAX];


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgMoveFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgMoveFolder)
	afx_msg void OnSelChangeListFormatType();
	afx_msg void OnBtAlbumName();
	afx_msg void OnBtArtistName();
	afx_msg void OnBtComment();
	afx_msg void OnBtFileName();
	afx_msg void OnBtFixString();
	afx_msg void OnBtNameChange();
	afx_msg void OnBtReleaseYear();
	afx_msg void OnBtTrackName();
	afx_msg void OnBtTrackNumber();
	afx_msg void OnBtTrackNumber2();
	afx_msg void OnBtTrackNumber3();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtRef();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OutputFormatString(LPCTSTR sText);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CDlgMoveFolder_H__812CB3F9_1690_46AC_9DA9_FFE17C564166__INCLUDED_)
