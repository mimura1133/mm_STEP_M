// DlgEnvPlayer.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "SHBrowseForFolder.h"
#include "DlgEnvPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvPlayer �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDlgEnvPlayer, COptionPage)

CDlgEnvPlayer::CDlgEnvPlayer() : COptionPage(CDlgEnvPlayer::IDD)
{
	//{{AFX_DATA_INIT(CDlgEnvPlayer)
	m_nPlayerType = -1;
	m_strWinAmpPath = _T("");
	//}}AFX_DATA_INIT
}

CDlgEnvPlayer::~CDlgEnvPlayer()
{
}

void CDlgEnvPlayer::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnvPlayer)
	DDX_Radio(pDX, IDC_RA_PLAYER_WINAMP, m_nPlayerType);
	DDX_Text(pDX, IDC_ED_WINAMP_PATH, m_strWinAmpPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnvPlayer, COptionPage)
	//{{AFX_MSG_MAP(CDlgEnvPlayer)
	ON_BN_CLICKED(IDC_BT_REF_WINAMP, OnBtRefWinamp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvPlayer ���b�Z�[�W �n���h��

BOOL CDlgEnvPlayer::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgEnvPlayer::OnBtRefWinamp() 
{
	// ���ݎw�肳��Ă��郍�O�t�@�C�������擾
	CString	strFileName;
	GetDlgItemText(IDC_ED_WINAMP_PATH, strFileName);

	// �t�@�C���I���_�C�A���O���J��
	static	LPCTSTR	sFileFilter =	TEXT("Execute File(*.exe)|*.EXE|" \
									"All Files(*.*)|*.*|");
	CFileDialog		dialog(TRUE, TEXT(".exe"), strFileName,
	                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	                       sFileFilter, NULL);
	if (dialog.DoModal() == IDOK) {
		CString	strFileName(dialog.GetPathName());
		strFileName.MakeUpper();
		if (strFileName.Find(TEXT("WINAMP.EXE")) >= 0) {
			m_nPlayerType = PLAYER_WINAMP;
		} else if (strFileName.Find(TEXT("SCMPX.EXE")) >= 0) {
			m_nPlayerType = PLAYER_SCMPX;
		} else if (strFileName.Find(TEXT("KBMPLAY.EXE")) >= 0) {
			m_nPlayerType = PLAYER_KBMPLAY;
		} else if (strFileName.Find(TEXT("LILITH.EXE")) >= 0) { /* WildCherry 070 */
			m_nPlayerType = PLAYER_LILITH;
		} else {
			m_nPlayerType = PLAYER_EXTEND;
		}
		m_strWinAmpPath = dialog.GetPathName();
		UpdateData(FALSE);
	}
}
