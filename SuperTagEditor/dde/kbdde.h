//---------------------------------------------------------------------------
#ifndef kbddeH
#define kbddeH
//---------------------------------------------------------------------------
#include <ddeml.h>

class KbDDE
{
protected:
    DWORD m_ddeInst;
    HSZ m_hszService;
    HSZ m_hszTopic;
    TCHAR m_szTopicName[256];
    TCHAR m_szServiceName[256];
public:
    KbDDE(PFNCALLBACK pfnCallBack, LPCTSTR cszTopic, LPCTSTR cszService);
    ~KbDDE(void);
};
///////////////////////////////////////////////////////////////////////////////
class KbDDEServer : public KbDDE
{
private:

public:
    DWORD __fastcall QueryString(HSZ hsz, LPTSTR szBuffer, int Size);
    HDDEDATA __fastcall CreateDataHandle(LPBYTE pSrc, DWORD cb, HSZ hsz, UINT wFmt);
    KbDDEServer(PFNCALLBACK pfnCallBack, LPCTSTR cszTopic, LPCTSTR cszService);
    ~KbDDEServer(void);
};
///////////////////////////////////////////////////////////////////////////////
class KbDDEClient : public KbDDE
{
private:
    HCONV m_hConv;

public:
    HDDEDATA ClientTransaction(
        LPBYTE pData,       // サーバーに渡すデータの先頭バイトのポインタ
        DWORD cbData,       // データの長さ
    //    HCONV hConv,        // 通信ハンドル
        HSZ hszItem,        // データ項目のハンドル
        UINT wFmt,          // クリップボードフォーマット
        UINT wType,         // トランザクションタイプ
        DWORD dwTimeout,    // 待ち時間
        LPDWORD pdwResult   // トランザクションの結果へのポインタ
    );
    bool __fastcall Execute(LPCTSTR cszFileName, LPCTSTR cszCommand);
    bool __fastcall Execute2(LPCTSTR cszFileName, LPCTSTR cszCommand); /* WildCherry 070 */
	bool __fastcall Execute(LPCSTR cszCommand, DWORD dwWait); /* RockDance2 138 */
    KbDDEClient(PFNCALLBACK pfnCallBack, LPCTSTR cszTopic, LPCTSTR cszService);
    ~KbDDEClient(void);
};
///////////////////////////////////////////////////////////////////////////////
//起動時のコマンドライン引数を返す
//（実行ファイル名の部分は除く）
const char* __fastcall kbGetCommandLineParams(void);
#endif
