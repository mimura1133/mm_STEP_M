// DlgSetupGenre.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "FileMP3.h"
#include "DlgSetupGenre.h"
#include "DlgAddUserGenre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ListView_SetCheckState
#define ListView_SetCheckState(hwndLV, i, fCheck)	ListView_SetItemState(hwndLV, i, INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif

#define ListView_GetSelectedItem(listCtrl)			listCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED | LVIS_FOCUSED)

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupGenre �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDlgSetupGenre, COptionPage)

CDlgSetupGenre::CDlgSetupGenre() : COptionPage(CDlgSetupGenre::IDD)
{
	//{{AFX_DATA_INIT(CDlgSetupGenre)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�̏�����������ǉ����܂��B
	//}}AFX_DATA_INIT
}

CDlgSetupGenre::~CDlgSetupGenre()
{
}

void CDlgSetupGenre::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetupGenre)
	DDX_Control(pDX, IDC_LIST_GENRE, m_listGenre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupGenre, COptionPage)
	//{{AFX_MSG_MAP(CDlgSetupGenre)
	ON_BN_CLICKED(IDC_BT_CHECK_ALL, OnBtCheckAll)
	ON_BN_CLICKED(IDC_BT_UNCHECK_ALL, OnBtUncheckAll)
	ON_BN_CLICKED(IDC_BT_CHECK_ID3, OnBtCheckId3)
	ON_BN_CLICKED(IDC_BT_CHECK_WINAMP, OnBtCheckWinamp)
	ON_BN_CLICKED(IDC_BT_CHECK_SCMPX, OnBtCheckScmpx)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	ON_BN_CLICKED(IDC_BT_ADD_USER, OnBtAddUser)
	ON_BN_CLICKED(IDC_BT_DEL_USER, OnBtDelUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupGenre ���b�Z�[�W �n���h��

BOOL CDlgSetupGenre::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	// �g���X�^�C���̐ݒ�F�`�F�b�N�{�b�N�X�ǉ��A�P�s�I��
	DWORD	dwStyle;
	dwStyle = m_listGenre.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
	m_listGenre.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	RECT	rect;
	m_listGenre.GetClientRect(&rect);

	m_listGenre.InsertColumn(1, TEXT("�W������"), LVCFMT_LEFT, rect.right-rect.left-16, -1);
	m_listGenre.DeleteAllItems();					// �N���A

	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		CString	strText;
		strText.Format(TEXT("%3d�F%s"), pGenre->byGenre, pGenre->sName);
		m_listGenre.InsertItem(nIndex, strText);
		//m_listGenre.SetItemData(nIndex, pGenre->byGenre);
		m_listGenre.SetItemData(nIndex, (DWORD)pGenre);
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, pGenre->bAddList ? TRUE : FALSE);
	}
	AddUserGenre(nIndex);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgSetupGenre::OnBtCheckAll() 
{
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, TRUE);
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == false) {
			continue;
		}
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, TRUE);
		nIndex++;
	}
}

void CDlgSetupGenre::OnBtUncheckAll() 
{
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, FALSE);
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == false) {
			continue;
		}
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, FALSE);
		nIndex++;
	}
}

void CDlgSetupGenre::OnBtCheckId3() 
{
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, (pGenre->byGenre <= 83) ? TRUE : FALSE);
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == false) {
			continue;
		}
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, FALSE);
		nIndex++;
	}
}

void CDlgSetupGenre::OnBtCheckWinamp() 
{
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, (pGenre->byGenre <= 147) ? TRUE : FALSE);
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == false) {
			continue;
		}
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, FALSE);
		nIndex++;
	}
}

void CDlgSetupGenre::OnBtCheckScmpx() 
{
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, TRUE);
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == false) {
			continue;
		}
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, FALSE);
		nIndex++;
	}
}

void CDlgSetupGenre::OnOK() 
{
	// �ҏW��̏�Ԃ�ۑ�
	//int nIndex; for (nIndex = 0; nIndex < m_listGenre.GetItemCount(); nIndex++) {
	if (!::IsWindow(m_hWnd))	return;
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		pGenre->bAddList = ListView_GetCheckState(m_listGenre.GetSafeHwnd(), nIndex) ? true : false;
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (m_listGenre.GetItemCount() > nIndex) {
			USER_GENRE_LIST* pSelectGenre = (USER_GENRE_LIST*)m_listGenre.GetItemData(nIndex);
			pGenre->bAddList = ListView_GetCheckState(m_listGenre.GetSafeHwnd(), nIndex) ? true : false;
			pGenre->bUse = true;
			pGenre->byGenre = pSelectGenre->byGenre;
			pGenre->sName = pSelectGenre->sName;
		} else {
			pGenre->bUse = false;
			pGenre->bAddList = false;
			pGenre->sName = "";
		}
		nIndex++;
	}

	for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		g_genreListUSER[i] = m_genreListUSER[i];
	}
	// �ύX��̃W���������X�g�̏�Ԃ����W�X�g���ɕۑ�
	((CSuperTagEditorApp *)AfxGetApp())->WriteGenreList();

	COptionPage::OnOK();
}

// �����l�ɖ߂�
void CDlgSetupGenre::OnBtResetPage() 
{
	OnBtCheckAll();
}

void CDlgSetupGenre::OnBtAddUser() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CDlgAddUserGenre dialog;
	dialog.m_genreListUSER = m_genreListUSER;
	if (dialog.DoModal() == IDOK) {
	}
	// ���[�U�w��W���������ĕ\��
	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
	}
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		if (m_listGenre.GetItemCount() > nIndex) {
			m_listGenre.DeleteItem(nIndex);
		}
	}
	AddUserGenre(nIndex);
}

void CDlgSetupGenre::OnBtDelUser() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	POSITION pos = m_listGenre.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nIndex = m_listGenre.GetNextSelectedItem(pos);
	// �I������Ă���̂����[�U�W���������`�F�b�N
	int i; for (i = 0; g_genreListSCMPX[i].sName != NULL; i++) {
	}
	if (nIndex >= i) {
		m_listGenre.DeleteItem(nIndex);
	}
}

void CDlgSetupGenre::AddUserGenre(int nIndex)
{
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == false) {
			continue;
		}
		CString	strText;
		CString strDefaultName;
		for (int j = 0; g_genreListSCMPX[j].sName != NULL; j++) {
			GENRE_LIST	*pGenreSCMPX = &g_genreListSCMPX[j];
			if (pGenreSCMPX->byGenre == pGenre->byGenre) {
				strDefaultName = pGenreSCMPX->sName;
				break;
			}
		}
		strText.Format(TEXT("%s �� %s"), pGenre->sName, strDefaultName);
		m_listGenre.InsertItem(nIndex, strText);
		m_listGenre.SetItemData(nIndex, pGenre->byGenre);
		m_listGenre.SetItemData(nIndex, (DWORD)pGenre);
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, pGenre->bAddList ? TRUE : FALSE);
		nIndex++;
	}
}
