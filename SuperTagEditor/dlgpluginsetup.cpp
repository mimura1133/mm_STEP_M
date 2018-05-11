// CDlgPluginSetup.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgPluginSetup.h"

#include "STEP_api.h"
#include "Plugin.h"

#include "Registry.h"

extern CPlugin plugins;

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
// CDlgPluginSetup �_�C�A���O


CDlgPluginSetup::CDlgPluginSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPluginSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPluginSetup)
	m_strPluginInfo = _T("");
	//}}AFX_DATA_INIT
}


void CDlgPluginSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPluginSetup)
	DDX_Control(pDX, IDC_LIST_PLUGIN, m_listPlugin);
	DDX_Text(pDX, IDC_EDIT_INFO, m_strPluginInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPluginSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgPluginSetup)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PLUGIN, OnItemchangedListPlugin)
	ON_BN_CLICKED(IDC_BT_SETUP, OnBtSetup)
	ON_BN_CLICKED(IDC_BT_INSTALL, OnBtInstall)
	ON_BN_CLICKED(IDC_BT_UP, OnBtUp)
	ON_BN_CLICKED(IDC_BT_DOWN, OnBtDown)
	ON_BN_CLICKED(IDC_BT_UNINSTALL, OnBtUninstall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPluginSetup ���b�Z�[�W �n���h��

BOOL CDlgPluginSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	DWORD	dwStyle;
	dwStyle = m_listPlugin.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
	m_listPlugin.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	RECT	rect;
	m_listPlugin.GetClientRect(&rect);

	m_listPlugin.InsertColumn(1, TEXT("�v���O�C��"), LVCFMT_LEFT, rect.right-rect.left-16, -1);
	m_listPlugin.DeleteAllItems();					// �N���A

	for (int nIndex=0;nIndex<plugins.arPlugins.GetSize();nIndex++) {
		PSTEPlugin pPlugin = (PSTEPlugin)plugins.arPlugins.GetAt(nIndex);
		m_listPlugin.InsertItem(nIndex, pPlugin->sPluginName);
		m_listPlugin.SetItemData(nIndex, (DWORD)pPlugin);
		ListView_SetCheckState(m_listPlugin.GetSafeHwnd(), nIndex, pPlugin->bUse ? TRUE : FALSE);
	}
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgPluginSetup::OnItemchangedListPlugin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nIndex =  ListView_GetSelectedItem(m_listPlugin);
	if (nIndex < 0) {
		GetDlgItem(IDC_BT_SETUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_UNINSTALL)->EnableWindow(FALSE);
		return;
	}
	GetDlgItem(IDC_BT_UNINSTALL)->EnableWindow(TRUE);

	PSTEPlugin pPlugin = (PSTEPlugin)m_listPlugin.GetItemData(nIndex);
	m_strPluginInfo = pPlugin->sFileName;
	if (pPlugin->STEPGetPluginInfo != NULL) {
		m_strPluginInfo += "\r\n";
		m_strPluginInfo += pPlugin->STEPGetPluginInfo();
	}

	if (pPlugin->STEPShowOptionDialog == NULL) {
		GetDlgItem(IDC_BT_SETUP)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BT_SETUP)->EnableWindow(TRUE);
	}
	if (nIndex == 0) {
		GetDlgItem(IDC_BT_UP)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BT_UP)->EnableWindow(TRUE);
	}
	if (nIndex == m_listPlugin.GetItemCount()-1) {
		GetDlgItem(IDC_BT_DOWN)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BT_DOWN)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgPluginSetup::OnBtSetup() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nIndex =  ListView_GetSelectedItem(m_listPlugin);
	if (nIndex < 0) return;

	PSTEPlugin pPlugin = (PSTEPlugin)m_listPlugin.GetItemData(nIndex);
	if (pPlugin->STEPShowOptionDialog != NULL) {
		pPlugin->STEPShowOptionDialog(GetSafeHwnd());
	}
}

extern "C" STEP_API void WINAPI STEPUpdateCellInfo(void);
void CDlgPluginSetup::OnBtInstall() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	TCHAR szFilter[] = TEXT("STE�v���O�C�� (*.ste)|*.ste|�S�� (*.*)|*.*||");
	CFileDialog dialog(TRUE, TEXT("ste"), NULL, 0, szFilter, this);
	if (dialog.DoModal() == IDOK) {
		CString strPluginFile = dialog.GetPathName();
		extern PSTEPlugin STEPluginLoadFile(LPCTSTR);
		PSTEPlugin pPlugin = STEPluginLoadFile(strPluginFile);
		if (pPlugin == NULL) {
			MessageBox(TEXT("�I�����ꂽ�v���O�C���͎g�p�ł��܂���B"), TEXT("�v���O�C���̃C���X�g�[��"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return;
		}
		pPlugin->bUse = true;
		int nIndex = m_listPlugin.GetItemCount();
		m_listPlugin.InsertItem(nIndex, pPlugin->sPluginName);
		m_listPlugin.SetItemData(nIndex, (DWORD)pPlugin);
		ListView_SetCheckState(m_listPlugin.GetSafeHwnd(), nIndex, pPlugin->bUse ? TRUE : FALSE);

		STEPUpdateCellInfo();
	}
}

void CDlgPluginSetup::OnBtUp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nIndex =  ListView_GetSelectedItem(m_listPlugin);
	PSTEPlugin pPlugin = (PSTEPlugin)m_listPlugin.GetItemData(nIndex);
	nIndex--;
	m_listPlugin.InsertItem(nIndex, pPlugin->sPluginName);
	m_listPlugin.SetItemData(nIndex, (DWORD)pPlugin);
	ListView_SetCheckState(m_listPlugin.GetSafeHwnd(), nIndex, pPlugin->bUse ? TRUE : FALSE);
	m_listPlugin.DeleteItem(nIndex+2);
	m_listPlugin.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}

void CDlgPluginSetup::OnBtDown() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nIndex =  ListView_GetSelectedItem(m_listPlugin);
	PSTEPlugin pPlugin = (PSTEPlugin)m_listPlugin.GetItemData(nIndex);
	nIndex += 2;
	m_listPlugin.InsertItem(nIndex, pPlugin->sPluginName);
	m_listPlugin.SetItemData(nIndex, (DWORD)pPlugin);
	ListView_SetCheckState(m_listPlugin.GetSafeHwnd(), nIndex, pPlugin->bUse ? TRUE : FALSE);
	m_listPlugin.DeleteItem(nIndex-2);
	m_listPlugin.SetItemState(nIndex-1, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);	
}

void CDlgPluginSetup::OnOK() 
{
	auto pApp = (CSuperTagEditorApp*)AfxGetApp();

	auto strINI = pApp->MakeFileName(TEXT("Plugin"), TEXT("ini"));
	Profile_Initialize(strINI.c_str(), FALSE);

	for (int nIndex = 0; nIndex < m_listPlugin.GetItemCount(); nIndex++) {
		auto pPlugin = (PSTEPlugin)m_listPlugin.GetItemData(nIndex);
		pPlugin->bUse = ListView_GetCheckState(m_listPlugin.GetSafeHwnd(), nIndex) ? true : false;
		// ���΃p�X�ɕϊ�
		TCHAR buff[_MAX_PATH] = {};
		if (!PathRelativePathTo(buff, strINI.c_str(), 0, pPlugin->sFileName, 0)) {
			// �ϊ��Ȃ�
			lstrcpy(buff, pPlugin->sFileName);
		}

		CString strSection;
		strSection.Format(TEXT("Load%03d"), nIndex);
		Profile_WriteString(strSection, TEXT("Path"), buff, strINI.c_str());
		Profile_WriteString(strSection, TEXT("Use"), pPlugin->bUse ? TEXT("1") : TEXT("0"), strINI.c_str());
	}

	Profile_Flush(strINI.c_str());
	Profile_Free();
	CDialog::OnOK();
}

void CDlgPluginSetup::OnBtUninstall() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if (MessageBox(TEXT("�I������Ă���v���O�C�����A���C���X�g�[�����܂����H\n���t�@�C���͍폜����܂���B"), TEXT("�A���C���X�g�[��"), MB_YESNO|MB_TOPMOST) == IDYES) {
		int nIndex =  ListView_GetSelectedItem(m_listPlugin);
		PSTEPlugin pPlugin = (PSTEPlugin)m_listPlugin.GetItemData(nIndex);
		pPlugin->bUse = false;
		m_listPlugin.DeleteItem(nIndex);
	}
}
