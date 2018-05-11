#if !defined(AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)
#define AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySuperGrid.h : header file
//
#include "FileMP3.h"
#include "SuperGridCtrl.h"
#include "OXToolTipCtrl.h"


enum	{
	CONV_STR_UPPER,			// �啶���ɕϊ�
	CONV_STR_LOWER,			// �������ɕϊ�
	CONV_STR_HANKAKU,		// ���p�ɕϊ�
	CONV_STR_HIRA,			// �S�p���Ȃɕϊ�
	CONV_STR_KATA,			// �S�p�J�i�ɕϊ�

	CONV_STR_HAN_ALL,		// �S�p => ���p(�S��)�ϊ�
	CONV_STR_HAN_KIGOU,		// �S�p => ���p(�L��)�ϊ�
	CONV_STR_HAN_SUJI,		// �S�p => ���p(����)�ϊ�
	CONV_STR_HAN_KATA,		// �S�p => ���p(�J�^�J�i)�ϊ�
	CONV_STR_HAN_ALPHA,		// �S�p => ���p(�A���t�@�x�b�g)�ϊ�
	CONV_STR_ZEN_ALL,		// ���p => �S�p(�S��)�ϊ�
	CONV_STR_ZEN_KIGOU,		// ���p => �S�p(�L��)�ϊ�
	CONV_STR_ZEN_SUJI,		// ���p => �S�p(����)�ϊ�
	CONV_STR_ZEN_KATA,		// ���p => �S�p(�J�^�J�i)�ϊ�
	CONV_STR_ZEN_ALPHA,		// ���p => �S�p(�A���t�@�x�b�g)�ϊ�
	CONV_STR_TO_UPPER,		// ������ => �啶���ϊ�
	CONV_STR_TO_LOWER,		// �啶�� => �������ϊ�
	CONV_STR_FIRST_UPPER,	// �P��̂P�����ڂ̂ݑ啶��
	CONV_STR_HIRA2KATA,		// �Ђ炪�� => �J�^�J�i�ϊ�
	CONV_STR_KATA2HIRA,		// �J�^�J�i => �Ђ炪�ȕϊ�
	CONV_STR_KAN2HIRA,		// ���� => �Ђ炪�ȕϊ�
	CONV_STR_ROMAJI,		// ���[�}���ϊ�
	CONV_STR_FIXED_UP_LOW,	// �啶���������Œ�ϊ�
};

struct	COLUMN_STATUS	{
	char	*sRegKey;		// ���W�X�g���[�L�[��
	BOOL	bShowFlag;		// �\���t���O
	int		nNumber;		// ���ڂ̏���
	int		nWidth;			// �J������
	int		nWidthMax;		// �ő�J������
	int		nFormat;		// 
};
extern	struct	COLUMN_STATUS	g_columnStatus[COLUMN_MAX];

struct	LIST_WRITE_STATUS	{
	CString	strDirectory;			// ���X�g�t�@�C���o�͐�̃f�B���N�g��
	int		nFileNumber;			// ���݂̃t�@�C���ԍ�
	int		nFileCount;				// �t�@�C������
	float	fTotalSize;				// �����T�C�Y
	int		nTotalTime;				// �������t����
};

class CSuperTagEditorDoc;
extern	int		g_nColumnNumberList[];

typedef struct _CONTROL_TYPE_TABLE {
	CONTROLTYPE	file_name;
	CONTROLTYPE	track_name;
	CONTROLTYPE	artist_name;
	CONTROLTYPE	album_name;
	CONTROLTYPE	track_number;
	CONTROLTYPE	release_year;
	CONTROLTYPE	genre;
	CONTROLTYPE	comment;
	CONTROLTYPE	copyright;
	CONTROLTYPE	engineer;
	CONTROLTYPE	source;
	CONTROLTYPE	software;
	CONTROLTYPE	keyword;
	CONTROLTYPE	technician;
	CONTROLTYPE	lyric;
	CONTROLTYPE	commision;
	CONTROLTYPE	composer;
	CONTROLTYPE	orig_artist;
	CONTROLTYPE	url;
	CONTROLTYPE	encodest;
	CONTROLTYPE	other;
} CONTROL_TYPE_TABLE,*PCONTROL_TYPE_TABLE;

typedef struct _COLUMN_MAX_TABLE {
	int	file_name;
	int	track_name;
	int	artist_name;
	int	album_name;
	int	track_number;
	int	release_year;
	int	genre;
	int	comment;
	int	copyright;
	int	engineer;
	int	source;
	int	software;
	int	keyword;
	int	technician;
	int	lyric;
	int	commision;
	int	composer;
	int	orig_artist;
	int	url;
	int	encodest;
	int	other;
} COLUMN_MAX_TABLE,*PCOLUMN_MAX_TABLE;

////////////////////////////////////////////////////////////////////////////
// CMySuperGrid window

// an example of how to use the CSuperGridCtrl class.

class CMySuperGrid : public CSuperGridCtrl
{
// Construction
public:
	CMySuperGrid();

	CStringList	m_strListGenre;
	CSuperTagEditorDoc	*m_pDoc;
	BOOL	m_bSortAscending;
	int		m_nSortedCol;
	int		m_nColumnWidthTable[COLUMN_MAX];
	int		m_nFolderSyncCounter;
	CSuperGridCtrl::CTreeItem	*m_pItemRoot;

	inline	void	UpdateStatusLocation(void) {
				unsigned short	wItem		= (unsigned short)GetSelectedItem() + 1;
				unsigned short	wSubItem	= (unsigned short)GetCurSubItem() + 1;
				AfxGetMainWnd()->SendMessage(WM_USER_SET_STATUS_POS, 0, (wItem<<16)|(wSubItem));
			}
	inline	LPARAM	GetLParamFromIndex(int nIndex) {
				CTreeItem	*pItem = GetTreeItem(nIndex);
				CItemInfo	*pInfo = pItem ? GetData(pItem) : NULL;
				return(pInfo ? pInfo->GetLParam() : -1);
			}
	inline	void	DeleteItemFromIndex(int nIndex) {
				DeleteItemExNoRedraw(GetTreeItem(nIndex), nIndex);
			}
	inline	void	UpdateFileStatusFromIndex(int nIndex, FILE_MP3 *fileMP3) {
				UpdateFileStatus(GetTreeItem(nIndex), fileMP3);
			}
	inline	void	SetColumnControlType(CItemInfo *pInfo, CONTROLTYPE ctrlType, int nColumnType) {
				int		nColNum = g_nColumnNumberList[nColumnType];
				if (nColNum >= 0) pInfo->SetControlType(ctrlType, nColNum-1);
			}
	inline	CONTROLTYPE GetColumnControlType(CItemInfo *pInfo, int nColumnType) {
				int		nColNum = g_nColumnNumberList[nColumnType];
				CONTROLTYPE	type = invalid;
				if (nColNum >= 0) pInfo->GetControlType(nColNum-1, type);
				return(invalid);
			}
	inline	void	UpdateSyncFolderItemName(void) {
				if (m_pItemRoot) {
					int		iItem = NodeToIndex(m_pItemRoot);
					CItemInfo	*pInfo = GetData(m_pItemRoot);
					if (iItem >= 0 && pInfo != NULL) {
						CString	strName;
						if (g_bEnableFolderSync) {
							strName.Format(TEXT("Root[%s]"), g_strRootFolder);
						} else {
							strName = _T("Root");
						}
						pInfo->SetItemText(strName);
						SetItemText(iItem, -1, strName);
					}
				}
			}
	inline	BOOL	IsSupportSIF(const FILE_MP3 *fileMP3) {
				extern BOOL IsPluginSupportSIF(const FILE_MP3 *fileMP3);
				return IsPluginSupportSIF(fileMP3);
			}
	inline	BOOL	IsSupportTrackNumberSIF(const FILE_MP3 *fileMP3) {
				extern BOOL IsPluginSupportTrackNumberSIF(const FILE_MP3 *fileMP3);
				return IsPluginSupportTrackNumberSIF(fileMP3);
			}
	inline	BOOL	IsSupportGenreSIF(const FILE_MP3 *fileMP3) {
				extern BOOL IsPluginSupportGenreSIF(const FILE_MP3 *fileMP3);
				return IsPluginSupportGenreSIF(fileMP3);
			}
	inline	BOOL	IsEditFieldSIF(const FILE_MP3 *fileMP3) {
				return(g_bOptEditFieldSIF && IsSupportSIF(fileMP3));
			}
	inline	BOOL	IsEditTrackNumberSIF(const FILE_MP3 *fileMP3) {
				return(g_bOptEditFieldSIF && IsSupportTrackNumberSIF(fileMP3));
			}
	inline	BOOL	IsEditGenreSIF(const FILE_MP3 *fileMP3) {
				return(g_bOptEditFieldSIF && IsSupportGenreSIF(fileMP3));
			}

			void	AutoSizeColumns(int col = -1);
			int		GetColumnCount(void) const;
			void	DeleteAllEx(void);
			void	SetItemModifyFlag(CTreeItem *, bool);
			bool	GetItemModifyFlag(const CTreeItem *);
			int		GetItemFormat(const CTreeItem *);
			void	MakeStrListGenre(void);
			CSuperGridCtrl::CTreeItem *AddDirectory(LPCTSTR, CTreeItem * = NULL, int = 2);
			bool	AddFile(const FILE_MP3 *, CTreeItem * = NULL, LPARAM = 0, bool = true);
			CSuperGridCtrl::CTreeItem *SearchChildrenItem(const CString &, CTreeItem *);
			bool	AddFile2(const FILE_MP3 *, CTreeItem * = NULL, LPARAM = 0, bool = true);
			bool	UpdateFileStatus(CTreeItem *, const FILE_MP3 *);
			bool	UpdateAllFiles(void);
			bool	ClipboardCopy(void);
			bool	ClipboardCut(void);
			bool	ClipboardPaste(void);
			void	PasteString(LPTSTR);
			bool	CellCopyDown(void);
			bool	CellClear(int, int);
			bool	CellClearSelected(void);
			bool	SetFillNumber(int, int, bool = false/* Conspiracy 194 */);
			void	ChangeSubItemText(int, int, LPCTSTR);
			void	CellStateControl(int);

			int		CompFunc(CString &, CString &, int, bool);
			int		CompFunc(SORT_STATE *, int, int, int);
			BOOL	SortTextItems(SORT_STATE *, int, int = 0, int = -1);
			void	MultiColumnSort(SORT_STATE *, int);

			// �ϊ�
			bool	ConvTagInfo(int, int, LPCTSTR = NULL);
			bool	ConvTagInfoSelected(int, LPCTSTR = NULL);
			bool	ConvTagInfo(CTreeItem *, int, LPCTSTR = NULL, LPCTSTR = NULL /* STEP 034 */);
			bool	ConvUserFormatEx(USER_CONV_FORMAT_EX *);
			bool	ConvString(int);
			CString	ReplaceFileName(const FILE_MP3 *, CString);

			bool	WritePlayList(LPCTSTR);
			bool	WritePlayList(LPCTSTR, POSITION);
			bool	WritePlayList(CFile &, CTreeItem *, CString &);

			bool	WriteTreePlayList(LPCTSTR);
			bool	WriteTreePlayList(LPCTSTR, CTreeItem *, CString &);

			bool	WriteFormatFile(LPCTSTR, const CString &, const CString &, const CString &, bool, bool, bool, bool/* BeachMonster5 120 */);
			bool	WriteFormatFileBody(CFile &, CTreeItem *, const CString &, bool, LIST_WRITE_STATUS *, bool/* BeachMonster5 120 */);
			bool	WriteFormatFileFoot(CFile &, const CString &, bool, LIST_WRITE_STATUS *, bool/* BeachMonster5 120 */);
			CString	WriteFormatFileHeader(CFile &file, CTreeItem *pItem, const CString &strHead, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml); /* Rumble 190 */

			CString	GetFileColumnText(const FILE_MP3 *, int);

			LPCTSTR GetSelectFileName(void);

			int		MakeSelectFileArray(CArray <int, const int &> &);
			int		MakeSelectFileArray(int nIndex, CArray <int, const int &> &);
			int		AddSelectFileArray(int, CArray <int, const int &> &, int);

			void	SetCheckFileSelect(void);
			void	SetSelectFileCheck(BOOL);
			void	SetCheckAllFiles(BOOL);
			void	ReverseCheckFiles(void);

			void	CheckFileAtColumnState(CHECK_WORD_STATE *);
			void	CheckFileAtColumnState(CHECK_WORD_STATE *, CTreeItem *);
			bool	IsMatchItem(CHECK_WORD_STATE *, CTreeItem *);
			int		FindNextMatchItem(int, CHECK_WORD_STATE *, bool);
			void	ReplaceMatchItem(CHECK_WORD_STATE *, bool);
			void	ReplaceMatchItem(CHECK_WORD_STATE *, CTreeItem *);

			void	SelectAndVisibleColumn(int, int);
			bool	GetSelectedRange(int &, int &, int &, int &);

			void	ExecFolderTreeSync(LPCTSTR, bool);
			void	ExecFolderTreeSync(LPCTSTR, CTreeItem *, bool, bool = false);
			bool	DirectoryRemove(CString &);
			bool	DirectoryMake(CString &);
			bool	FileDelete(const char *);
			bool	FileCopy(const char *, const char *);
			void	ChangeFileAttr(LPCTSTR, DWORD);
			bool	CheckFileName(LPCTSTR);
			bool	CheckExist(LPCTSTR);
			void	UpdateCellInfo();

// Attributes
public:
// Operations
public:
	//HOWTO:
	
	void InitializeGrid(void);
	void SetNewImage(int nItem);
	CImageList *CreateDragImageEx(int nItem);
	BOOL m_bDrag;
	// Overrides
	bool OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);	
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);
	BOOL OnVkReturn(void);
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	BOOL OnChangeSelect(void);
	// �`�F�b�N��Ԃ��ύX���ꂽ
	void OnChangeCheck(CTreeItem *, BOOL);
	BOOL OnItemDblClick(int);
	int OnGetColumLimitText(int, int);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySuperGrid)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool ConvSiFieldToId3tag(CTreeItem *pItem);
	bool ConvSiFieldToId3tag(void);
	int GetIndexFromFileMP3(FILE_MP3* pFileMP3);
	bool ConvUserTag2Tag(USER_CONV_FORMAT_TAG2TAG *pForm);
	void MoveToPrevious();
	void MoveToNext();
	void MoveToParent();
	void SelectTreeFile();
	bool ProcessSelectedFiles(LPCTSTR szProgressBarTitle, STEPProcessSelectedFilesCB* callback);
	bool ProcessSelectedFilesForUpdate(LPCTSTR szProgressBarTitle, STEPProcessSelectedFilesCB* callback);
	bool DeleteCharSpace(int nPos=0);
	void ClipboardCopyFormat(USER_COPY_FORMAT_FORMAT *pForm);
	void ChangeSubItemText(int iItem, int iSubItem, LPCTSTR sUpdateText, int nPos, bool bAddSpace, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */, bool bUpdateInternal = FALSE/* STEP 037 */);
	void PasteString(LPTSTR sBuffer, int nPastePos, bool bText/* RockDance 124 */, bool bAddSpace/* Baja 171 */, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */);
	bool ClipboardPaste(int nPastePos, bool bAddSpace, const CString& strAddFront, const CString& strAddBack/* FunnyCorn 187 */);
	void OnUpdateTotal();
	void OnRefreshTreeItem(CTreeItem* pOldItem, CTreeItem* pNewItem);
	int  GetFolderFileCount(CTreeItem* pTop, CTreeItem* pTotalParent, int fileCount);
	int  GetFolderFileCount(int nIndex);
	void CalcSum(CTreeItem* pTop, CTreeItem* pTotalParent);
	void CalcSum(int nIndex);
	CString quoteForComment(CString& str);
	int GetSelectedItem();
	void ChangeSubItemText(int iItem, int iSubItem, LPCTSTR sText, int nPos);
	void PasteString(LPTSTR sBuffer, int nPos, bool bText = true);
	void CheckFileNameMax();
	void SelectTreeColumn();
	void SetHeaderFont(LOGFONT logFont);
	void UpdateParentAtDeleteItem(UINT nIndex, FILE_MP3 *fileMP3);
	bool MoveFolderFormat(USER_MOVE_FODLER_FORMAT *pForm, CString strFolder, bool isCopy);
	bool AddFillNumber(int nStart, int nStep, int nWidth, CString strBefore/* Conspiracy 194 */, CString strAfter/* Conspiracy 194 */, int nPos, bool bSpaceInitNumber/* Conspiracy 194 */);
	bool DeleteChar(int nCount, int nPos);
	virtual ~CMySuperGrid();

protected:
	CImageList m_image;//Must provide an imagelist
	CImageList m_imageCheck;//Must provide an imagelist
	//positions and creates/initalize a combobox control
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	//helper function called from ShowList...calcs the lists max horz extent
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);
	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CMySuperGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

	DECLARE_MESSAGE_MAP()
private:
	CString MakeFormatFileFoot(const CString &strFoot, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml); /* Rumble 190 */
	CString MakeFormatFileBody(FILE_MP3	*fileMP3, const CString &strBody, bool bIsHtml, LIST_WRITE_STATUS *pStatus, bool bWriteHtml); /* Rumble 190 */
	CString unquoteForComment(CString str);
	bool isOneCellClipboard(CString str);
	CString ConvHTML(CString& str, bool bConv);
	BOOL OnDrawCustomCellBG(CDC *pDC, int nItem, int nColumn, RECT &rcItem);
	UINT Time2Sec(CString strTime);
	COXToolTipCtrl m_toolTip;
	CString m_strToolTip;
	int m_nPrevTipSubItem;
	int m_nPrevTipItem;
	BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	BOOL OnNotifyMessage(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)
