// DlgDefaultValue.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "STEP_mp3.h"
#include "DlgDefaultValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultValue プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgDefaultValue, CPropertyPage)

CDlgDefaultValue::CDlgDefaultValue() : CPropertyPage(CDlgDefaultValue::IDD)
{
	//{{AFX_DATA_INIT(CDlgDefaultValue)
	m_strSoftwareTag = _T("");
	m_bID3v2UnSync = FALSE;
	m_bID3v2UnSyncNew = FALSE;
	m_nId3v2Encode = -1;
	m_nId3v2Version = -1;
	m_strInfo = _T("");
	m_nId3v2VersionNew = -1;
	m_nId3v2EncodeNew = -1;
	//}}AFX_DATA_INIT
}

CDlgDefaultValue::~CDlgDefaultValue()
{
}

void CDlgDefaultValue::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDefaultValue)
	DDX_Control(pDX, IDC_COMBO_ID3V2_ENCODE_NEW, m_cbId3v2EncodeNew);
	DDX_Control(pDX, IDC_COMBO_ID3V2_VERSION_NEW, m_cbId3v2VersionNew);
	DDX_Control(pDX, IDC_COMBO_ID3V2_ENCODE, m_cbId3v2Encode);
	DDX_Control(pDX, IDC_COMBO_ID3V2_VERSION, m_cbId3v2Version);
	DDX_Text(pDX, IDC_SOFTWARE_TAG, m_strSoftwareTag);
	DDV_MaxChars(pDX, m_strSoftwareTag, 255);
	DDX_Check(pDX, IDC_CHECK_UNSYNC, m_bID3v2UnSync);
	DDX_Check(pDX, IDC_CHECK_UNSYNC_NEW, m_bID3v2UnSyncNew);
	DDX_CBIndex(pDX, IDC_COMBO_ID3V2_ENCODE, m_nId3v2Encode);
	DDX_CBIndex(pDX, IDC_COMBO_ID3V2_VERSION, m_nId3v2Version);
	DDX_Text(pDX, IDC_STATIC_INFO, m_strInfo);
	DDX_CBIndex(pDX, IDC_COMBO_ID3V2_VERSION_NEW, m_nId3v2VersionNew);
	DDX_CBIndex(pDX, IDC_COMBO_ID3V2_ENCODE_NEW, m_nId3v2EncodeNew);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDefaultValue, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgDefaultValue)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	ON_CBN_SELCHANGE(IDC_COMBO_ID3V2_VERSION, OnSelchangeComboId3v2Version)
	ON_CBN_SELCHANGE(IDC_COMBO_ID3V2_ENCODE, OnSelchangeComboId3v2Version)
	ON_CBN_SELCHANGE(IDC_COMBO_ID3V2_VERSION_NEW, OnSelchangeComboId3v2VersionNew)
	ON_CBN_SELCHANGE(IDC_COMBO_ID3V2_ENCODE_NEW, OnSelchangeComboId3v2VersionNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDefaultValue メッセージ ハンドラ

void CDlgDefaultValue::OnBtResetPage() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_strSoftwareTag = "";
	m_bID3v2UnSync = FALSE;
	m_bID3v2UnSyncNew = FALSE;
	m_cbId3v2Version.SetCurSel(0);
	m_cbId3v2Encode.SetCurSel(0);
	m_cbId3v2VersionNew.SetCurSel(1);
	m_cbId3v2EncodeNew.SetCurSel(0);
	m_nId3v2Version = 0;
	m_nId3v2Encode = 0;
	m_nId3v2VersionNew = 1;
	m_nId3v2EncodeNew = 0;
	UpdateData(FALSE);
}

BOOL CDlgDefaultValue::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
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

	m_cbId3v2EncodeNew.AddString(TEXT("ISO-8859-1"));
	m_cbId3v2EncodeNew.AddString(TEXT("UTF-16"));
	m_cbId3v2EncodeNew.AddString(TEXT("UTF-8"));
	m_cbId3v2EncodeNew.SetCurSel(m_nId3v2EncodeNew);

	m_cbId3v2VersionNew.AddString(TEXT("v2.2"));
	m_cbId3v2VersionNew.AddString(TEXT("v2.3"));
	m_cbId3v2VersionNew.AddString(TEXT("v2.4"));
	m_cbId3v2VersionNew.SetCurSel(m_nId3v2VersionNew);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgDefaultValue::OnSelchangeComboId3v2Version() 
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

void CDlgDefaultValue::OnSelchangeComboId3v2VersionNew() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	int nVer = m_cbId3v2VersionNew.GetCurSel();
	int nEnc = m_cbId3v2EncodeNew.GetCurSel();
	if (nVer != 2) {
		if (m_cbId3v2EncodeNew.FindStringExact(0, TEXT("UTF-8")) != CB_ERR) {
			m_cbId3v2EncodeNew.DeleteString(2);
			if (nEnc == 2) {
				m_cbId3v2EncodeNew.SetCurSel(1);
			}
		}
	} else {
		if (m_cbId3v2EncodeNew.FindStringExact(0, TEXT("UTF-8")) == CB_ERR) {
			m_cbId3v2EncodeNew.AddString(TEXT("UTF-8"));
		}
	}
	UpdateData(FALSE);
}
