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
        LPBYTE pData,       // �T�[�o�[�ɓn���f�[�^�̐擪�o�C�g�̃|�C���^
        DWORD cbData,       // �f�[�^�̒���
    //    HCONV hConv,        // �ʐM�n���h��
        HSZ hszItem,        // �f�[�^���ڂ̃n���h��
        UINT wFmt,          // �N���b�v�{�[�h�t�H�[�}�b�g
        UINT wType,         // �g�����U�N�V�����^�C�v
        DWORD dwTimeout,    // �҂�����
        LPDWORD pdwResult   // �g�����U�N�V�����̌��ʂւ̃|�C���^
    );
    bool __fastcall Execute(LPCTSTR cszFileName, LPCTSTR cszCommand);
    bool __fastcall Execute2(LPCTSTR cszFileName, LPCTSTR cszCommand); /* WildCherry 070 */
	bool __fastcall Execute(LPCSTR cszCommand, DWORD dwWait); /* RockDance2 138 */
    KbDDEClient(PFNCALLBACK pfnCallBack, LPCTSTR cszTopic, LPCTSTR cszService);
    ~KbDDEClient(void);
};
///////////////////////////////////////////////////////////////////////////////
//�N�����̃R�}���h���C��������Ԃ�
//�i���s�t�@�C�����̕����͏����j
const char* __fastcall kbGetCommandLineParams(void);
#endif
