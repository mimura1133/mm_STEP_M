// DlgCopyFormat.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgCopyFormat.h"
#include "DlgNameChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCopyFormat ダイアログ


CDlgCopyFormat::CDlgCopyFormat(CWnd* pParent /*=NULL*/)
	: COptionPage(CDlgCopyFormat::IDD)
{
	//{{AFX_DATA_INIT(CDlgCopyFormat)
	m_nFormatType = -1;
	//}}AFX_DATA_INIT
}


void CDlgCopyFormat::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCopyFormat)
	DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
	DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
	DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCopyFormat, COptionPage)
	//{{AFX_MSG_MAP(CDlgCopyFormat)
	ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
	ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
	ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
	ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
	ON_BN_CLICKED(IDC_BT_FILE_NAME, OnBtFileName)
	ON_BN_CLICKED(IDC_BT_FIX_STRING, OnBtFixString)
	ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
	ON_BN_CLICKED(IDC_BT_NUMBER, OnBtNumber)
	ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
	ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
	ON_BN_CLICKED(IDC_BT_TRACK_NUMBER3, OnBtTrackNumber3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCopyFormat メッセージ ハンドラ
void CDlgCopyFormat::OnSelChangeListFormatType() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// 現在編集中の内容を反映させる
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);

	m_nFormatType = m_listFormatType.GetCurSel();
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
}

void CDlgCopyFormat::OutputFormatString(const char *sText)
{
	m_editFormat.ReplaceSel(sText, FALSE);
	m_editFormat.SetFocus();
}

void CDlgCopyFormat::OnBtAlbumName() 
{
	OutputFormatString("%ALBUM_NAME%");
}

void CDlgCopyFormat::OnBtArtistName() 
{
	OutputFormatString("%ARTIST_NAME%");
}

void CDlgCopyFormat::OnBtComment() 
{
	OutputFormatString("%COMMENT%");
}

void CDlgCopyFormat::OnBtFileName() 
{
	OutputFormatString("%FILE_NAME%");
}

void CDlgCopyFormat::OnBtFixString() 
{
	OutputFormatString("%STRING%");
}

void CDlgCopyFormat::OnBtNumber() 
{
	OutputFormatString("%NUMBER%");
}

void CDlgCopyFormat::OnBtReleaseYear() 
{
	OutputFormatString("%RELEASE_YEAR%");
}

void CDlgCopyFormat::OnBtTrackName() 
{
	OutputFormatString("%TRACK_NAME%");
}

void CDlgCopyFormat::OnBtTrackNumber() 
{
	OutputFormatString("%TRACK_NUMBER%");
}

void CDlgCopyFormat::OnBtTrackNumber2() 
{
	OutputFormatString("%TRACK_NUMBER2%");
}

void CDlgCopyFormat::OnBtTrackNumber3() 
{
	OutputFormatString("%TRACK_NUMBER3%");
}

void CDlgCopyFormat::OnBtNameChange() 
{
	CDlgNameChange	dialog;

	dialog.m_strName = m_userFormatEx[m_nFormatType].strName;
	if (dialog.DoModal() == IDOK) {
		// 新しい名称に変更
		m_userFormatEx[m_nFormatType].strName = dialog.m_strName;

		// リスト更新
		OnSelChangeListFormatType();
		m_listFormatType.ResetContent();
		int i; for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
			CString	strName;
			strName.Format("%d: %s", i+1, m_userFormatEx[i].strName);
			m_listFormatType.AddString(strName);
		}
		// 選択しなおす
		m_listFormatType.SetCurSel(m_nFormatType);
	}
}

BOOL CDlgCopyFormat::OnInitDialog() 
{
	// TODO: この位置に初期化の補足処理を追加してください
	COptionPage::OnInitDialog();

	int		i;
	m_listFormatType.ResetContent();
	for (i = 0; i < USER_COPY_FORMAT_FORMAT_MAX; i++) {
		CString	strName;
		strName.Format("%d: %s", i+1, m_userFormatEx[i].strName);
		m_listFormatType.AddString(strName);
	}

	m_listFormatType.SetCurSel(m_nFormatType);
	m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgCopyFormat::OnOK()
{
	if (!::IsWindow(m_hWnd))	return;
	m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);

	COptionPage::OnOK();

}
