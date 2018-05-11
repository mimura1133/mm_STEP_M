//  DlgCommonProg.cpp : �C���v�������e�[�V���� �t�@�C��
// CG: ���̃t�@�C���́u�v���O���X �_�C�A���O�v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B

#include "stdafx.h"
#include "resource.h"
#include "DlgCommonProg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCommonProgress �_�C�A���O

CDlgCommonProgress::CDlgCommonProgress(UINT nCaptionID)
{
	m_nCaptionID = CG_IDS_PROGRESS_CAPTION;
	if (nCaptionID != 0)
		m_nCaptionID = nCaptionID;

    m_nLower=0;
    m_nUpper=100;
    m_nStep=10;
    //{{AFX_DATA_INIT(CDlgCommonProgress)
    // ����: ClassWizard �́A���̈ʒu�Ƀ����o�̏������R�[�h��ǉ����܂��B
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;
    m_bCanceled = FALSE;//�ǉ� by Kobarin
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
    // �_�C�A���O�̎��ۂ̐e�E�B���h�E���擾���܂��B
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled �́A���̃_�C�A���O���j�����ꂽ���ɁA�e�E�B���h�E��
    // �ĂїL���ɂ��邽�߂Ɏg�p���܂��B�]���āA���̎��_�Őe�E�B���h�E�����ł�
    // �L���ȏꍇ�̂݁A���̕ϐ��� TRUE ��ݒ肵�܂��B

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
	//�ǉ� by Kobarin
	if (MessageBox(TEXT("���f���Ă���낵���ł����H"), TEXT("�����̒��f"), MB_ICONQUESTION|MB_YESNOCANCEL|MB_DEFBUTTON2|MB_TOPMOST) == IDYES) {
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
    m_nStep = nStep; // ��Ńp�[�Z���e�[�W�̌v�Z���s���ۂɗ��p���܂��B
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
    // �_�C�A���O���g�p����O�� Create() ���Ă�ł��������B
	if (GetSafeHwnd() == NULL) return;
//    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // �_�C�A���O ���b�Z�[�W�̏���
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
    ASSERT(nDivisor>0);  // m_nLower �� m_nUpper �����łȂ���΂����܂���B

    int nDividend = (nNewPos - m_nLower);
    ASSERT(nDividend>=0);   // ���݈ʒu�� m_nLower �ȏ�łȂ���΂����܂���B

    nPercent = nDividend * 100 / nDivisor;

    // �v���O���X �R���g���[���͐܂�Ԃ��\�Ȃ̂ŁA����ɉ����ăp�[�Z���e�[�W��
    // �܂�Ԃ��悤�ɂ��܂��B�������A��]�����ہA100% �� 0% �Ƃ��Ă͂����܂���B
    if(nPercent!=100)
      nPercent %= 100;

    // �p�[�Z���e�[�W��\�����܂��B
    CString strBuf;
    strBuf.Format(_T("%d%c"),nPercent,_T('%'));

	CString strCur; // ���݂̃p�[�Z���e�[�W���擾
    pWndPercent->GetWindowText(strCur);

	if (strCur != strBuf)
		pWndPercent->SetWindowText(strBuf);
}
    
/////////////////////////////////////////////////////////////////////////////
// CDlgCommonProgress ���b�Z�[�W �n���h��

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
