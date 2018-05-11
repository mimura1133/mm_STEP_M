// DlgFixedUpperLower.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgFixedUpperLower.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedUpperLower �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDlgFixedUpperLower, COptionPage)

CDlgFixedUpperLower::CDlgFixedUpperLower() : COptionPage(CDlgFixedUpperLower::IDD)
{
	//{{AFX_DATA_INIT(CDlgFixedUpperLower)
	m_strFixedWord = _T("");
	//}}AFX_DATA_INIT
}

CDlgFixedUpperLower::~CDlgFixedUpperLower()
{
}

void CDlgFixedUpperLower::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFixedUpperLower)
	DDX_Control(pDX, IDC_EDIT_FIXED_WORD, m_editFixedWord);
	DDX_Control(pDX, IDC_BT_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BT_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_LIST_UPPER_LOWER, m_listUpperLower);
	DDX_Text(pDX, IDC_EDIT_FIXED_WORD, m_strFixedWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFixedUpperLower, COptionPage)
	//{{AFX_MSG_MAP(CDlgFixedUpperLower)
	ON_BN_CLICKED(IDC_BT_ADD, OnBtAdd)
	ON_EN_CHANGE(IDC_EDIT_FIXED_WORD, OnChangeEditFixedWord)
	ON_BN_CLICKED(IDC_BT_DELETE, OnBtDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UPPER_LOWER, OnItemchangedListUpperLower)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedUpperLower ���b�Z�[�W �n���h��

void CDlgFixedUpperLower::OnBtAdd() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);
	m_listUpperLower.InsertItem(m_listUpperLower.GetItemCount(), m_strFixedWord);
	m_arFixedWords.Add(m_strFixedWord);
	m_strFixedWord.Empty();
	UpdateData(FALSE);
	EnableButton();
	m_buttonAdd.EnableWindow(FALSE);
	m_bModify = TRUE;
}

BOOL CDlgFixedUpperLower::OnInitDialog() 
{
	COptionPage::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	DWORD	dwStyle;
	dwStyle = m_listUpperLower.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |=LVS_EX_FULLROWSELECT;
	m_listUpperLower.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	m_bModify = FALSE;
	
	RECT	rect;
	m_listUpperLower.GetClientRect(&rect);
	m_listUpperLower.InsertColumn(1, TEXT("�Œ�P��"), LVCFMT_LEFT, rect.right-rect.left-16, -1);
	m_listUpperLower.DeleteAllItems();					// �N���A
	for (int i=0;i<m_arFixedWords.GetSize();i++) {
		m_listUpperLower.InsertItem(i, m_arFixedWords.GetAt(i));
	}

	m_buttonAdd.EnableWindow(FALSE);
	EnableButton();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgFixedUpperLower::OnChangeEditFixedWord() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� CRichEditCrtl().SetEventMask()
	// ���b�Z�[�W���R���g���[���֑��邽�߂� COptionPage::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString strWord;
	m_editFixedWord.GetWindowText(strWord);
	if (strWord.IsEmpty()) {
		m_buttonAdd.EnableWindow(FALSE);
	} else {
		m_buttonAdd.EnableWindow(TRUE);
	}
}

void CDlgFixedUpperLower::EnableButton()
{
	if (m_listUpperLower.GetSelectedCount() == 0) {
		m_buttonDelete.EnableWindow(FALSE);
	} else {
		m_buttonDelete.EnableWindow(TRUE);
	}
}

void CDlgFixedUpperLower::OnItemchangedListUpperLower(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EnableButton();

	*pResult = 0;
}

void CDlgFixedUpperLower::OnBtDelete() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	POSITION pos = m_listUpperLower.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nIndex = m_listUpperLower.GetNextSelectedItem(pos);
	m_listUpperLower.DeleteItem(nIndex);
	m_arFixedWords.RemoveAt(nIndex);
	m_bModify = TRUE;
}
