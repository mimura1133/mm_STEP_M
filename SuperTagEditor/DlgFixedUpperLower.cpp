// DlgFixedUpperLower.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFixedUpperLower.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedUpperLower プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgFixedUpperLower, COptionPage)

CDlgFixedUpperLower::CDlgFixedUpperLower() : COptionPage(CDlgFixedUpperLower::IDD)
{
	//{{AFX_DATA_INIT(CDlgFixedUpperLower)
	m_strFixedWord = _T("");
	//}}AFX_DATA_INIT
}

CDlgFixedUpperLower::~CDlgFixedUpperLower()
{
}

void CDlgFixedUpperLower::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFixedUpperLower)
	DDX_Control(pDX, IDC_EDIT_FIXED_WORD, m_editFixedWord);
	DDX_Control(pDX, IDC_BT_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BT_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_LIST_UPPER_LOWER, m_listUpperLower);
	DDX_Text(pDX, IDC_EDIT_FIXED_WORD, m_strFixedWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFixedUpperLower, COptionPage)
	//{{AFX_MSG_MAP(CDlgFixedUpperLower)
	ON_BN_CLICKED(IDC_BT_ADD, OnBtAdd)
	ON_EN_CHANGE(IDC_EDIT_FIXED_WORD, OnChangeEditFixedWord)
	ON_BN_CLICKED(IDC_BT_DELETE, OnBtDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UPPER_LOWER, OnItemchangedListUpperLower)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedUpperLower メッセージ ハンドラ

void CDlgFixedUpperLower::OnBtAdd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);
	m_listUpperLower.InsertItem(m_listUpperLower.GetItemCount(), m_strFixedWord);
	m_arFixedWords.Add(m_strFixedWord);
	m_strFixedWord.Empty();
	UpdateData(FALSE);
	EnableButton();
	m_buttonAdd.EnableWindow(FALSE);
	m_bModify = TRUE;
}

BOOL CDlgFixedUpperLower::OnInitDialog() 
{
	COptionPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	DWORD	dwStyle;
	dwStyle = m_listUpperLower.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |=LVS_EX_FULLROWSELECT;
	m_listUpperLower.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	m_bModify = FALSE;
	
	RECT	rect;
	m_listUpperLower.GetClientRect(&rect);
	m_listUpperLower.InsertColumn(1, TEXT("固定単語"), LVCFMT_LEFT, rect.right-rect.left-16, -1);
	m_listUpperLower.DeleteAllItems();					// クリア
	for (int i=0;i<m_arFixedWords.GetSize();i++) {
		m_listUpperLower.InsertItem(i, m_arFixedWords.GetAt(i));
	}

	m_buttonAdd.EnableWindow(FALSE);
	EnableButton();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgFixedUpperLower::OnChangeEditFixedWord() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために COptionPage::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString strWord;
	m_editFixedWord.GetWindowText(strWord);
	if (strWord.IsEmpty()) {
		m_buttonAdd.EnableWindow(FALSE);
	} else {
		m_buttonAdd.EnableWindow(TRUE);
	}
}

void CDlgFixedUpperLower::EnableButton()
{
	if (m_listUpperLower.GetSelectedCount() == 0) {
		m_buttonDelete.EnableWindow(FALSE);
	} else {
		m_buttonDelete.EnableWindow(TRUE);
	}
}

void CDlgFixedUpperLower::OnItemchangedListUpperLower(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EnableButton();

	*pResult = 0;
}

void CDlgFixedUpperLower::OnBtDelete() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	POSITION pos = m_listUpperLower.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nIndex = m_listUpperLower.GetNextSelectedItem(pos);
	m_listUpperLower.DeleteItem(nIndex);
	m_arFixedWords.RemoveAt(nIndex);
	m_bModify = TRUE;
}
