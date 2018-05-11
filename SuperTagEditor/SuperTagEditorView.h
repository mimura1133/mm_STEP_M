// SuperTagEditorView.h : CSuperTagEditorView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMTLOGVIEWVIEW_H__D67B644C_BFAB_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_DMTLOGVIEWVIEW_H__D67B644C_BFAB_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dde/kbdde.h"
#include "MainFrm.h"
#include "HtmlHelp.h"
#include "MySuperGrid.h"

class	CSuperTagEditorDoc;

class CSuperTagEditorView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CSuperTagEditorView();
	DECLARE_DYNCREATE(CSuperTagEditorView)

// アトリビュート
public:
	CMySuperGrid	m_List;
	CSuperTagEditorDoc* GetDocument();
	bool	m_bFirstInit;
	CFont	*m_fontListView;

	inline	CMySuperGrid	&GetListCtrl(void) {return(m_List);}
	inline	void	UpdateRegistory(void)	{
				((CSuperTagEditorApp *)AfxGetApp())->WriteRegistry();
			}
	inline	void	ReleaseFontListView(void) {
				delete	m_fontListView;
				m_fontListView = NULL;
			}
	inline	void	UpdateFontListView(void) {
				ReleaseFontListView();
				if (g_fontReport.lfFaceName[0] != '\0') {
					m_fontListView = new CFont;
					if (m_fontListView->CreateFontIndirect(&g_fontReport)) {
						m_List.SetFont(m_fontListView, TRUE);
						m_List.SetHeaderFont(g_fontReport);
						// カラム幅の自動調節
						if (g_bOptLoadFileAdjustColumn) m_List.AutoSizeColumns(-1);
					}
				}
			}
	inline	BOOL	PlayerTypeIsKbmplay(void) {
				return((g_nOptPlayerType == PLAYER_KBMPLAY) ? TRUE : FALSE);
			}
	inline	BOOL	PlayerTypeIsLilith(void) { /* WildCherry 070 */
				return((g_nOptPlayerType == PLAYER_LILITH) ? TRUE : FALSE);
			}

	bool	ExecuteWinamp();
	bool	ExecuteLilith(void); /* WildCherry 070 */
	void	ExecWinampCommand(int, LPARAM = 0);
	void	ExecKbmplayCommand(int, LPARAM = 0, BOOL bPlay = FALSE);//追加 by Kobarin
	void	ExecLilithCommand(int, LPARAM = 0, BOOL bPlay = TRUE); /* WildCherry 070 */
	void	ExecReplace(bool);
	void	LoadPlayList(LPCTSTR);
	void	PlayFile(int);
	bool	LoadFormatFile(LPCTSTR, CString *, CString *, CString *);
	bool	ExecWriteList(WRITE_FORMAT *);
	//追加 by Kobarin
	static HDDEDATA CALLBACK DdemlCallback(UINT uType, UINT uFmt,
										   HCONV hConv, HSZ hszTpc1, HSZ hszTpc2, 
										   HDDEDATA hData,
										   DWORD dwData1, DWORD dwData2);
	void   OnDDE(LPTSTR sFileName);
	//追加 ここまで

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSuperTagEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // 構築後の最初の１度だけ呼び出されます。
	virtual void PostNcDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSuperTagEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSuperTagEditorView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateCellCopyDown(CCmdUI* pCmdUI);
	afx_msg void OnCellCopyDown();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateConvFilenameToTrackname(CCmdUI* pCmdUI);
	afx_msg void OnConvFilenameToTrackname();
	afx_msg void OnConvTracknameToFilename();
	afx_msg void OnUpdateConvTracknameToFilename(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConvTag2fileUser(CCmdUI* pCmdUI);
	afx_msg void OnConvTag2fileUser();
	afx_msg void OnUpdateConvFile2tagUser(CCmdUI* pCmdUI);
	afx_msg void OnConvFile2tagUser();
	afx_msg void OnUpdateSetNumber(CCmdUI* pCmdUI);
	afx_msg void OnSetNumber();
	afx_msg void OnUpdateWinampPlay(CCmdUI* pCmdUI);
	afx_msg void OnWinampPlay();
	afx_msg void OnUpdateWinampStop(CCmdUI* pCmdUI);
	afx_msg void OnWinampStop();
	afx_msg void OnUpdateWinampExit(CCmdUI* pCmdUI);
	afx_msg void OnWinampExit();
	afx_msg void OnUpdateSortList(CCmdUI* pCmdUI);
	afx_msg void OnSortList();
	afx_msg void OnUpdateWritePlaylist(CCmdUI* pCmdUI);
	afx_msg void OnWritePlaylist();
	afx_msg void OnUpdateWinampPlayNext(CCmdUI* pCmdUI);
	afx_msg void OnWinampPlayNext();
	afx_msg void OnUpdateWinampPlayPrev(CCmdUI* pCmdUI);
	afx_msg void OnWinampPlayPrev();
	afx_msg void OnUpdateLoadPlaylist(CCmdUI* pCmdUI);
	afx_msg void OnLoadPlaylist();
	afx_msg void OnUpdateCheckWord(CCmdUI* pCmdUI);
	afx_msg void OnCheckWord();
	afx_msg void OnUpdateSelectFilesCheck(CCmdUI* pCmdUI);
	afx_msg void OnSelectFilesCheck();
	afx_msg void OnUpdateSelectFilesUncheck(CCmdUI* pCmdUI);
	afx_msg void OnSelectFilesUncheck();
	afx_msg void OnUpdateSelectDeleteList(CCmdUI* pCmdUI);
	afx_msg void OnSelectDeleteList();
	afx_msg void OnUpdateSelectDeleteFile(CCmdUI* pCmdUI);
	afx_msg void OnSelectDeleteFile();
	afx_msg void OnUpdateSelectEditDestory(CCmdUI* pCmdUI);
	afx_msg void OnSelectEditDestory();
	afx_msg void OnUpdateAllFilesCheck(CCmdUI* pCmdUI);
	afx_msg void OnAllFilesCheck();
	afx_msg void OnReverseCheck();
	afx_msg void OnUpdateAllFilesUncheck(CCmdUI* pCmdUI);
	afx_msg void OnAllFilesUncheck();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateCheckFilesSelect(CCmdUI* pCmdUI);
	afx_msg void OnCheckFilesSelect();
	afx_msg void OnUpdateAdjustColumnWidth(CCmdUI* pCmdUI);
	afx_msg void OnAdjustColumnWidth();
	afx_msg void OnUpdateHelpIndex(CCmdUI* pCmdUI);
	afx_msg void OnHelpIndex();
	afx_msg void OnUpdateConvFormatEx(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatEx01();
	afx_msg void OnConvFormatEx02();
	afx_msg void OnConvFormatEx03();
	afx_msg void OnConvFormatEx04();
	afx_msg void OnConvFormatEx05();
	afx_msg void OnConvFormatEx06();
	afx_msg void OnConvFormatEx07();
	afx_msg void OnConvFormatEx08();
	afx_msg void OnConvFormatEx09();
	afx_msg void OnConvFormatEx10();
	afx_msg void OnUpdateConvStr(CCmdUI* pCmdUI);
	afx_msg void OnConvStrUpper();
	afx_msg void OnConvStrLower();
	afx_msg void OnConvStrHankaku();
	afx_msg void OnConvStrHira();
	afx_msg void OnConvStrKata();
	afx_msg void OnConvStrHanAll();
	afx_msg void OnConvStrHanAlpha();
	afx_msg void OnConvStrHanKata();
	afx_msg void OnConvStrHanSuji();
	afx_msg void OnConvStrZenAll();
	afx_msg void OnConvStrZenAlpha();
	afx_msg void OnConvStrZenKata();
	afx_msg void OnConvStrZenSuji();
	afx_msg void OnConvStrToUpper();
	afx_msg void OnConvStrToLower();
	afx_msg void OnConvStrFirstUpper();
	afx_msg void OnConvStrHira2kata();
	afx_msg void OnConvStrKata2hira();
	afx_msg void OnUpdateFolderTreeSync(CCmdUI* pCmdUI);
	afx_msg void OnFolderTreeSync();
	afx_msg void OnUpdateCheckFileSync(CCmdUI* pCmdUI);
	afx_msg void OnCheckFileSync();
	afx_msg void OnConvStrHanKigou();
	afx_msg void OnConvStrZenKigou();
	afx_msg void OnUpdateSortListDirect(CCmdUI* pCmdUI);
	afx_msg void OnSortListDirect();
	afx_msg void OnUpdateWriteList(CCmdUI* pCmdUI);
	afx_msg void OnWriteList1();
	afx_msg void OnWriteList2();
	afx_msg void OnWriteList3();
	afx_msg void OnWriteList4();
	afx_msg void OnWriteList5();
	afx_msg void OnUpdateDeleteTag(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWriteTreePlaylist(CCmdUI* pCmdUI);
	afx_msg void OnWriteTreePlaylist();
	afx_msg void OnSetListFont();
	afx_msg void OnDeleteChar();
	afx_msg void OnUpdateDeleteChar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetNumberAdd(CCmdUI* pCmdUI);
	afx_msg void OnSetNumberAdd();
	afx_msg void OnUpdateMoveFolder(CCmdUI* pCmdUI);
	afx_msg void OnMoveFolder01();
	afx_msg void OnMoveFolder02();
	afx_msg void OnMoveFolder03();
	afx_msg void OnMoveFolder04();
	afx_msg void OnMoveFolder05();
	afx_msg void OnSelectTreeColum();
	afx_msg void OnUpdateSelectTreeColum(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatUser01();
	afx_msg void OnUpdateConvFormatUser01(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatUser02();
	afx_msg void OnUpdateConvFormatUser02(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatUser03();
	afx_msg void OnUpdateConvFormatUser03(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTeikei01(CCmdUI* pCmdUI);
	afx_msg void OnTeikei01();
	afx_msg void OnTeikei02();
	afx_msg void OnUpdateTeikei02(CCmdUI* pCmdUI);
	afx_msg void OnTeikei03();
	afx_msg void OnUpdateTeikei03(CCmdUI* pCmdUI);
	afx_msg void OnTeikei04();
	afx_msg void OnUpdateTeikei04(CCmdUI* pCmdUI);
	afx_msg void OnTeikei05();
	afx_msg void OnUpdateTeikei05(CCmdUI* pCmdUI);
	afx_msg void OnTeikei06();
	afx_msg void OnUpdateTeikei06(CCmdUI* pCmdUI);
	afx_msg void OnTeikei07();
	afx_msg void OnUpdateTeikei07(CCmdUI* pCmdUI);
	afx_msg void OnTeikei08();
	afx_msg void OnUpdateTeikei08(CCmdUI* pCmdUI);
	afx_msg void OnTeikei09();
	afx_msg void OnUpdateTeikei09(CCmdUI* pCmdUI);
	afx_msg void OnTeikei10();
	afx_msg void OnUpdateTeikei10(CCmdUI* pCmdUI);
	afx_msg void OnCheckFilenameMax();
	afx_msg void OnUpdateCheckFilenameMax(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatUser();
	afx_msg void OnUpdateConvFormatUser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTeikei(CCmdUI* pCmdUI);
	afx_msg void OnTeikeiConfig();
	afx_msg void OnTeikei_ShowSubMenu();
	afx_msg void OnUpdateDlgTeikei(CCmdUI* pCmdUI);
	afx_msg void OnTeikei201();
	afx_msg void OnUpdateTeikei201(CCmdUI* pCmdUI);
	afx_msg void OnTeikei202();
	afx_msg void OnUpdateTeikei202(CCmdUI* pCmdUI);
	afx_msg void OnTeikei203();
	afx_msg void OnUpdateTeikei203(CCmdUI* pCmdUI);
	afx_msg void OnTeikei204();
	afx_msg void OnUpdateTeikei204(CCmdUI* pCmdUI);
	afx_msg void OnTeikei205();
	afx_msg void OnUpdateTeikei205(CCmdUI* pCmdUI);
	afx_msg void OnTeikei206();
	afx_msg void OnUpdateTeikei206(CCmdUI* pCmdUI);
	afx_msg void OnTeikei207();
	afx_msg void OnUpdateTeikei207(CCmdUI* pCmdUI);
	afx_msg void OnTeikei208();
	afx_msg void OnUpdateTeikei208(CCmdUI* pCmdUI);
	afx_msg void OnTeikei209();
	afx_msg void OnUpdateTeikei209(CCmdUI* pCmdUI);
	afx_msg void OnTeikei210();
	afx_msg void OnUpdateTeikei210(CCmdUI* pCmdUI);
	afx_msg void OnTeikei301();
	afx_msg void OnUpdateTeikei301(CCmdUI* pCmdUI);
	afx_msg void OnTeikei302();
	afx_msg void OnUpdateTeikei302(CCmdUI* pCmdUI);
	afx_msg void OnTeikei303();
	afx_msg void OnUpdateTeikei303(CCmdUI* pCmdUI);
	afx_msg void OnTeikei304();
	afx_msg void OnUpdateTeikei304(CCmdUI* pCmdUI);
	afx_msg void OnTeikei305();
	afx_msg void OnUpdateTeikei305(CCmdUI* pCmdUI);
	afx_msg void OnTeikei306();
	afx_msg void OnUpdateTeikei306(CCmdUI* pCmdUI);
	afx_msg void OnTeikei307();
	afx_msg void OnUpdateTeikei307(CCmdUI* pCmdUI);
	afx_msg void OnTeikei308();
	afx_msg void OnUpdateTeikei308(CCmdUI* pCmdUI);
	afx_msg void OnTeikei309();
	afx_msg void OnUpdateTeikei309(CCmdUI* pCmdUI);
	afx_msg void OnTeikei310();
	afx_msg void OnUpdateTeikei310(CCmdUI* pCmdUI);
	afx_msg void OnUnifyChar();
	afx_msg void OnUpdateUnifyChar(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatUser04();
	afx_msg void OnUpdateConvFormatUser04(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatUser05();
	afx_msg void OnUpdateConvFormatUser05(CCmdUI* pCmdUI);
	afx_msg void OnEndEditRight();
	afx_msg void OnUpdateEndEditRight(CCmdUI* pCmdUI);
	afx_msg void OnDlgFavorites();
	afx_msg void OnUpdateDlgFavorites(CCmdUI* pCmdUI);
	afx_msg void OnFavorites();
	afx_msg void OnUpdateFavorites(CCmdUI* pCmdUI);
	afx_msg void OnCalcFolderTotal();
	afx_msg void OnUpdateCalcFolderTotal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileConvAutoId3(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteAdd();
	afx_msg void OnUpdateEditPasteAdd(CCmdUI* pCmdUI);
	afx_msg void OnEditCopyFormat01();
	afx_msg void OnEditCopyFormat02();
	afx_msg void OnEditCopyFormat03();
	afx_msg void OnEditCopyFormat04();
	afx_msg void OnEditCopyFormat05();
	afx_msg void OnUpdateEditCopyFormat(CCmdUI* pCmdUI);
	afx_msg void OnDeleteCharSpace();
	afx_msg void OnUpdateDeleteCharSpace(CCmdUI* pCmdUI);
	afx_msg void OnConvExSetup();
	afx_msg void OnUpdateConvExSetup(CCmdUI* pCmdUI);
	afx_msg void OnConvUserSetup();
	afx_msg void OnUpdateConvUserSetup(CCmdUI* pCmdUI);
	afx_msg void OnSelectTreeFile();
	afx_msg void OnUpdateSetListFont(CCmdUI* pCmdUI);
	afx_msg void OnMoveToParent();
	afx_msg void OnUpdateMoveToParent(CCmdUI* pCmdUI);
	afx_msg void OnMoveToNext();
	afx_msg void OnUpdateMoveToNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReverseCheck(CCmdUI* pCmdUI);
	afx_msg void OnMoveToPrevious();
	afx_msg void OnUpdateMoveToPrevious(CCmdUI* pCmdUI);
	afx_msg void OnConvFormatUserT2f01();
	afx_msg void OnConvFormatUserT2f02();
	afx_msg void OnConvFormatUserT2f03();
	afx_msg void OnConvFormatUserT2f04();
	afx_msg void OnConvFormatUserT2f05();
	afx_msg void OnConvFormatUserF2t01();
	afx_msg void OnConvFormatUserF2t02();
	afx_msg void OnConvFormatUserF2t03();
	afx_msg void OnConvFormatUserF2t04();
	afx_msg void OnConvFormatUserF2t05();
	afx_msg void OnConvTagToTag01();
	afx_msg void OnUpdateConvTagToTag(CCmdUI* pCmdUI);
	afx_msg void OnConvTagToTag02();
	afx_msg void OnConvTagToTag03();
	afx_msg void OnConvTagToTag04();
	afx_msg void OnConvTagToTag05();
	afx_msg void OnConvTag2tagSetup();
	afx_msg void OnUpdateConvTag2tagSetup(CCmdUI* pCmdUI);
	afx_msg void OnConvSiFieldToId3tag();
	afx_msg void OnUpdateConvSiFieldToId3tag(CCmdUI* pCmdUI);
	afx_msg void OnConvStrFixedUpperLower();
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
private:
	void DoMoveFolder(UINT index);
	void OnUpdateTeikei(CCmdUI* pCmdUI, int nIndex);
	void OnTeikei(int nIndex);
	BOOL SelectDirectory(LPTSTR sLocal, bool bCopy);
	void OnPluginCommand(UINT nID);
	void OnUpdatePluginCommand(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // SuperTagEditorView.cpp ファイルがデバッグ環境の時使用されます。
inline CSuperTagEditorDoc* CSuperTagEditorView::GetDocument()
   { return (CSuperTagEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMTLOGVIEWVIEW_H__D67B644C_BFAB_11D3_9459_00402641B29B__INCLUDED_)
