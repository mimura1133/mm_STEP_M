// ======================================================
// �T  �v    �F �t�H���_�I���_�C�A���O�̃��b�p�N���X
// �^�[�Q�b�g�F Windows95/98/NT
// �����n    �F Visual C++ Ver 6.0
// �쐬��    �F MERCURY
// �쐬��    �F 00/06/17(�y)
// ���쌠�\�L�F Copyright(C) 2000 MERCURY.
// ======================================================

#ifndef __SH_BROWSE_FOR_FOLDER_H__
#define __SH_BROWSE_FOR_FOLDER_H__


// ======================================
// =====   �����R���p�C���t���O     =====
// ======================================



// ======================================
// =====   �C���N���[�h�t�@�C��     =====
// ======================================



// ======================================
// =====           ��  ��           =====
// ======================================



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



// ======================================
// =====        static �ϐ�         =====
// ======================================



// ======================================
// =====     �֐��v���g�^�C�v       =====
// ======================================



// ======================================
// =====      �v���O�����̈�        =====
// ======================================


#ifdef __cplusplus

class	CSHBrowseForFolder	{
public:
	CSHBrowseForFolder(bool = false, bool = true);
	virtual	~CSHBrowseForFolder();

public:		// ======================================
			// =====      public �����o�֐�     =====
			// ======================================
	inline	static	void	SetEnableSubDirButton(bool bFlag) {m_bEnableSubDirButton = bFlag;}
	inline	static	bool	GetEnableSubDirButton(void) {return(m_bEnableSubDirButton);}
	inline	static	void	SetSearchSubDirState(bool bFlag) {m_bSearchSubDirState = bFlag;}
	inline	static	bool	GetSearchSubDirState(void) {return(GetEnableSubDirButton() ? m_bSearchSubDirState : false);}
			bool		Exec(LPTSTR);
	inline	static	void	SetCheckBoxTitle(LPCTSTR title) {pCheckBoxTitle = title;}
	inline	static	LPCTSTR GetCheckBoxTitle(void) {return (pCheckBoxTitle);}


private:	// ======================================
			// =====     private �����o�֐�     =====
			// ======================================
	static	bool	m_bEnableSubDirButton;
	static	bool	m_bSearchSubDirState;
	static	WNDPROC	m_VSSelectOrgProc;
	static	LPCTSTR pCheckBoxTitle;

	static	LRESULT CALLBACK VSSelectFolderSubProc(HWND, UINT, WPARAM, LPARAM);
	static	int CALLBACK CallbackSelectDir(HWND, UINT, LPARAM, LPARAM);

protected:	// ======================================
			// =====    protected �����o�֐�    =====
			// ======================================
};

#endif

#endif
