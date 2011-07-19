// DlgCommonProg.h : ヘッダ ファイル
// CG: このファイルは「プログレス ダイアログ」コンポーネントにより追加されています。

/////////////////////////////////////////////////////////////////////////////
// CDlgCommonProgress ダイアログ

#ifndef __DLGCOMMONPROG_H__
#define __DLGCOMMONPROG_H__

class CDlgCommonProgress : public CDialog
{
// 構築/消滅
public:
    CDlgCommonProgress(UINT nCaptionID = 0);   // 標準コンストラクタ
    ~CDlgCommonProgress();

    BOOL Create(CWnd *pParent=NULL);

    // プログレス ダイアログの処理
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
	void EnableCancelButton(BOOL);
    BOOL IsCanceled(void){return m_bCanceled;}//追加 by Kobarin    
    void SetCanceled(BOOL bCanceled){m_bCanceled = bCanceled;}//追加 by Kobarin
        
// ダイアログ データ
    //{{AFX_DATA(CDlgCommonProgress)
    enum { IDD = CG_IDD_PROGRESS };
    CProgressCtrl	m_Progress;
    //}}AFX_DATA

// オーバーライド
    // ClassWizard は仮想関数のオーバライドを生成します。
    //{{AFX_VIRTUAL(CDlgCommonProgress)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

// インプリメンテーション
protected:
	UINT m_nCaptionID;
    int m_nLower;
    int m_nUpper;
    int m_nStep;
    BOOL m_bParentDisabled;
    BOOL m_bCanceled;// 追加 by Kobarin

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    // 生成されたメッセージ マップ関数
    //{{AFX_MSG(CDlgCommonProgress)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __DLGCOMMONPROG_H__
