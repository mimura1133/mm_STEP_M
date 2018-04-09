// DlgUserConvFormartTag2Tag.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgUserConvFormartTag2Tag.h"
#include "DlgNameChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUserConvFormartTag2Tag ダイアログ

IMPLEMENT_DYNCREATE(CDlgUserConvFormartTag2Tag, COptionPage)

CDlgUserConvFormartTag2Tag::CDlgUserConvFormartTag2Tag() : COptionPage(CDlgUserConvFormartTag2Tag::IDD)
{
	//{{AFX_DATA_INIT(CDlgUserConvFormartTag2Tag)
	m_nFormatType = -1;
	//}}AFX_DATA_INIT
}

CDlgUserConvFormartTag2Tag::~CDlgUserConvFormartTag2Tag()
{
}

void CDlgUserConvFormartTag2Tag::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUserConvFormartTag2Tag)
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_Control(pDX, IDC_ED_FORMAT, m_editFormat);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUserConvFormartTag2Tag, COptionPage)
	//{{AFX_MSG_MAP(CDlgUserConvFormartTag2Tag)
	ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
	ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
	ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
	ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER3, OnBtTrackNumber3)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUserConvFormartTag2Tag メッセージ ハンドラ
void CDlgUserConvFormartTag2Tag::OutputFormatString(LPCTSTR sText)
{
	m_editFormat.ReplaceSel(sText, FALSE);
	m_editFormat.SetFocus();
}

void CDlgUserConvFormartTag2Tag::OnBtAlbumName() 
{
	OutputFormatString(TEXT("%ALBUM_NAME%"));
}

void CDlgUserConvFormartTag2Tag::OnBtArtistName() 
{
	OutputFormatString(TEXT("%ARTIST_NAME%"));
}

void CDlgUserConvFormartTag2Tag::OnBtComment() 
{
	OutputFormatString(TEXT("%COMMENT%"));
}

void CDlgUserConvFormartTag2Tag::OnBtReleaseYear() 
{
	OutputFormatString(TEXT("%RELEASE_YEAR%"));
}

void CDlgUserConvFormartTag2Tag::OnBtTrackName() 
{
	OutputFormatString(TEXT("%TRACK_NAME%"));
}

void CDlgUserConvFormartTag2Tag::OnBtTrackNumber() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER%"));
}

void CDlgUserConvFormartTag2Tag::OnBtTrackNumber2() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER2%"));
}

void CDlgUserConvFormartTag2Tag::OnBtTrackNumber3() 
{
	OutputFormatString(TEXT("%TRACK_NUMBER3%"));
}

BOOL CDlgUserConvFormartTag2Tag::OnInitDialog() 
{
	COptionPage::OnInitDialog();
	
	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < USER_CONV_FORMAT_TAG2TAG_MAX; i++) {
		CString	strName;
		strName.Format(TEXT("%d: %s"), i+1, m_userFormatTag2Tag[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editFormat.SetWindowText(m_userFormatTag2Tag[m_nFormatType].strFormat);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgUserConvFormartTag2Tag::OnBtNameChange() 
{
	CDlgNameChange	dialog;

	dialog.m_strName = m_userFormatTag2Tag[m_nFormatType].strName;
	if (dialog.DoModal() == IDOK) {
		// 新しい名称に変更
		m_userFormatTag2Tag[m_nFormatType].strName = dialog.m_strName;

		// リスト更新
		OnSelChangeListFormatType();
		m_listFormatType.ResetContent();
		int i; for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
			CString	strName;
			strName.Format(TEXT("%d: %s"), i+1, m_userFormatTag2Tag[i].strName);
			m_listFormatType.AddString(strName);
		}
		// 選択しなおす
		m_listFormatType.SetCurSel(m_nFormatType);
	}
}

void CDlgUserConvFormartTag2Tag::OnSelChangeListFormatType() 
{
	// 現在編集中の内容を反映させる
	m_editFormat.GetWindowText(m_userFormatTag2Tag[m_nFormatType].strFormat);

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editFormat.SetWindowText(m_userFormatTag2Tag[m_nFormatType].strFormat);
}

void CDlgUserConvFormartTag2Tag::OnOK() 
{
	if (!::IsWindow(m_hWnd))	return;
	m_editFormat.GetWindowText(m_userFormatTag2Tag[m_nFormatType].strFormat);

	COptionPage::OnOK();
}
