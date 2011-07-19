//---------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h>
#include "kbdde.h"

//---------------------------------------------------------------------------
HDDEDATA CALLBACK DefCallback(UINT uType, UINT uFmt,
    HCONV hConv, HSZ hszTpc1, HSZ hszTpc2, HDDEDATA hdata,
    DWORD dwData1, DWORD dwData2);
HDDEDATA CALLBACK DefCallback(UINT uType, UINT uFmt,
    HCONV hConv, HSZ hszTpc1, HSZ hszTpc2, HDDEDATA hdata,
    DWORD dwData1, DWORD dwData2)
{
    return NULL;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
KbDDE::KbDDE(PFNCALLBACK pfnCallBack, LPCSTR cszTopic, LPCSTR cszService)
{
    m_ddeInst = 0;
    m_hszService = NULL;
    m_hszTopic = NULL;
    lstrcpy(m_szTopicName, cszTopic);
    lstrcpy(m_szServiceName, cszService);
    if(!pfnCallBack){
        pfnCallBack = DefCallback;
    }
    if(DdeInitialize(&m_ddeInst, pfnCallBack, APPCLASS_STANDARD, 0)
        != DMLERR_NO_ERROR) {
        m_ddeInst = 0;
        return;
    }
    m_hszService = DdeCreateStringHandle(m_ddeInst, m_szServiceName, CP_WINANSI);
    m_hszTopic = DdeCreateStringHandle(m_ddeInst, m_szTopicName, CP_WINANSI);
    if(DdeGetLastError(m_ddeInst) != DMLERR_NO_ERROR) {
        DdeUninitialize(m_ddeInst);
        m_ddeInst= 0;
        m_hszService = NULL;
        m_hszTopic = NULL;
        return;
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
KbDDE::~KbDDE(void)
{
    if(!m_ddeInst)
        return;
    DdeFreeStringHandle(m_ddeInst, m_hszService);
    DdeFreeStringHandle(m_ddeInst, m_hszTopic);
    DdeUninitialize(m_ddeInst);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DWORD __fastcall KbDDEServer::QueryString(HSZ hsz, char *szBuffer, int Size)
{
    return DdeQueryString(m_ddeInst, hsz, szBuffer, Size, CP_WINANSI);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
HDDEDATA __fastcall KbDDEServer::CreateDataHandle(LPBYTE pSrc, DWORD cb, HSZ hsz, UINT wFmt)
{
    return DdeCreateDataHandle(m_ddeInst, pSrc, cb, 0, hsz, wFmt, 0);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*

    KbDDEServer

*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
KbDDEServer::KbDDEServer(PFNCALLBACK pfnCallBack, LPCSTR cszTopic, LPCSTR cszService)
    :KbDDE(pfnCallBack, cszTopic, cszService)
{
    DdeNameService(m_ddeInst, m_hszService, 0, DNS_REGISTER);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
KbDDEServer::~KbDDEServer(void)
{
    if(m_ddeInst)
        DdeNameService(m_ddeInst, m_hszService, 0, DNS_UNREGISTER);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*

    KbDDEClient

*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
KbDDEClient::KbDDEClient(PFNCALLBACK pfnCallBack, LPCSTR cszTopic, LPCSTR cszService)
    :KbDDE(pfnCallBack, cszTopic, cszService)
{
    m_hConv = DdeConnect(m_ddeInst, m_hszService, m_hszTopic, NULL);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
KbDDEClient::~KbDDEClient(void)
{
    if(m_hConv){
        DdeDisconnect(m_hConv);
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
HDDEDATA KbDDEClient::ClientTransaction(
            LPBYTE pData,       // サーバーに渡すデータの先頭バイトのポインタ
            DWORD cbData,       // データの長さ
        //    HCONV hConv,        // 通信ハンドル
            HSZ hszItem,        // データ項目のハンドル
            UINT wFmt,          // クリップボードフォーマット
            UINT wType,         // トランザクションタイプ
            DWORD dwTimeout,    // 待ち時間
            LPDWORD pdwResult   // トランザクションの結果へのポインタ
        )
{
    if(!m_hConv)return NULL;
    return DdeClientTransaction(
            pData,       // サーバーに渡すデータの先頭バイトのポインタ
            cbData,      // データの長さ
            m_hConv,     // 通信ハンドル
            hszItem,     // データ項目のハンドル
            wFmt,        // クリップボードフォーマット
            wType,       // トランザクションタイプ
            dwTimeout,   // 待ち時間
            pdwResult    // トランザクションの結果へのポインタ
           );
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool __fastcall KbDDEClient::Execute(LPCSTR cszFileName, 
                                     LPCSTR cszCommand)
{
    HDDEDATA hRet;
    HSZ hszTopic;
    static LPCSTR cszEmpty = "";
    if(!m_hConv){
        return false;
    }
    if(!cszCommand)
        cszCommand = cszEmpty;
    if(!cszFileName)
        cszFileName = cszEmpty;

    int lenFileName = lstrlen(cszFileName);
    int lenCommand = lstrlen(cszCommand);
    int szTopicLen = lenFileName + lenCommand + 8;
    char *szTopic = new char[szTopicLen + 1];
    if(lenFileName){
        szTopic[0] = '\"';
        lstrcpy(&szTopic[1], cszFileName);
        lstrcat(szTopic, "\" ");
    }
    else{
        szTopic[0] = 0;
    }
    lstrcat(szTopic, cszCommand);
    hszTopic = DdeCreateStringHandle(m_ddeInst, szTopic, CP_WINANSI);
    hRet = DdeClientTransaction(
            (BYTE*)szTopic,
            szTopicLen,
            m_hConv,
            NULL,
            NULL,/*CF_TEXT,*/
            XTYP_EXECUTE,
            2000,//待機時間
            NULL);

    delete[] szTopic;
    if(!hRet && DdeGetLastError(m_ddeInst) != DMLERR_NO_ERROR){
        //MessageBeep(MB_OK);
        return false;
    }
    else if(hRet){
        DdeFreeStringHandle(m_ddeInst, hszTopic);
        DdeFreeDataHandle(hRet);
    }
    return true;
}

bool __fastcall KbDDEClient::Execute2(LPCSTR cszFileName, 
                                     LPCSTR cszCommand)
{
    HDDEDATA hRet;
    HSZ hszTopic;
    static LPCSTR cszEmpty = "";
    if(!m_hConv){
        return false;
    }
    if(!cszCommand)
        cszCommand = cszEmpty;
    if(!cszFileName)
        cszFileName = cszEmpty;

    int lenFileName = lstrlen(cszFileName);
    int lenCommand = lstrlen(cszCommand);
    int szTopicLen = lenFileName + lenCommand + 8;
    char *szFileName = new char[lenFileName + 3 + 1];
    if(lenFileName){
        lstrcpy(szFileName, " \"");
        lstrcat(&szFileName[2], cszFileName);
        lstrcat(szFileName, "\"");
    }
    else{
        szFileName[0] = 0;
    }
    char *szTopic = new char[szTopicLen + 1];
    lstrcpy(szTopic, cszCommand);
    lstrcat(szTopic, szFileName);
    hszTopic = DdeCreateStringHandle(m_ddeInst, szTopic, CP_WINANSI);
    hRet = DdeClientTransaction(
            (BYTE*)szTopic,
            szTopicLen,
            m_hConv,
            NULL,
            NULL,/*CF_TEXT,*/
            XTYP_EXECUTE,
            2000,//待機時間
            NULL);

    delete[] szTopic;
	delete[] szFileName;
    if(!hRet && DdeGetLastError(m_ddeInst) != DMLERR_NO_ERROR){
        //MessageBeep(MB_OK);
        return false;
    }
    else if(hRet){
        DdeFreeStringHandle(m_ddeInst, hszTopic);
        DdeFreeDataHandle(hRet);
    }
    return true;
}

bool __fastcall KbDDEClient::Execute(LPCSTR cszCommand, DWORD dwWait) /* Misirlou 138 */
{
    if(!m_hConv){
        return false;
    }
    if(!cszCommand)
        return false;
    int cbData = strlen(cszCommand) + 1;
    BYTE *pData = new BYTE[cbData];
    strcpy((char*)pData, cszCommand);
    HDDEDATA hRet = DdeClientTransaction(
                        pData,
                        cbData,
                        m_hConv,
                        NULL,
                        CF_TEXT,
                        XTYP_EXECUTE,
                        dwWait,//待機時間
                        NULL);
    delete[] pData;
    if(!hRet && DdeGetLastError(m_ddeInst) != DMLERR_NO_ERROR){
        return false;
    }
    else{
        if(hRet){
            DdeFreeDataHandle(hRet);
        }
        return true;
    }
}
/*
const char* __fastcall kbGetCommandLineParams(void)
{//起動時のコマンドライン引数を返す
 //（実行ファイル名の部分は除く）
    const char *szCommandLine = ::GetCommandLine();
    const char *p = szCommandLine;
    bool bDblQuote = false;
    while(*p == ' ')p++;
    if(*p == '"'){//二重引用符で括られている
        p++;
        bDblQuote = true;
    }
    while(*p){
        if(bDblQuote && *p == '"'){//二重引用符の終わり
            p++;
            break;
        }
        else if(!bDblQuote && *p == ' '){
            p++;
            break;
        }
        if(IsDBCSLeadByte((BYTE)*p)){
            p+=2;
        }
        else{
            p++;
        }
    }
    while(*p == ' ')p++;
    return p;
}
*/