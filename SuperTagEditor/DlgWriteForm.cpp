// DlgWriteForm.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgNameChange.h"
#include "DlgWriteForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWriteForm �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDlgWriteForm, COptionPage)

CDlgWriteForm::CDlgWriteForm() : COptionPage(CDlgWriteForm::IDD)
{
	//{{AFX_DATA_INIT(CDlgWriteForm)
	m_nFormatType = 0;
	//}}AFX_DATA_INIT
}

CDlgWriteForm::~CDlgWriteForm()
{
}

void CDlgWriteForm::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWriteForm)
	DDX_Control(pDX, IDC_CH_WRITE_HTML, m_btWriteHtml);
	DDX_Control(pDX, IDC_CH_IS_HTML, m_btIsHtml);
	DDX_Control(pDX, IDC_CH_WRITE_SELECTED, m_btWriteSelected);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_editFileName);
	DDX_Control(pDX, IDC_EDIT_EXT_NAME, m_editExtName);
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWriteForm, COptionPage)
	//{{AFX_MSG_MAP(CDlgWriteForm)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_BN_CLICKED(IDC_BT_REF_FILE, OnBtRefFile)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWriteForm ���b�Z�[�W �n���h��

BOOL CDlgWriteForm::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < WRITE_FORMAT_MAX; i++) {
		CString	strName;
		strName.Format(TEXT("%d: %s"), i+1, m_writeFormat[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editFileName.SetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.SetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_btWriteSelected.SetCheck(m_writeFormat[m_nFormatType].bWriteSelected ? TRUE : FALSE);
	m_btIsHtml.SetCheck(m_writeFormat[m_nFormatType].bIsHtml ? TRUE : FALSE);
	m_btWriteHtml.SetCheck(m_writeFormat[m_nFormatType].bWriteHtml ? TRUE : FALSE); /* BeachMonster5 120 */

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgWriteForm::OnBtNameChange() 
{
	CDlgNameChange	dialog;

	dialog.m_strName = m_writeFormat[m_nFormatType].strName;
	if (dialog.DoModal() == IDOK) {
		// �V�������̂ɕύX
		m_writeFormat[m_nFormatType].strName = dialog.m_strName;

		// ���X�g�X�V
		OnSelChangeListFormatType();
		m_listFormatType.ResetContent();
		int i; for (i = 0; i < WRITE_FORMAT_MAX; i++) {
			CString	strName;
			strName.Format(TEXT("%d: %s"), i+1, m_writeFormat[i].strName);
			m_listFormatType.AddString(strName);
		}
		// �I�����Ȃ���
		m_listFormatType.SetCurSel(m_nFormatType);
	}
}

void CDlgWriteForm::OnBtRefFile() 
{
	CString	strFileName;
	m_editFileName.GetWindowText(strFileName);

	// �t�@�C���I���_�C�A���O���J��
	static	LPCTSTR	sFileFilter =	TEXT("�����t�@�C��(*.txt)|*.txt|" \
									"All Files(*.*)|*.*|");
	CFileDialog		dialog(TRUE, TEXT(".txt"), strFileName,
	                       OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXTENSIONDIFFERENT | OFN_PATHMUSTEXIST,
	                       sFileFilter, NULL);
	if (dialog.DoModal() == IDOK) {
		m_editFileName.SetWindowText(dialog.GetPathName());
	}
}

void CDlgWriteForm::OnSelChangeListFormatType() 
{
	// ���ݕҏW���̓��e�𔽉f������
	m_editFileName.GetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.GetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_writeFormat[m_nFormatType].bWriteSelected	= m_btWriteSelected.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bIsHtml		= m_btIsHtml.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bWriteHtml		= m_btWriteHtml.GetCheck() ? true : false;

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editFileName.SetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.SetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_btWriteSelected.SetCheck(m_writeFormat[m_nFormatType].bWriteSelected ? TRUE : FALSE);
	m_btIsHtml.SetCheck(m_writeFormat[m_nFormatType].bIsHtml ? TRUE : FALSE);
	m_btWriteHtml.SetCheck(m_writeFormat[m_nFormatType].bWriteHtml ? TRUE : FALSE);
}

void CDlgWriteForm::OnOK() 
{
	if (!::IsWindow(m_hWnd))	return;
	m_editFileName.GetWindowText(m_writeFormat[m_nFormatType].strFileName);
	m_editExtName.GetWindowText(m_writeFormat[m_nFormatType].strExtName);
	m_writeFormat[m_nFormatType].bWriteSelected	= m_btWriteSelected.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bIsHtml		= m_btIsHtml.GetCheck() ? true : false;
	m_writeFormat[m_nFormatType].bWriteHtml		= m_btWriteHtml.GetCheck() ? true : false;

	COptionPage::OnOK();
}
