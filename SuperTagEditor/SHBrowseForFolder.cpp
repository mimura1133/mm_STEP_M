// ======================================================
// �T  �v    �F �t�H���_�I���_�C�A���O�̃��b�p�N���X
// �^�[�Q�b�g�F Windows95/98/NT
// �����n    �F Visual C++ Ver 6.0
// �쐬��    �F MERCURY
// �쐬��    �F 00/06/17(�y)
// ���쌠�\�L�F Copyright(C) 2000 MERCURY.
// ======================================================



// ======================================
// =====   �����R���p�C���t���O     =====
// ======================================



// ======================================
// =====   �C���N���[�h�t�@�C��     =====
// ======================================
#include "stdafx.h"
#include "SHBrowseForFolder.h"



// ======================================
// =====           ��  ��           =====
// ======================================
#define IDC_CH_SUB_DIR		1000		// �t�H���_�Q�Ƃ�[�T�u�f�B���N�g��������]�{�^����ID



// ======================================
// =====           �}�N��           =====
// ======================================



// ======================================
// =====       typedef�^enum        =====
// ======================================



// ======================================
// =====       �\���́^���p��       =====
// ======================================



// ======================================
// =====         const �ϐ�         =====
// ======================================



// ======================================
// =====        extern �ϐ�         =====
// ======================================
bool	CSHBrowseForFolder::m_bEnableSubDirButton = false;
bool	CSHBrowseForFolder::m_bSearchSubDirState = false;
LPCTSTR CSHBrowseForFolder::pCheckBoxTitle = NULL;/* FunnyCorn 185 *///"�T�u�f�B���N�g��������";
WNDPROC	CSHBrowseForFolder::m_VSSelectOrgProc;



// ======================================
// =====        static �ϐ�         =====
// ======================================
static	CButton	*pSubDir = NULL; /* WildCherry2 080 */



// ======================================
// =====     �֐��v���g�^�C�v       =====
// ======================================



// ======================================
// =====      �v���O�����̈�        =====
// ======================================
// =============================================
// CSHBrowseForFolder::CSHBrowseForFolder
// Func  : �R���X�g���N�^
// Input : bEnable		= [�T�u�f�B���N�g��������]�{�^����ǉ����邩?
//       : bState		= [�T�u�f�B���N�g��������]�{�^���̏������
// Output: none
// =============================================
CSHBrowseForFolder::CSHBrowseForFolder(bool bEnable, bool bState)
{
	SetEnableSubDirButton(bEnable);
	SetSearchSubDirState(bState);
	SetCheckBoxTitle(TEXT("�T�u�f�B���N�g��������")); /* FunnyCorn 185 */
}

// =============================================
// CSHBrowseForFolder::~CSHBrowseForFolder
// Func  : �f�X�g���N�^
// Input : none
// Output: none
// =============================================
CSHBrowseForFolder::~CSHBrowseForFolder()
{
}

// =============================================
// VSSelectFolderSubProc
// �T�v  : �t�H���_�Q�Ƃ�[�T�u�f�B���N�g��������]�{�^������
// ����  : ...
// �߂�l: int CALLBACK
// =============================================
LRESULT CALLBACK CSHBrowseForFolder::VSSelectFolderSubProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ((uMsg == WM_COMMAND) && (LOWORD(wParam) == IDC_CH_SUB_DIR)) {
		// �{�^���̏���
		CWnd	*pClient = CWnd::FromHandle(hWnd);
		CButton	*pSubDir = (CButton *)pClient->GetDlgItem(IDC_CH_SUB_DIR);

		// �`�F�b�N��Ԃ𔽓]
		SetSearchSubDirState(GetSearchSubDirState() ? false : true);
		pSubDir->SetCheck(GetSearchSubDirState() ? TRUE : FALSE);
	}
	return(CallWindowProc(m_VSSelectOrgProc, hWnd, uMsg, wParam, lParam));
}

// =============================================
// CallbackSelectDir
// �T�v  : �t�H���_�Q�ƂŎg�p����R�[���o�b�N�֐�
//       : �J�����Ƃ��Ƀt�H���_��I��������ԂŊJ�����鏈��������ꍇ�ɕK�v
// ����  : ...
// �߂�l: int CALLBACK
// =============================================
int CALLBACK CSHBrowseForFolder::CallbackSelectDir(HWND hWnd, UINT uMsg, LPARAM, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED) {
		// �f�t�H���g�̃t�H���_��I��������
		SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
		{
			// �t�H���_�[�c���[�ŏ�ɑI����Ԃ�\��
			HWND hwndTree = FindWindowEx(hWnd, NULL, TEXT("SysTreeView32"), NULL);
			if (hwndTree != NULL) {
				LONG style;
				style = GetWindowLong(hwndTree, GWL_STYLE);
				style |= TVS_SHOWSELALWAYS;
				SetWindowLong(hwndTree, GWL_STYLE, style);
			}
		}

		// [�T�u�f�B���N�g��������] �{�^����ǉ�
		if (GetEnableSubDirButton()) {
			pSubDir = new CButton; /* WildCherry2 080 */
			if (pSubDir) {
				RECT	rectClient, rectOK, rectCancel, rect;
				CWnd	*pClient	= CWnd::FromHandle(hWnd);
				CWnd	*pOK		= pClient->GetDlgItem(IDOK);
				CWnd	*pCANCEL	= pClient->GetDlgItem(IDCANCEL);
				// �N���C�A���g�E�B���h�E�T�C�Y���擾
				pClient->GetClientRect(&rectClient);
				// [OK] / [CANCEL] �̃E�B���h�E�ʒu���擾
				pOK->GetWindowRect(&rectOK);
				pCANCEL->GetWindowRect(&rectCancel);
				pClient->ScreenToClient(&rectOK) ;
				pClient->ScreenToClient(&rectCancel) ;
				// �T�u�f�B���N�g���Ώۃ`�F�b�N�{�^���� RECT �����߂�
				rect = CRect(rectClient.right - rectCancel.right,
				             rectCancel.top,
				             rectOK.left,
				             rectCancel.bottom);
				// �`�F�b�N�{�^���쐬
				if (pSubDir->Create(GetCheckBoxTitle(),
				                    WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP,
				                    rect, pClient, IDC_CH_SUB_DIR)) {
					// �t�H���g��ݒ�
					HFONT hFontCurr = (HFONT)pClient->SendMessage(WM_GETFONT, 0, 0);
					if (hFontCurr != NULL) pSubDir->PostMessage(WM_SETFONT, (WPARAM)hFontCurr, 0); 

					// �`�F�b�N��Ԃ�ݒ�
					pSubDir->SetCheck(GetSearchSubDirState() ? TRUE : FALSE);

					// �T�u�N���X��
					m_VSSelectOrgProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)VSSelectFolderSubProc);
				}
			}
		}
	}
	return 0;
}

// =============================================
// CSHBrowseForFolder::SelectDirectory
// �T�v  : �t�H���_�Q�ƃ_�C�A���O����
// ����  : sLocal			= �p�X(���o��)
// �߂�l: bool
// =============================================
bool CSHBrowseForFolder::Exec(LPTSTR sLocal)
{
	BOOL			bResult = FALSE;
	BROWSEINFO		bi;
	LPTSTR			lpBuffer;
	LPITEMIDLIST	pidlRoot;	   // �u���E�Y�̃��[�gPIDL
	LPITEMIDLIST	pidlBrowse;    // ���[�U�[���I������PIDL
	LPMALLOC		lpMalloc = NULL;

	HRESULT hr = SHGetMalloc(&lpMalloc);
	if (FAILED(hr)) return(FALSE);

	HWND	hwnd = AfxGetMainWnd()->GetSafeHwnd();

	// �u���E�Y���󂯎��o�b�t�@�̈�̊m��
	if ((lpBuffer = (LPTSTR) lpMalloc->Alloc(_MAX_PATH)) == NULL) {
		lpMalloc->Release(); /* WildCherry2 080 */
		return(FALSE);
	}
	// �_�C�A���O�\�����̃��[�g�t�H���_��PIDL���擾
	// ���ȉ��̓f�X�N�g�b�v�����[�g�Ƃ��Ă���B�f�X�N�g�b�v�����[�g�Ƃ���
	//	 �ꍇ�́A�P�� bi.pidlRoot �ɂO��ݒ肷�邾���ł��悢�B���̑��̓�
	//	 ��t�H���_�����[�g�Ƃ��鎖���ł���B�ڍׂ�SHGetSpecialFolderLoca
	//	 tion�̃w���v���Q�Ƃ̎��B
	if (!SUCCEEDED(SHGetSpecialFolderLocation(	hwnd,
												CSIDL_DESKTOP,
												&pidlRoot))) {
		lpMalloc->Free(lpBuffer);
		lpMalloc->Release(); /* WildCherry2 080 */
		return(FALSE);
	}
	// BROWSEINFO�\���̂̏����l�ݒ�
	// ��BROWSEINFO�\���̂̊e�����o�̏ڍא������w���v���Q��
	bi.hwndOwner = hwnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = lpBuffer;
	bi.lpszTitle = TEXT("�t�H���_��I�����ĉ������B");
	bi.ulFlags = 0;
	bi.lpfn = CallbackSelectDir;		// �R�[���o�b�N�֐��̃A�h���X��ݒ�
	bi.lParam = (LPARAM)sLocal;			// �w�肵�����p�X��ݒ�
	// �t�H���_�I���_�C�A���O�̕\��
	pidlBrowse = SHBrowseForFolder(&bi);
	if (pidlBrowse != NULL) {
		// PIDL�`���̖߂�l�̃t�@�C���V�X�e���̃p�X�ɕϊ�
		if (SHGetPathFromIDList(pidlBrowse, lpBuffer)) {
			// �擾����
			lstrcpy(sLocal, lpBuffer);
			bResult = TRUE;
		}
		// SHBrowseForFolder�̖߂�lPIDL�����
		lpMalloc->Free(pidlBrowse);
	}
	// �N���[���A�b�v����
	lpMalloc->Free(pidlRoot);
	lpMalloc->Free(lpBuffer);
	lpMalloc->Release();
	if (pSubDir) { /* WildCherry2 080 */
		delete pSubDir;
		pSubDir = NULL;
	}
	return(bResult ? true : false);
}
