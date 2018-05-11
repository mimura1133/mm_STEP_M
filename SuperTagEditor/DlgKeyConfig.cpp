// DlgKeyConfig.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgKeyConfig.h"

#include "Plugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyConfig �_�C�A���O
static	int		g_nLastSelectGroup = 0;
static	int		g_nLastSelectCommand = 0;

#define KEY_CODE_CTRL	0x0002
#define KEY_CODE_ALT	0x0004

struct	KEY_DATA	{
	char	*sKeyName;
	WORD	wKeyCode;
};

static	KEY_DATA	g_keyData[] = {
	{"(�Ȃ�)", 0x0000},
	{"F1", VK_F1},
	{"F2", VK_F2},
	{"F3", VK_F3},
	{"F4", VK_F4},
	{"F5", VK_F5},
	{"F6", VK_F6},
	{"F7", VK_F7},
	{"F8", VK_F8},
	{"F9", VK_F9},
	{"F10", VK_F10},
	{"F11", VK_F11},
	{"F12", VK_F12},
	{"Bksp", VK_BACK},
	{"Ins", VK_INSERT},
	{"Del", VK_DELETE},
	{"Return", VK_RETURN},
	{"Home", VK_HOME},
	{"End", VK_END},
	{"Space", VK_SPACE},
//	{"Up", VK_UP},
//	{"Down", VK_DOWN},
//	{"Right", VK_RIGHT},
//	{"Left", VK_LEFT},
	{"1", '1'},
	{"2", '2'},
	{"3", '3'},
	{"4", '4'},
	{"5", '5'},
	{"6", '6'},
	{"7", '7'},
	{"8", '8'},
	{"9", '9'},
	{"0", '0'},
	{"A", 'A'},
	{"B", 'B'},
	{"C", 'C'},
	{"D", 'D'},
	{"E", 'E'},
	{"F", 'F'},
	{"G", 'G'},
	{"H", 'H'},
	{"I", 'I'},
	{"J", 'J'},
	{"K", 'K'},
	{"L", 'L'},
	{"M", 'M'},
	{"N", 'N'},
	{"O", 'O'},
	{"P", 'P'},
	{"Q", 'Q'},
	{"R", 'R'},
	{"S", 'S'},
	{"T", 'T'},
	{"U", 'U'},
	{"V", 'V'},
	{"W", 'W'},
	{"X", 'X'},
	{"Y", 'Y'},
	{"Z", 'Z'},
//	{"[", '['},
//	{"\\", '\\'},
//	{"]", ']'},
//	{"^", '^'},
	{NULL, 0x0000},
};

IMPLEMENT_DYNCREATE(CDlgKeyConfig, COptionPage)


CDlgKeyConfig::CDlgKeyConfig() : COptionPage(CDlgKeyConfig::IDD)
{
	//{{AFX_DATA_INIT(CDlgKeyConfig)
	//}}AFX_DATA_INIT
}

CDlgKeyConfig::~CDlgKeyConfig()
{
}

void CDlgKeyConfig::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgKeyConfig)
	DDX_Control(pDX, IDC_CH_ALT, m_btAlt);
	DDX_Control(pDX, IDC_CH_CTRL, m_btCtrl);
	DDX_Control(pDX, IDC_LIST_KEY, m_listKey);
	DDX_Control(pDX, IDC_LIST_GROUP, m_listGroup);
	DDX_Control(pDX, IDC_LIST_COMMAND, m_listCommand);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKeyConfig, COptionPage)
	//{{AFX_MSG_MAP(CDlgKeyConfig)
	ON_BN_CLICKED(IDC_CH_CTRL, OnChCtrl)
	ON_BN_CLICKED(IDC_CH_ALT, OnChAlt)
	ON_LBN_SELCHANGE(IDC_LIST_COMMAND, OnSelChangeListCommand)
	ON_CBN_SELCHANGE(IDC_LIST_GROUP, OnSelChangeListGroup)
	ON_LBN_SELCHANGE(IDC_LIST_KEY, OnSelChangeListKey)
	ON_BN_DOUBLECLICKED(IDC_CH_CTRL, OnChCtrl)
	ON_BN_DOUBLECLICKED(IDC_CH_ALT, OnChAlt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyConfig ���b�Z�[�W �n���h��
BOOL CDlgKeyConfig::OnInitDialog() 
{
	COptionPage::OnInitDialog();

	m_bExecCommandChange = false;

	// �R�}���h�O���[�v�̍쐬
	static const LPCTSTR sGroupList[COMMAND_GROUP_MAX+1] = {
		TEXT("�t�@�C��"), TEXT("�ҏW"), TEXT("�\��"), TEXT("�ϊ�")/* 2006.03.02 */, TEXT("�v���C���X�g"), TEXT("�v���C���[����"), TEXT("�v���O�C��"), NULL
	};
	m_listGroup.ResetContent();
	for (int nGroup = 0; sGroupList[nGroup] != NULL; nGroup++) {
		m_listGroup.AddString(sGroupList[nGroup]);
	}
	m_listGroup.SetCurSel(g_nLastSelectGroup);
	UpdateCommandList();
	UpdateKeyList();
	m_listCommand.SetCurSel(g_nLastSelectCommand);
	OnSelChangeListCommand();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgKeyConfig::UpdateHotKey(void)
{
	KEY_CONFIG *pKeyData;
	WORD	wCmdID = (WORD)m_listCommand.GetItemData(m_listCommand.GetCurSel());
	if (wCmdID != 0x0000 && (pKeyData = SearchKeyConfigID(wCmdID)) != NULL) {
		// �z�b�g�L�[�R�[�h���擾���X�V
		WORD	wKeyCode, wModifiers;
		wKeyCode = (WORD)m_listKey.GetItemData(m_listKey.GetCurSel());
		wModifiers = (((wKeyCode < 0x30)   ? 0x0008 : 0x0000)
		           |  (m_btCtrl.GetCheck() ? 0x0002 : 0x0000)
		           |  (m_btAlt.GetCheck()  ? 0x0004 : 0x0000));
		pKeyData->dwKeyCode = wKeyCode | ((DWORD)wModifiers << 16);

		// ���ɓo�^����Ă��Ȃ����ǂ������`�F�b�N
		if (wKeyCode != 0x0000) {
			int i; for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
				if (&g_listKeyConfig[i] != pKeyData
				&&  g_listKeyConfig[i].dwKeyCode == pKeyData->dwKeyCode) {
					CString	strBuffer;
					strBuffer.Format(TEXT("�w�肳�ꂽ�V���[�g�J�b�g�L�[�́A\n\n[%s]\n\n�ł��g�p����Ă��܂�\n\n�ǂ��炩�̃V���[�g�J�b�g��ύX���Ă�������"), g_listKeyConfig[i].sName);
					MessageBox(strBuffer, TEXT("�V���[�g�J�b�g�L�[�̏d��"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
					break;
				}
			}
			extern CPlugin plugins;
			for (i=0;i<plugins.arPluginKey.GetSize();i++) {
				KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
				if (pKey != pKeyData
				&&  pKey->dwKeyCode == pKeyData->dwKeyCode) {
					CString	strBuffer;
					strBuffer.Format(TEXT("�w�肳�ꂽ�V���[�g�J�b�g�L�[�́A\n\n[%s]\n\n�ł��g�p����Ă��܂�\n\n�ǂ��炩�̃V���[�g�J�b�g��ύX���Ă�������"), pKey->sName);
					MessageBox(strBuffer, TEXT("�V���[�g�J�b�g�L�[�̏d��"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
					break;
				}
			}
		}
	}
}

void CDlgKeyConfig::UpdateKeyList(void)
{
	bool	bCtrl = m_btCtrl.GetCheck() ? true : false;
	bool	bAlt = m_btAlt.GetCheck() ? true : false;

	CString	strExt;
	if (bCtrl) strExt += _T("Ctrl+");
	if (bAlt) strExt += _T("Alt+");

	// ���݂̑I����ۑ�
	int		nOldSel = m_listKey.GetCurSel();

	// �L�[�ꗗ�̍쐬
	m_listKey.ResetContent();
	int i; for (i = 0; g_keyData[i].sKeyName != NULL; i++) {
		int		nIndex;
		CString	strName;
		WORD	wKeyCode = g_keyData[i].wKeyCode;

		if (bAlt && bCtrl && wKeyCode == VK_DELETE) {
			// Alt + Ctrl + Del �͊��蓖�ċ֎~
		} else {
			strName.Format(TEXT("%s%s"), strExt, g_keyData[i].sKeyName);
			nIndex = m_listKey.AddString(strName);
			// �A�C�e���f�[�^�ɃL�[�R�[�h��ݒ�
			m_listKey.SetItemData(nIndex, wKeyCode);

			// Ctrl �� Alt ���`�F�b�N����Ă��Ȃ��ꍇ�́ADEL �L�[�܂ł����g���Ȃ�
			if (bAlt == false && bCtrl == false) {
				if (wKeyCode == VK_DELETE) {
					break;
				}
			}
		}
	}
	
	// �I�����Ȃ���
	m_listKey.SetCurSel((nOldSel >= 0 && nOldSel < i) ? nOldSel : 0);
}

void CDlgKeyConfig::UpdateCommandList(void)
{
	// �R�}���h�ꗗ�̍쐬
	m_listCommand.ResetContent();
	int i; for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
		KEY_CONFIG	*pKey = &g_listKeyConfig[i];
		if (pKey->nGroupID == m_listGroup.GetCurSel()) {
			int		nIndex;
			CString	strName = pKey->sName;
			switch(pKey->wCmdID) {
			case ID_CONV_FORMAT_EX_01:
				strName.Format(TEXT("01�F%s"), g_userConvFormatEx[0].strName);
				break;
			case ID_CONV_FORMAT_EX_02:
				strName.Format(TEXT("02�F%s"), g_userConvFormatEx[1].strName);
				break;
			case ID_CONV_FORMAT_EX_03:
				strName.Format(TEXT("03�F%s"), g_userConvFormatEx[2].strName);
				break;
			case ID_CONV_FORMAT_EX_04:
				strName.Format(TEXT("04�F%s"), g_userConvFormatEx[3].strName);
				break;
			case ID_CONV_FORMAT_EX_05:
				strName.Format(TEXT("05�F%s"), g_userConvFormatEx[4].strName);
				break;
			case ID_CONV_FORMAT_EX_06:
				strName.Format(TEXT("06�F%s"), g_userConvFormatEx[5].strName);
				break;
			case ID_CONV_FORMAT_EX_07:
				strName.Format(TEXT("07�F%s"), g_userConvFormatEx[6].strName);
				break;
			case ID_CONV_FORMAT_EX_08:
				strName.Format(TEXT("08�F%s"), g_userConvFormatEx[7].strName);
				break;
			case ID_CONV_FORMAT_EX_09:
				strName.Format(TEXT("09�F%s"), g_userConvFormatEx[8].strName);
				break;
			case ID_CONV_FORMAT_EX_10:
				strName.Format(TEXT("10�F%s"), g_userConvFormatEx[9].strName);
				break;
			case ID_CONV_FORMAT_USER_01: /* TyphoonSwell 027 */
				strName.Format(TEXT("01�F%s"), g_userConvFormat[0].strName);
			case ID_CONV_FORMAT_USER_T2F_01: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_01: /* STEP 030 */
				strName.Format(TEXT("01�F%s"), g_userConvFormat[0].strName);
				break;
			case ID_CONV_FORMAT_USER_02: /* TyphoonSwell 027 */
			case ID_CONV_FORMAT_USER_T2F_02: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_02: /* STEP 030 */
				strName.Format(TEXT("02�F%s"), g_userConvFormat[1].strName);
				break;
			case ID_CONV_FORMAT_USER_03: /* TyphoonSwell 027 */
			case ID_CONV_FORMAT_USER_T2F_03: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_03: /* STEP 030 */
				strName.Format(TEXT("03�F%s"), g_userConvFormat[2].strName);
				break;
			case ID_CONV_FORMAT_USER_04: /* LastTrain 057 */
			case ID_CONV_FORMAT_USER_T2F_04: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_04: /* STEP 030 */
				strName.Format(TEXT("04�F%s"), g_userConvFormat[3].strName);
				break;
			case ID_CONV_FORMAT_USER_05: /* LastTrain 057 */
			case ID_CONV_FORMAT_USER_T2F_05: /* STEP 030 */
			case ID_CONV_FORMAT_USER_F2T_05: /* STEP 030 */
				strName.Format(TEXT("05�F%s"), g_userConvFormat[4].strName);
				break;
			case ID_CONV_TAG_TO_TAG_01: /* STEP 034 */
				strName.Format(TEXT("01�F%s"), g_userConvFormatTag2Tag[0].strName);
				break;
			case ID_CONV_TAG_TO_TAG_02: /* STEP 034 */
				strName.Format(TEXT("02�F%s"), g_userConvFormatTag2Tag[1].strName);
				break;
			case ID_CONV_TAG_TO_TAG_03: /* STEP 034 */
				strName.Format(TEXT("03�F%s"), g_userConvFormatTag2Tag[2].strName);
				break;
			case ID_CONV_TAG_TO_TAG_04: /* STEP 034 */
				strName.Format(TEXT("04�F%s"), g_userConvFormatTag2Tag[3].strName);
				break;
			case ID_CONV_TAG_TO_TAG_05: /* STEP 034 */
				strName.Format(TEXT("05�F%s"), g_userConvFormatTag2Tag[4].strName);
				break;
			case ID_TEIKEI_01: /* SeaKnows 030 */
				strName.Format(TEXT("01: %s"), shortString(g_teikeiInfo[0].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_02: /* SeaKnows 030 */
				strName.Format(TEXT("02: %s"), shortString(g_teikeiInfo[1].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_03: /* SeaKnows 030 */
				strName.Format(TEXT("03: %s"), shortString(g_teikeiInfo[2].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_04: /* SeaKnows 030 */
				strName.Format(TEXT("04: %s"), shortString(g_teikeiInfo[3].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_05: /* SeaKnows 030 */
				strName.Format(TEXT("05: %s"), shortString(g_teikeiInfo[4].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_06: /* SeaKnows 030 */
				strName.Format(TEXT("06: %s"), shortString(g_teikeiInfo[5].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_07: /* SeaKnows 030 */
				strName.Format(TEXT("07: %s"), shortString(g_teikeiInfo[6].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_08: /* SeaKnows 030 */
				strName.Format(TEXT("08: %s"), shortString(g_teikeiInfo[7].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_09: /* SeaKnows 030 */
				strName.Format(TEXT("09: %s"), shortString(g_teikeiInfo[8].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_10: /* SeaKnows 030 */
				strName.Format(TEXT("10: %s"), shortString(g_teikeiInfo[9].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_01: /* FreeFall 046 */
				strName.Format(TEXT("01: %s"), shortString(g_teikeiInfo[10].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_02: /* FreeFall 046 */
				strName.Format(TEXT("02: %s"), shortString(g_teikeiInfo[11].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_03: /* FreeFall 046 */
				strName.Format(TEXT("03: %s"), shortString(g_teikeiInfo[12].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_04: /* FreeFall 046 */
				strName.Format(TEXT("04: %s"), shortString(g_teikeiInfo[13].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_05: /* FreeFall 046 */
				strName.Format(TEXT("05: %s"), shortString(g_teikeiInfo[14].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_06: /* FreeFall 046 */
				strName.Format(TEXT("06: %s"), shortString(g_teikeiInfo[15].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_07: /* FreeFall 046 */
				strName.Format(TEXT("07: %s"), shortString(g_teikeiInfo[16].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_08: /* FreeFall 046 */
				strName.Format(TEXT("08: %s"), shortString(g_teikeiInfo[17].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_09: /* FreeFall 046 */
				strName.Format(TEXT("09: %s"), shortString(g_teikeiInfo[18].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_2_10: /* FreeFall 046 */
				strName.Format(TEXT("10: %s"), shortString(g_teikeiInfo[19].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_01: /* FreeFall 046 */
				strName.Format(TEXT("01: %s"), shortString(g_teikeiInfo[20].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_02: /* FreeFall 046 */
				strName.Format(TEXT("02: %s"), shortString(g_teikeiInfo[21].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_03: /* FreeFall 046 */
				strName.Format(TEXT("03: %s"), shortString(g_teikeiInfo[22].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_04: /* FreeFall 046 */
				strName.Format(TEXT("04: %s"), shortString(g_teikeiInfo[23].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_05: /* FreeFall 046 */
				strName.Format(TEXT("05: %s"), shortString(g_teikeiInfo[24].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_06: /* FreeFall 046 */
				strName.Format(TEXT("06: %s"), shortString(g_teikeiInfo[25].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_07: /* FreeFall 046 */
				strName.Format(TEXT("07: %s"), shortString(g_teikeiInfo[26].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_08: /* FreeFall 046 */
				strName.Format(TEXT("08: %s"), shortString(g_teikeiInfo[27].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_09: /* FreeFall 046 */
				strName.Format(TEXT("09: %s"), shortString(g_teikeiInfo[28].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX) /* FreeFall 051 */);
				break;
			case ID_TEIKEI_3_10: /* Nightmare */
				strName.Format(TEXT("10: %s"), shortString(g_teikeiInfo[29].strTeikei/* STEP 035 */, TEIKEI_MENU_STRING_MAX));
				break;
			}
			nIndex = m_listCommand.AddString(strName);

			// �A�C�e���f�[�^�ɃR�}���h�h�c��ݒ�
			m_listCommand.SetItemData(nIndex, pKey->wCmdID);
		}
	}
	extern CPlugin plugins;
	for (i=0;i<plugins.arPluginKey.GetSize();i++) {
		KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
		if (pKey->nGroupID == m_listGroup.GetCurSel()) {
			int		nIndex;
			CString	strName = pKey->sName;
			nIndex = m_listCommand.AddString(strName);

			// �A�C�e���f�[�^�ɃR�}���h�h�c��ݒ�
			m_listCommand.SetItemData(nIndex, pKey->wCmdID);
		}
	}
	m_listCommand.SetCurSel(0);
}

void CDlgKeyConfig::OnChCtrl() 
{
	if (m_bExecCommandChange == false) {
		UpdateHotKey();
		UpdateKeyList();
	}
}

void CDlgKeyConfig::OnChAlt() 
{
	if (m_bExecCommandChange == false) {
		UpdateHotKey();
		UpdateKeyList();
	}
}

void CDlgKeyConfig::OnSelChangeListKey() 
{
	if (m_bExecCommandChange == false) {
		UpdateHotKey();
	}
}

void CDlgKeyConfig::OnSelChangeListGroup() 
{
	g_nLastSelectGroup = m_listGroup.GetCurSel();
	UpdateCommandList();
	OnSelChangeListCommand();
}

void CDlgKeyConfig::OnSelChangeListCommand() 
{
	g_nLastSelectCommand = m_listGroup.GetCurSel();

	m_bExecCommandChange = true;

	KEY_CONFIG *pKeyData;
	WORD	wCmdID = (WORD)m_listCommand.GetItemData(m_listCommand.GetCurSel());
	if (wCmdID != 0x0000 && (pKeyData = SearchKeyConfigID(wCmdID)) != NULL) {
		// �z�b�g�L�[�R�[�h���擾
		WORD	wKeyCode, wModifiers;
		wKeyCode = (WORD)pKeyData->dwKeyCode;
		wModifiers = (WORD)(pKeyData->dwKeyCode >> 16);

		if (wKeyCode != 0x0000) {
			// ����L�[�̃`�F�b�N��Ԃ�ݒ�
			BOOL	bCtrlOld = m_btCtrl.GetCheck();
			BOOL	bAltOld = m_btAlt.GetCheck();
			m_btCtrl.SetCheck((wModifiers & 0x0002) ? TRUE : FALSE);
			m_btAlt.SetCheck((wModifiers & 0x0004) ? TRUE : FALSE);
			if (bCtrlOld != m_btCtrl.GetCheck() || bAltOld != m_btAlt.GetCheck()) {
				// �L�[�ꗗ���X�V
				UpdateKeyList();
			}
		}

		// �L�[��I����Ԃɂ���
		int i; for (i = 0; g_keyData[i].sKeyName != NULL; i++) {
			if (wKeyCode == g_keyData[i].wKeyCode) {
				m_listKey.SetCurSel(i);
				break;
			}
		}
	}
	
	m_bExecCommandChange = false;
}
