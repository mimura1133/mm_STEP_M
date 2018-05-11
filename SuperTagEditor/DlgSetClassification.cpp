// DlgSetClassification.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgSetClassification.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetClassification �_�C�A���O
IMPLEMENT_DYNCREATE(CDlgSetClassification, COptionPage)


CDlgSetClassification::CDlgSetClassification() : COptionPage(CDlgSetClassification::IDD)
{
	//{{AFX_DATA_INIT(CDlgSetClassification)
	m_nClassType = -1;
	m_nClass1 = -1;
	m_nClass2 = -1;
	m_nClass3 = -1;
	m_nClass4 = -1;
	m_nClass5 = -1;
	m_bExecClassification = FALSE;
	//}}AFX_DATA_INIT
	m_bFirstEdit = true;
}

CDlgSetClassification::~CDlgSetClassification()
{
}


void CDlgSetClassification::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetClassification)
	DDX_Control(pDX, IDC_CH_EXEC_CLASSIFICATION, m_btExecClassificasion);
	DDX_Control(pDX, IDC_ST_CLASS5, m_stClass5);
	DDX_Control(pDX, IDC_ST_CLASS4, m_stClass4);
	DDX_Control(pDX, IDC_ST_CLASS3, m_stClass3);
	DDX_Control(pDX, IDC_ST_CLASS2, m_stClass2);
	DDX_Control(pDX, IDC_ST_CLASS1, m_stClass1);
	DDX_Control(pDX, IDC_LIST_CLASS5, m_listClass5);
	DDX_Control(pDX, IDC_LIST_CLASS4, m_listClass4);
	DDX_Control(pDX, IDC_LIST_CLASS3, m_listClass3);
	DDX_Control(pDX, IDC_LIST_CLASS2, m_listClass2);
	DDX_Control(pDX, IDC_LIST_CLASS1, m_listClass1);
	DDX_Radio(pDX, IDC_RA_FOLDER, m_nClassType);
	DDX_CBIndex(pDX, IDC_LIST_CLASS1, m_nClass1);
	DDX_CBIndex(pDX, IDC_LIST_CLASS2, m_nClass2);
	DDX_CBIndex(pDX, IDC_LIST_CLASS3, m_nClass3);
	DDX_CBIndex(pDX, IDC_LIST_CLASS4, m_nClass4);
	DDX_CBIndex(pDX, IDC_LIST_CLASS5, m_nClass5);
	DDX_Check(pDX, IDC_CH_EXEC_CLASSIFICATION, m_bExecClassification);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetClassification, COptionPage)
	//{{AFX_MSG_MAP(CDlgSetClassification)
	ON_BN_CLICKED(IDC_RA_FILE_TAG, OnRaFileTag)
	ON_BN_CLICKED(IDC_RA_FOLDER, OnRaFolder)
	ON_CBN_SELCHANGE(IDC_LIST_CLASS1, OnSelChangeListClass)
	ON_CBN_SELCHANGE(IDC_LIST_CLASS2, OnSelChangeListClass)
	ON_CBN_SELCHANGE(IDC_LIST_CLASS3, OnSelChangeListClass)
	ON_CBN_SELCHANGE(IDC_LIST_CLASS4, OnSelChangeListClass)
	ON_CBN_SELCHANGE(IDC_LIST_CLASS5, OnSelChangeListClass)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetClassification ���b�Z�[�W �n���h��
void CDlgSetClassification::UpdateStatus(void)
{
	BOOL	bFlag = m_nClassType ? TRUE : FALSE;
	m_listClass1.EnableWindow(bFlag);
	m_listClass2.EnableWindow(bFlag);
	m_listClass3.EnableWindow(bFlag);
	m_listClass4.EnableWindow(bFlag);
	m_listClass5.EnableWindow(bFlag);
	m_stClass1.EnableWindow(bFlag);
	m_stClass2.EnableWindow(bFlag);
	m_stClass3.EnableWindow(bFlag);
	m_stClass4.EnableWindow(bFlag);
	m_stClass5.EnableWindow(bFlag);
	m_btExecClassificasion.EnableWindow(bFlag);
}

void CDlgSetClassification::OnRaFileTag() 
{
	m_nClassType = 1;
	UpdateStatus();
	OnSelChangeListClass();			// [�������ɍX�V]�̃`�F�b�N������������
}

void CDlgSetClassification::OnRaFolder() 
{
	m_nClassType = 0;
	UpdateStatus();
}

BOOL CDlgSetClassification::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	extern	const TCHAR	***g_sNameList;
	int i; for (i = 0; g_sNameList[0][i] != NULL; i++) {
		m_listClass1.AddString(g_sNameList[0][i]);
		m_listClass2.AddString(g_sNameList[0][i]);
		m_listClass3.AddString(g_sNameList[0][i]);
		m_listClass4.AddString(g_sNameList[0][i]);
		m_listClass5.AddString(g_sNameList[0][i]);
	}
	m_listClass1.SetCurSel(m_nClass1);
	m_listClass2.SetCurSel(m_nClass2);
	m_listClass3.SetCurSel(m_nClass3);
	m_listClass4.SetCurSel(m_nClass4);
	m_listClass5.SetCurSel(m_nClass5);
	UpdateStatus();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgSetClassification::OnSelChangeListClass() 
{
	if (m_bFirstEdit) {
		// �ŏ��̕ҏW���ɂ�[�������ɍX�V]���`�F�b�N����
		m_btExecClassificasion.SetCheck(TRUE);
		m_bFirstEdit = false;
	}
}

// �����l�ɖ߂�
void CDlgSetClassification::OnBtResetPage() 
{
	// ���ޕ��@
	((CButton *)GetDlgItem(IDC_RA_FOLDER))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RA_FILE_TAG))->SetCheck(FALSE);

	// �K�w
	m_listClass1.SetCurSel(0);
	m_listClass2.SetCurSel(0);
	m_listClass3.SetCurSel(0);
	m_listClass4.SetCurSel(0);
	m_listClass5.SetCurSel(0);

	// [�������ɍX�V]���`�F�b�N����
	m_btExecClassificasion.SetCheck(TRUE);
}
