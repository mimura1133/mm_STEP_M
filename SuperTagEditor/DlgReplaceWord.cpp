// DlgReplaceWord.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "SuperTagEditor.h"
#include "SuperTagEditorDoc.h"
#include "SuperTagEditorView.h"
#include "MySuperGrid.h"
#include "DlgReplaceWord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReplaceWord �_�C�A���O


CDlgReplaceWord::CDlgReplaceWord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReplaceWord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReplaceWord)
	m_bCheckDiffUL = FALSE;
	m_bMatchComplete = FALSE;
	m_bRangeSelected = FALSE;
	m_nTargetColumn = -1;
	m_bRegExp = FALSE;
	m_strReplaceWord = _T("");
	m_strSearchWord = _T("");
	//}}AFX_DATA_INIT
	m_pList = NULL;
}


void CDlgReplaceWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReplaceWord)
	DDX_Control(pDX, IDC_ED_REPLACE_WORD, m_listReplaceWord);
	DDX_Control(pDX, IDC_ED_SEARCH_WORD, m_listSearchWord);
	DDX_Control(pDX, IDC_ST_REPLACE, m_stReplace);
	DDX_Control(pDX, IDOK, m_btAllReplace);
	DDX_Control(pDX, ID_BT_REPLACE, m_btReplace);
	DDX_Control(pDX, IDC_CH_REGEXP, m_btRegExp);
	DDX_Control(pDX, IDC_CH_RANGE_SELECTED, m_btRangeSelected);
	DDX_Control(pDX, IDC_CH_MATCH_COMPLETE, m_btMatchComplete);
	DDX_Control(pDX, IDC_CH_CHECK_UL, m_btCheckDiffUL);
	DDX_Control(pDX, IDC_LIST_TARGET, m_listTargetColumn);
	DDX_Check(pDX, IDC_CH_CHECK_UL, m_bCheckDiffUL);
	DDX_Check(pDX, IDC_CH_MATCH_COMPLETE, m_bMatchComplete);
	DDX_Check(pDX, IDC_CH_RANGE_SELECTED, m_bRangeSelected);
	DDX_CBIndex(pDX, IDC_LIST_TARGET, m_nTargetColumn);
	DDX_Check(pDX, IDC_CH_REGEXP, m_bRegExp);
	DDX_CBString(pDX, IDC_ED_REPLACE_WORD, m_strReplaceWord);
	DDX_CBString(pDX, IDC_ED_SEARCH_WORD, m_strSearchWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReplaceWord, CDialog)
	//{{AFX_MSG_MAP(CDlgReplaceWord)
	ON_BN_CLICKED(ID_BT_SEARCH_BEFORE, OnBtSearchBefore)
	ON_BN_CLICKED(ID_BT_SEARCH_NEXT, OnBtSearchNext)
	ON_BN_CLICKED(ID_BT_REPLACE, OnBtReplace)
	ON_CBN_SELCHANGE(IDC_LIST_TARGET, OnSelChangeListTarget)
	ON_BN_CLICKED(IDC_CH_REGEXP, OnChRegexp)
	ON_BN_CLICKED(IDC_CH_RANGE_SELECTED, OnChRangeSelected)
	ON_BN_DOUBLECLICKED(ID_BT_SEARCH_BEFORE, OnBtSearchBefore)
	ON_BN_DOUBLECLICKED(ID_BT_SEARCH_NEXT, OnBtSearchNext)
	ON_BN_DOUBLECLICKED(ID_BT_REPLACE, OnBtReplace)
	ON_BN_DOUBLECLICKED(IDC_CH_REGEXP, OnChRegexp)
	ON_BN_DOUBLECLICKED(IDC_CH_RANGE_SELECTED, OnChRangeSelected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReplaceWord ���b�Z�[�W �n���h��

BOOL CDlgReplaceWord::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	extern	const TCHAR	***g_sNameList;
	// m_listTargetColumn.AddString("�S�Ă̍���");
	for (int i = 2; g_sNameList[0][i] != NULL; i++) {
		m_listTargetColumn.AddString(g_sNameList[0][i]);
	}
	/*m_listTargetColumn.AddString("(�S����)");*/
	m_listTargetColumn.SetCurSel(m_nTargetColumn);

	SetWindowText(m_bModeReplace ? TEXT("�u��") : TEXT("����"));

	UpdateStatus();

	if (m_rect.top != -1 && m_rect.left != -1) { /* WildCherry4 086 */
		CRect rect;
		GetWindowRect(rect);
		MoveWindow(m_rect.left, m_rect.top, rect.Width(), rect.Height());
	}

	{ /* BeachMonster 088 */
		m_strSearchWord = "";
		m_strReplaceWord = "";
		UpdateData(FALSE);
		m_listSearchWord.LoadHistory(TEXT("haseta\\history"), TEXT("SearchWord"));
		m_listReplaceWord.LoadHistory(TEXT("haseta\\history"), TEXT("ReplaceWord"));
		m_bAddCurrentItemtoHistory = false;
	}
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgReplaceWord::UpdateStatus(void)
{
	BOOL	bReplace = m_bModeReplace;

	/* �����������s�P�ʂȂ̂Œ��r���[
	if (m_nTargetColumn >= 0) {
		CString strSelText;
		m_listTargetColumn.GetLBText(m_nTargetColumn, strSelText);
		if (strSelText == "(�S����)") {
			m_nTargetColumn = -1;
			UpdateData(FALSE);
		}
	}
	*/

	if (
			((m_nTargetColumn == COLUMN_TRACK_NUMBER-COLUMN_FILE_NAME) |
			(m_nTargetColumn == COLUMN_DISK_NUMBER-COLUMN_FILE_NAME))
		&& FALSE/* BeachMonster 105 */) {
		m_btCheckDiffUL.EnableWindow(FALSE);
		m_btRegExp.EnableWindow(FALSE);
		m_btMatchComplete.EnableWindow(FALSE);
		bReplace = FALSE;				// �u���͕s��
	} else {
		m_btCheckDiffUL.EnableWindow(m_bRegExp ? FALSE : TRUE);
		m_btRegExp.EnableWindow(TRUE);
		m_btRangeSelected.EnableWindow(m_bEnableRangeSelected);
		m_btMatchComplete.EnableWindow(TRUE);
	}
	m_btRangeSelected.EnableWindow(m_bEnableRangeSelected);

	// �����̏ꍇ�́A�u���֘A�̍��ڂ��g�p�s�ɂ���
	m_stReplace.EnableWindow(bReplace);
	m_listReplaceWord.EnableWindow(bReplace);
	m_btReplace.EnableWindow(bReplace);
	m_btAllReplace.EnableWindow(bReplace);
}

void CDlgReplaceWord::UpdateCheckWordState(void)
{
	UpdateData();

	CHECK_WORD_STATE	*pState = &g_chkWord[CHECK_STATE_REPLACE];
	pState->strSearchWord	= m_strSearchWord;		// ������
	pState->strReplaceWord	= m_strReplaceWord;		// ������
	pState->nTargetColumn	= COLUMN_FILE_NAME + m_nTargetColumn;	// �����ΏۃJ����
	/*
	if (m_nTargetColumn == -1) {
		pState->nTargetColumn = -1;
	}
	*/
	pState->bCheckDiffUL	= m_bCheckDiffUL;		// �啶���^�������̋��
	pState->bRegExp			= m_bRegExp;			// ���K�\��
	pState->bRangeSelected	= m_bRangeSelected;		// �I��͈͂̂�
	pState->bMatchComplete	= m_bMatchComplete;		// ���S�Ɉ�v
}

void CDlgReplaceWord::ExecSearch(bool bNext)
{
	CHECK_WORD_STATE	*pState = &g_chkWord[CHECK_STATE_REPLACE];
	UpdateCheckWordState();

	if (m_pList->GetSelectedItem() != -1) {
		int		nIndex = m_pList->FindNextMatchItem(m_pList->GetSelectedItem(), pState, bNext);
		if (nIndex != -1) {
#ifdef FLICKERFREE
			m_pList->SetRedraw(FALSE);
#endif
			if (pState->bRangeSelected == FALSE) {
				// �S�s�I������
				m_pList->SelItemRange(FALSE, 0, -1);
			}
			m_pList->SelectAndVisibleColumn(nIndex, pState->nMatchColumn);
			if (bNext) { /* 2003.06.30 add */
				m_pList->EnsureVisible(nIndex+1, FALSE);
			} else {
				m_pList->EnsureVisible(nIndex-1, FALSE);
			}
			m_pList->UpdateWindow();
#ifdef FLICKERFREE
			m_pList->SetRedraw(TRUE);
#endif
		} else {
			MessageBox(TEXT("������܂���ł���"), TEXT("�������s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
		}
	}
}

void CDlgReplaceWord::OnBtSearchBefore()
{
	// �O���������s
	ExecSearch(false);

	m_bAddCurrentItemtoHistory = true; /* BeachMonster 088 */
}

void CDlgReplaceWord::OnBtSearchNext()
{
	// ����������s
	ExecSearch(true);

	m_bAddCurrentItemtoHistory = true; /* BeachMonster 088 */
}

void CDlgReplaceWord::OnBtReplace()
{
	CHECK_WORD_STATE	*pState = &g_chkWord[CHECK_STATE_REPLACE];
	UpdateCheckWordState();

	// �u��
	m_pList->ReplaceMatchItem(pState, false);

	m_bAddCurrentItemtoHistory = true; /* BeachMonster 088 */
}

void CDlgReplaceWord::OnOK()
{
	CHECK_WORD_STATE	*pState = &g_chkWord[CHECK_STATE_REPLACE];
	UpdateCheckWordState();

	// �S�Ēu��
	m_pList->ReplaceMatchItem(pState, true);

	GetWindowRect(m_rect); /* WildCherry4 086 */
	m_listSearchWord.SaveHistory(); /* BeachMonster 088 */
	m_listReplaceWord.SaveHistory(); /* BeachMonster 088 */
	m_bAddCurrentItemtoHistory = true; /* BeachMonster 088 */

	CDialog::OnOK();
}

void CDlgReplaceWord::OnSelChangeListTarget()
{
	UpdateData();
	UpdateStatus();
}

void CDlgReplaceWord::OnChRegexp()
{
	UpdateData();
	UpdateStatus();
}

void CDlgReplaceWord::OnChRangeSelected()
{
	UpdateData();
	UpdateStatus();
}

void CDlgReplaceWord::OnCancel() /* WildCherry4 086 */
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	GetWindowRect(m_rect);
	m_listSearchWord.SaveHistory(m_bAddCurrentItemtoHistory); /* BeachMonster 088 */
	m_listReplaceWord.SaveHistory(m_bAddCurrentItemtoHistory); /* BeachMonster 088 */

	CDialog::OnCancel();
}
