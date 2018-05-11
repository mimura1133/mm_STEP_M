// SuperTagEditorDoc.h : CSuperTagEditorDoc �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMTLOGVIEWDOC_H__D67B644A_BFAB_11D3_9459_00402641B29B__INCLUDED_)
#define AFX_DMTLOGVIEWDOC_H__D67B644A_BFAB_11D3_9459_00402641B29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define INIT_ARRAY_SIZE			256
#define ADD_ARRAY_SIZE			256

#include "FileMP3.h"
#include "SuperTagEditorView.h"

#include "Registry.h"

#include <string>

struct FILE_STATE	{					// �t�@�C�����
	CString	strFullPathName;			// �t���p�X��
	CSuperGridCtrl::CTreeItem *pParent;	// �e�̃A�C�e��
};

class CDlgCommonProgress;

class CSuperTagEditorDoc : public CDocument
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CSuperTagEditorDoc();
	DECLARE_DYNCREATE(CSuperTagEditorDoc)


public:		// ======================================
			// =====      public �����o�֐�     =====
			// ======================================
	FILE_MP3	*GetListMP3(int nIndex) {return(&m_arrayFiles[nIndex]);}
	int			GetIndexFromListMP3(FILE_MP3* pFileMP3) { /* STEP 037 */
				for (int no = 0; no < m_nArrayFileCount; no++) {
					if (GetListMP3(no) == pFileMP3) {
						return no;
					}
				}
				return -1;
	}
	int			GetArrayFileCount(void) {return(m_nArrayFileCount);}
	FILE_STATE	*GetRequestFile(int nIndex) {return(&m_arrayRequestFiles[nIndex]);}
	int			GetRequestFileCount(void) {return(m_nArrayRequestFileCount);}

	CDlgCommonProgress	*GetDlgLoadProgressBarPtr(void) {return(m_dlgLoadProgress);}

	inline	bool	CheckFileModified(void) {
				for (int no = 0; no < m_nArrayFileCount; no++) {
					if (GetListMP3(no)->bModifyFlag) return(true);
				}
				return(false);
			}
	inline	void UpdateAllFiles(bool bRedraw) {
				CMySuperGrid	&listCtrl = GetListCtrl();
				listCtrl.UpdateAllFiles();
				if (bRedraw) UpdateAllViews(NULL);
			}
	inline	void ExecClassification(void) {
				// ���ޕ\���X�V
				OnExecClassification();
			}

	inline	void SaveAllTag(void) {OnSaveAllTag();}

	bool	LoadPlayList(LPCTSTR);
	bool	FoundFile(LPCTSTR, CSuperGridCtrl::CTreeItem * = NULL, bool = true);
	bool	SearchFileReent(LPCTSTR, CSuperGridCtrl::CTreeItem * = NULL);
	int		AddRequestFile(LPCTSTR, CSuperGridCtrl::CTreeItem * = NULL);
	void	ExecRequestFiles(bool = true, bool = true);
	void	StartLoadFile(LPCTSTR);
	void	EndLoadFile(void);
	void	ClearRequestFiles(void);
	void	OpenFolder(LPCTSTR);
	bool	CheckFileAttribute(FILE_MP3 *);
	//�ǉ� by Kobarin
	//�^�O���X�V�����ǂ���
	bool	IsTagUpdating(void){return m_TagUpdatingCount != 0;}

private:	// ======================================
			// =====     private �����o�֐�     =====
			// ======================================
			std::basic_string<TCHAR> m_sTargetFile;				// ���������F���݂̃t�@�C��
			CArray <FILE_MP3, const FILE_MP3 &>	m_arrayFiles;
			CArray <FILE_STATE, const FILE_STATE &>	m_arrayRequestFiles;
			int		m_nArrayFileCount;
			int		m_nArrayRequestFileCount;
			bool	m_bSaveAllTagResult;
			CDlgCommonProgress	*m_dlgLoadProgress;

	inline	void	InitGridList(void) {
				// �O���b�h��������
				CMySuperGrid	&listCtrl = GetListCtrl();
				listCtrl.DeleteAllEx();
			}
	inline	void	InitFileList(void) {
				for (int no = 0; no < m_nArrayFileCount; no++) {
					FILE_MP3	*fileMP3 = GetListMP3(no);
					extern void InitFileSpecificInfo(FILE_MP3* pFileMP3);
					if (fileMP3->fileSpecificInfo != NULL) {
						InitFileSpecificInfo(fileMP3);
					}
				}
				m_arrayFiles.RemoveAll();
				m_arrayFiles.SetSize(INIT_ARRAY_SIZE);
				m_nArrayFileCount = 0;
			}
	inline	CSuperTagEditorView *GetView(void) {
				POSITION	pos = GetFirstViewPosition();
				return(((CSuperTagEditorView *)GetNextView(pos)));
			}
	inline	CMySuperGrid &GetListCtrl(void) {
				return(GetView()->GetListCtrl());
			}
	inline	void	InitPlugin(void) {
				extern void STEPluginLoad(HWND);
				STEPluginLoad(GetView()->GetSafeHwnd());
				CString strINI = AfxGetApp()->m_pszProfileName;
				InitProfile();
			}
	CTime	GetFileTime(LPCTSTR);
	BOOL	SelectDirectory(LPTSTR);

	//�ǉ� by Kobarin
	//StartLoadFile/EndLoadFile �͓����񐔌Ăяo���Ȃ���΂Ȃ�Ȃ�
	//�悤�Ɏd�l�ύX
	int		m_StartLoadFileCount;
	int		m_TagUpdatingCount;//�^�O�����X�V��
	bool	m_bInitialized; /* StartInaction2 055 */

protected:	// ======================================
			// =====    protected �����o�֐�    =====
			// ======================================

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSuperTagEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SaveAllTag(BOOL bSaveOnly);
	virtual ~CSuperTagEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSuperTagEditorDoc)
	afx_msg void OnUpdateOpenFolder(CCmdUI* pCmdUI);
	afx_msg void OnOpenFolder();
	afx_msg void OnUpdateSaveAllTag(CCmdUI* pCmdUI);
	afx_msg void OnSaveAllTag();
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFieldSi(CCmdUI* pCmdUI);
	afx_msg void OnEditFieldSi();
	afx_msg void OnUpdateEditTd3Tag(CCmdUI* pCmdUI);
	afx_msg void OnEditTd3Tag();
	afx_msg void OnUpdateEditChangeField(CCmdUI* pCmdUI);
	afx_msg void OnEditChangeField();
	afx_msg void OnUpdateExecClassification(CCmdUI* pCmdUI);
	afx_msg void OnExecClassification();
	afx_msg void OnDlgSetupPlugin();
	afx_msg void OnUpdateDlgSetupPlugin(CCmdUI* pCmdUI);
	afx_msg void OnDlgEnvironment();
	afx_msg void OnUpdateDlgEnvironment(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DMTLOGVIEWDOC_H__D67B644A_BFAB_11D3_9459_00402641B29B__INCLUDED_)
