// DlgUserConvFormat.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgNameChange.h"
#include "DlgUserConvFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUserConvFormat ダイアログ

IMPLEMENT_DYNCREATE(CDlgUserConvFormat, COptionPage)

CDlgUserConvFormat::CDlgUserConvFormat() : COptionPage(CDlgUserConvFormat::IDD)
{
	//{{AFX_DATA_INIT(CDlgUserConvFormat)
	m_nFormatType = -1;
	//}}AFX_DATA_INIT
}

CDlgUserConvFormat::~CDlgUserConvFormat()
{
}


void CDlgUserConvFormat::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUserConvFormat)
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_Control(pDX, IDC_ED_FORMAT_TAG2FILE, m_editTag2File);
	DDX_Control(pDX, IDC_ED_FORMAT_FILE2TAG, m_editFile2Tag);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUserConvFormat, COptionPage)
	//{{AFX_MSG_MAP(CDlgUserConvFormat)
	ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
	ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
	ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
	ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
	ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_EN_SETFOCUS(IDC_ED_FORMAT_FILE2TAG, OnSetfocusEdFormatFile2tag)
	ON_EN_SETFOCUS(IDC_ED_FORMAT_TAG2FILE, OnSetfocusEdFormatTag2file)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER3, OnBtTrackNumber3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUserConvFormat メッセージ ハンドラ

void CDlgUserConvFormat::OutputFormatString(LPCTSTR sText)
{
	CEdit	*pEdit = m_nOutputFormatType ? &m_editTag2File : &m_editFile2Tag;
	pEdit->ReplaceSel(sText, FALSE);
	pEdit->SetFocus();
}

void CDlgUserConvFormat::OnBtTrackName() 
{
	OutputFormatString(TEXT("%TRACK_NAME%"));
}

void CDlgUserConvFormat::OnBtArtistName() 
{
	OutputFormatString(TEXT("%ARTIST_NAME%"));
}

void CDlgUserConvFormat::OnBtAlbumName() 
{
	OutputFormatString(TEXT("%ALBUM_NAME%"));
}

void CDlgUserConvFormat::OnBtReleaseYear() 
{
	OutputFormatString(TEXT("%RELEASE_YEAR%"));
}

void CDlgUserConvFormat::OnBtTrackNumber() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER%"));
}

void CDlgUserConvFormat::OnBtTrackNumber2() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER2%"));
}

void CDlgUserConvFormat::OnBtTrackNumber3() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER3%"));
}

void CDlgUserConvFormat::OnBtComment() 
{
	OutputFormatString(TEXT("%COMMENT%"));
}

void CDlgUserConvFormat::OnSetfocusEdFormatFile2tag() 
{
	m_nOutputFormatType = 0;
}

void CDlgUserConvFormat::OnSetfocusEdFormatTag2file() 
{
	m_nOutputFormatType = 1;
}

void CDlgUserConvFormat::OnBtNameChange() 
{
	CDlgNameChange	dialog;

	dialog.m_strName = m_userFormat[m_nFormatType].strName;
	if (dialog.DoModal() == IDOK) {
		// 新しい名称に変更
		m_userFormat[m_nFormatType].strName = dialog.m_strName;

		// リスト更新
		OnSelChangeListFormatType();
		m_listFormatType.ResetContent();
		int i; for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
			CString	strName;
			strName.Format(TEXT("%d: %s"), i+1, m_userFormat[i].strName);
			m_listFormatType.AddString(strName);
		}
		// 選択しなおす
		m_listFormatType.SetCurSel(m_nFormatType);
	}
}

void CDlgUserConvFormat::OnSelChangeListFormatType() 
{
	// 現在編集中の内容を反映させる
	m_editTag2File.GetWindowText(m_userFormat[m_nFormatType].strTag2File);
	m_editFile2Tag.GetWindowText(m_userFormat[m_nFormatType].strFile2Tag);

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editTag2File.SetWindowText(m_userFormat[m_nFormatType].strTag2File);
	m_editFile2Tag.SetWindowText(m_userFormat[m_nFormatType].strFile2Tag);
}

BOOL CDlgUserConvFormat::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
		CString	strName;
		strName.Format(TEXT("%d: %s"), i+1, m_userFormat[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editTag2File.SetWindowText(m_userFormat[m_nFormatType].strTag2File);
	m_editFile2Tag.SetWindowText(m_userFormat[m_nFormatType].strFile2Tag);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgUserConvFormat::OnOK() 
{
	if (!::IsWindow(m_hWnd))	return;
	m_editTag2File.GetWindowText(m_userFormat[m_nFormatType].strTag2File);
	m_editFile2Tag.GetWindowText(m_userFormat[m_nFormatType].strFile2Tag);

	COptionPage::OnOK();
}

