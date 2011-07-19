// DlgFileNameChange.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFileNameChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileNameChange ダイアログ


CDlgFileNameChange::CDlgFileNameChange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileNameChange::IDD, pParent)
{
	m_nMaxChar = 255;
	//{{AFX_DATA_INIT(CDlgFileNameChange)
	m_strFileName = _T("");
	m_strMsg = _T("");
	m_strOrgFileName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFileNameChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileNameChange)
	DDX_Control(pDX, IDC_STATIC_MSG, m_staticMsg);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strFileName);
	DDV_MaxChars(pDX, m_strFileName, m_nMaxChar);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	DDX_Text(pDX, IDC_EDIT_NAME_ORG, m_strOrgFileName);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CDlgFileNameChange, CDialog)
	//{{AFX_MSG_MAP(CDlgFileNameChange)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileNameChange メッセージ ハンドラ

void CDlgFileNameChange::OnChangeEditName() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString strFileName;
	m_editName.GetLine(0, strFileName.GetBuffer(m_editName.LineLength(0)*2+sizeof(DWORD)+2), m_editName.LineLength(0)*2+sizeof(DWORD)+2); /* STEP 010 バッファサイズ増やした *//* なんでかちゃんと取得されない環境があるのでさらに２倍に */
	strFileName.ReleaseBuffer();
	m_strMsg.Format(m_strMsgFormat, m_nMaxChar, strFileName.GetLength());
	m_staticMsg.SetWindowText(m_strMsg);
}

BOOL CDlgFileNameChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	OnChangeEditName();
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
