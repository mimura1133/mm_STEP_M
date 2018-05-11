// SuperTagEditorDoc.cpp : CSuperTagEditorDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "Shlwapi.h"
#include "FileMP3.h"
#include "SuperTagEditor.h"
#include "DlgCommonProg.h"
#include "DlgEnvSheet.h"
#include "SHBrowseForFolder.h"
#include "SuperTagEditorView.h"
#include "SuperTagEditorDoc.h"
#include "DlgPluginSetup.h"

#include "INI/Profile.h"

#include <vector>

//#include "NewAPIs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if false
//////////////////////////////////////////////////////////////////////////////
//GetLongPathName
//�Z���t�@�C�����𒷂��t�@�C�����ɕϊ�
//////////////////////////////////////////////////////////////////////////////
/*
�����F
    LPSTR lpszLongFileName  �����t�@�C�������󂯎��o�b�t�@�ւ̃|�C���^
    LPSTR lpszShortPathName �Z���t�@�C�������܂ރp�X���ւ̃|�C���^
    DWORD dwSize            �o�b�t�@�T�C�Y
�߂�l:
    DWORD �o�b�t�@�ɃR�s�[����������̒���
        �O�̂Ƃ��ُ�I��
*/
DWORD GetLongPathName(LPSTR lpszLongFileName, LPSTR lpszShortPathName, DWORD dwSize)
{
	WIN32_FIND_DATA fd;
	HANDLE	hFind;

	//�Z���t�@�C�����Ńt�@�C��������
	if((hFind = FindFirstFile(lpszShortPathName,&fd)) != INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		if((DWORD)lstrlen(fd.cFileName) <= dwSize) {
			//�����t�@�C�������o�b�t�@�ɃR�s�[
			lstrcpy(lpszLongFileName,fd.cFileName);
			//�o�b�t�@�ɃR�s�[�����������Ԃ�
			return lstrlen(lpszLongFileName);
		}
	}
	lstrcpy(lpszLongFileName, "");
	//�o�b�t�@�̃T�C�Y�𒴂������t�@�C����������Ȃ������Ƃ��ُ͈�I��
	return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc

IMPLEMENT_DYNCREATE(CSuperTagEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSuperTagEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CSuperTagEditorDoc)
	ON_UPDATE_COMMAND_UI(ID_OPEN_FOLDER, OnUpdateOpenFolder)
	ON_COMMAND(ID_OPEN_FOLDER, OnOpenFolder)
	ON_UPDATE_COMMAND_UI(ID_SAVE_ALL_TAG, OnUpdateSaveAllTag)
	ON_COMMAND(ID_SAVE_ALL_TAG, OnSaveAllTag)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIELD_SI, OnUpdateEditFieldSi)
	ON_COMMAND(ID_EDIT_FIELD_SI, OnEditFieldSi)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TD3_TAG, OnUpdateEditTd3Tag)
	ON_COMMAND(ID_EDIT_TD3_TAG, OnEditTd3Tag)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CHANGE_FIELD, OnUpdateEditChangeField)
	ON_COMMAND(ID_EDIT_CHANGE_FIELD, OnEditChangeField)
	ON_UPDATE_COMMAND_UI(ID_EXEC_CLASSIFICATION, OnUpdateExecClassification)
	ON_COMMAND(ID_EXEC_CLASSIFICATION, OnExecClassification)
	ON_COMMAND(ID_DLG_SETUP_PLUGIN, OnDlgSetupPlugin)
	ON_UPDATE_COMMAND_UI(ID_DLG_SETUP_PLUGIN, OnUpdateDlgSetupPlugin)
	ON_COMMAND(ID_DLG_ENVIRONMENT, OnDlgEnvironment)
	ON_UPDATE_COMMAND_UI(ID_DLG_ENVIRONMENT, OnUpdateDlgEnvironment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc �N���X�̍\�z/����

CSuperTagEditorDoc::CSuperTagEditorDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
	m_dlgLoadProgress = new CDlgCommonProgress;
	m_nArrayFileCount = 0;
	m_nArrayRequestFileCount = 0;

	m_StartLoadFileCount = 0;//�ǉ� by Kobarin
	m_TagUpdatingCount = 0;//�ǉ� by Kobarin

	m_bInitialized = false; /* StartInaction2 055 */
}

CSuperTagEditorDoc::~CSuperTagEditorDoc()
{
	delete	m_dlgLoadProgress;
	m_dlgLoadProgress = NULL;
}

BOOL CSuperTagEditorDoc::OnNewDocument()
{
	// �ۑ��m�F
	if (m_nArrayFileCount && CanCloseFrame((CFrameWnd *)AfxGetMainWnd()) == FALSE) {
		return FALSE;
	}

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	InitFileList();			// �t�@�C�����X�g�̏�����
	ClearRequestFiles();	// �ǉ����N�G�X�g�t�@�C�����X�g�̏�����


	if (!m_bInitialized) {
		InitPlugin();			// �v���O�C���̃��[�h

		// ����������
		GetView()->m_List.InitializeGrid();
		// �t�H���g�̐ݒ�
		GetView()->UpdateFontListView();
	}
	InitGridList();			// �O���b�h��������

	m_bInitialized = true;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc �V���A���C�[�[�V����

void CSuperTagEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	} else {
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc �N���X�̐f�f

#ifdef _DEBUG
void CSuperTagEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSuperTagEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorDoc �R�}���h
//****************************************************************************
//                              �t�@�C�����������֘A
//****************************************************************************
// =============================================
// CSuperTagEditorDoc::GetFileTime
// �T�v  : �t�@�C���̃^�C���X�^���v���擾
// ����  : sFileName	= �t�@�C����
// �߂�l: CTime		= �^�C���X�^���v(CTime(0):�G���[)
// =============================================
CTime CSuperTagEditorDoc::GetFileTime(LPCTSTR sFileName)
{
	CFileStatus	status;
	if (sFileName == NULL
	||  CFile::GetStatus(sFileName, status) == FALSE) {
		return(CTime(0));
	}
	if (status.m_mtime == -1) return(CTime(0));
	if (status.m_mtime.GetTime() == -1) return(CTime(0));
	return(status.m_mtime);
}

// =============================================
// CSuperTagEditorDoc::AddRequestFile
// �T�v  : �t�@�C���ǉ����N�G�X�g���X�g�Ƀt�@�C����ǉ�����
// ����  : sFillPath		= �t���p�X��
//       : pParent			= �e�A�C�e��(NULL����)
// �߂�l: int				= �C���f�b�N�X(�ǉ����Ȃ������ꍇ��-1��Ԃ�)
// =============================================
int CSuperTagEditorDoc::AddRequestFile(LPCTSTR sFillPath, CSuperGridCtrl::CTreeItem *pParent)
{
	//�ǉ� by Kobarin
	if(IsTagUpdating())//�^�O���X�V���͒ǉ����Ȃ�
		return -1;
	if(m_dlgLoadProgress->IsCanceled()){
		//�L�����Z���{�^����������Ă���
		return -1;
	}

	FILE_STATE	stat;
	stat.strFullPathName = sFillPath;
	stat.pParent = pParent;
	// �z��ɒǉ�
	if (m_nArrayRequestFileCount > m_arrayRequestFiles.GetUpperBound()) {
		// �z�񂪑���Ȃ��ꍇ�́A�m�ۃT�C�Y�𑝉�������
		m_arrayRequestFiles.SetSize(m_arrayRequestFiles.GetUpperBound() + ADD_ARRAY_SIZE);
	}

	m_arrayRequestFiles[m_nArrayRequestFileCount] = stat;
	m_nArrayRequestFileCount++;
	return(m_nArrayRequestFileCount);
}

void CSuperTagEditorDoc::StartLoadFile(LPCTSTR sTitle)
{
	if (m_dlgLoadProgress->GetSafeHwnd() == NULL) {
		// �v���O���X�o�[�𐶐�
//		m_dlgLoadProgress = new CDlgCommonProgress;
		m_dlgLoadProgress->Create(AfxGetMainWnd());
		m_dlgLoadProgress->SetWindowText(sTitle);
		UpdateAllViews(NULL);
	}
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("�t�@�C��������..."));
    m_StartLoadFileCount++;//�ǉ� by Kobarin
}

void CSuperTagEditorDoc::EndLoadFile(void)
{
	if(--m_StartLoadFileCount != 0)return;//�ǉ� by Kobarin
	if (m_dlgLoadProgress->GetSafeHwnd() != NULL) {
		m_dlgLoadProgress->DestroyWindow();
		m_dlgLoadProgress->SetCanceled(FALSE);//�ǉ� by Kobarin
//		delete	m_dlgLoadProgress;
//		m_dlgLoadProgress = NULL;
	}
}

// =============================================
// CSuperTagEditorDoc::ExecRequestFiles
// �T�v  : �ǉ����N�G�X�g���X�g�̃t�@�C������������
// ����  : bListClear		= �ǉ������I����ɒǉ����N�G�X�g�̃��X�g���N���A���邩�H
//       : bCheck			= �`�F�b�N�}�[�N��t���邩�ǂ���
// �߂�l: none
// =============================================
void CSuperTagEditorDoc::ExecRequestFiles(bool bListClear, bool bCheck)
{
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("�^�O���̓ǂݍ��ݒ�..."));

//	CMySuperGrid	&listCtrl = GetListCtrl();
//	listCtrl.SetRedraw(FALSE);

	//int		nFileCount = m_nArrayFileCount; /* STEP 015 */
	for (int no = 0; no < m_nArrayRequestFileCount; no++) {
		int		nFileCount = m_nArrayFileCount; /* STEP 015 */
		// �v���O���X�o�[�X�V
		if (m_nArrayRequestFileCount > 1) {
			m_dlgLoadProgress->SetPos(no * 100 / (m_nArrayRequestFileCount-1));
		}
		// �ǉ� by Kobarin
		if(m_dlgLoadProgress->IsCanceled()){
			//�L�����Z���{�^����������Ă���
			break;
		}

		// ���d�o�^�`�F�b�N
		if (nFileCount > 0) {
			bool	bFound = false;
			CString	*strFileName = &m_arrayRequestFiles[no].strFullPathName;
			int i; for (i = 0; i < nFileCount; i++) {
				if (GetListMP3(i)->strFullPathName == *strFileName) {
					bFound = true;
					break;
				}
			}
			if (bFound == false) {
				// �l�o�R�t�@�C�����̎擾
				FoundFile(*strFileName, m_arrayRequestFiles[no].pParent, bCheck);
			}
		} else {
			// �l�o�R�t�@�C�����̎擾
			FoundFile(m_arrayRequestFiles[no].strFullPathName, m_arrayRequestFiles[no].pParent, bCheck);
		}

	}

//	listCtrl.SetRedraw(TRUE);

	if (bListClear) {
		ClearRequestFiles();
	}

	if (g_bOptShowTotalParent) {
		CMySuperGrid	&listCtrl = GetListCtrl();
		listCtrl.CalcSum(0);
	}

	// �J�������̎�������
	if (g_bOptLoadFileAdjustColumn) {
		CMySuperGrid	&listCtrl = GetListCtrl();
		listCtrl.AutoSizeColumns(-1);
	}
}

// =============================================
// CSuperTagEditorDoc::ClearRequestFiles
// �T�v  : �ǉ����N�G�X�g�t�@�C�����X�g�̏�����
// ����  : none
// �߂�l: none
// =============================================
void CSuperTagEditorDoc::ClearRequestFiles(void)
{
	m_arrayRequestFiles.RemoveAll();
	m_arrayRequestFiles.SetSize(INIT_ARRAY_SIZE);
	m_nArrayRequestFileCount = 0;
}

// =============================================
// CSuperTagEditorDoc::LoadPlayList
// �T�v  : �v���C���X�g��ǂ݂��݂܂�
// ����  : sPlayList	= �v���C���X�g�t�@�C����(��΃p�X�Ŏw��)
// �߂�l: bool			= true:����I�� / false:�G���[
// =============================================
bool CSuperTagEditorDoc::LoadPlayList(LPCTSTR sPlayList)
{
	TRY {
		CFile	file;

		// �v���C���X�g�t�@�C���̃J�����g�f�B���N�g�����擾
		TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
		_tsplitpath(sPlayList, drive, dir, NULL, NULL);

		if (file.Open(sPlayList, CFile::modeRead)) {
			CString		strLine;
			CArchive	ar(&file, CArchive::load);
			while(ar.ReadString(strLine)) {
				if (strLine.GetLength() >= 3) {
					if (strLine[0] != '#') {
						// ���X�g�ɒǉ�
						if ((strLine[0] == '\\' && strLine[1] == '\\')
						||  strLine[1] == ':') {
							// ��΃p�X
							AddRequestFile(strLine, NULL);
						} else {
							// ���΃p�X
							CString	strName;
							if (strLine[0] == '\\') {
								// �h���C�u�������̃��[�g�w��
								strName.Format(TEXT("%s%s"), drive, strLine);
							} else {
								// ���[�g�ȊO
								strName.Format(TEXT("%s%s%s"), drive, dir, strLine);
							}
							AddRequestFile(strName, NULL);
						}
					}
				}
			}
		}
	}
	CATCH( CFileException, e) {
		CString	str;
		str.Format(TEXT("%s �̓ǂݍ��݂Ɏ��s���܂���"), sPlayList);
		MessageBox(NULL, str, TEXT("�t�@�C���G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
	}
	END_CATCH

	return(true);
}

// =============================================
// CSuperTagEditorDoc::FoundFile
// �T�v  : �t�@�C�������F�t�@�C���𔭌�
// ����  : sFileName	= �t�@�C����
//       : pItemParent	= �e�A�C�e��(NULL=���[�g)
//       : bCheck		= �`�F�b�N�}�[�N��t���邩�ǂ���
// �߂�l: bool			= true:����I�� / false:�ُ�I��
// =============================================
bool CSuperTagEditorDoc::FoundFile(LPCTSTR sFileName, CSuperGridCtrl::CTreeItem *pItemParent, bool bCheck)
{
	// �l�o�R�t�@�C�����̎擾
	CFileMP3	fileMP3;
	if (fileMP3.Attach(sFileName)) {
		int		nIndex = m_nArrayFileCount;

		// �z��ɒǉ�
		if (m_nArrayFileCount > m_arrayFiles.GetUpperBound()) {
			// �z�񂪑���Ȃ��ꍇ�́A�m�ۃT�C�Y�𑝉�������
			m_arrayFiles.SetSize(m_arrayFiles.GetUpperBound() + ADD_ARRAY_SIZE);
		}
		m_arrayFiles[m_nArrayFileCount] = fileMP3.m_fileMP3;
		m_nArrayFileCount++;

		// ���X�g�R���g���[�����擾
		CMySuperGrid	&listCtrl = GetListCtrl();
		if (g_classInfo.nType == 0) {
			// �t�H���_�ŕ���
			listCtrl.AddFile(GetListMP3(nIndex), pItemParent, (LPARAM)nIndex, bCheck);
		} else {
			// �^�O���ŕ���
			listCtrl.AddFile2(GetListMP3(nIndex), pItemParent, (LPARAM)nIndex, bCheck);
		}

		return(true);
	} else {
		CString	strMessage;
		strMessage.Format(TEXT("%s �̓ǂݍ��݂Ɏ��s���܂���"), sFileName);
		//MessageBox(NULL, strMessage, "�t�@�C���ǂݍ��݃G���[", MB_ICONSTOP|MB_OK|MB_TOPMOST);
		((CMainFrame *)AfxGetMainWnd())->SetStatusBarText(strMessage);	// �X�e�[�^�X�o�[�ɃG���[��\��
	}

	return(false);
}

// =============================================
// CSuperTagEditorDoc::SearchFileReent
// �T�v  : �t�@�C�������F��������
// ����  : sDir		= ��������f�B���N�g��
//       : pItemDir	= �f�B���N�g���A�C�e��(NULL=���[�g)
// �߂�l: bool		= true:����I�� / false:�ُ�I��
// =============================================
bool CSuperTagEditorDoc::SearchFileReent(LPCTSTR sDir, CSuperGridCtrl::CTreeItem *pItemDir)
{
	//�ǉ� by Kobarin
	if(m_dlgLoadProgress->IsCanceled()){
		return true;
	}

	HANDLE	hFindFile;
	WIN32_FIND_DATA	data;
	std::vector<TCHAR> sCurFile(MAX_PATH + 1);

	// �����J�n�f�B���N�g���̏ꍇ�̏���
	if (pItemDir == NULL) {
		// ���[�g�Ƀf�B���N�g���A�C�e���ǉ�
		if (g_classInfo.nType == 0) {
			// �t�H���_�ŕ���
			CMySuperGrid	&listCtrl = GetListCtrl();
//			pItemDir = listCtrl.AddDirectory(sDir, listCtrl.m_itemClassOK, 0);
			pItemDir = listCtrl.m_pItemRoot;
		}
	}

	// ����f�B���N�g���̑S�Ẵt�@�C��������
	// �T�u�f�B���N�g���̌����͍s��Ȃ�
	//static	char	*sFindPattern[] = {
	//	"*.rmp", "*.mp3", "*.ape", "*.wma", "*.wmv", "*.asf", "*.ogg"/* Conspiracy 196 */, "*.wav"/* Conspiracy 197 */, "*.avi"/* Conspiracy 197 */, NULL,
	//};
	extern void GetFileExtList(CStringArray& arExt);
	CStringArray arExt;
	GetFileExtList(arExt);
	for (int nType = 0; nType < arExt.GetSize() ; nType++) {
		// �����t�@�C���̃p�^�[����ݒ�
		_stprintf(sCurFile.data(), TEXT("%s%s"), sDir, TEXT("*.") + arExt[nType]);

		// �t�@�C������
		if ((hFindFile = FindFirstFile(sCurFile.data(), &data)) != INVALID_HANDLE_VALUE) {
			do {
				m_dlgLoadProgress->SetPos(0);//�ǉ� by Kobarin
				if(m_dlgLoadProgress->IsCanceled()){
					break;
				}
				if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					// �f�B���N�g���t���t�@�C�����쐬
					lstrcpy(sCurFile.data(), sDir);
					lstrcat(sCurFile.data(), data.cFileName);
					m_sTargetFile = sCurFile.data();
					// �t�@�C����������
					AddRequestFile(m_sTargetFile.c_str(), pItemDir);
				}
			} while(FindNextFile(hFindFile, &data) == TRUE);
		}
		if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile);
	}

	if (g_bEnableSearchSubDir) {	// �T�u�f�B���N�g���̌���
		_stprintf(sCurFile.data(), TEXT("%s*.*"), sDir);
		if ((hFindFile = FindFirstFile(sCurFile.data(), &data)) != INVALID_HANDLE_VALUE) {
			do {
				m_dlgLoadProgress->SetPos(0);//�ǉ� by Kobarin
				if(m_dlgLoadProgress->IsCanceled()){
					break;
				}
				if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
					if (lstrcmp(data.cFileName, TEXT(".") ) != 0		// �J�����g�f�B���N�g���ȊO
					&&  lstrcmp(data.cFileName, TEXT("..")) != 0		// �e�f�B���N�g���ȊO
					&& (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0/* STEP 032 */) { // �B���t�H���_�ȊO
						// �f�B���N�g���t���t�@�C�����쐬
						lstrcpy(sCurFile.data(), sDir);
						lstrcat(sCurFile.data(), data.cFileName);
						m_sTargetFile = sCurFile.data();
						// �c���[�Ƀf�B���N�g���A�C�e���ǉ�
						CSuperGridCtrl::CTreeItem	*pItemSubDir = NULL;
						if (g_classInfo.nType == 0) {
							// �t�H���_�ŕ���
							CMySuperGrid	&listCtrl = GetListCtrl();
							pItemSubDir = listCtrl.AddDirectory(data.cFileName, pItemDir);
						}
						// �T�u�f�B���N�g���˓�����
						lstrcat(sCurFile.data(), TEXT("\\"));
						if (!SearchFileReent(sCurFile.data(), pItemSubDir)) {
							break;				// �T�u�f�B���N�g�������G���[
						}
						sCurFile[lstrlen(sCurFile.data())-1] = NULL;
						m_sTargetFile = sCurFile.data();	// �T�u�f�B���N�g���̏���������̂ōĐݒ�
					}
				}
			} while(FindNextFile(hFindFile, &data) == TRUE);
		}
		if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile); /* 2004.02.18 �������Ɉړ� */
	}

	return(true);
}

// =============================================
// CSenkyokuMeijinDoc::SelectDirectory
// �T�v  : �t�H���_�Q�ƃ_�C�A���O����
// ����  : sLocal			= �p�X(���o��)
// �߂�l: bool
// =============================================
BOOL CSuperTagEditorDoc::SelectDirectory(LPTSTR sLocal)
{
	bool	bResult;
	CSHBrowseForFolder	browse(true, g_bEnableSearchSubDir);
	bResult = browse.Exec(sLocal);
	g_bEnableSearchSubDir = browse.GetSearchSubDirState();
	return(bResult);
}

void CSuperTagEditorDoc::OpenFolder(LPCTSTR sOpenDir)
{
	//�ǉ� by Kobarin
	if(IsTagUpdating())//�^�O���X�V���͒ǉ����Ȃ�
		return;
	if(m_dlgLoadProgress->IsCanceled()){
		//�L�����Z���{�^����������Ă���
		return ;
	}

	((CSuperTagEditorApp *)AfxGetApp())->AddToRecentFileList (sOpenDir); /* StartInaction 053 */

	// �f�B���N�g���̃t�@�C����ǂݍ���
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};

	lstrcpy(sFolderName, sOpenDir);

	int		nLen;
	nLen = lstrlen(sFolderName);
	if (nLen > 0) {
		if (IsFolderName(sFolderName) == false) {
			// �Ō�� '\\' �ł��鎖
			lstrcat(sFolderName, TEXT("\\"));
		}

		// �v���O���X�o�[������
		BOOL	bIsOpen = (m_dlgLoadProgress->GetSafeHwnd() == NULL) ? TRUE : FALSE;
		if (bIsOpen) StartLoadFile(TEXT("�l�o�R�t�@�C���ǂݍ��ݒ�..."));

		// �t�H���_�ǂݍ���
		if (g_classInfo.nType == 0) {
			// �t�H���_�ŕ���
			CMySuperGrid	&listCtrl = GetListCtrl();
			TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
			_tsplitpath(sFolderName, drive, dir, NULL, NULL);
			CString	str;
			str.Format(TEXT("%s%s"), drive, dir);
			SearchFileReent(sFolderName, listCtrl.AddDirectory(str, listCtrl.m_pItemRoot, 2));
		} else {
			// �^�O���ŕ���
			SearchFileReent(sFolderName);
		}

		// �ǉ����N�G�X�g�̂������t�@�C���̃^�O����ǂݍ���
#ifdef FLICKERFREE
		//CMySuperGrid	&listCtrl = GetListCtrl();
		//listCtrl.SetRedraw(FALSE);
		//GetView()->SetRedraw(FALSE);
#endif
		ExecRequestFiles(true, g_bOptLoadFileChecked);
#ifdef FLICKERFREE
		//listCtrl.SetRedraw(TRUE);
		//GetView()->SetRedraw(TRUE);
#endif

		// �v���O���X�o�[�I��
		if (bIsOpen) EndLoadFile();

		UpdateAllViews(NULL);
	}
}

void CSuperTagEditorDoc::OnUpdateOpenFolder(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorDoc::OnOpenFolder()
{
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};

	// �t�H���_�I���_�C�A���O���J��
	lstrcpy(sFolderName, g_strCurrentDirectory);
	if (SelectDirectory(sFolderName) == TRUE) {
		g_strCurrentDirectory = sFolderName;

		OpenFolder(g_strCurrentDirectory);
	}
}

BOOL CSuperTagEditorDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	if (CheckFileModified()) {
		int		nResult;
		nResult = MessageBox(AfxGetMainWnd()->GetSafeHwnd(), TEXT("�^�O��񂪕ύX����Ă��܂��B\n"
								   "�ύX����Ă���t�@�C�����X�V���Ă���낵���ł����H"),
								   TEXT("�ۑ��m�F"), MB_YESNOCANCEL);
		switch(nResult) {
		case IDYES:
			//OnSaveAllTag();
			SaveAllTag(TRUE);
			if (m_bSaveAllTagResult == false) {
				return(FALSE);
			}
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return(FALSE);
		}
	}
	InitFileList(); /* Conspiracy 196 */
	return CDocument::CanCloseFrame(pFrame);
}

void CSuperTagEditorDoc::OnUpdateSaveAllTag(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorDoc::OnSaveAllTag()
{
	SaveAllTag(FALSE);
}

void CSuperTagEditorDoc::OnUpdateFileOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

// [�I�v�V�����ݒ�]-[�S��]
void CSuperTagEditorDoc::OnUpdateDlgEnvironment(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorDoc::OnDlgEnvironment()
{
	CDlgEnvSheet	dialog(TEXT("�I�v�V�����ݒ�"));
	bool	bKeepTimeStamp = g_bOptKeepTimeStamp;
	bool	bHideMP3ListFile = g_bOptHideMP3ListFile;
	bool	bFileNameMaxCellColor = g_bFileNameMaxCellColor; /* SeaKnows 036 */

	dialog.m_pDoc = this;
	dialog.DoModal(CDlgEnvSheet::ENV_ALL);

	CMySuperGrid	&listCtrl = GetListCtrl();

	// �^�C���X�^���v�ێ��̐ݒ荀�ڂ��ύX���ꂽ
	// MP3 List �̃t�@�C�����\���̗L�����ύX���ꂽ
	if (bKeepTimeStamp != g_bOptKeepTimeStamp
	||  bHideMP3ListFile != g_bOptHideMP3ListFile) {
		// �\�����X�V������
		listCtrl.UpdateAllFiles();
		// �J�������̎�������
		if (g_bOptLoadFileAdjustColumn) listCtrl.AutoSizeColumns(-1);
	}
	if (bFileNameMaxCellColor != g_bFileNameMaxCellColor) { /* SeaKnows 036 */
		// �\�����X�V������
		listCtrl.UpdateAllFiles();
	}

	// ���ލς݃t�H���_�̖��̂��X�V����
	listCtrl.UpdateSyncFolderItemName();

	// �W���������X�g���X�V������
	listCtrl.MakeStrListGenre();
}

BOOL CSuperTagEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;

	TCHAR	sFileName[FILENAME_MAX];
	lstrcpy(sFileName, lpszPathName);

	// ����������
	if (m_bInitialized == false) { /* StartInaction2 055 */
		InitFileList();			// �t�@�C�����X�g�̏�����
		ClearRequestFiles();	// �ǉ����N�G�X�g�t�@�C�����X�g�̏�����
		m_bInitialized = true;
		InitPlugin();			// �v���O�C���̃��[�h
		// ����������
		GetView()->m_List.InitializeGrid();
		// �t�H���g�̐ݒ�
		GetView()->UpdateFontListView();
		InitGridList();			// �O���b�h��������
	}

	// �v���C���X�g���H
	CString	strFileName = sFileName;
	strFileName.MakeLower();
	if (strFileName.Find(TEXT(".m3u")) != -1) {
		// �v���C���X�g�ǉ�
		GetView()->LoadPlayList(sFileName);
		return(TRUE);
	}

	// �v���O���X�o�[������
	StartLoadFile(TEXT("�l�o�R�t�@�C���ǂݍ��ݒ�..."));

	CFileStatus	status;
	if (CFile::GetStatus(sFileName, status) == TRUE && status.m_mtime != -1) {
		if (status.m_attribute & CFile::directory) {
			// �f�B���N�g���̏ꍇ
			// �Ō�� '\\' �͕K�v
			if (IsFolderName(sFileName) == false) {
				lstrcat(sFileName, TEXT("\\"));
			}
			bool bEnableSearchSubDir = g_bEnableSearchSubDir; /* TyphoonSwell 026 */
			if (g_bOptDropSearchSubFolder) {
				g_bEnableSearchSubDir = true;
			}

			/* WildCherry2 078 */
			//SearchFileReent(sFileName);
			OpenFolder(sFileName);
			g_bEnableSearchSubDir = bEnableSearchSubDir;
		} else {
			// �t�@�C���̏ꍇ(�����O�t�@�C�����ɕϊ����Ă��珈������)
			if (GetLongPathName(sFileName, status.m_szFullName, FILENAME_MAX) != 0) {
				CString	strLongFileName;
				TCHAR	drive[_MAX_DRIVE], dir[_MAX_DIR];
				_tsplitpath(status.m_szFullName, drive, dir, NULL, NULL);
				strLongFileName.Format(TEXT("%s%s%s"), drive, dir, sFileName);
				AddRequestFile(strLongFileName, NULL);
			}
		}
	} else if (lstrlen(sFileName) == 3 && sFileName[1] == ':' && sFileName[2] == '\\') { /* STEP 027 */
			// �h���C�u�̏ꍇ
			// ���ʊK�w������
			bool bEnableSearchSubDir = g_bEnableSearchSubDir;
			if (g_bOptDropSearchSubFolder) {
				g_bEnableSearchSubDir = true;
			}
			OpenFolder(sFileName);
			g_bEnableSearchSubDir = bEnableSearchSubDir;
	}

	// �ǉ����N�G�X�g�̂������t�@�C���̃^�O����ǂݍ���
#ifdef FLICKERFREE
		//GetView()->SetRedraw(FALSE);
#endif
	ExecRequestFiles(true, g_bOptLoadFileChecked);
#ifdef FLICKERFREE
		//GetView()->SetRedraw(TRUE);
#endif

	// �v���O���X�o�[�I��
	EndLoadFile();

	UpdateAllViews(NULL);

	return(TRUE);
}

// SI �t�B�[���h��\���^�ҏW
void CSuperTagEditorDoc::OnUpdateEditFieldSi(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_bOptEditFieldSIF ? TRUE : FALSE);
}
void CSuperTagEditorDoc::OnEditFieldSi()
{
	if (g_bOptEditFieldSIF != true) {
		g_bOptEditFieldSIF = true;

		// �����X�V
		UpdateAllFiles(true);
	}
}

// ID3 tag ����\���^�ҏW
void CSuperTagEditorDoc::OnUpdateEditTd3Tag(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_bOptEditFieldSIF ? FALSE : TRUE);
}
void CSuperTagEditorDoc::OnEditTd3Tag()
{
	if (g_bOptEditFieldSIF != false) {
		g_bOptEditFieldSIF = false;

		// �����X�V
		UpdateAllFiles(true);
	}
}

// [ID3 tag] <=> [SI�t�B�[���h]��ؑ�
void CSuperTagEditorDoc::OnUpdateEditChangeField(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorDoc::OnEditChangeField()
{
	g_bOptEditFieldSIF = g_bOptEditFieldSIF ? false : true;

	// �����X�V
	UpdateAllFiles(true);
}

// ���ޕ\���̍X�V
void CSuperTagEditorDoc::OnUpdateExecClassification(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_nArrayFileCount > 0 && g_classInfo.nType) ? TRUE : FALSE);
}
void CSuperTagEditorDoc::OnExecClassification()
{
	// �v���C���X�g�ǂ݂���
	StartLoadFile(TEXT("���ޕ\�����X�V��..."));
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("������..."));
	m_dlgLoadProgress->EnableCancelButton(FALSE);

	// �O���b�h�ɍēo�^
	CMySuperGrid	&listCtrl = GetListCtrl();
	listCtrl.DeleteAllEx();			// �O���b�h��������
	for (int no = 0; no < m_nArrayFileCount; no++) {
		// �v���O���X�o�[�X�V
		if (m_nArrayFileCount > 1) {
			m_dlgLoadProgress->SetPos(no * 100 / (m_nArrayFileCount-1));
		}
		// �t�H���_�ŕ���
		if (GetListMP3(no)->nFormat != FILE_FORMAT_UNKNOWN) {
			listCtrl.AddFile2(GetListMP3(no), NULL, (LPARAM)no, g_bOptLoadFileChecked);
		}
	}

	// �t�@�C���̃^�O����ǂݍ���
	EndLoadFile();

	UpdateAllViews(NULL);
}

#include "DlgFileOverWrite.h"
bool CSuperTagEditorDoc::CheckFileAttribute(FILE_MP3 *fileMP3)
{
	extern	int		g_nWriteTagProcFlag;
	if (g_nWriteTagProcFlag == 3) {
		return(false);						// �ȍ~���~
	}

	// �t�@�C�������擾
	CFileStatus	fileStatus;
	if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
		return(false);
	}
	// �ǂݏo����p�t�@�C���́A�㏑���m�F���s��
	if (fileStatus.m_attribute & CFile::readOnly) {
		if (g_nWriteTagProcFlag != 1) {
			// �㏑���m�F�_�C�A���O���J��
			CDlgFileOverWrite	dialog;
			dialog.m_strFileName = fileMP3->strFullPathName;
			dialog.m_strSize.Format(TEXT("%ld byte"), fileStatus.m_size);
			if (fileStatus.m_mtime.GetTime() < 0) {
				dialog.m_strTimeStamp.Format(TEXT("----/--/-- --:--:--"));
			} else {
				dialog.m_strTimeStamp.Format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
											 fileStatus.m_mtime.GetYear(),
											 fileStatus.m_mtime.GetMonth(),
											 fileStatus.m_mtime.GetDay(),
											 fileStatus.m_mtime.GetHour(),
											 fileStatus.m_mtime.GetMinute(),
											 fileStatus.m_mtime.GetSecond());
			}
			dialog.DoModal();
			g_nWriteTagProcFlag = dialog.m_nResult;
			switch(dialog.m_nResult) {
			case 0:				// ���̃t�@�C�����㏑��
			case 1:				// �ȍ~�S�ď㏑��
				break;
			case 2:				// �X�L�b�v
			case 3:				// ���~
			default:
				return(false);
			}
		}

		// �ǂݍ��ݐ�p����������
		SetFileAttributes(fileMP3->strFullPathName, CFile::normal);
	}
	return(true);
}

void CSuperTagEditorDoc::OnDlgSetupPlugin()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CDlgPluginSetup	dialog;

	if (dialog.DoModal() == IDOK) {
		((CSuperTagEditorApp *)AfxGetApp())->UpdateAccelerator(); /* STEP 030 */
	}
}

void CSuperTagEditorDoc::OnUpdateDlgSetupPlugin(CCmdUI* pCmdUI)
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorDoc::SaveAllTag(BOOL bSaveOnly)
{
	m_bSaveAllTagResult = true;
	extern	int		g_nWriteTagProcFlag;

	// �v���O���X�o�[�\��
	StartLoadFile(TEXT("�^�O�����X�V��..."));
	m_dlgLoadProgress->SetDlgItemText(IDC_ST_MESSAGE, TEXT("������..."));

	// �X�V�t�@�C���̃^�O���X�V
	g_nWriteTagProcFlag = 0;
	for (int no = 0; no < m_nArrayFileCount; no++) {
		FILE_MP3	*fileMP3 = GetListMP3(no);
		if (m_nArrayFileCount != 1) {
			// �v���O���X�o�[�X�V
			m_dlgLoadProgress->SetPos(no * 100 / (m_nArrayFileCount-1));
			// �ǉ� by Kobarin
			if(m_dlgLoadProgress->IsCanceled()){
				m_bSaveAllTagResult = false;
				break;
			}
		}
		if (fileMP3->nFormat != FILE_FORMAT_UNKNOWN && fileMP3->bModifyFlag) {
			if (CFileMP3::WriteTag(fileMP3, g_bOptKeepTimeStamp)) {
				fileMP3->bModifyFlag = FALSE;
			} else {
				MessageBox(NULL, TEXT("�^�O�����t�@�C�����X�V�ł��܂���ł���"), fileMP3->strFullPathName, MB_ICONSTOP|MB_OK|MB_TOPMOST);
				m_bSaveAllTagResult = false;
			}
		}
	}

	// �v���O���X�o�[�I��
	EndLoadFile();

	if (bSaveOnly)	return;

	// �����X�V
	CMySuperGrid	&listCtrl = GetListCtrl();
#ifdef FLICKERFREE
	//listCtrl.SetRedraw(FALSE);
#endif
	listCtrl.UpdateAllFiles();
	UpdateAllFiles(true);
	if (g_bOptLoadFileAdjustColumn) listCtrl.AutoSizeColumns(-1); /* 2003.06.26 */
#ifdef FLICKERFREE
	//listCtrl.SetRedraw(TRUE);
#endif
}
