// CDlgMoveFolder.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgMoveFolder.h"
#include "DlgNameChange.h"
#include "SHBrowseForFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMoveFolder �_�C�A���O


CDlgMoveFolder::CDlgMoveFolder(CWnd* pParent /*=NULL*/)
	: COptionPage(CDlgMoveFolder::IDD)
{
	//{{AFX_DATA_INIT(CDlgMoveFolder)
	m_nFormatType = -1;
	//}}AFX_DATA_INIT
}


void CDlgMoveFolder::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMoveFolder)
	DDX_Control(pDX, IDC_EDIT_INIT_PATH, m_editInitPath);
	DDX_Control(pDX, IDC_CHECK_COPY, m_chkCopy);
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMoveFolder, COptionPage)
	//{{AFX_MSG_MAP(CDlgMoveFolder)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
	ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
	ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
	ON_BN_CLICKED(IDC_BT_FILE_NAME, OnBtFileName)
	ON_BN_CLICKED(IDC_BT_FIX_STRING, OnBtFixString)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
	ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER3, OnBtTrackNumber3)
	ON_BN_CLICKED(IDC_BT_REF, OnBtRef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMoveFolder ���b�Z�[�W �n���h��

void CDlgMoveFolder::OnSelChangeListFormatType() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	// ���ݕҏW���̓��e�𔽉f������
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_userFormatEx[m_nFormatType].bCopy = m_chkCopy.GetCheck() ? true : false;
	m_editInitPath.GetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_chkCopy.SetCheck(m_userFormatEx[m_nFormatType].bCopy);
	m_editInitPath.SetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */
}

void CDlgMoveFolder::OutputFormatString(LPCTSTR sText)
{
	m_editFormat.ReplaceSel(sText, FALSE);
	m_editFormat.SetFocus();
}

void CDlgMoveFolder::OnBtAlbumName() 
{
	OutputFormatString(TEXT("%ALBUM_NAME%"));
}

void CDlgMoveFolder::OnBtArtistName() 
{
	OutputFormatString(TEXT("%ARTIST_NAME%"));
}

void CDlgMoveFolder::OnBtComment() 
{
	OutputFormatString(TEXT("%COMMENT%"));
}

void CDlgMoveFolder::OnBtFileName() 
{
	OutputFormatString(TEXT("%FILE_NAME%"));
}

void CDlgMoveFolder::OnBtFixString() 
{
	OutputFormatString(TEXT("%STRING%"));
}

//void CDlgMoveFolder::OnBtNumber() 
//{
//	OutputFormatString(TEXT("%NUMBER%"));
//}

void CDlgMoveFolder::OnBtReleaseYear() 
{
	OutputFormatString(TEXT("%RELEASE_YEAR%"));
}

void CDlgMoveFolder::OnBtTrackName() 
{
	OutputFormatString(TEXT("%TRACK_NAME%"));
}

void CDlgMoveFolder::OnBtTrackNumber() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER%"));
}

void CDlgMoveFolder::OnBtTrackNumber2() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER2%"));
}

void CDlgMoveFolder::OnBtTrackNumber3() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER3%"));
}

void CDlgMoveFolder::OnBtNameChange() 
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

BOOL CDlgMoveFolder::OnInitDialog() 
{
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	COptionPage::OnInitDialog();

	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
		CString	strName;
		strName.Format(TEXT("%d: %s"), i+1, m_userFormatEx[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_chkCopy.SetCheck(m_userFormatEx[m_nFormatType].bCopy);
	m_editInitPath.SetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgMoveFolder::OnOK()
{
	if (!::IsWindow(m_hWnd))	return;
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
	m_userFormatEx[m_nFormatType].bCopy = m_chkCopy.GetCheck() ? true : false;
	m_editInitPath.GetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */

	COptionPage::OnOK();

}

void CDlgMoveFolder::OnBtRef() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString	strFolder;
	GetDlgItemText(IDC_EDIT_INIT_PATH, strFolder);

	// �t�H���_�I���_�C�A���O���J��
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};
	lstrcpy(sFolderName, strFolder);

	CSHBrowseForFolder	browse;
	browse.SetEnableSubDirButton(false);
	if (browse.Exec(sFolderName)) {
		SetDlgItemText(IDC_EDIT_INIT_PATH, sFolderName);
	}
}
