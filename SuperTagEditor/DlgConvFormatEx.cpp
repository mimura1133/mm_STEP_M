// DlgConvFormatEx.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgNameChange.h"
#include "DlgConvFormatEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConvFormatEx �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDlgConvFormatEx, COptionPage)

CDlgConvFormatEx::CDlgConvFormatEx() : COptionPage(CDlgConvFormatEx::IDD)
{
	//{{AFX_DATA_INIT(CDlgConvFormatEx)
	m_nAddNumber = 1;
	m_nInitNumber = 1;
	m_nMinColumn = 1;
	m_nFormatType = 0;
	//}}AFX_DATA_INIT
}

CDlgConvFormatEx::~CDlgConvFormatEx()
{
}

void CDlgConvFormatEx::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConvFormatEx)
	DDX_Control(pDX, IDC_CH_SPACE_INIT, m_btSpaceInitNumber);
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
	DDX_Text(pDX, IDC_ED_ADD_NUMBER, m_nAddNumber);
	DDV_MinMaxInt(pDX, m_nAddNumber, 0, 999999);
	DDX_Text(pDX, IDC_ED_INIT_NUMBER, m_nInitNumber);
	DDV_MinMaxInt(pDX, m_nInitNumber, 0, 999999);
	DDX_Text(pDX, IDC_ED_MIN_COLUMN, m_nMinColumn);
	DDV_MinMaxUInt(pDX, m_nMinColumn, 1, 9);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConvFormatEx, COptionPage)
	//{{AFX_MSG_MAP(CDlgConvFormatEx)
	ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
	ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
	ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
	ON_BN_CLICKED(IDC_BT_FILE_NAME, OnBtFileName)
	ON_BN_CLICKED(IDC_BT_FIX_STRING, OnBtFixString)
	ON_BN_CLICKED(IDC_BT_NUMBER, OnBtNumber)
	ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
	ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER3, OnBtTrackNumber3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConvFormatEx ���b�Z�[�W �n���h��

void CDlgConvFormatEx::OutputFormatString(LPCTSTR sText)
{
	m_editFormat.ReplaceSel(sText, FALSE);
	m_editFormat.SetFocus();
}

void CDlgConvFormatEx::OnBtAlbumName() 
{
	OutputFormatString(TEXT("%ALBUM_NAME%"));
}

void CDlgConvFormatEx::OnBtArtistName() 
{
	OutputFormatString(TEXT("%ARTIST_NAME%"));
}

void CDlgConvFormatEx::OnBtComment() 
{
	OutputFormatString(TEXT("%COMMENT%"));
}

void CDlgConvFormatEx::OnBtFileName() 
{
	OutputFormatString(TEXT("%FILE_NAME%"));
}

void CDlgConvFormatEx::OnBtFixString() 
{
	OutputFormatString(TEXT("%STRING%"));
}

void CDlgConvFormatEx::OnBtNumber() 
{
	OutputFormatString(TEXT("%NUMBER%"));
}

void CDlgConvFormatEx::OnBtReleaseYear() 
{
	OutputFormatString(TEXT("%RELEASE_YEAR%"));
}

void CDlgConvFormatEx::OnBtTrackName() 
{
	OutputFormatString(TEXT("%TRACK_NAME%"));
}

void CDlgConvFormatEx::OnBtTrackNumber() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER%"));
}

void CDlgConvFormatEx::OnBtTrackNumber2() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER2%"));
}

void CDlgConvFormatEx::OnBtTrackNumber3() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER3%"));
}

void CDlgConvFormatEx::OnBtNameChange() 
{
	CDlgNameChange	dialog;

	dialog.m_strName = m_userFormatEx[m_nFormatType].strName;
	if (dialog.DoModal() == IDOK) {
		// �V�������̂ɕύX
		m_userFormatEx[m_nFormatType].strName = dialog.m_strName;

		// ���X�g�X�V
		OnSelChangeListFormatType();
		m_listFormatType.ResetContent();
		int i; for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
			CString	strName;
			strName.Format(TEXT("%d: %s"), i+1, m_userFormatEx[i].strName);
			m_listFormatType.AddString(strName);
		}
		// �I�����Ȃ���
		m_listFormatType.SetCurSel(m_nFormatType);
	}
}

void CDlgConvFormatEx::OnSelChangeListFormatType() 
{
	// ���ݕҏW���̓��e�𔽉f������
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_userFormatEx[m_nFormatType].bSpaceInitNumber = m_btSpaceInitNumber.GetCheck() ? true : false;
	m_userFormatEx[m_nFormatType].nInitNumber = GetDlgItemInt(IDC_ED_INIT_NUMBER, NULL, FALSE);
	m_userFormatEx[m_nFormatType].nAddNumber = GetDlgItemInt(IDC_ED_ADD_NUMBER, NULL, FALSE);
	m_userFormatEx[m_nFormatType].nColumnCount = GetDlgItemInt(IDC_ED_MIN_COLUMN, NULL, FALSE);

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_btSpaceInitNumber.SetCheck(m_userFormatEx[m_nFormatType].bSpaceInitNumber ? TRUE : FALSE);
	SetDlgItemInt(IDC_ED_INIT_NUMBER, m_userFormatEx[m_nFormatType].nInitNumber, FALSE);
	SetDlgItemInt(IDC_ED_ADD_NUMBER, m_userFormatEx[m_nFormatType].nAddNumber, FALSE);
	SetDlgItemInt(IDC_ED_MIN_COLUMN, m_userFormatEx[m_nFormatType].nColumnCount, FALSE);

	m_nInitNumber = m_userFormatEx[m_nFormatType].nInitNumber;
	m_nAddNumber = m_userFormatEx[m_nFormatType].nAddNumber;
	m_nMinColumn = m_userFormatEx[m_nFormatType].nColumnCount;
}

BOOL CDlgConvFormatEx::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
		CString	strName;
		strName.Format(TEXT("%d: %s"), i+1, m_userFormatEx[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_btSpaceInitNumber.SetCheck(m_userFormatEx[m_nFormatType].bSpaceInitNumber ? TRUE : FALSE);
	SetDlgItemInt(IDC_ED_INIT_NUMBER, m_userFormatEx[m_nFormatType].nInitNumber, FALSE);
	SetDlgItemInt(IDC_ED_ADD_NUMBER, m_userFormatEx[m_nFormatType].nAddNumber, FALSE);
	SetDlgItemInt(IDC_ED_MIN_COLUMN, m_userFormatEx[m_nFormatType].nColumnCount, FALSE);

	m_nInitNumber = m_userFormatEx[m_nFormatType].nInitNumber;
	m_nAddNumber = m_userFormatEx[m_nFormatType].nAddNumber;
	m_nMinColumn = m_userFormatEx[m_nFormatType].nColumnCount;

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgConvFormatEx::OnOK() 
{
	if (!::IsWindow(m_hWnd))	return;
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_userFormatEx[m_nFormatType].bSpaceInitNumber = m_btSpaceInitNumber.GetCheck() ? true : false;
	m_userFormatEx[m_nFormatType].nInitNumber = GetDlgItemInt(IDC_ED_INIT_NUMBER, NULL, FALSE);
	m_userFormatEx[m_nFormatType].nAddNumber = GetDlgItemInt(IDC_ED_ADD_NUMBER, NULL, FALSE);
	m_userFormatEx[m_nFormatType].nColumnCount = GetDlgItemInt(IDC_ED_MIN_COLUMN, NULL, FALSE);

	COptionPage::OnOK();
}
