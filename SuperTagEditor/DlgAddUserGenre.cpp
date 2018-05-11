// DlgAddUserGenre.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "FileMP3.h"
#include "DlgAddUserGenre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUserGenre �_�C�A���O


CDlgAddUserGenre::CDlgAddUserGenre(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddUserGenre::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddUserGenre)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgAddUserGenre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddUserGenre)
	DDX_Control(pDX, IDC_BT_ADD, m_cBtAdd);
	DDX_Control(pDX, IDC_LIST_GENRE, m_listGenre);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddUserGenre, CDialog)
	//{{AFX_MSG_MAP(CDlgAddUserGenre)
	ON_BN_CLICKED(IDC_BT_ADD, OnBtAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GENRE, OnItemchangedListGenre)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUserGenre ���b�Z�[�W �n���h��

BOOL CDlgAddUserGenre::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	DWORD	dwStyle;
	dwStyle = m_listGenre.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |=LVS_EX_FULLROWSELECT;
	m_listGenre.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	RECT	rect;
	m_listGenre.GetClientRect(&rect);

	m_listGenre.InsertColumn(1, TEXT("�W������"), LVCFMT_LEFT, rect.right-rect.left-16, -1);
	m_listGenre.DeleteAllItems();					// �N���A

	int nIndex; for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
		GENRE_LIST	*pGenre = &g_genreListSCMPX[nIndex];
		CString	strText;
		strText.Format(TEXT("%3d�F%s"), pGenre->byGenre, pGenre->sName);
		m_listGenre.InsertItem(nIndex, strText);
		m_listGenre.SetItemData(nIndex, pGenre->byGenre);
	}
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgAddUserGenre::OnBtAdd() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData(true);
	if (m_strName.GetLength() == 0) {
		return;
	}
	POSITION pos = m_listGenre.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nIndex = m_listGenre.GetNextSelectedItem(pos);
	int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
		USER_GENRE_LIST	*pGenre = &m_genreListUSER[i];
		if (pGenre->bUse == true) {
			if (pGenre->sName != m_strName)	continue;
		}
		pGenre->bAddList = true;
		pGenre->bUse = true;
		pGenre->sName = m_strName;
		pGenre->byGenre = g_genreListSCMPX[nIndex].byGenre;
		CDialog::OnOK();
		break;
	}
}


void CDlgAddUserGenre::OnItemchangedListGenre(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EnableButton();

	*pResult = 0;
}

void CDlgAddUserGenre::EnableButton()
{
	UpdateData(true);

	BOOL bEnable = FALSE;
	int nItem = m_listGenre.GetNextItem(-1, LVNI_SELECTED);
	if (nItem > -1 && m_strName.IsEmpty() != TRUE) {
		bEnable = TRUE;
	} else {
		bEnable = FALSE;
	}
	m_cBtAdd.EnableWindow(bEnable);
}

void CDlgAddUserGenre::OnChangeEditName() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� CRichEditCrtl().SetEventMask()
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CDialog::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EnableButton();
}

void CDlgAddUserGenre::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
}
