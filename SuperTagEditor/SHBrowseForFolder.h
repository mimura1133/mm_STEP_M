// ======================================================
// 概  要    ： フォルダ選択ダイアログのラッパクラス
// ターゲット： Windows95/98/NT
// 処理系    ： Visual C++ Ver 6.0
// 作成者    ： MERCURY
// 作成日    ： 00/06/17(土)
// 著作権表記： Copyright(C) 2000 MERCURY.
// ======================================================

#ifndef __SH_BROWSE_FOR_FOLDER_H__
#define __SH_BROWSE_FOR_FOLDER_H__


// ======================================
// =====   条件コンパイルフラグ     =====
// ======================================



// ======================================
// =====   インクルードファイル     =====
// ======================================



// ======================================
// =====           定  数           =====
// ======================================



// ======================================
// =====           マクロ           =====
// ======================================



// ======================================
// =====       typedef／enum        =====
// ======================================



// ======================================
// =====       構造体／共用体       =====
// ======================================



// ======================================
// =====         const 変数         =====
// ======================================



// ======================================
// =====        extern 変数         =====
// ======================================



// ======================================
// =====        static 変数         =====
// ======================================



// ======================================
// =====     関数プロトタイプ       =====
// ======================================



// ======================================
// =====      プログラム領域        =====
// ======================================


#ifdef __cplusplus

class	CSHBrowseForFolder	{
public:
	CSHBrowseForFolder(bool = false, bool = true);
	virtual	~CSHBrowseForFolder();

public:		// ======================================
			// =====      public メンバ関数     =====
			// ======================================
	inline	static	void	SetEnableSubDirButton(bool bFlag) {m_bEnableSubDirButton = bFlag;}
	inline	static	bool	GetEnableSubDirButton(void) {return(m_bEnableSubDirButton);}
	inline	static	void	SetSearchSubDirState(bool bFlag) {m_bSearchSubDirState = bFlag;}
	inline	static	bool	GetSearchSubDirState(void) {return(GetEnableSubDirButton() ? m_bSearchSubDirState : false);}
			bool		Exec(char *);
	inline	static	void	SetCheckBoxTitle(const char* title) {pCheckBoxTitle = title;}
	inline	static	const char* GetCheckBoxTitle(void) {return (pCheckBoxTitle);}


private:	// ======================================
			// =====     private メンバ関数     =====
			// ======================================
	static	bool	m_bEnableSubDirButton;
	static	bool	m_bSearchSubDirState;
	static	WNDPROC	m_VSSelectOrgProc;
	static	const char* pCheckBoxTitle;

	static	LRESULT CALLBACK VSSelectFolderSubProc(HWND, UINT, WPARAM, LPARAM);
	static	int CALLBACK CallbackSelectDir(HWND, UINT, LPARAM, LPARAM);

protected:	// ======================================
			// =====    protected メンバ関数    =====
			// ======================================
};

#endif

#endif
