// DlgCopyFormat.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgCopyFormat.h"
#include "DlgNameChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCopyFormat �_�C�A���O


CDlgCopyFormat::CDlgCopyFormat(CWnd* pParent /*=NULL*/)
	: COptionPage(CDlgCopyFormat::IDD)
{
	//{{AFX_DATA_INIT(CDlgCopyFormat)
	m_nFormatType = -1;
	//}}AFX_DATA_INIT
}


void CDlgCopyFormat::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCopyFormat)
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCopyFormat, COptionPage)
	//{{AFX_MSG_MAP(CDlgCopyFormat)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
	ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
	ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
	ON_BN_CLICKED(IDC_BT_FILE_NAME, OnBtFileName)
	ON_BN_CLICKED(IDC_BT_FIX_STRING, OnBtFixString)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_BN_CLICKED(IDC_BT_NUMBER, OnBtNumber)
	ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
	ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER3, OnBtTrackNumber3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCopyFormat ���b�Z�[�W �n���h��
void CDlgCopyFormat::OnSelChangeListFormatType() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	// ���ݕҏW���̓��e�𔽉f������
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
}

void CDlgCopyFormat::OutputFormatString(LPCTSTR sText)
{
	m_editFormat.ReplaceSel(sText, FALSE);
	m_editFormat.SetFocus();
}

void CDlgCopyFormat::OnBtAlbumName() 
{
	OutputFormatString(TEXT("%ALBUM_NAME%"));
}

void CDlgCopyFormat::OnBtArtistName() 
{
	OutputFormatString(TEXT("%ARTIST_NAME%"));
}

void CDlgCopyFormat::OnBtComment() 
{
	OutputFormatString(TEXT("%COMMENT%"));
}

void CDlgCopyFormat::OnBtFileName() 
{
	OutputFormatString(TEXT("%FILE_NAME%"));
}

void CDlgCopyFormat::OnBtFixString() 
{
	OutputFormatString(TEXT("%STRING%"));
}

void CDlgCopyFormat::OnBtNumber() 
{
	OutputFormatString(TEXT("%NUMBER%"));
}

void CDlgCopyFormat::OnBtReleaseYear() 
{
	OutputFormatString(TEXT("%RELEASE_YEAR%"));
}

void CDlgCopyFormat::OnBtTrackName() 
{
	OutputFormatString(TEXT("%TRACK_NAME%"));
}

void CDlgCopyFormat::OnBtTrackNumber() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER%"));
}

void CDlgCopyFormat::OnBtTrackNumber2() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER2%"));
}

void CDlgCopyFormat::OnBtTrackNumber3() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER3%"));
}

void CDlgCopyFormat::OnBtNameChange() 
{
	CDlgNameChange	dialog;

	dialog.m_strName = m_userFormatEx[m_nFormatType].strName;
	if (dialog.DoModal() == IDOK) {
		// �V�������̂ɕύX
		m_userFormatEx[m_nFormatType].strName = dialog.m_strName;

		// ���X�g�X�V
		OnSelChangeListFormatType();
		m_listFormatType.ResetContent();
		int i; for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
			CString	strName;
			strName.Format(TEXT("%d: %s"), i+1, m_userFormatEx[i].strName);
			m_listFormatType.AddString(strName);
		}
		// �I�����Ȃ���
		m_listFormatType.SetCurSel(m_nFormatType);
	}
}

BOOL CDlgCopyFormat::OnInitDialog() 
{
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	COptionPage::OnInitDialog();

	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < USER_COPY_FORMAT_FORMAT_MAX; i++) {
		CString	strName;
		strName.Format(TEXT("%d: %s"), i+1, m_userFormatEx[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgCopyFormat::OnOK()
{
	if (!::IsWindow(m_hWnd))	return;
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);

	COptionPage::OnOK();

}
