// DlgConvID3v2Version.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "step_mp3.h"
#include "DlgConvID3v2Version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConvID3v2Version ダイアログ


CDlgConvID3v2Version::CDlgConvID3v2Version(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConvID3v2Version::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConvID3v2Version)
	m_nId3v2Encode = -1;
	m_nId3v2Version = -1;
	//}}AFX_DATA_INIT
}


void CDlgConvID3v2Version::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConvID3v2Version)
	DDX_Control(pDX, IDC_COMBO_ID3V2_VERSION, m_cbId3v2Version);
	DDX_Control(pDX, IDC_COMBO_ID3V2_ENCODE, m_cbId3v2Encode);
	DDX_CBIndex(pDX, IDC_COMBO_ID3V2_ENCODE, m_nId3v2Encode);
	DDX_CBIndex(pDX, IDC_COMBO_ID3V2_VERSION, m_nId3v2Version);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConvID3v2Version, CDialog)
	//{{AFX_MSG_MAP(CDlgConvID3v2Version)
	ON_CBN_SELCHANGE(IDC_COMBO_ID3V2_VERSION, OnSelchangeComboId3v2Version)
	ON_CBN_SELENDCANCEL(IDC_COMBO_ID3V2_ENCODE, OnSelchangeComboId3v2Version)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConvID3v2Version メッセージ ハンドラ

BOOL CDlgConvID3v2Version::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cbId3v2Encode.AddString(TEXT("変更しない"));
	m_cbId3v2Encode.AddString(TEXT("ISO-8859-1"));
	m_cbId3v2Encode.AddString(TEXT("UTF-16"));
	m_cbId3v2Encode.AddString(TEXT("UTF-8"));
	m_cbId3v2Encode.SetCurSel(m_nId3v2Encode);

	m_cbId3v2Version.AddString(TEXT("変更しない"));
	m_cbId3v2Version.AddString(TEXT("v2.2"));
	m_cbId3v2Version.AddString(TEXT("v2.3"));
	m_cbId3v2Version.AddString(TEXT("v2.4"));
	m_cbId3v2Version.SetCurSel(m_nId3v2Version);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgConvID3v2Version::OnSelchangeComboId3v2Version() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	int nVer = m_cbId3v2Version.GetCurSel();
	int nEnc = m_cbId3v2Encode.GetCurSel();
	if (nVer != 0 && nVer != 3) {
		if (m_cbId3v2Encode.FindStringExact(0, TEXT("UTF-8")) != CB_ERR) {
			m_cbId3v2Encode.DeleteString(3);
			if (nEnc == 3) {
				m_cbId3v2Encode.SetCurSel(2);
			}
		}
	} else {
		if (m_cbId3v2Encode.FindStringExact(0, TEXT("UTF-8")) == CB_ERR) {
			m_cbId3v2Encode.AddString(TEXT("UTF-8"));
		}
	}
	UpdateData(FALSE);
}
