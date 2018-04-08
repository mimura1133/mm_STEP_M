//  DlgCommonProg.cpp : インプリメンテーション ファイル
// CG: このファイルは「プログレス ダイアログ」コンポーネントにより追加されています。

#include "stdafx.h"
#include "resource.h"
#include "DlgCommonProg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCommonProgress ダイアログ

CDlgCommonProgress::CDlgCommonProgress(UINT nCaptionID)
{
	m_nCaptionID = CG_IDS_PROGRESS_CAPTION;
	if (nCaptionID != 0)
		m_nCaptionID = nCaptionID;

    m_nLower=0;
    m_nUpper=100;
    m_nStep=10;
    //{{AFX_DATA_INIT(CDlgCommonProgress)
    // メモ: ClassWizard は、この位置にメンバの初期化コードを追加します。
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;
    m_bCanceled = FALSE;//追加 by Kobarin
}

CDlgCommonProgress::~CDlgCommonProgress()
{
    if(m_hWnd!=NULL)
      DestroyWindow();
}

BOOL CDlgCommonProgress::DestroyWindow()
{
    PumpMessages();
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CDlgCommonProgress::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

void CDlgCommonProgress::EnableCancelButton(BOOL bEnable)
{
	GetDlgItem(IDCANCEL)->EnableWindow(bEnable);
}

BOOL CDlgCommonProgress::Create(CWnd *pParent)
{
    // ダイアログの実際の親ウィンドウを取得します。
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled は、このダイアログが破棄された時に、親ウィンドウを
    // 再び有効にするために使用します。従って、この時点で親ウィンドウがすでに
    // 有効な場合のみ、この変数に TRUE を設定します。

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
      m_pParentWnd->EnableWindow(FALSE);
      m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CDlgCommonProgress::IDD,pParent))
    {
      ReEnableParent();
      return FALSE;
    }

    return TRUE;
}

void CDlgCommonProgress::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgCommonProgress)
    DDX_Control(pDX, CG_IDC_PROGDLG_PROGRESS, m_Progress);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgCommonProgress, CDialog)
    //{{AFX_MSG_MAP(CDlgCommonProgress)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgCommonProgress::OnCancel()
{
	//追加 by Kobarin
	if (MessageBox(TEXT("中断してもよろしいですか？"), TEXT("処理の中断"), MB_ICONQUESTION|MB_YESNOCANCEL|MB_DEFBUTTON2|MB_TOPMOST) == IDYES) {
		m_bCanceled = TRUE;
	}
}

void CDlgCommonProgress::SetRange(int nLower,int nUpper)
{
    m_nLower = nLower;
    m_nUpper = nUpper;
    m_Progress.SetRange(nLower,nUpper);
}
  
int CDlgCommonProgress::SetPos(int nPos)
{
    int iResult = TRUE;
    PumpMessages();
	if (GetSafeHwnd() != NULL) {
		m_Progress.SetPos(nPos);
		UpdatePercent(nPos);
	}
    return iResult;
}

int CDlgCommonProgress::SetStep(int nStep)
{
    m_nStep = nStep; // 後でパーセンテージの計算を行う際に利用します。
    return m_Progress.SetStep(nStep);
}

int CDlgCommonProgress::OffsetPos(int nPos)
{
    PumpMessages();
    int iResult = m_Progress.OffsetPos(nPos);
    UpdatePercent(iResult+nPos);
    return iResult;
}

int CDlgCommonProgress::StepIt()
{
    PumpMessages();
    int iResult = m_Progress.StepIt();
    UpdatePercent(iResult+m_nStep);
    return iResult;
}

void CDlgCommonProgress::PumpMessages()
{
    // ダイアログを使用する前に Create() を呼んでください。
	if (GetSafeHwnd() == NULL) return;
//    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // ダイアログ メッセージの処理
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
		if (GetSafeHwnd() == NULL) break;
      }
    }
}


void CDlgCommonProgress::UpdatePercent(int nNewPos)
{
    CWnd *pWndPercent = GetDlgItem(CG_IDC_PROGDLG_PERCENT);
    int nPercent;
    
    int nDivisor = m_nUpper - m_nLower;
    ASSERT(nDivisor>0);  // m_nLower は m_nUpper 未満でなければいけません。

    int nDividend = (nNewPos - m_nLower);
    ASSERT(nDividend>=0);   // 現在位置は m_nLower 以上でなければいけません。

    nPercent = nDividend * 100 / nDivisor;

    // プログレス コントロールは折り返し可能なので、それに応じてパーセンテージも
    // 折り返すようにします。ただし、剰余を取る際、100% を 0% としてはいけません。
    if(nPercent!=100)
      nPercent %= 100;

    // パーセンテージを表示します。
    CString strBuf;
    strBuf.Format(_T("%d%c"),nPercent,_T('%'));

	CString strCur; // 現在のパーセンテージを取得
    pWndPercent->GetWindowText(strCur);

	if (strCur != strBuf)
		pWndPercent->SetWindowText(strBuf);
}
    
/////////////////////////////////////////////////////////////////////////////
// CDlgCommonProgress メッセージ ハンドラ

BOOL CDlgCommonProgress::OnInitDialog() 
{
    CDialog::OnInitDialog();
    m_Progress.SetRange(m_nLower,m_nUpper);
    m_Progress.SetStep(m_nStep);
    m_Progress.SetPos(m_nLower);

	CString strCaption;
	VERIFY(strCaption.LoadString(m_nCaptionID));
    SetWindowText(strCaption);

    return TRUE;  
}
