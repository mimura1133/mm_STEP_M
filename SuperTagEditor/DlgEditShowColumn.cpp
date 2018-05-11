// DlgEditShowColumn.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "MySuperGrid.h"
#include "DlgEditShowColumn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ListView_SetCheckState
#define ListView_SetCheckState(hwndLV, i, fCheck)	ListView_SetItemState(hwndLV, i, INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif

#define ListView_GetSelectedItem(listCtrl)			listCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED | LVIS_FOCUSED)

/////////////////////////////////////////////////////////////////////////////
// CDlgEditShowColumn �_�C�A���O
IMPLEMENT_DYNCREATE(CDlgEditShowColumn, COptionPage)


CDlgEditShowColumn::CDlgEditShowColumn() : COptionPage(CDlgEditShowColumn::IDD)
{
	//{{AFX_DATA_INIT(CDlgEditShowColumn)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}

CDlgEditShowColumn::~CDlgEditShowColumn()
{
}


void CDlgEditShowColumn::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditShowColumn)
	DDX_Control(pDX, IDC_LIST_COLUMN, m_listColumn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditShowColumn, COptionPage)
	//{{AFX_MSG_MAP(CDlgEditShowColumn)
	ON_BN_CLICKED(IDC_BT_DOWN, OnBtDown)
	ON_BN_CLICKED(IDC_BT_UP, OnBtUp)
	ON_BN_CLICKED(IDC_BT_RESET_PAGE, OnBtResetPage)
	ON_EN_CHANGE(IDC_EDIT_MAX_WIDTH, OnChangeEditMaxWidth)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COLUMN, OnItemChangedListColumn)
	ON_BN_DOUBLECLICKED(IDC_BT_DOWN, OnBtDown)
	ON_BN_DOUBLECLICKED(IDC_BT_UP, OnBtUp)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_COLUMN, OnItemChangingListColumn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditShowColumn ���b�Z�[�W �n���h��

BOOL CDlgEditShowColumn::OnInitDialog()
{
	COptionPage::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	// �g���X�^�C���̐ݒ�F�`�F�b�N�{�b�N�X�ǉ��A�P�s�I��
	DWORD	dwStyle;
	dwStyle = m_listColumn.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
	m_listColumn.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	RECT	rect;
	m_listColumn.GetClientRect(&rect);

	m_listColumn.InsertColumn(0, TEXT("���ږ�"), LVCFMT_LEFT, rect.right-rect.left-16-48);
	m_listColumn.InsertColumn(1, TEXT("�ő啝"), LVCFMT_LEFT, 48, 1);
	m_listColumn.DeleteAllItems();					// �N���A

	bool bFoundFormat = false; /* Conspiracy 198 */
	for (int nColumn = COLUMN_TREE_ITEM; nColumn < COLUMN_MAX; nColumn++) {
		for (int nType = COLUMN_TREE_ITEM; nType < COLUMN_MAX; nType++) {
			struct COLUMN_STATUS	*Stat = &g_columnStatus[nType];
			if (Stat->nNumber == nColumn) {
				extern	const TCHAR	***g_sNameList;
				int		nIndex = m_listColumn.GetItemCount();
				CString	strMax;
				strMax.Format(TEXT("%d"), Stat->nWidthMax);
				m_listColumn.InsertItem(nIndex, g_sNameList[0][1+nType]);
				m_listColumn.SetItemText(nIndex, 1, strMax);
				m_listColumn.SetItemData(nIndex, nType);
				ListView_SetCheckState(m_listColumn.GetSafeHwnd(), nIndex, Stat->bShowFlag);
				if (nType == COLUMN_FORMAT) {
					bFoundFormat = true;
				}
				break;
			}
		}
	}
	if (!bFoundFormat) { /* Conspiracy 198 */
		struct COLUMN_STATUS	*Stat = &g_columnStatus[COLUMN_FORMAT];
		extern	const TCHAR	***g_sNameList;
		int		nIndex = m_listColumn.GetItemCount();
		CString	strMax;
		strMax.Format(TEXT("%d"), Stat->nWidthMax);
		m_listColumn.InsertItem(nIndex, g_sNameList[0][1+COLUMN_FORMAT]);
		m_listColumn.SetItemText(nIndex, 1, strMax);
		m_listColumn.SetItemData(nIndex, COLUMN_FORMAT);
		ListView_SetCheckState(m_listColumn.GetSafeHwnd(), nIndex, Stat->bShowFlag);
	}

	SelChangeList();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgEditShowColumn::SwapListItem(int nNum1, int nNum2)
{
	LV_ITEM	item1, item2;
	_TCHAR	szBuff1[256], szBuff2[256];
	item1.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_INDENT | LVIF_PARAM;
	item1.iItem = nNum1;
	item1.iSubItem = 0;
	item1.pszText = szBuff1;
	item1.cchTextMax = sizeof(szBuff1);
	item1.stateMask = 0xFFFF;
	m_listColumn.GetItem(&item1);
	item2 = item1;
	item2.iItem = nNum2;
	item2.pszText = szBuff2;
	item2.cchTextMax = sizeof(szBuff2);
	m_listColumn.GetItem(&item2);

	item1.iItem = nNum2;
	m_listColumn.SetItem(&item1);
	item2.iItem = nNum1;
	m_listColumn.SetItem(&item2);

	// �ő啝������ւ�
	CString	strWidth1, strWidth2;
	strWidth1 = m_listColumn.GetItemText(nNum1, 1);
	strWidth2 = m_listColumn.GetItemText(nNum2, 1);
	m_listColumn.SetItemText(nNum1, 1, strWidth2);
	m_listColumn.SetItemText(nNum2, 1, strWidth1);
}

void CDlgEditShowColumn::OnBtDown()
{
	int		nIndex;
	nIndex = ListView_GetSelectedItem(m_listColumn);
	if (nIndex >= 1) {
		if (nIndex < m_listColumn.GetItemCount()-1) {
			// �f�[�^�����ς���
			SwapListItem(nIndex, nIndex+1);
			m_listColumn.EnsureVisible(nIndex+1, FALSE);
		}
	}
}

void CDlgEditShowColumn::OnBtUp()
{
	int		nIndex;
	nIndex = ListView_GetSelectedItem(m_listColumn);
	if (nIndex >= 1) {
		// �f�[�^�����ς���
		SwapListItem(nIndex, nIndex-1);
		m_listColumn.EnsureVisible(nIndex-1, FALSE);
	}
}

void CDlgEditShowColumn::OnOK()
{
	if (!::IsWindow(m_hWnd))	return;
	// �ҏW��̏�Ԃ�ۑ�
	int nIndex; for (nIndex = 0; nIndex < m_listColumn.GetItemCount(); nIndex++) {
		struct COLUMN_STATUS	*Stat;
		Stat = &g_columnStatus[m_listColumn.GetItemData(nIndex)];
		Stat->bShowFlag = nIndex ? ListView_GetCheckState(m_listColumn.GetSafeHwnd(), nIndex) : TRUE;
		Stat->nWidthMax = _tstoi(m_listColumn.GetItemText(nIndex, 1));
		Stat->nNumber = COLUMN_TREE_ITEM + nIndex;
	}

	if (g_columnStatus[COLUMN_FILE_NAME].bShowFlag == false) {
		MessageBox(TEXT("[�\�����ڐݒ�]�Ńt�@�C��������\���ɐݒ肳��Ă��܂�\n\n"
		           "STE ��Ńt�@�C������ύX�������ꍇ�́A�t�@�C�����̕\����\n"
				   "�L���ɂ��Ă�������"),
		           TEXT("�x��"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
	}

	COptionPage::OnOK();
}

// �����l�ɖ߂�
void CDlgEditShowColumn::OnBtResetPage()
{
	m_listColumn.DeleteAllItems();					// �N���A

	static	int		nColumnMax[] = {
		0,0,0,0,0,48,48,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,128,128,128,128,128,0,128,128/* 2003.06.19 add */ /* Conspiracy 196 */ /* Fix 2010 Mimura */
	};

	for (int nType = COLUMN_TREE_ITEM; nType < COLUMN_MAX; nType++) {
		extern	const TCHAR	***g_sNameList;
		int		nIndex = m_listColumn.GetItemCount();
		struct COLUMN_STATUS	*Stat;
		//Stat = &g_columnStatus[m_listColumn.GetItemData(nType)];
		Stat = &g_columnStatus[nType];
		CString	strMax;
		strMax.Format(TEXT("%d"), nColumnMax[nType]);
		m_listColumn.InsertItem(nIndex, g_sNameList[0][1+nType]);
		m_listColumn.SetItemText(nIndex, 1, strMax);
		m_listColumn.SetItemData(nIndex, nType);
		ListView_SetCheckState(m_listColumn.GetSafeHwnd(), nIndex, Stat->bShowFlag);
	}
}

void CDlgEditShowColumn::OnChangeEditMaxWidth()
{
	int		nIndex = m_listColumn.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nIndex != -1) {
		CString	strText;
		GetDlgItemText(IDC_EDIT_MAX_WIDTH, strText);
		m_listColumn.SetItemText(nIndex, 1, strText);
	}
}

void CDlgEditShowColumn::OnItemChangedListColumn(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// �I����Ԃ��ύX���ꂽ���H
	UINT	uMask = LVIS_FOCUSED | LVIS_SELECTED;
	if ((pNMListView->uOldState & uMask) != (pNMListView->uNewState & uMask)) {
		SelChangeList();
	}

	*pResult = 0;
}

void CDlgEditShowColumn::SelChangeList(void)
{
	int		nIndex = m_listColumn.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nIndex != -1) {
		SetDlgItemText(IDC_EDIT_MAX_WIDTH, m_listColumn.GetItemText(nIndex, 1));

		if (nIndex == 0) {
			GetDlgItem(IDC_BT_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BT_DOWN)->EnableWindow(FALSE);
		} else {
			GetDlgItem(IDC_BT_UP)->EnableWindow((nIndex == 1) ? FALSE : TRUE);
			GetDlgItem(IDC_BT_DOWN)->EnableWindow((nIndex == m_listColumn.GetItemCount()-1) ? FALSE : TRUE);
		}
		GetDlgItem(IDC_EDIT_MAX_WIDTH)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_BT_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_WIDTH)->EnableWindow(FALSE);
	}
}

void CDlgEditShowColumn::OnItemChangingListColumn(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// MP3 List �̃`�F�b�N��Ԃ͕ύX�s��
	if (pNMListView->iItem == 0) {
		BOOL bChecked = (BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);
		bChecked = (bChecked < 0) ? FALSE : TRUE;
		if (bChecked) {								// �`�F�b�N���悤�Ƃ��Ă���
			// ������
			*pResult = 1;
			return;
		}
	}

	*pResult = 0;
}
