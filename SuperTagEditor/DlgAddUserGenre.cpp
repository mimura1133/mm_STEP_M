// DlgAddUserGenre.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "FileMP3.h"
#include "DlgAddUserGenre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUserGenre ダイアログ


CDlgAddUserGenre::CDlgAddUserGenre(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddUserGenre::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddUserGenre)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgAddUserGenre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddUserGenre)
	DDX_Control(pDX, IDC_BT_ADD, m_cBtAdd);
	DDX_Control(pDX, IDC_LIST_GENRE, m_listGenre);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddUserGenre, CDialog)
	//{{AFX_MSG_MAP(CDlgAddUserGenre)
	ON_BN_CLICKED(IDC_BT_ADD, OnBtAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GENRE, OnItemchangedListGenre)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUserGenre メッセージ ハンドラ

BOOL CDlgAddUserGenre::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	DWORD	dwStyle;
	dwStyle = m_listGenre.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |=LVS_EX_FULLROWSELECT;
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
		m_listGenre.SetItemData(nIndex, pGenre->byGenre);
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgAddUserGenre::OnBtAdd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(true);
	if (m_strName.GetLength() == 0) {
		return;
	}
	POSITION pos = m_listGenre.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nIndex = m_listGenre.GetNextSelectedItem(pos);
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == true) {
			if (pGenre->sName != m_strName)	continue;
		}
		pGenre->bAddList = true;
		pGenre->bUse = true;
		pGenre->sName = m_strName;
		pGenre->byGenre = g_genreListSCMPX[nIndex].byGenre;
		CDialog::OnOK();
		break;
	}
}


void CDlgAddUserGenre::OnItemchangedListGenre(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EnableButton();

	*pResult = 0;
}

void CDlgAddUserGenre::EnableButton()
{
	UpdateData(true);

	BOOL bEnable = FALSE;
	int nItem = m_listGenre.GetNextItem(-1, LVNI_SELECTED);
	if (nItem > -1 && m_strName.IsEmpty() != TRUE) {
		bEnable = TRUE;
	} else {
		bEnable = FALSE;
	}
	m_cBtAdd.EnableWindow(bEnable);
}

void CDlgAddUserGenre::OnChangeEditName() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EnableButton();
}

void CDlgAddUserGenre::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
}
