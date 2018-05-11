//---------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h>
#include "kbdde.h"
#include <vector>

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
KbDDE::KbDDE(PFNCALLBACK pfnCallBack, LPCTSTR cszTopic, LPCTSTR cszService)
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
DWORD __fastcall KbDDEServer::QueryString(HSZ hsz, LPTSTR szBuffer, int Size)
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
KbDDEServer::KbDDEServer(PFNCALLBACK pfnCallBack, LPCTSTR cszTopic, LPCTSTR cszService)
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
KbDDEClient::KbDDEClient(PFNCALLBACK pfnCallBack, LPCTSTR cszTopic, LPCTSTR cszService)
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
            LPBYTE pData,       // �T�[�o�[�ɓn���f�[�^�̐擪�o�C�g�̃|�C���^
            DWORD cbData,       // �f�[�^�̒���
        //    HCONV hConv,        // �ʐM�n���h��
            HSZ hszItem,        // �f�[�^���ڂ̃n���h��
            UINT wFmt,          // �N���b�v�{�[�h�t�H�[�}�b�g
            UINT wType,         // �g�����U�N�V�����^�C�v
            DWORD dwTimeout,    // �҂�����
            LPDWORD pdwResult   // �g�����U�N�V�����̌��ʂւ̃|�C���^
        )
{
    if(!m_hConv)return NULL;
    return DdeClientTransaction(
            pData,       // �T�[�o�[�ɓn���f�[�^�̐擪�o�C�g�̃|�C���^
            cbData,      // �f�[�^�̒���
            m_hConv,     // �ʐM�n���h��
            hszItem,     // �f�[�^���ڂ̃n���h��
            wFmt,        // �N���b�v�{�[�h�t�H�[�}�b�g
            wType,       // �g�����U�N�V�����^�C�v
            dwTimeout,   // �҂�����
            pdwResult    // �g�����U�N�V�����̌��ʂւ̃|�C���^
           );
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool __fastcall KbDDEClient::Execute(LPCTSTR cszFileName, 
                                     LPCTSTR cszCommand)
{
    HDDEDATA hRet;
    HSZ hszTopic;
    static LPCTSTR cszEmpty = TEXT("");
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
    std::vector<TCHAR> szTopic(szTopicLen + 1);
    if(lenFileName){
        szTopic[0] = '\"';
        lstrcpy(&szTopic[1], cszFileName);
        lstrcat(szTopic.data(), TEXT("\" "));
    }
    else{
        szTopic[0] = 0;
    }
    lstrcat(szTopic.data(), cszCommand);
    hszTopic = DdeCreateStringHandle(m_ddeInst, szTopic.data(), CP_WINANSI);
    hRet = DdeClientTransaction(
            (BYTE*)szTopic.data(),
            szTopicLen,
            m_hConv,
            NULL,
            NULL,/*CF_TEXT,*/
            XTYP_EXECUTE,
            2000,//�ҋ@����
            NULL);

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

bool __fastcall KbDDEClient::Execute2(LPCTSTR cszFileName, 
                                     LPCTSTR cszCommand)
{
    HDDEDATA hRet;
    HSZ hszTopic;
    static LPCTSTR cszEmpty = TEXT("");
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
    std::vector<TCHAR> szFileName(lenFileName + 3 + 1);
    if(lenFileName){
        lstrcpy(szFileName.data(), TEXT(" \""));
        lstrcat(&szFileName[2], cszFileName);
        lstrcat(szFileName.data(), TEXT("\""));
    }
    else{
        szFileName[0] = 0;
    }
    std::vector<TCHAR> szTopic(szTopicLen + 1);
    lstrcpy(szTopic.data(), cszCommand);
    lstrcat(szTopic.data(), szFileName.data());
    hszTopic = DdeCreateStringHandle(m_ddeInst, szTopic.data(), CP_WINANSI);
    hRet = DdeClientTransaction(
            (BYTE*)szTopic.data(),
            szTopicLen,
            m_hConv,
            NULL,
            NULL,/*CF_TEXT,*/
            XTYP_EXECUTE,
            2000,//�ҋ@����
            NULL);

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
                        dwWait,//�ҋ@����
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
{//�N�����̃R�}���h���C��������Ԃ�
 //�i���s�t�@�C�����̕����͏����j
    const char *szCommandLine = ::GetCommandLine();
    const char *p = szCommandLine;
    bool bDblQuote = false;
    while(*p == ' ')p++;
    if(*p == '"'){//��d���p���Ŋ����Ă���
        p++;
        bDblQuote = true;
    }
    while(*p){
        if(bDblQuote && *p == '"'){//��d���p���̏I���
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