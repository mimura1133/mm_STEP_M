// DlgUnifyChar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgUnifyChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUnifyChar ダイアログ


CDlgUnifyChar::CDlgUnifyChar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUnifyChar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUnifyChar)
	m_nConvAlpha = -1;
	m_nConvHiraKata = -1;
	m_nConvKata = -1;
	m_nConvKigou = -1;
	m_nConvSuji = -1;
	m_nConvUpLow = -1;
	m_nConvFixedUpLow = -1;
	//}}AFX_DATA_INIT
}


void CDlgUnifyChar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUnifyChar)
	DDX_Radio(pDX, IDC_CONV_ALPHA_NONE, m_nConvAlpha);
	DDX_Radio(pDX, IDC_CONV_HIRAKATA_NONE, m_nConvHiraKata);
	DDX_Radio(pDX, IDC_CONV_KATA_NONE, m_nConvKata);
	DDX_Radio(pDX, IDC_CONV_KIGOU_NONE, m_nConvKigou);
	DDX_Radio(pDX, IDC_CONV_SUJI_NONE, m_nConvSuji);
	DDX_Radio(pDX, IDC_CONV_UPLOW_NONE, m_nConvUpLow);
	DDX_Radio(pDX, IDC_CONV_FIXED_UPPER_LOWER_NONE, m_nConvFixedUpLow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUnifyChar, CDialog)
	//{{AFX_MSG_MAP(CDlgUnifyChar)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUnifyChar メッセージ ハンドラ
