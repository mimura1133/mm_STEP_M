// DlgSetupGenre.cpp : インプリメンテーション ファイル
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
// CDlgSetupGenre プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgSetupGenre, COptionPage)

CDlgSetupGenre::CDlgSetupGenre() : COptionPage(CDlgSetupGenre::IDD)
{
	//{{AFX_DATA_INIT(CDlgSetupGenre)
		// メモ - ClassWizard はこの位置にメンバの初期化処理を追加します。
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
// CDlgSetupGenre メッセージ ハンドラ

BOOL CDlgSetupGenre::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	// 拡張スタイルの設定：チェックボックス追加、１行選択
	DWORD	dwStyle;
	dwStyle = m_listGenre.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
	m_listGenre.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	RECT	rect;
	m_listGenre.GetClientRect(&rect);

	m_listGenre.InsertColumn(1, TEXT("ジャンル"), LVCFMT_LEFT, rect.right-rect.left-16, -1);
	m_listGenre.DeleteAllItems();					// クリア

	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		CString	strText;
		strText.Format(TEXT("%3d：%s"), pGenre->byGenre, pGenre->sName);
		m_listGenre.InsertItem(nIndex, strText);
		//m_listGenre.SetItemData(nIndex, pGenre->byGenre);
		m_listGenre.SetItemData(nIndex, (DWORD)pGenre);
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, pGenre->bAddList ? TRUE : FALSE);
	}
	AddUserGenre(nIndex);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
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
	// 編集後の状態を保存
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
	// 変更後のジャンルリストの状態をレジストリに保存
	((CSuperTagEditorApp *)AfxGetApp())->WriteGenreList();

	COptionPage::OnOK();
}

// 初期値に戻す
void CDlgSetupGenre::OnBtResetPage() 
{
	OnBtCheckAll();
}

void CDlgSetupGenre::OnBtAddUser() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CDlgAddUserGenre dialog;
	dialog.m_genreListUSER = m_genreListUSER;
	if (dialog.DoModal() == IDOK) {
	}
	// ユーザ指定ジャンルを再表示
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
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	POSITION pos = m_listGenre.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nIndex = m_listGenre.GetNextSelectedItem(pos);
	// 選択されているのがユーザジャンルかチェック
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
		strText.Format(TEXT("%s → %s"), pGenre->sName, strDefaultName);
		m_listGenre.InsertItem(nIndex, strText);
		m_listGenre.SetItemData(nIndex, pGenre->byGenre);
		m_listGenre.SetItemData(nIndex, (DWORD)pGenre);
		ListView_SetCheckState(m_listGenre.GetSafeHwnd(), nIndex, pGenre->bAddList ? TRUE : FALSE);
		nIndex++;
	}
}
