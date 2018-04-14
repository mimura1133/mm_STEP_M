// SuperTagEditorView.cpp : CSuperTagEditorView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "SuperTagEditor.h"
#include "SuperTagEditorDoc.h"
#include "SuperTagEditorView.h"
#include "MySuperGrid.h"
#include "winamp.h"
#include "scmpx.h"
#include "MyFileDialog.h"
#include "DlgListSort.h"
#include "DlgLoadPlayList.h"
#include "DlgSetCheck.h"
#include "DlgReplaceWord.h"
#include "DlgDeleteChar.h"
#include "DlgAddNumber.h"
#include "SHBrowseForFolder.h"
//追加 by Kobarin
#include "dde/kbdde.h"

#define DDE_TOPIC_NAME TEXT("SuperTagEditor")
#define DDE_SERVICE_NAME  DDE_TOPIC_NAME

static KbDDEServer *g_DdeServer = NULL;
static CSuperTagEditorView *g_SteView = NULL;
//追加ここまで

#include "DlgUserConvFormat.h" /* AstralCircle 041 */
#include "DlgTeikei.h" /* AstralCircle 041 */
#include "DlgTeikeiPaste.h"	/* FreeFall 052 */
#include "DlgUnifyChar.h" /* StartInaction 054 */

#include "DoubleZeroString.h" /* WildCherry4 082 */

#include "MyFileDialogPlaylist.h" /* RockDance 126 */

#include "DlgFavorites.h" /* RockDance 129 */

#include "Registry.h"

#include "OptionPage/OptionSheet.h"
#include "OptionPage/OptionListBox.h"

#include "DlgConvFormatEx.h" /* STEP 009 */
#include "DlgUserConvFormat.h" /* STEP 009 */
#include "DlgUserConvFormartTag2Tag.h" /* STEP 034 */

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum	{
	CONTROL_WINAMP_LAUNCH,			// WinAmp を起動させる
	CONTROL_WINAMP_QUIT,			// WinAmp を終了させる
	CONTROL_WINAMP_PLAYFILE,		// WinAmp ファイル再生
	CONTROL_WINAMP_PLAY,			// WinAmp [X] 演奏
	CONTROL_WINAMP_STOP,			// WinAmp [V] 曲の停止
	CONTROL_WINAMP_PLAY_PREV,		// WinAmp 前の曲へ
	CONTROL_WINAMP_PLAY_NEXT,		// WinAmp 次の曲へ
	CONTROL_WINAMP_DELETE,			// プレイリストを削除
};


/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorView
extern UINT nCommandID;

IMPLEMENT_DYNCREATE(CSuperTagEditorView, CView)

BEGIN_MESSAGE_MAP(CSuperTagEditorView, CView)
	//{{AFX_MSG_MAP(CSuperTagEditorView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_CELL_COPY_DOWN, OnUpdateCellCopyDown)
	ON_COMMAND(ID_CELL_COPY_DOWN, OnCellCopyDown)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_CONV_FILENAME_TO_TRACKNAME, OnUpdateConvFilenameToTrackname)
	ON_COMMAND(ID_CONV_FILENAME_TO_TRACKNAME, OnConvFilenameToTrackname)
	ON_COMMAND(ID_CONV_TRACKNAME_TO_FILENAME, OnConvTracknameToFilename)
	ON_UPDATE_COMMAND_UI(ID_CONV_TRACKNAME_TO_FILENAME, OnUpdateConvTracknameToFilename)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG2FILE_USER, OnUpdateConvTag2fileUser)
	ON_COMMAND(ID_CONV_TAG2FILE_USER, OnConvTag2fileUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FILE2TAG_USER, OnUpdateConvFile2tagUser)
	ON_COMMAND(ID_CONV_FILE2TAG_USER, OnConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_SET_NUMBER, OnUpdateSetNumber)
	ON_COMMAND(ID_SET_NUMBER, OnSetNumber)
	ON_UPDATE_COMMAND_UI(ID_WINAMP_PLAY, OnUpdateWinampPlay)
	ON_COMMAND(ID_WINAMP_PLAY, OnWinampPlay)
	ON_UPDATE_COMMAND_UI(ID_WINAMP_STOP, OnUpdateWinampStop)
	ON_COMMAND(ID_WINAMP_STOP, OnWinampStop)
	ON_UPDATE_COMMAND_UI(ID_WINAMP_EXIT, OnUpdateWinampExit)
	ON_COMMAND(ID_WINAMP_EXIT, OnWinampExit)
	ON_UPDATE_COMMAND_UI(ID_SORT_LIST, OnUpdateSortList)
	ON_COMMAND(ID_SORT_LIST, OnSortList)
	ON_UPDATE_COMMAND_UI(ID_WRITE_PLAYLIST, OnUpdateWritePlaylist)
	ON_COMMAND(ID_WRITE_PLAYLIST, OnWritePlaylist)
	ON_UPDATE_COMMAND_UI(ID_WINAMP_PLAY_NEXT, OnUpdateWinampPlayNext)
	ON_COMMAND(ID_WINAMP_PLAY_NEXT, OnWinampPlayNext)
	ON_UPDATE_COMMAND_UI(ID_WINAMP_PLAY_PREV, OnUpdateWinampPlayPrev)
	ON_COMMAND(ID_WINAMP_PLAY_PREV, OnWinampPlayPrev)
	ON_UPDATE_COMMAND_UI(ID_LOAD_PLAYLIST, OnUpdateLoadPlaylist)
	ON_COMMAND(ID_LOAD_PLAYLIST, OnLoadPlaylist)
	ON_UPDATE_COMMAND_UI(ID_CHECK_WORD, OnUpdateCheckWord)
	ON_COMMAND(ID_CHECK_WORD, OnCheckWord)
	ON_UPDATE_COMMAND_UI(ID_SELECT_FILES_CHECK, OnUpdateSelectFilesCheck)
	ON_COMMAND(ID_SELECT_FILES_CHECK, OnSelectFilesCheck)
	ON_UPDATE_COMMAND_UI(ID_SELECT_FILES_UNCHECK, OnUpdateSelectFilesUncheck)
	ON_COMMAND(ID_SELECT_FILES_UNCHECK, OnSelectFilesUncheck)
	ON_UPDATE_COMMAND_UI(ID_SELECT_DELETE_LIST, OnUpdateSelectDeleteList)
	ON_COMMAND(ID_SELECT_DELETE_LIST, OnSelectDeleteList)
	ON_UPDATE_COMMAND_UI(ID_SELECT_DELETE_FILE, OnUpdateSelectDeleteFile)
	ON_COMMAND(ID_SELECT_DELETE_FILE, OnSelectDeleteFile)
	ON_UPDATE_COMMAND_UI(ID_SELECT_EDIT_DESTORY, OnUpdateSelectEditDestory)
	ON_COMMAND(ID_SELECT_EDIT_DESTORY, OnSelectEditDestory)
	ON_UPDATE_COMMAND_UI(ID_ALL_FILES_CHECK, OnUpdateAllFilesCheck)
	ON_COMMAND(ID_ALL_FILES_CHECK, OnAllFilesCheck)
	ON_COMMAND(ID_REVERSE_CHECK, OnReverseCheck)
	ON_UPDATE_COMMAND_UI(ID_ALL_FILES_UNCHECK, OnUpdateAllFilesUncheck)
	ON_COMMAND(ID_ALL_FILES_UNCHECK, OnAllFilesUncheck)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FILES_SELECT, OnUpdateCheckFilesSelect)
	ON_COMMAND(ID_CHECK_FILES_SELECT, OnCheckFilesSelect)
	ON_UPDATE_COMMAND_UI(ID_ADJUST_COLUMN_WIDTH, OnUpdateAdjustColumnWidth)
	ON_COMMAND(ID_ADJUST_COLUMN_WIDTH, OnAdjustColumnWidth)
	ON_UPDATE_COMMAND_UI(ID_HELP_INDEX, OnUpdateHelpIndex)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_01, OnUpdateConvFormatEx)
	ON_COMMAND(ID_CONV_FORMAT_EX_01, OnConvFormatEx01)
	ON_COMMAND(ID_CONV_FORMAT_EX_02, OnConvFormatEx02)
	ON_COMMAND(ID_CONV_FORMAT_EX_03, OnConvFormatEx03)
	ON_COMMAND(ID_CONV_FORMAT_EX_04, OnConvFormatEx04)
	ON_COMMAND(ID_CONV_FORMAT_EX_05, OnConvFormatEx05)
	ON_COMMAND(ID_CONV_FORMAT_EX_06, OnConvFormatEx06)
	ON_COMMAND(ID_CONV_FORMAT_EX_07, OnConvFormatEx07)
	ON_COMMAND(ID_CONV_FORMAT_EX_08, OnConvFormatEx08)
	ON_COMMAND(ID_CONV_FORMAT_EX_09, OnConvFormatEx09)
	ON_COMMAND(ID_CONV_FORMAT_EX_10, OnConvFormatEx10)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_UPPER, OnUpdateConvStr)
	ON_COMMAND(ID_CONV_STR_UPPER, OnConvStrUpper)
	ON_COMMAND(ID_CONV_STR_LOWER, OnConvStrLower)
	ON_COMMAND(ID_CONV_STR_HANKAKU, OnConvStrHankaku)
	ON_COMMAND(ID_CONV_STR_HIRA, OnConvStrHira)
	ON_COMMAND(ID_CONV_STR_KATA, OnConvStrKata)
	ON_COMMAND(ID_CONV_STR_HAN_ALL, OnConvStrHanAll)
	ON_COMMAND(ID_CONV_STR_HAN_ALPHA, OnConvStrHanAlpha)
	ON_COMMAND(ID_CONV_STR_HAN_KATA, OnConvStrHanKata)
	ON_COMMAND(ID_CONV_STR_HAN_SUJI, OnConvStrHanSuji)
	ON_COMMAND(ID_CONV_STR_ZEN_ALL, OnConvStrZenAll)
	ON_COMMAND(ID_CONV_STR_ZEN_ALPHA, OnConvStrZenAlpha)
	ON_COMMAND(ID_CONV_STR_ZEN_KATA, OnConvStrZenKata)
	ON_COMMAND(ID_CONV_STR_ZEN_SUJI, OnConvStrZenSuji)
	ON_COMMAND(ID_CONV_STR_TO_UPPER, OnConvStrToUpper)
	ON_COMMAND(ID_CONV_STR_TO_LOWER, OnConvStrToLower)
	ON_COMMAND(ID_CONV_STR_FIRST_UPPER, OnConvStrFirstUpper)
	ON_COMMAND(ID_CONV_STR_HIRA2KATA, OnConvStrHira2kata)
	ON_COMMAND(ID_CONV_STR_KATA2HIRA, OnConvStrKata2hira)
	ON_UPDATE_COMMAND_UI(ID_FOLDER_TREE_SYNC, OnUpdateFolderTreeSync)
	ON_COMMAND(ID_FOLDER_TREE_SYNC, OnFolderTreeSync)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FILE_SYNC, OnUpdateCheckFileSync)
	ON_COMMAND(ID_CHECK_FILE_SYNC, OnCheckFileSync)
	ON_COMMAND(ID_CONV_STR_HAN_KIGOU, OnConvStrHanKigou)
	ON_COMMAND(ID_CONV_STR_ZEN_KIGOU, OnConvStrZenKigou)
	ON_UPDATE_COMMAND_UI(ID_SORT_LIST_DIRECT, OnUpdateSortListDirect)
	ON_COMMAND(ID_SORT_LIST_DIRECT, OnSortListDirect)
	ON_UPDATE_COMMAND_UI(ID_WRITE_LIST1, OnUpdateWriteList)
	ON_COMMAND(ID_WRITE_LIST1, OnWriteList1)
	ON_COMMAND(ID_WRITE_LIST2, OnWriteList2)
	ON_COMMAND(ID_WRITE_LIST3, OnWriteList3)
	ON_COMMAND(ID_WRITE_LIST4, OnWriteList4)
	ON_COMMAND(ID_WRITE_LIST5, OnWriteList5)
	ON_UPDATE_COMMAND_UI(ID_DELETE_TAG, OnUpdateDeleteTag)
	ON_UPDATE_COMMAND_UI(ID_WRITE_TREE_PLAYLIST, OnUpdateWriteTreePlaylist)
	ON_COMMAND(ID_WRITE_TREE_PLAYLIST, OnWriteTreePlaylist)
	ON_COMMAND(ID_SET_LIST_FONT, OnSetListFont)
	ON_COMMAND(ID_DELETE_CHAR, OnDeleteChar)
	ON_UPDATE_COMMAND_UI(ID_DELETE_CHAR, OnUpdateDeleteChar)
	ON_UPDATE_COMMAND_UI(ID_SET_NUMBER_ADD, OnUpdateSetNumberAdd)
	ON_COMMAND(ID_SET_NUMBER_ADD, OnSetNumberAdd)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FOLDER_01, OnUpdateMoveFolder)
	ON_COMMAND(ID_MOVE_FOLDER_01, OnMoveFolder01)
	ON_COMMAND(ID_MOVE_FOLDER_02, OnMoveFolder02)
	ON_COMMAND(ID_MOVE_FOLDER_03, OnMoveFolder03)
	ON_COMMAND(ID_MOVE_FOLDER_04, OnMoveFolder04)
	ON_COMMAND(ID_MOVE_FOLDER_05, OnMoveFolder05)
	ON_COMMAND(ID_SELECT_TREE_COLUM, OnSelectTreeColum)
	ON_UPDATE_COMMAND_UI(ID_SELECT_TREE_COLUM, OnUpdateSelectTreeColum)
	ON_COMMAND(ID_CONV_FORMAT_USER_01, OnConvFormatUser01)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_01, OnUpdateConvFormatUser01)
	ON_COMMAND(ID_CONV_FORMAT_USER_02, OnConvFormatUser02)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_02, OnUpdateConvFormatUser02)
	ON_COMMAND(ID_CONV_FORMAT_USER_03, OnConvFormatUser03)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_03, OnUpdateConvFormatUser03)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_01, OnUpdateTeikei01)
	ON_COMMAND(ID_TEIKEI_01, OnTeikei01)
	ON_COMMAND(ID_TEIKEI_02, OnTeikei02)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_02, OnUpdateTeikei02)
	ON_COMMAND(ID_TEIKEI_03, OnTeikei03)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_03, OnUpdateTeikei03)
	ON_COMMAND(ID_TEIKEI_04, OnTeikei04)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_04, OnUpdateTeikei04)
	ON_COMMAND(ID_TEIKEI_05, OnTeikei05)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_05, OnUpdateTeikei05)
	ON_COMMAND(ID_TEIKEI_06, OnTeikei06)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_06, OnUpdateTeikei06)
	ON_COMMAND(ID_TEIKEI_07, OnTeikei07)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_07, OnUpdateTeikei07)
	ON_COMMAND(ID_TEIKEI_08, OnTeikei08)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_08, OnUpdateTeikei08)
	ON_COMMAND(ID_TEIKEI_09, OnTeikei09)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_09, OnUpdateTeikei09)
	ON_COMMAND(ID_TEIKEI_10, OnTeikei10)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_10, OnUpdateTeikei10)
	ON_COMMAND(ID_CHECK_FILENAME_MAX, OnCheckFilenameMax)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FILENAME_MAX, OnUpdateCheckFilenameMax)
	ON_COMMAND(ID_CONV_FORMAT_USER, OnConvFormatUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER, OnUpdateConvFormatUser)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI, OnUpdateTeikei)
	ON_COMMAND(ID_TEIKEI, OnTeikeiConfig)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_TOOL, OnUpdateTeikei)
	ON_COMMAND(ID_TEIKEI_TOOL, OnTeikei_ShowSubMenu)
	ON_UPDATE_COMMAND_UI(ID_DLG_TEIKEI, OnUpdateDlgTeikei)
	ON_COMMAND(ID_TEIKEI_2_01, OnTeikei201)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_01, OnUpdateTeikei201)
	ON_COMMAND(ID_TEIKEI_2_02, OnTeikei202)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_02, OnUpdateTeikei202)
	ON_COMMAND(ID_TEIKEI_2_03, OnTeikei203)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_03, OnUpdateTeikei203)
	ON_COMMAND(ID_TEIKEI_2_04, OnTeikei204)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_04, OnUpdateTeikei204)
	ON_COMMAND(ID_TEIKEI_2_05, OnTeikei205)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_05, OnUpdateTeikei205)
	ON_COMMAND(ID_TEIKEI_2_06, OnTeikei206)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_06, OnUpdateTeikei206)
	ON_COMMAND(ID_TEIKEI_2_07, OnTeikei207)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_07, OnUpdateTeikei207)
	ON_COMMAND(ID_TEIKEI_2_08, OnTeikei208)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_08, OnUpdateTeikei208)
	ON_COMMAND(ID_TEIKEI_2_09, OnTeikei209)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_09, OnUpdateTeikei209)
	ON_COMMAND(ID_TEIKEI_2_10, OnTeikei210)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_2_10, OnUpdateTeikei210)
	ON_COMMAND(ID_TEIKEI_3_01, OnTeikei301)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_01, OnUpdateTeikei301)
	ON_COMMAND(ID_TEIKEI_3_02, OnTeikei302)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_02, OnUpdateTeikei302)
	ON_COMMAND(ID_TEIKEI_3_03, OnTeikei303)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_03, OnUpdateTeikei303)
	ON_COMMAND(ID_TEIKEI_3_04, OnTeikei304)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_04, OnUpdateTeikei304)
	ON_COMMAND(ID_TEIKEI_3_05, OnTeikei305)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_05, OnUpdateTeikei305)
	ON_COMMAND(ID_TEIKEI_3_06, OnTeikei306)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_06, OnUpdateTeikei306)
	ON_COMMAND(ID_TEIKEI_3_07, OnTeikei307)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_07, OnUpdateTeikei307)
	ON_COMMAND(ID_TEIKEI_3_08, OnTeikei308)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_08, OnUpdateTeikei308)
	ON_COMMAND(ID_TEIKEI_3_09, OnTeikei309)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_09, OnUpdateTeikei309)
	ON_COMMAND(ID_TEIKEI_3_10, OnTeikei310)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_3_10, OnUpdateTeikei310)
	ON_COMMAND(ID_UNIFY_CHAR, OnUnifyChar)
	ON_UPDATE_COMMAND_UI(ID_UNIFY_CHAR, OnUpdateUnifyChar)
	ON_COMMAND(ID_CONV_FORMAT_USER_04, OnConvFormatUser04)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_04, OnUpdateConvFormatUser04)
	ON_COMMAND(ID_CONV_FORMAT_USER_05, OnConvFormatUser05)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_05, OnUpdateConvFormatUser05)
	ON_COMMAND(ID_END_EDIT_RIGHT, OnEndEditRight)
	ON_UPDATE_COMMAND_UI(ID_END_EDIT_RIGHT, OnUpdateEndEditRight)
	ON_COMMAND(ID_DLG_FAVORITES, OnDlgFavorites)
	ON_UPDATE_COMMAND_UI(ID_DLG_FAVORITES, OnUpdateDlgFavorites)
	ON_COMMAND(ID_FAVORITES_FOLDER, OnFavorites)
	ON_UPDATE_COMMAND_UI(ID_FAVORITES_FOLDER, OnUpdateFavorites)
	ON_COMMAND(ID_CALC_FOLDER_TOTAL, OnCalcFolderTotal)
	ON_UPDATE_COMMAND_UI(ID_CALC_FOLDER_TOTAL, OnUpdateCalcFolderTotal)
	ON_UPDATE_COMMAND_UI(ID_FILE_CONV_AUTO_ID3, OnUpdateFileConvAutoId3)
	ON_COMMAND(ID_EDIT_PASTE_ADD, OnEditPasteAdd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_ADD, OnUpdateEditPasteAdd)
	ON_COMMAND(ID_EDIT_COPY_FORMAT_01, OnEditCopyFormat01)
	ON_COMMAND(ID_EDIT_COPY_FORMAT_02, OnEditCopyFormat02)
	ON_COMMAND(ID_EDIT_COPY_FORMAT_03, OnEditCopyFormat03)
	ON_COMMAND(ID_EDIT_COPY_FORMAT_04, OnEditCopyFormat04)
	ON_COMMAND(ID_EDIT_COPY_FORMAT_05, OnEditCopyFormat05)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_FORMAT_01, OnUpdateEditCopyFormat)
	ON_COMMAND(ID_DELETE_CHAR_SPACE, OnDeleteCharSpace)
	ON_UPDATE_COMMAND_UI(ID_DELETE_CHAR_SPACE, OnUpdateDeleteCharSpace)
	ON_COMMAND(ID_CONV_EX_SETUP, OnConvExSetup)
	ON_UPDATE_COMMAND_UI(ID_CONV_EX_SETUP, OnUpdateConvExSetup)
	ON_COMMAND(ID_CONV_USER_SETUP, OnConvUserSetup)
	ON_UPDATE_COMMAND_UI(ID_CONV_USER_SETUP, OnUpdateConvUserSetup)
	ON_COMMAND(ID_SELECT_TREE_FILE, OnSelectTreeFile)
	ON_UPDATE_COMMAND_UI(ID_SET_LIST_FONT, OnUpdateSetListFont)
	ON_COMMAND(ID_MOVE_TO_PARENT, OnMoveToParent)
	ON_UPDATE_COMMAND_UI(ID_MOVE_TO_PARENT, OnUpdateMoveToParent)
	ON_COMMAND(ID_MOVE_TO_NEXT, OnMoveToNext)
	ON_UPDATE_COMMAND_UI(ID_MOVE_TO_NEXT, OnUpdateMoveToNext)
	ON_UPDATE_COMMAND_UI(ID_REVERSE_CHECK, OnUpdateReverseCheck)
	ON_COMMAND(ID_MOVE_TO_PREVIOUS, OnMoveToPrevious)
	ON_UPDATE_COMMAND_UI(ID_MOVE_TO_PREVIOUS, OnUpdateMoveToPrevious)
	ON_COMMAND(ID_CONV_FORMAT_USER_T2F_01, OnConvFormatUserT2f01)
	ON_COMMAND(ID_CONV_FORMAT_USER_T2F_02, OnConvFormatUserT2f02)
	ON_COMMAND(ID_CONV_FORMAT_USER_T2F_03, OnConvFormatUserT2f03)
	ON_COMMAND(ID_CONV_FORMAT_USER_T2F_04, OnConvFormatUserT2f04)
	ON_COMMAND(ID_CONV_FORMAT_USER_T2F_05, OnConvFormatUserT2f05)
	ON_COMMAND(ID_CONV_FORMAT_USER_F2T_01, OnConvFormatUserF2t01)
	ON_COMMAND(ID_CONV_FORMAT_USER_F2T_02, OnConvFormatUserF2t02)
	ON_COMMAND(ID_CONV_FORMAT_USER_F2T_03, OnConvFormatUserF2t03)
	ON_COMMAND(ID_CONV_FORMAT_USER_F2T_04, OnConvFormatUserF2t04)
	ON_COMMAND(ID_CONV_FORMAT_USER_F2T_05, OnConvFormatUserF2t05)
	ON_COMMAND(ID_CONV_TAG_TO_TAG_01, OnConvTagToTag01)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG_TO_TAG_01, OnUpdateConvTagToTag)
	ON_COMMAND(ID_CONV_TAG_TO_TAG_02, OnConvTagToTag02)
	ON_COMMAND(ID_CONV_TAG_TO_TAG_03, OnConvTagToTag03)
	ON_COMMAND(ID_CONV_TAG_TO_TAG_04, OnConvTagToTag04)
	ON_COMMAND(ID_CONV_TAG_TO_TAG_05, OnConvTagToTag05)
	ON_COMMAND(ID_CONV_TAG2TAG_SETUP, OnConvTag2tagSetup)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG2TAG_SETUP, OnUpdateConvTag2tagSetup)
	ON_COMMAND(ID_CONV_SI_FIELD_TO_ID3TAG, OnConvSiFieldToId3tag)
	ON_UPDATE_COMMAND_UI(ID_CONV_SI_FIELD_TO_ID3TAG, OnUpdateConvSiFieldToId3tag)
	ON_COMMAND(ID_CONV_STR_FIXED_UPPER_LOWER, OnConvStrFixedUpperLower)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_02, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_03, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_04, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_05, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_06, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_07, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_08, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_09, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_EX_10, OnUpdateConvFormatEx)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_LOWER, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_KATA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HIRA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HANKAKU, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HAN_ALPHA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HAN_KATA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HAN_SUJI, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HAN_ALL, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_FIRST_UPPER, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HIRA2KATA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_KATA2HIRA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_ZEN_ALL, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_ZEN_ALPHA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_ZEN_KATA, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_ZEN_SUJI, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_ZEN_KIGOU, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_HAN_KIGOU, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_WRITE_LIST2, OnUpdateWriteList)
	ON_UPDATE_COMMAND_UI(ID_WRITE_LIST3, OnUpdateWriteList)
	ON_UPDATE_COMMAND_UI(ID_WRITE_LIST4, OnUpdateWriteList)
	ON_UPDATE_COMMAND_UI(ID_WRITE_LIST5, OnUpdateWriteList)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FOLDER_02, OnUpdateMoveFolder)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FOLDER_03, OnUpdateMoveFolder)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FOLDER_04, OnUpdateMoveFolder)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FOLDER_05, OnUpdateMoveFolder)
	ON_UPDATE_COMMAND_UI(ID_TEIKEI_01, OnUpdateTeikei01)
	ON_COMMAND(ID_DLG_TEIKEI, OnTeikeiConfig)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_FORMAT_02, OnUpdateEditCopyFormat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_FORMAT_03, OnUpdateEditCopyFormat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_FORMAT_04, OnUpdateEditCopyFormat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_FORMAT_05, OnUpdateEditCopyFormat)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_TO_UPPER, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_TO_LOWER, OnUpdateConvStr)
	ON_UPDATE_COMMAND_UI(ID_SELECT_TREE_FILE, OnUpdateSelectTreeColum)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_T2F_01, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_T2F_02, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_T2F_03, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_T2F_04, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_T2F_05, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_F2T_01, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_F2T_02, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_F2T_03, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_F2T_04, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FORMAT_USER_F2T_05, OnUpdateConvFile2tagUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_FILE2TAG_USER_SUB, OnUpdateConvFile2tagUser)
	ON_COMMAND(ID_CONV_FILE2TAG_USER_SUB, OnConvFile2tagUser)
	ON_COMMAND(ID_CONV_TAG2FILE_USER_SUB, OnConvTag2fileUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG2FILE_USER_SUB, OnUpdateConvTag2fileUser)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG_TO_TAG_02, OnUpdateConvTagToTag)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG_TO_TAG_03, OnUpdateConvTagToTag)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG_TO_TAG_04, OnUpdateConvTagToTag)
	ON_UPDATE_COMMAND_UI(ID_CONV_TAG_TO_TAG_05, OnUpdateConvTagToTag)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_CONV_STR_FIXED_UPPER_LOWER, OnUpdateConvStr)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(nCommandID, nCommandID+200, OnPluginCommand)
	ON_UPDATE_COMMAND_UI_RANGE(nCommandID, nCommandID+200, OnUpdatePluginCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorView クラスの構築/消滅

CSuperTagEditorView::CSuperTagEditorView()
{
	m_bFirstInit = true;
	m_fontListView = NULL;

	// VBMP3 の初期化
//	if (vbmp3_init() == 0){
//		MessageBox("VBMP3 の初期化に失敗しました", "VBMP3エラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
//	}

	// 追加 by Kobarin
	// 多重起動時に DDE でファイル名を渡せるようにする
	g_SteView = this;
	g_DdeServer = new KbDDEServer(DdemlCallback, 
								  DDE_TOPIC_NAME,
								  DDE_SERVICE_NAME);
}

CSuperTagEditorView::~CSuperTagEditorView()
{
	// VBMP3 で使用したメモリを解放
//	vbmp3_free();

	// 追加 by Kobarin
	// DDE 関係の後始末
	delete g_DdeServer;
	g_DdeServer = NULL;

	ReleaseFontListView();
}

BOOL CSuperTagEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorView クラスの描画

void CSuperTagEditorView::OnDraw(CDC* pDC)
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

	void CSuperTagEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// 起動後の初期化の初期化時のみ行う処理
	if (m_bFirstInit) {
		DragAcceptFiles(TRUE);

		// バージョンアップ後の初回起動時処理
		if (g_bIsVersionUp) {
			g_bIsVersionUp = false;
		}

		// キー割り当てをレジストリから読み込む
		((CSuperTagEditorApp *)AfxGetApp())->ReadKeyConfig(true);

		// ジャンルリストの設定をレジストリから読み込む
		((CSuperTagEditorApp *)AfxGetApp())->ReadGenreList();
		m_List.MakeStrListGenre();

		((CSuperTagEditorApp *)AfxGetApp())->ReadFixedWordList();

		// 起動時の前回開いていたフォルダを開く
		if (g_bOptAutoOpenFolder && g_strCurrentDirectory.IsEmpty() == false) {
			// フォルダを開く
			CSuperTagEditorDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->OpenFolder(g_strCurrentDirectory);
		}

		SetTimer(1, 50, NULL);

		m_bFirstInit = false;
	}

	// TODO: GetListCtrl() メンバ関数の呼び出しを通して直接そのリスト コントロールに
	//  アクセスすることによって ListView をアイテムで固定できます。
}

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorView クラスの診断

#ifdef _DEBUG
void CSuperTagEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CSuperTagEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSuperTagEditorDoc* CSuperTagEditorView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSuperTagEditorDoc)));
	return (CSuperTagEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorView クラスのメッセージ ハンドラ
void CSuperTagEditorView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	// TODO: ユーザーによるウインドウのビュー スタイル変更に対応するコードを追加してください
}

// =============================================
// CSuperTagEditorView::PostNcDestroy
// 概要  : ウィンドウの破棄
// 引数  : none
// 戻り値: none
// =============================================
void CSuperTagEditorView::PostNcDestroy() 
{
	// ウィンドウの状態を保存
	WINDOWPLACEMENT wndpl;
	AfxGetMainWnd()->GetWindowPlacement(&wndpl);
	g_bMainFrameZoomed		= AfxGetMainWnd()->IsZoomed();
	g_bMainFrameIconic		= AfxGetMainWnd()->IsIconic();
	if (g_bMainFrameZoomed == FALSE && g_bMainFrameIconic == FALSE) {
		AfxGetMainWnd()->GetWindowRect(&g_rectMainWindow);
	}

	UpdateRegistory();					// INI ファイルの更新

	CView::PostNcDestroy();
}


// =============================================
// CSuperTagEditorView::OnCreate
// 概要  : WM_CREATE メッセージ処理
// 引数  : lpCreateStruct
// 戻り値: int
// =============================================
int CSuperTagEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_List.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);	
	m_List.m_pDoc = GetDocument();
	//m_List.InitializeGrid();

	// フォントの設定
	//UpdateFontListView();

	extern  CMySuperGrid* STEP_List;
	STEP_List = &m_List;

	return 0;
}

// =============================================
// CSuperTagEditorView::OnEraseBkgnd
// 概要  : WM_ERASEBKGND メッセージ処理
// 引数  : pDC
// 戻り値: BOOL
// =============================================
BOOL CSuperTagEditorView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
//	return CView::OnEraseBkgnd(pDC);
}

// =============================================
// CSuperTagEditorView::OnSize
// 概要  : WM_SIZE メッセージ処理
// 引数  : nType
//       : cx
//       : cy
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if (m_List.GetSafeHwnd()) {
		m_List.SetWindowPos(0,-1,-1,cx+1,cy+1,SWP_SHOWWINDOW);
	}
}

// =============================================
// CSuperTagEditorView::OnDropFiles
// 概要  : WM_DROPFILES メッセージ処理
// 引数  : hDropInfo
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnDropFiles(HDROP hDropInfo) 
{
	TCHAR	sFileName[FILENAME_MAX];
	int		nFileCount = (int)DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//追加 by Kobarin
	if(pDoc->IsTagUpdating()){
	//タグを更新中は何もしない
		DragFinish(hDropInfo);
		return;
	}

	CWaitCursor	wait;

	// ファイルが１個の場合は、プレイリストの読み込みに対応
	if (nFileCount == 1) {
		// ファイルが１個の場合のみ、プレイリストの読みこみに対応
		DragQueryFile(hDropInfo, 0, sFileName, _MAX_PATH);
		CString	strFileName = sFileName;
		strFileName.MakeLower();
		if (strFileName.Find(TEXT(".m3u")) != -1) {
			// プレイリスト追加
			LoadPlayList(sFileName);
			DragFinish(hDropInfo);
			return;
		}
	}

	// プログレスバー初期化
	pDoc->StartLoadFile(TEXT("ＭＰ３ファイル読み込み中..."));

	// ドロップされたファイルを処理する
	int i; for (i = 0; i < nFileCount; i++) {
		DragQueryFile(hDropInfo, i, sFileName, _MAX_PATH);

		CFileStatus	status;
		if (CFile::GetStatus(sFileName, status) == TRUE && status.m_mtime != -1) {
			if (status.m_attribute & CFile::directory) {
				// ディレクトリの場合
				// 下位階層を検索 /* TyphoonSwell 026 */
				bool bEnableSearchSubDir = g_bEnableSearchSubDir;
				if (g_bOptDropSearchSubFolder) {
					g_bEnableSearchSubDir = true;
				}
				pDoc->OpenFolder(sFileName);
				g_bEnableSearchSubDir = bEnableSearchSubDir;
			} else {
				// ファイルの場合
				pDoc->AddRequestFile(sFileName, NULL);
			}
		} else if (lstrlen(sFileName) == 3 && sFileName[1] == ':' && sFileName[2] == '\\') { /* STEP 027 */
				// ドライブの場合
				// 下位階層を検索
				bool bEnableSearchSubDir = g_bEnableSearchSubDir;
				if (g_bOptDropSearchSubFolder) {
					g_bEnableSearchSubDir = true;
				}
				pDoc->OpenFolder(sFileName);
				g_bEnableSearchSubDir = bEnableSearchSubDir;
		}
	}
	DragFinish(hDropInfo);

	// 追加リクエストのあったファイルのタグ情報を読み込む
#ifdef FLICKERFREE
		//m_List.SetRedraw(FALSE);
#endif
	pDoc->ExecRequestFiles(true, g_bOptLoadFileChecked);
#ifdef FLICKERFREE
		//m_List.SetRedraw(TRUE);
#endif

	// プログレスバー終了
	pDoc->EndLoadFile();

	pDoc->UpdateAllViews(NULL);
}

// =============================================
// CSuperTagEditorView::OnSetFocus
// 概要  : WM_SETFOCUS メッセージ処理
// 引数  : pOldWnd
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);

	if (m_List.GetSafeHwnd()) {
		m_List.SetFocus();
	}
}

// =============================================
// CSuperTagEditorView::OnUpdateEditCopy
// CSuperTagEditorView::OnEditCopy
// 概要  : [コピー]メニュー処理
// 引数  : 
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if (m_List.IsRangeSelected()) {
		// 範囲選択あり
		pCmdUI->Enable(TRUE);
	} else if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		//pCmdUI->Enable(m_List.IsCurrentCellEditOK() ? TRUE : FALSE);
		pCmdUI->Enable(TRUE);
	}
}
void CSuperTagEditorView::OnEditCopy() 
{
	m_List.ClipboardCopy();
}

// =============================================
// CSuperTagEditorView::OnUpdateEditCut
// CSuperTagEditorView::OnEditCut
// 概要  : [カット]メニュー処理
// 引数  : 
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// [コピー]の条件と同じ
	//OnUpdateEditCopy(pCmdUI);
	if (m_List.IsRangeSelected()) {
		// 範囲選択あり
		pCmdUI->Enable(TRUE);
	} else if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		pCmdUI->Enable(m_List.IsCurrentCellEditOK() ? TRUE : FALSE);
	}
}
void CSuperTagEditorView::OnEditCut() 
{
	m_List.ClipboardCut();
#ifndef FLICKERFREE
	GetDocument()->UpdateAllViews(NULL);
#endif
}

// =============================================
// CSuperTagEditorView::OnUpdateEditPaste
// CSuperTagEditorView::OnEditPaste
// 概要  : [ペースト]メニュー処理
// 引数  : 
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// [コピー]の条件と同じ
	//OnUpdateEditCopy(pCmdUI);
	OnUpdateEditCut(pCmdUI);
}
void CSuperTagEditorView::OnEditPaste() 
{
	m_List.ClipboardPaste();
#ifndef FLICKERFREE
	GetDocument()->UpdateAllViews(NULL);
#endif
}

// =============================================
// CSuperTagEditorView::OnUpdateCellCopyDown
// CSuperTagEditorView::OnCellCopyDown
// 概要  : [下方向へコピー]メニュー処理
// 引数  : 
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnUpdateCellCopyDown(CCmdUI* pCmdUI) 
{
	if (m_List.IsRangeSelected() == false) {
		// 範囲選択無し
		pCmdUI->Enable(m_List.IsRangeSelected() ? TRUE : FALSE);
	} else {
		// 範囲選択あり
		pCmdUI->Enable(((m_List.GetRangeSelect())[0].y == (m_List.GetRangeSelect())[1].y) ? FALSE : TRUE);
	}
}
void CSuperTagEditorView::OnCellCopyDown() 
{
	m_List.CellCopyDown();

#ifndef FLICKERFREE
	GetDocument()->UpdateAllViews(NULL);
#endif
}

// =============================================
// CSuperTagEditorView::OnUpdateConvFilenameToTrackname
// CSuperTagEditorView::OnConvFilenameToTrackname
// 概要  : [ファイル名 => TrackName]メニュー処理
// 引数  : 
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnUpdateConvFilenameToTrackname(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_List.GetSelectedItem() < 0) ? FALSE : TRUE);
}
void CSuperTagEditorView::OnConvFilenameToTrackname() 
{
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(1);
}

// =============================================
// CSuperTagEditorView::OnUpdateConvTracknameToFilename
// CSuperTagEditorView::OnConvTracknameToFilename
// 概要  : [TrackName => ファイル名]メニュー処理
// 引数  : 
// 戻り値: none
// =============================================
void CSuperTagEditorView::OnUpdateConvTracknameToFilename(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_List.GetSelectedItem() < 0) ? FALSE : TRUE);
}
void CSuperTagEditorView::OnConvTracknameToFilename() 
{
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(2);
}

void CSuperTagEditorView::OnUpdateConvTag2fileUser(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_List.GetSelectedItem() < 0) ? FALSE : TRUE);
}
void CSuperTagEditorView::OnConvTag2fileUser() 
{
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(3, g_userConvFormat[g_nUserConvFormatType].strTag2File);
}

void CSuperTagEditorView::OnUpdateConvFile2tagUser(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_List.GetSelectedItem() < 0) ? FALSE : TRUE);
}
void CSuperTagEditorView::OnConvFile2tagUser() 
{
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(4, g_userConvFormat[g_nUserConvFormatType].strFile2Tag);
}

void CSuperTagEditorView::OnUpdateSetNumber(CCmdUI* pCmdUI) 
{
	if (m_List.IsRangeSelected() == false) {
		// 範囲選択無し
		pCmdUI->Enable(FALSE);
	} else {
		// 範囲選択あり
		pCmdUI->Enable(((m_List.GetRangeSelect())[0].y == (m_List.GetRangeSelect())[1].y) ? FALSE : TRUE);
	}
}
void CSuperTagEditorView::OnSetNumber() 
{
	m_List.SetFillNumber(1, 1);
}


//---------------------------------------------------------------------------
// func:	Winamp を起動させる
// input:	none
// output:	bool = TRUE : 起動成功
//---------------------------------------------------------------------------
bool CSuperTagEditorView::ExecuteWinamp()
{
	if (g_sOptWinAmpPath.IsEmpty()) return false;

	SHELLEXECUTEINFO	sei;

	// コマンドの設定
	sei.cbSize			= sizeof(SHELLEXECUTEINFO);
	sei.fMask			= SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd			= GetSafeHwnd();
	sei.lpVerb			= TEXT("open"); 
	sei.lpFile			= g_sOptWinAmpPath;
	sei.lpParameters	= TEXT("");
	sei.lpDirectory		= NULL;
	sei.nShow			= SW_SHOW;
	sei.hInstApp		= AfxGetInstanceHandle();

	if (ShellExecuteEx(&sei) == NULL) return false;		// 起動させる
	Sleep(200);											// 0.2sec のウエイト
	for (DWORD dwWait = WAIT_TIMEOUT;					// 完全に起動するまで待つ
		dwWait == WAIT_TIMEOUT;
		dwWait = WaitForInputIdle(sei.hProcess, 10)){
		// Application->ProcessMessages();					// 一旦他にプロセスを回す
	}
	return true;
}

//---------------------------------------------------------------------------
// func:	Lilith を起動させる
// input:	none
// output:	bool = TRUE : 起動成功
//---------------------------------------------------------------------------
bool CSuperTagEditorView::ExecuteLilith(void)
{
	if (g_sOptWinAmpPath.IsEmpty()) return false;

	SHELLEXECUTEINFO	sei;

	// コマンドの設定
	sei.cbSize			= sizeof(SHELLEXECUTEINFO);
	sei.fMask			= SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd			= GetSafeHwnd();
	sei.lpVerb			= TEXT("open"); 
	sei.lpFile			= g_sOptWinAmpPath;
	sei.lpParameters	= TEXT("/nomultiboot");
	sei.lpDirectory		= NULL;
	sei.nShow			= SW_SHOW;
	sei.hInstApp		= AfxGetInstanceHandle();

	if (ShellExecuteEx(&sei) == NULL) return false;		// 起動させる
	Sleep(200);											// 0.2sec のウエイト
	for (DWORD dwWait = WAIT_TIMEOUT;					// 完全に起動するまで待つ
		dwWait == WAIT_TIMEOUT;
		dwWait = WaitForInputIdle(sei.hProcess, 10)){
		// Application->ProcessMessages();					// 一旦他にプロセスを回す
	}
	return true;
}

//---------------------------------------------------------------------------
// func:	Winamp関連コマンドの実行
// input:	int = ショートカットコマンド番号
// output:	none
//---------------------------------------------------------------------------
void CSuperTagEditorView::ExecWinampCommand(int nCommand, LPARAM lParam)
{
	//追加 by Kobarin
	if(PlayerTypeIsKbmplay()){
		ExecKbmplayCommand(nCommand, lParam);
		return;
	}
	/* LastTrain 070 */
	if(PlayerTypeIsLilith()){
		ExecLilithCommand(nCommand, lParam);
		return;
	}
	if (g_nOptPlayerType == PLAYER_EXTEND) {
		// その他の外部プレイヤー
		if (nCommand == CONTROL_WINAMP_PLAYFILE && !g_sOptWinAmpPath.IsEmpty()) {
			// 引数にファイル名を渡して登録されているプレイヤーを起動
			SHELLEXECUTEINFO	sei;
			sei.cbSize			= sizeof(SHELLEXECUTEINFO);
			sei.fMask			= SEE_MASK_NOCLOSEPROCESS;
			sei.hwnd			= GetSafeHwnd();
			sei.lpVerb			= TEXT("open"); 
			sei.lpFile			= g_sOptWinAmpPath;
			CString param = CString("\"") + (LPCSTR)((COPYDATASTRUCT *)lParam)->lpData + "\""; /* Misirlou 145 */
			sei.lpParameters	= param;//(LPCSTR)((COPYDATASTRUCT *)lParam)->lpData; /* Misirlou 145 */
			sei.lpDirectory		= NULL;
			sei.nShow			= SW_SHOW;
			sei.hInstApp		= AfxGetInstanceHandle();
			ShellExecuteEx(&sei);
		}
		return;
	}

	/* Winamp/SCMPX の起動とハンドルの取得 */
	HWND hWinamp;
	const auto sClassName = (g_nOptPlayerType == PLAYER_WINAMP) ? TEXT("Winamp v1.x") : TEXT("SCMPX");
	for (int nErr = 0; nErr < 5; nErr++){				// 起動リトライは5回まで
		if ((hWinamp = ::FindWindow(sClassName, NULL)) != NULL) break;	// ウィンドウハンドルの取得
		if ((nCommand == CONTROL_WINAMP_QUIT)			// 終了コマンドなら意味がない
		||  (nCommand == CONTROL_WINAMP_STOP)			// 停止コマンドなら意味がない
		||  (!ExecuteWinamp())) return;					// 起動を試みる
		// Application->ProcessMessages();
	}
	if (hWinamp == NULL) return;

	/* コマンド別機能 */
	switch (nCommand){
	case CONTROL_WINAMP_LAUNCH:
		/* WinAmp/SCMPX を起動させる */
		break;

	case CONTROL_WINAMP_QUIT:
		/* WinAmp/SCMPX を終了させる */
		::SendMessage(hWinamp, WM_CLOSE, NULL, NULL);
		break;

	case CONTROL_WINAMP_PLAYFILE:
		if (g_nOptPlayerType == PLAYER_WINAMP) {
			// Winamp
			::SendMessage(hWinamp, WM_COPYDATA, NULL, lParam);
		} else {
			// SCMPX
			COPYDATASTRUCT	*cds = (COPYDATASTRUCT *)lParam;
			int i; for (i = 0; i < (int)cds->cbData; i++) {
				::SendMessage(hWinamp, WM_WA_IPC, ((char *)cds->lpData)[i], IPC_PLAYFILE);
			}
		}
		break;

	case CONTROL_WINAMP_PLAY:
		/* WinAmp [X] 演奏 */
		// Winamp/SCMPX 互換
		::SendMessage(hWinamp, WM_WA_IPC, 0, IPC_STARTPLAY);
		break;

	case CONTROL_WINAMP_DELETE:
		// IPC_DELETE deletes the internal Winamp playlist.
		::SendMessage(hWinamp, WM_WA_IPC, 0, IPC_DELETE);

	case CONTROL_WINAMP_STOP:
		/* WinAmp [V] 曲の停止 */
		::SendMessage(hWinamp, WM_COMMAND, WINAMP_BUTTON4, 0);
		break;

	case CONTROL_WINAMP_PLAY_PREV:
		/* WinAmp [Z] 前の曲へ */
		::SendMessage(hWinamp, WM_COMMAND, WINAMP_BUTTON1, 0);
		break;

	case CONTROL_WINAMP_PLAY_NEXT:
		/* WinAmp [B] 次の曲へ */
		::SendMessage(hWinamp, WM_COMMAND, WINAMP_BUTTON5, 0);
		break;
	}
}

void CSuperTagEditorView::ExecKbmplayCommand(int nCommand, LPARAM lParam,BOOL bPlay)
{
//追加 by Kobarin
 //KbMedia Player を制御するための関数
    // KbMedia Player が起動済みかどうか調べる
	//static const char cszMutexKbmplay[] = "KbMIDI Player"; /* Misirlou 138 */
	CWinApp* pApp = AfxGetApp();
	CString strINI = pApp->m_pszProfileName;
	//Profile_Initialize(strINI, TRUE);
	CString cszMutexKbmplay = MyGetProfileString(TEXT("haseta"), TEXT("KbmplayName"), TEXT("KbMedia Player")); /* Misirlou 138 */
	//Profile_Free();
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, cszMutexKbmplay);
	if(!hMutex){
        // 終了・演奏停止コマンドの実行のためにわざわざ起動する必要はない
		if( nCommand == CONTROL_WINAMP_QUIT	||
            nCommand == CONTROL_WINAMP_STOP){
            return;
        }
        if(!ExecuteWinamp())return;
	    hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, cszMutexKbmplay);
        if(!hMutex)return;
    }
    CloseHandle(hMutex);

    // DDE による制御
	/* Misirlou 138 */
	/*
    KbDDEClient ddeClient(NULL, 
                          "KbMIDI Player", //サービス名
                          "KbMIDI Player");//トピック名
	*/
    KbDDEClient ddeClient(NULL, 
                          cszMutexKbmplay, //サービス名
                          cszMutexKbmplay);//トピック名

	// コマンド別機能
	switch (nCommand){
	case CONTROL_WINAMP_LAUNCH:
		break;

    case CONTROL_WINAMP_QUIT:
		// KbMedia Player を終了させる 
		ddeClient.Execute(TEXT(""), TEXT("/quit"));
		break;

    case CONTROL_WINAMP_PLAYFILE:{
        // ファイルを開く
	    COPYDATASTRUCT	*cds = (COPYDATASTRUCT *)lParam;
        if(bPlay){
            //リストに追加＋演奏開始
            ddeClient.Execute((LPCTSTR)cds->lpData, TEXT("/ADD"));
        }
        else{
            //リストに追加のみ
            ddeClient.Execute((LPCTSTR)cds->lpData, TEXT("/ADD /NOPLAY /NP")/* Misirlou 138 */);
        }
        //ちなみに、
        //ddeClient.Execute(szFileName, "/NOPLAY");//ファイルは開くが再生はしない
        //ddeClient.Execute(szFileName, "/ADD /NOPLAY");//リストに追加
        //ddeClient.Execute(szFileName, "/ADD");//リストに追加＋演奏開始
        //ddeClient.Execute(szFileName, "");//リストに追加しないで演奏開始
        //のようになります(^^;
        break;
    }
	case CONTROL_WINAMP_PLAY:
		// 演奏開始
        ddeClient.Execute(TEXT(""), TEXT("/play"));
        break;

	case CONTROL_WINAMP_DELETE:
        // リスト破棄
        // KbMedia Player Version 2.07β4 以降でのみ機能します
        ddeClient.Execute(TEXT(""), TEXT("/clear"));
        break;

    case CONTROL_WINAMP_STOP:
		// 演奏停止 
        ddeClient.Execute(TEXT(""), TEXT("/stop"));
        // すぐに停止するのではなく、フェードアウトさせて停止する場合は
        //ddeClient.Execute("", "/fadeout");
		break;

	case CONTROL_WINAMP_PLAY_PREV:
		// 前の曲
        ddeClient.Execute(TEXT(""), TEXT("/prev"));
		break;

	case CONTROL_WINAMP_PLAY_NEXT:
		// 次の曲
        ddeClient.Execute(TEXT(""), TEXT("/next"));
		break;
	}
}

void CSuperTagEditorView::ExecLilithCommand(int nCommand, LPARAM lParam, BOOL bPlay) /* WildCherry 070 */
{
	HWND hLilith;
	const auto sClassName = TEXT("Lilith");
	for (int nErr = 0; nErr < 5; nErr++){				// 起動リトライは5回まで
		if ((hLilith = ::FindWindow(sClassName, NULL)) != NULL) break;	// ウィンドウハンドルの取得
		if ((nCommand == CONTROL_WINAMP_QUIT)			// 終了コマンドなら意味がない
		||  (nCommand == CONTROL_WINAMP_STOP)			// 停止コマンドなら意味がない
		||  (!ExecuteLilith())) return;					// 起動を試みる
		// Application->ProcessMessages();
	}
	if (hLilith == NULL) return;

    // DDE による制御  
	KbDDEClient ddeClient(NULL, 
						  TEXT("LILITH_EXT_CONTROL"), //サービス名
						  TEXT("LILITH_EXT_CONTROL"));//トピック名

	// コマンド別機能
	switch (nCommand){
	case CONTROL_WINAMP_LAUNCH:
		break;

    case CONTROL_WINAMP_QUIT:
		// KbMedia Player を終了させる 
		ddeClient.Execute(TEXT(""), TEXT("/quit"));
		break;

    case CONTROL_WINAMP_PLAYFILE:{
        // ファイルを開く
	    COPYDATASTRUCT	*cds = (COPYDATASTRUCT *)lParam;
        if(bPlay){
            //リストに追加＋演奏開始
            ddeClient.Execute2((LPCTSTR)cds->lpData, TEXT("/add"));
            ddeClient.Execute2(TEXT(""), TEXT("/add /play"));
        }
        else{
            //リストに追加のみ
            ddeClient.Execute2((LPCTSTR)cds->lpData, TEXT("/add"));
        }
        break;
    }
	case CONTROL_WINAMP_PLAY:
		// 演奏開始
        ddeClient.Execute(TEXT(""), TEXT("/play"));
        break;

	case CONTROL_WINAMP_DELETE:
        // リスト破棄
        ddeClient.Execute(TEXT(""), TEXT("/delete all"));
        break;

    case CONTROL_WINAMP_STOP:
		// 演奏停止 
        ddeClient.Execute(TEXT(""), TEXT("/stop"));
		break;

	case CONTROL_WINAMP_PLAY_PREV:
		// 前の曲
        //ddeClient.Execute("", "/prev");
		::SendMessage(hLilith, WM_COMMAND, 40027, 0);
		break;

	case CONTROL_WINAMP_PLAY_NEXT:
		// 次の曲
        //ddeClient.Execute("", "/next");
		::SendMessage(hLilith, WM_COMMAND, 40028, 0);
		break;
	}
}

// WinAmp で再生させる
void CSuperTagEditorView::OnUpdateWinampPlay(CCmdUI* pCmdUI) 
{
	if (g_sOptWinAmpPath.IsEmpty()) {
		pCmdUI->Enable(FALSE);
	} else {
		//pCmdUI->Enable(m_List.GetSelectFileName() ? TRUE : FALSE);
		pCmdUI->Enable(m_List.GetSelectedItem() != -1 ? TRUE : FALSE);
	}
}
void CSuperTagEditorView::OnWinampPlay() 
{
	CWaitCursor		wait;
	COPYDATASTRUCT	cds;

	if (!PlayerTypeIsKbmplay()) {
		// 再生を停止させる
		ExecWinampCommand(CONTROL_WINAMP_STOP);
	}

	// プレイリストをクリア
	ExecWinampCommand(CONTROL_WINAMP_DELETE);

	if (m_List.GetSelectedItem() == 1) {
		// １曲のみ選択
		cds.dwData = IPC_PLAYFILE;
		cds.lpData = (void *)m_List.GetSelectFileName();
		if (cds.lpData != NULL) {
			cds.cbData = strlen((char *)cds.lpData)+1;
			// 再生開始
			ExecWinampCommand(CONTROL_WINAMP_PLAYFILE, (LPARAM)&cds);
			ExecWinampCommand(CONTROL_WINAMP_PLAY);
		}
	} else {
		// 複数選択
		CSuperTagEditorDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		bool	bEntryOK = false;
		int		nIndex = -1;
		while((nIndex = m_List.GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED)) >=0) {
			if (m_List.IsItemFile(m_List.GetTreeItem(nIndex)) == TRUE) {
				// ファイルのみ処理する
				int		nNumber = (int)m_List.GetLParamFromIndex(nIndex);
				if (nNumber >= 0) {
					FILE_MP3	*fileMP3 = pDoc->GetListMP3(nNumber);
					cds.dwData = IPC_PLAYFILE;
					cds.lpData = (void *)(implicit_cast<LPCTSTR>(fileMP3->strFullPathName));
					if (cds.lpData != NULL) {
						cds.cbData = strlen((char *)cds.lpData)+1;
						// 再生開始
						// 追加 by Kobarin
						// KbMedia Player の場合、１曲目は開くと同時に再生
						// するが、２曲目以降は追加するだけ
						if(PlayerTypeIsKbmplay()){
							ExecKbmplayCommand(CONTROL_WINAMP_PLAYFILE, 
											   (LPARAM)&cds,
											   !bEntryOK); 
						}
						else if(PlayerTypeIsLilith()){ /* WildCherry 070 */
							ExecLilithCommand(CONTROL_WINAMP_PLAYFILE, 
											   (LPARAM)&cds,
											   !bEntryOK);
						}
						else{
							ExecWinampCommand(CONTROL_WINAMP_PLAYFILE, (LPARAM)&cds);
						}
						bEntryOK = true;
					}
				}
			}
		}
		if (bEntryOK) {
			// 再生開始
			// 追加 by Kobarin
			// KbMedia Player の場合はすでに演奏が開始されている
			if(!PlayerTypeIsKbmplay()) {
				ExecWinampCommand(CONTROL_WINAMP_PLAY);
			}
		}
	}
}

// WinAmp の再生を停止させる
void CSuperTagEditorView::OnUpdateWinampStop(CCmdUI* pCmdUI) 
{
	//修正 by Kobarin
	//pCmdUI->Enable((g_nOptPlayerType != PLAYER_WINAMP || g_sOptWinAmpPath.IsEmpty()) ? FALSE : TRUE);
	pCmdUI->Enable((g_sOptWinAmpPath.IsEmpty()) ? FALSE : TRUE);
}
void CSuperTagEditorView::OnWinampStop() 
{
	CWaitCursor	wait;
	ExecWinampCommand(CONTROL_WINAMP_STOP);
}

// WinAmp を終了させる
void CSuperTagEditorView::OnUpdateWinampExit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_sOptWinAmpPath.IsEmpty() ? FALSE : TRUE);
}
void CSuperTagEditorView::OnWinampExit() 
{
	CWaitCursor	wait;
	ExecWinampCommand(CONTROL_WINAMP_QUIT);
}

// WinAmp ：前の曲へ
void CSuperTagEditorView::OnUpdateWinampPlayPrev(CCmdUI* pCmdUI) 
{
	//修正 by Kobarin
	//pCmdUI->Enable((g_nOptPlayerType != PLAYER_WINAMP || g_sOptWinAmpPath.IsEmpty()) ? FALSE : TRUE);
	pCmdUI->Enable((g_nOptPlayerType == PLAYER_SCMPX || g_sOptWinAmpPath.IsEmpty()) ? FALSE : TRUE);
}
void CSuperTagEditorView::OnWinampPlayPrev() 
{
	CWaitCursor	wait;
	ExecWinampCommand(CONTROL_WINAMP_PLAY_PREV);
}

// WinAmp ：次の曲へ
void CSuperTagEditorView::OnUpdateWinampPlayNext(CCmdUI* pCmdUI) 
{
	//修正 by Kobarin
	//pCmdUI->Enable((g_nOptPlayerType != PLAYER_WINAMP || g_sOptWinAmpPath.IsEmpty()) ? FALSE : TRUE);
	pCmdUI->Enable((g_nOptPlayerType == PLAYER_SCMPX || g_sOptWinAmpPath.IsEmpty()) ? FALSE : TRUE);
}
void CSuperTagEditorView::OnWinampPlayNext() 
{
	CWaitCursor	wait;
	ExecWinampCommand(CONTROL_WINAMP_PLAY_NEXT);
}

// 並べ替え
void CSuperTagEditorView::OnUpdateSortList(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnSortList() 
{
	CDlgListSort	dialog;

	dialog.m_nKeyColumn1	= g_sortState[0].nKeyColumn + 1;
	dialog.m_nSortType1		= g_sortState[0].nType;
	dialog.m_nKeyColumn2	= g_sortState[1].nKeyColumn + 1;
	dialog.m_nSortType2		= g_sortState[1].nType;
	dialog.m_nKeyColumn3	= g_sortState[2].nKeyColumn + 1;
	dialog.m_nSortType3		= g_sortState[2].nType;
	dialog.m_nKeyColumn4	= g_sortState[3].nKeyColumn + 1;
	dialog.m_nSortType4		= g_sortState[3].nType;
	dialog.m_nKeyColumn5	= g_sortState[4].nKeyColumn + 1;
	dialog.m_nSortType5		= g_sortState[4].nType;
	if (dialog.DoModal() == IDOK) {
		g_sortState[0].nKeyColumn	= dialog.m_nKeyColumn1 - 1;
		g_sortState[0].nType		= dialog.m_nSortType1;
		g_sortState[1].nKeyColumn	= dialog.m_nKeyColumn2 - 1;
		g_sortState[1].nType		= dialog.m_nSortType2;
		g_sortState[2].nKeyColumn	= dialog.m_nKeyColumn3 - 1;
		g_sortState[2].nType		= dialog.m_nSortType3;
		g_sortState[3].nKeyColumn	= dialog.m_nKeyColumn4 - 1;
		g_sortState[3].nType		= dialog.m_nSortType4;
		g_sortState[4].nKeyColumn	= dialog.m_nKeyColumn5 - 1;
		g_sortState[4].nType		= dialog.m_nSortType5;

		// マルチカラムソート実行
		OnSortListDirect();
	}
}

// 並べ替えを再実行
void CSuperTagEditorView::OnUpdateSortListDirect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnSortListDirect() 
{
	// マルチカラムソート実行
	CWaitCursor	wait;
	m_List.ShowWindow(SW_HIDE);		// 描画禁止(ちらつき防止)
	m_List.MultiColumnSort(&g_sortState[0], SORT_KEY_MAX);
	m_List.ShowWindow(SW_NORMAL);	// 描画許可(ちらつき防止)
}

// プレイリストを出力
void CSuperTagEditorView::OnUpdateWritePlaylist(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnWritePlaylist()
{
	CString	strDefaultDir = g_strCurrentPlayList;
	if (IsFolderName(strDefaultDir) == false) strDefaultDir += '\\';
	strDefaultDir += "*.m3u";

	// ファイル選択ダイアログを開く
	static	LPCTSTR	sFileFilter =	TEXT("PlayList(*.m3u)|*.m3u|") \
									TEXT("All Files(*.*)|*.*|");
	CFileDialog		dialog(FALSE, TEXT(".m3u"), g_strCurrentPlayList,
	                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	                       sFileFilter, NULL);
	if (dialog.DoModal() == IDOK) {
		CWaitCursor	wait;
		g_strCurrentPlayList = dialog.GetPathName();

		// プレイリスト出力
		m_List.WritePlayList(g_strCurrentPlayList);
	}
}

// ツリー構成分類型のプレイリストファイル出力
void CSuperTagEditorView::OnUpdateWriteTreePlaylist(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnWriteTreePlaylist() 
{
	// プレイリストを出力するフォルダを指定する
	extern	BOOL SelectDirectory(LPTSTR);
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};
	_tcscpy(sFolderName, g_strCurrentPlayList);
	if (SelectDirectory(sFolderName) == FALSE) {
		return;
	}
	g_strCurrentPlayList = sFolderName;

	// プレイリストを出力
	CString	strOutputDir = g_strCurrentPlayList;
	if (IsFolderName(strOutputDir) == false) strOutputDir += '\\';
	m_List.WriteTreePlayList(strOutputDir);
}

// プレイリストの読みこみ
void CSuperTagEditorView::OnUpdateLoadPlaylist(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnLoadPlaylist()
{
	CString	strDefaultDir = g_strCurrentPlayList;
	if (IsFolderName(strDefaultDir) == false) strDefaultDir += '\\';
	strDefaultDir += "*.m3u";

	// ファイル選択ダイアログを開く
	static const auto sFileFilter =	TEXT("PlayList(*.m3u)|*.m3u|") \
									TEXT("All Files(*.*)|*.*|");
	CMyFileDialogPlaylist/* RockDance 126 */		dialog(TRUE, TEXT(".m3u"), g_strCurrentPlayList,
	                       OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXTENSIONDIFFERENT | OFN_PATHMUSTEXIST,
	                       sFileFilter, NULL);
	dialog.m_bShowLoadPlaylistDlg = g_bShowLoadPlaylistDlg; /* RockDance 126 */
	if (dialog.DoModal() == IDOK) {
		g_bShowLoadPlaylistDlg = dialog.m_bShowLoadPlaylistDlg ? true : false; /* RockDance 126 */
		LoadPlayList(dialog.GetPathName());
	}
}
void CSuperTagEditorView::LoadPlayList(LPCTSTR sFileName)
{
	CWaitCursor	wait;
	g_strCurrentPlayList = sFileName;

	// プレイリスト読みこみ
	CDlgLoadPlayList	dialog;
	dialog.m_bClearList		= g_bPlayListClearList ? TRUE : FALSE;
	dialog.m_bClearCheck	= g_bPlayListClearCheck ? TRUE : FALSE;
	dialog.m_nAddListFlag	= g_bPlayListAddList ? 0 : 1;
	dialog.m_nFileCheckFlag	= g_bPlayListFileCheck ? 0 : 1;
	if (!g_bShowLoadPlaylistDlg || dialog.DoModal() == IDOK) {
		CSuperTagEditorDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		g_bPlayListClearList	= dialog.m_bClearList ? true : false;
		g_bPlayListClearCheck	= dialog.m_bClearCheck ? true : false;
		g_bPlayListAddList		= dialog.m_nAddListFlag ? false : true;
		g_bPlayListFileCheck	= dialog.m_nFileCheckFlag ? false : true;

		// リストをクリアしてから読みこむ
		if (dialog.m_bClearList) {
			if (pDoc->OnNewDocument() == FALSE) {
				// 中止
				return;
			}
		}

		// チェックをクリアしてから読みこむ
		if (dialog.m_bClearCheck) {
			m_List.SetCheckAllFiles(FALSE);
		}

		// プレイリスト読みこみ
		pDoc->StartLoadFile(TEXT("ＭＰ３ファイル読み込み中..."));
		pDoc->LoadPlayList(g_strCurrentPlayList);
		if (g_bPlayListAddList) {
			// 追加リクエストのあったファイルのタグ情報を読み込む
#ifdef FLICKERFREE
			//m_List.SetRedraw(FALSE);
#endif
			pDoc->ExecRequestFiles(false, false);
#ifdef FLICKERFREE
			//m_List.SetRedraw(TRUE);
#endif
		}
		pDoc->EndLoadFile();

		// チェックマークを付ける
		if (g_bPlayListFileCheck) {
			int		no, nCount;
			nCount = pDoc->GetRequestFileCount();
			for (no = 0; no < nCount; no++) {
				FILE_STATE	*stat = pDoc->GetRequestFile(no);

				// チェック条件の設定
				CHECK_WORD_STATE	chkWord;
				chkWord.strSearchWord	= stat->strFullPathName;	// 文字列
				chkWord.strReplaceWord	= "";
				chkWord.nTargetColumn	= COLUMN_FULL_PATH_NAME;	// 検索対象カラム
				chkWord.bCheckDiffUL	= FALSE;					// 大文字／小文字の区別
				chkWord.bRegExp			= FALSE;					// 正規表現
				chkWord.bRangeSelected	= FALSE;					// 選択範囲のみ
				chkWord.bMatchComplete	= TRUE;						// 完全に一致
				chkWord.bMatchSelected	= FALSE;					// チェックせずに選択する
				m_List.CheckFileAtColumnState(&chkWord);
			}
		}
		pDoc->ClearRequestFiles();

		pDoc->UpdateAllViews(NULL);
	}
}

// 条件を指定してチェックする
void CSuperTagEditorView::OnUpdateCheckWord(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnCheckWord() 
{
	CDlgSetCheck	dialog;

	CHECK_WORD_STATE	*pState = &g_chkWord[CHECK_STATE_CHECK];

	dialog.m_bEnableRangeSelected	= (m_List.GetSelectedCount() >= 2) ? TRUE : FALSE;
	dialog.m_strSearchWord	= pState->strSearchWord;			// 文字列
	dialog.m_nTargetColumn	= (pState->nTargetColumn - COLUMN_FILE_NAME);	// 検索対象カラム
	dialog.m_bCheckDiffUL	= pState->bCheckDiffUL;				// 大文字／小文字の区別
	dialog.m_bRegExp		= pState->bRegExp;					// 正規表現
	dialog.m_bRangeSelected	= dialog.m_bEnableRangeSelected;	// 選択範囲のみ
	dialog.m_bMatchComplete	= pState->bMatchComplete;			// 完全に一致
	dialog.m_bMatchSelected	= pState->bMatchSelected;			// チェックせずに選択する
	if (dialog.DoModal() == IDOK) {
		pState->strSearchWord	= dialog.m_strSearchWord;		// 文字列
		pState->nTargetColumn	= COLUMN_FILE_NAME + dialog.m_nTargetColumn;	// 検索対象カラム
		pState->bCheckDiffUL	= dialog.m_bCheckDiffUL;		// 大文字／小文字の区別
		pState->bRegExp			= dialog.m_bRegExp;				// 正規表現
		pState->bRangeSelected	= dialog.m_bRangeSelected;		// 選択範囲のみ
		pState->bMatchComplete	= dialog.m_bMatchComplete;		// 完全に一致
		pState->bMatchSelected	= dialog.m_bMatchSelected;		// チェックせずに選択する
		m_List.CheckFileAtColumnState(pState);

		CSuperTagEditorDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->UpdateAllViews(NULL);
	}
}

// チェックされているファイルを選択
void CSuperTagEditorView::OnUpdateCheckFilesSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnCheckFilesSelect() 
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_List.SetCheckFileSelect();

	pDoc->UpdateAllViews(NULL);
}

// 選択範囲をチェック
void CSuperTagEditorView::OnUpdateSelectFilesCheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_List.GetSelectedCount() ? TRUE : FALSE);
}
void CSuperTagEditorView::OnSelectFilesCheck() 
{
	m_List.SetSelectFileCheck(TRUE);
}

// 選択範囲のチェックを解除
void CSuperTagEditorView::OnUpdateSelectFilesUncheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_List.GetSelectedCount() ? TRUE : FALSE);
}
void CSuperTagEditorView::OnSelectFilesUncheck() 
{
	m_List.SetSelectFileCheck(FALSE);
}

// ファイルをリストから削除する
void CSuperTagEditorView::OnUpdateSelectDeleteList(CCmdUI* pCmdUI) 
{
	if (m_List.GetSelectedCount() == 1 && m_List.GetSelectFileName() == NULL) {
		// フォルダの単一選択
		if (g_bValidFolderSelect) { /* SeaKnows 033 */
			pCmdUI->Enable(TRUE); // フォルダの単一選択はオッケー
		} else {
			pCmdUI->Enable(FALSE);
		}
	} else {
		// ファイルの単一選択／複数選択
		pCmdUI->Enable(m_List.GetSelectedCount() ? TRUE : FALSE);
	}
}
void CSuperTagEditorView::OnSelectDeleteList() 
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_List.GetSelectedCount() == 1 && m_List.GetSelectFileName() == NULL && FALSE/* SeaKnows 033 */) {
		// フォルダの単一選択
	} else {
		// ファイルの単一選択／複数選択
		// 選択されているファイルを全て処理する
		CArray <int, const int &> arrayList;
		int		nCount;
		nCount = m_List.MakeSelectFileArray(arrayList);

		CString	strMess;
		strMess.Format(TEXT("選択されているファイル(%d個)をリストから削除します\n\n"
		               "実行してもよろしいですか？"), nCount);
		if (g_bConfDeleteList == false || MessageBox(strMess, TEXT("ファイルをリストから削除"), MB_YESNO|MB_TOPMOST) == IDYES) {
			// リストから削除(インデックスがずれるので、後ろから削除していく)
			bool	bFileModified = false;
			for (int i = nCount-1; i >= 0; i--) {
				int		nIndex = arrayList[i];
				int		nNumber = (int)m_List.GetLParamFromIndex(nIndex);
				if (nNumber >= 0) {
					FILE_MP3	*fileMP3 = pDoc->GetListMP3(nNumber);
					if (fileMP3->bModifyFlag == false) {
						m_List.UpdateParentAtDeleteItem(nIndex, fileMP3);
						CFileMP3::InitData(fileMP3);
						m_List.DeleteItemFromIndex(nIndex);
					} else if (bFileModified == false) {
						// 変更が保存されていないファイルがある
						MessageBox(TEXT("変更が保存されていないファイルが含まれています\n\n"
						           "変更が保存されていないファイルは無視します"),
						           TEXT("選択ファイルをリストから削除"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
						bFileModified = true;
					}
				}
			}
			// 全てのアイテムの選択解除
			m_List.SelectAllItems(FALSE);
			pDoc->UpdateAllViews(NULL);
			m_List.OnChangeSelect(); /* WildCherry4 083 */
		}
	}
}

// ファイルをごみ箱に移動する
void CSuperTagEditorView::OnUpdateSelectDeleteFile(CCmdUI* pCmdUI) 
{
	// OnUpdateSelectDeleteList() と同じ条件
	OnUpdateSelectDeleteList(pCmdUI);
}
void CSuperTagEditorView::OnSelectDeleteFile() 
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_List.GetSelectedCount() == 1 && m_List.GetSelectFileName() == NULL && FALSE/* SeaKnows 033 */) {
		// フォルダの単一選択
	} else {
		// ファイルの単一選択／複数選択
		// 選択されているファイルを全て処理する
		CArray <int, const int &> arrayList;
		int		nCount;
		nCount = m_List.MakeSelectFileArray(arrayList);

		CString	strMess;
		strMess.Format(TEXT("選択されているファイル(%d個)を削除します\n\n"
		               "実行してもよろしいですか？"), nCount);
		if (g_bConfDeleteFile == false || MessageBox(strMess, TEXT("ファイルの削除"), MB_YESNO|MB_TOPMOST) == IDYES) {
			/* WildCherry4 082 *//*
			// 削除ファイルのリストを作成
			CString	strFileList;
			// インデックスがずれるので、後ろから削除していく
			for (int i = nCount-1; i >= 0; i--) {
				int		nIndex = arrayList[i];
				int		nNumber = (int)m_List.GetLParamFromIndex(nIndex);
				if (nNumber >= 0) {
					FILE_MP3	*fileMP3 = pDoc->GetListMP3(nNumber);
					// ファイル名を '\n' で連結
					strFileList = fileMP3->strFullPathName;

					// 削除を行う
					if (DeleteFile(fileMP3->strFullPathName) != 0) {
						// 削除成功
						// アイテムの選択解除
						m_List.SetItemState(nIndex, 0, LVIS_SELECTED);
						// アイテムの削除
						m_List.UpdateParentAtDeleteItem(nIndex, fileMP3);
						CFileMP3::InitData(fileMP3);
						m_List.DeleteItemFromIndex(nIndex);
					} else {
						// 削除失敗
						CString	strBuffer;
						LPVOID lpMsgBuf;
						FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
									  FORMAT_MESSAGE_FROM_SYSTEM | 
									  FORMAT_MESSAGE_IGNORE_INSERTS,
									  NULL, GetLastError(),
									  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
									  (LPTSTR)&lpMsgBuf, 0, NULL);
						strBuffer.Format("%s\n\nファイル名：%s", (const char *)lpMsgBuf, fileMP3->strFullPathName);
						LocalFree(lpMsgBuf);
						MessageBox(strBuffer, "ファイルの削除ができませんでした", MB_ICONSTOP|MB_OK|MB_TOPMOST);
					}
				}
			}
			*/
			SHFILEOPSTRUCT sfo;
			ZeroMemory(&sfo, sizeof(sfo));
			sfo.hwnd = GetSafeHwnd();
			sfo.wFunc = FO_DELETE;
			sfo.fFlags = FOF_ALLOWUNDO;
			sfo.lpszProgressTitle = _T("削除しています...");
			CDoubleZeroString strFileList;
			int i; for (i = 0; i < nCount; i++) {
				int		nIndex = arrayList[i];
				int		nNumber = (int)m_List.GetLParamFromIndex(nIndex);
				if (nNumber >= 0) {
					FILE_MP3	*fileMP3 = pDoc->GetListMP3(nNumber);
					strFileList.Add(fileMP3->strFullPathName);
				}
			}
			sfo.pFrom = strFileList;
			if (::SHFileOperation(&sfo) != 0) {
				//MessageBox("ファイルの削除ができませんでした", "ファイルの削除ができませんでした", MB_ICONSTOP|MB_OK|MB_TOPMOST);
			}
			// リストから削除
			for (i = nCount-1; i >= 0; i--) {
				int		nIndex = arrayList[i];
				int		nNumber = (int)m_List.GetLParamFromIndex(nIndex);
				if (nNumber >= 0) {
					FILE_MP3	*fileMP3 = pDoc->GetListMP3(nNumber);
					if (GetFileAttributes(fileMP3->strFullPathName) == 0xFFFFFFFF) { // 削除された
						// アイテムの選択解除
						m_List.SetItemState(nIndex, 0, LVIS_SELECTED);
						// アイテムの削除
						m_List.UpdateParentAtDeleteItem(nIndex, fileMP3);
						CFileMP3::InitData(fileMP3);
						m_List.DeleteItemFromIndex(nIndex);
					}
				}
			}

			pDoc->UpdateAllViews(NULL);
			m_List.OnChangeSelect(); /* WildCherry4 083 */
		}
	}
}

void CSuperTagEditorView::OnUpdateDeleteTag(CCmdUI* pCmdUI) 
{
	// OnUpdateSelectDeleteList() と同じ条件
	OnUpdateSelectDeleteList(pCmdUI);
}

// 変更前の状態に戻す
void CSuperTagEditorView::OnUpdateSelectEditDestory(CCmdUI* pCmdUI) 
{
	// OnUpdateSelectDeleteList() と同じ条件
	OnUpdateSelectDeleteList(pCmdUI);
}
void CSuperTagEditorView::OnSelectEditDestory() 
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_List.GetSelectedCount() == 1 && m_List.GetSelectFileName() == NULL && FALSE/* SeaKnows 033 */) {
		// フォルダの単一選択
	} else {
		// ファイルの単一選択／複数選択
		// 選択されているファイルを全て処理する
		CArray <int, const int &> arrayList;
		int		nCount;
		nCount = m_List.MakeSelectFileArray(arrayList);
		if (nCount == 0)	return; /* SeaKnows 033 */

		CString	strMess;
		strMess.Format(TEXT("選択されているファイル(%d個)を編集前の状態に戻します\n\n"
		               "実行してもよろしいですか？"), nCount);
		if (g_bConfEditModify == false || MessageBox(strMess, TEXT("編集前の状態に戻す"), MB_YESNO|MB_TOPMOST) == IDYES) {
			// 変更されているファイルのタグ情報を読みなおす
			int i; for (i = 0; i < nCount; i++) {
				int		nIndex = arrayList[i];
				int		nNumber = (int)m_List.GetLParamFromIndex(nIndex);
				if (nNumber >= 0) {
					FILE_MP3	*fileMP3 = pDoc->GetListMP3(nNumber);
					CFileMP3	file;
					if (file.Attach(fileMP3->strFullPathName)) {
						file.InitData(fileMP3);
						*fileMP3 = file.m_fileMP3;
						m_List.UpdateFileStatusFromIndex(nIndex, fileMP3);
					}
				}
			}
			pDoc->UpdateAllViews(NULL);
		}
	}
}

// 全てのファイルをチェックする
void CSuperTagEditorView::OnUpdateAllFilesCheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnAllFilesCheck() 
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_List.SetCheckAllFiles(TRUE);
	pDoc->UpdateAllViews(NULL);
}

// 全てのファイルをチェックする
void CSuperTagEditorView::OnUpdateAllFilesUncheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnAllFilesUncheck() 
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_List.SetCheckAllFiles(FALSE);
	pDoc->UpdateAllViews(NULL);
}

// チェックの状態を反転する
void CSuperTagEditorView::OnUpdateReverseCheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorView::OnReverseCheck() 
{
	CSuperTagEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_List.ReverseCheckFiles();
	pDoc->UpdateAllViews(NULL);
}

void CSuperTagEditorView::ExecReplace(bool bReplace)
{
	CDlgReplaceWord	dialog;

	CHECK_WORD_STATE	*pState = &g_chkWord[CHECK_STATE_REPLACE];
	dialog.m_pList = &m_List;
	dialog.m_bModeReplace = bReplace ? TRUE : FALSE;
	dialog.m_bEnableRangeSelected	= (m_List.GetSelectedCount() >= 2) ? TRUE : FALSE;
	dialog.m_strSearchWord	= pState->strSearchWord;			// 文字列
	dialog.m_strReplaceWord	= pState->strReplaceWord;			// 文字列
	dialog.m_nTargetColumn	= (pState->nTargetColumn - COLUMN_FILE_NAME);	// 検索対象カラム
	dialog.m_bCheckDiffUL	= pState->bCheckDiffUL;				// 大文字／小文字の区別
	dialog.m_bRegExp		= pState->bRegExp;					// 正規表現
	dialog.m_bRangeSelected	= dialog.m_bEnableRangeSelected;	// 選択範囲のみ
	dialog.m_bMatchComplete	= pState->bMatchComplete;			// 完全に一致
	if (g_bSaveRepDlgPos) { /* WildCherry4 086 */
		dialog.m_rect.left = g_nSaveRepDlgPosX; 
		dialog.m_rect.top = g_nSaveRepDlgPosY;
	} else {
		dialog.m_rect.left = -1; 
		dialog.m_rect.top = -1;
	}
	if (dialog.DoModal() == IDOK) {
		CSuperTagEditorDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->UpdateAllViews(NULL);
	}
	g_nSaveRepDlgPosX = dialog.m_rect.left; /* WildCherry4 086 */
	g_nSaveRepDlgPosY = dialog.m_rect.top; /* WildCherry4 086 */
}

// 検索
void CSuperTagEditorView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_List.GetSelectedCount() ? TRUE : FALSE);
}

void CSuperTagEditorView::OnEditFind() 
{
	// 検索
	ExecReplace(false);
}

// 置換
void CSuperTagEditorView::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_List.GetSelectedCount() ? TRUE : FALSE);
}
void CSuperTagEditorView::OnEditReplace() 
{
	// 置換
	ExecReplace(true);
}

// カラム幅を調整
void CSuperTagEditorView::OnUpdateAdjustColumnWidth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnAdjustColumnWidth() 
{
	// カラム幅の自動調節
	m_List.AutoSizeColumns(-1);
}

// ヘルプ：目次
void CSuperTagEditorView::OnUpdateHelpIndex(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnHelpIndex() 
{
	CSuperTagEditorApp	*pApp = (CSuperTagEditorApp *)AfxGetApp();
	
	// HTML ヘルプファイル名作成
	auto sHelpName = pApp->MakeFileName(TEXT(".chm"));
	::HtmlHelp(GetSafeHwnd(), sHelpName.c_str(), HH_DISPLAY_TOPIC, (DWORD)NULL);
}

// 拡張ユーザー変換書式
void CSuperTagEditorView::OnUpdateConvFormatEx(CCmdUI* pCmdUI) 
{
	if (m_List.IsRangeSelected()) {
		// 範囲選択あり
		pCmdUI->Enable(TRUE);
	} else if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		pCmdUI->Enable(m_List.IsCurrentCellEditOK() ? TRUE : FALSE);
	}
}
void CSuperTagEditorView::OnConvFormatEx01() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[0]);
}
void CSuperTagEditorView::OnConvFormatEx02() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[1]);
}
void CSuperTagEditorView::OnConvFormatEx03() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[2]);
}
void CSuperTagEditorView::OnConvFormatEx04() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[3]);
}
void CSuperTagEditorView::OnConvFormatEx05() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[4]);
}
void CSuperTagEditorView::OnConvFormatEx06() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[5]);
}
void CSuperTagEditorView::OnConvFormatEx07() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[6]);
}
void CSuperTagEditorView::OnConvFormatEx08() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[7]);
}
void CSuperTagEditorView::OnConvFormatEx09() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[8]);
}
void CSuperTagEditorView::OnConvFormatEx10() 
{
	CWaitCursor	wait;
	m_List.ConvUserFormatEx(&g_userConvFormatEx[9]);
}

// 文字変換
void CSuperTagEditorView::OnUpdateConvStr(CCmdUI* pCmdUI) 
{
	if (m_List.IsRangeSelected()) {
		// 範囲選択あり
		pCmdUI->Enable(TRUE);
	} else if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		pCmdUI->Enable(m_List.IsCurrentCellEditOK() ? TRUE : FALSE);
	}
}
void CSuperTagEditorView::OnConvStrUpper() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_UPPER);
}
void CSuperTagEditorView::OnConvStrLower() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_LOWER);
}
void CSuperTagEditorView::OnConvStrHankaku() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HANKAKU);
}
void CSuperTagEditorView::OnConvStrHira() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HIRA);
}
void CSuperTagEditorView::OnConvStrKata() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_KATA);
}
void CSuperTagEditorView::OnConvStrHanAll() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HAN_ALL);
}
void CSuperTagEditorView::OnConvStrHanKigou() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HAN_KIGOU);
}
void CSuperTagEditorView::OnConvStrHanAlpha() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HAN_ALPHA);
}
void CSuperTagEditorView::OnConvStrHanKata() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HAN_KATA);
}
void CSuperTagEditorView::OnConvStrHanSuji() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HAN_SUJI);
}
void CSuperTagEditorView::OnConvStrZenAll() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_ZEN_ALL);
}
void CSuperTagEditorView::OnConvStrZenKigou() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_ZEN_KIGOU);
}
void CSuperTagEditorView::OnConvStrZenAlpha() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_ZEN_ALPHA);
}
void CSuperTagEditorView::OnConvStrZenKata() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_ZEN_KATA);
}
void CSuperTagEditorView::OnConvStrZenSuji() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_ZEN_SUJI);
}
void CSuperTagEditorView::OnConvStrToUpper() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_TO_UPPER);
}
void CSuperTagEditorView::OnConvStrToLower() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_TO_LOWER);
}
void CSuperTagEditorView::OnConvStrFirstUpper() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_FIRST_UPPER);
}
void CSuperTagEditorView::OnConvStrHira2kata() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_HIRA2KATA);
}
void CSuperTagEditorView::OnConvStrKata2hira() 
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_KATA2HIRA);
}

// STEのツリー表示に合わせて実際のフォルダ構成を同期させます\nフォルダ構成の同期
void CSuperTagEditorView::OnUpdateFolderTreeSync(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_bEnableFolderSync ? TRUE : FALSE);
}
void CSuperTagEditorView::OnFolderTreeSync() 
{
	static	const auto sMessage = TEXT("フォルダの同期処理を実行します\n\n実行してもよろしいですか？");
	if (g_bConfFolderSync == false || MessageBox(sMessage, TEXT("フォルダの同期"), MB_YESNO|MB_TOPMOST) == IDYES) {
		CWaitCursor	wait;
		if (g_bSyncSelectAlways) {
			// 実行時にフォルダ指定ダイアログを開く
			extern	BOOL SelectDirectory(LPTSTR);
			TCHAR	sFolderName[_MAX_PATH] = {'\0'};
			_tcscpy(sFolderName, g_strRootFolder);
			if (SelectDirectory(sFolderName) == FALSE) {
				return;
			}
			g_strRootFolder = sFolderName;
			// 分類済みフォルダの名称を更新する
			m_List.UpdateSyncFolderItemName();
		}
		m_List.ExecFolderTreeSync(g_strRootFolder, false);
		GetDocument()->UpdateAllFiles(true);
	}
}

void CSuperTagEditorView::OnUpdateCheckFileSync(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_bEnableFolderSync ? TRUE : FALSE);
}
void CSuperTagEditorView::OnCheckFileSync() 
{
	static	const auto sMessage = TEXT("フォルダの同期処理を実行します\n\n実行してもよろしいですか？");
	if (g_bConfFolderSync == false || MessageBox(sMessage, TEXT("フォルダの同期"), MB_YESNO|MB_TOPMOST) == IDYES) {
		CWaitCursor	wait;
		if (g_bSyncSelectAlways) {
			// 実行時にフォルダ指定ダイアログを開く
			extern	BOOL SelectDirectory(LPTSTR);
			TCHAR	sFolderName[_MAX_PATH] = {'\0'};
			_tcscpy(sFolderName, g_strRootFolder);
			if (SelectDirectory(sFolderName) == FALSE) {
				return;
			}
			g_strRootFolder = sFolderName;
			// 分類済みフォルダの名称を更新する
			m_List.UpdateSyncFolderItemName();
		}
		m_List.ExecFolderTreeSync(g_strRootFolder, true);
		GetDocument()->UpdateAllFiles(true);
	}
}

void CSuperTagEditorView::PlayFile(int nNumber)
{
	OnWinampPlay();
}

bool CSuperTagEditorView::LoadFormatFile(LPCTSTR sFileName, CString *strHead, CString *strBody, CString *strFoot)
{
	strHead->Empty();
	strBody->Empty();
	strFoot->Empty();
	TRY {
		CFile	file;
		if (file.Open(sFileName, CFile::modeRead|CFile::shareDenyNone)) {
			enum	{TYPE_HEAD, TYPE_BODY, TYPE_FOOT};
			CString		strLine;
			CArchive	ar(&file, CArchive::load);
			int		nType = TYPE_HEAD;
			while(ar.ReadString(strLine)) {
				strLine += "\r\n";
				switch(nType) {
				case TYPE_HEAD:		// ヘッダ部分
					if (_tcsncmp(strLine, _T("[LOOP_START]"), 12) == 0) {
						nType = TYPE_BODY;
					} else {
						*strHead += strLine;
					}
					break;
				case TYPE_BODY:		// ボディー部分
					if (_tcsncmp(strLine, _T("[LOOP_END]"), 10) == 0) {
						nType = TYPE_FOOT;
					} else {
						*strBody += strLine;
					}
					break;
				case TYPE_FOOT:		// フッタ部分
					*strFoot += strLine;
					break;
				}
			}
		}
	}
	CATCH( CFileException, e) {
		CString	str;
		str.Format(TEXT("%s の読み込みに失敗しました"), sFileName);
		MessageBox(str, TEXT("ファイルエラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	END_CATCH
	
	return(true);
}

bool CSuperTagEditorView::ExecWriteList(WRITE_FORMAT *pFormat)
{
	CString	strHead, strBody, strFoot;
	if (pFormat->strFileName.IsEmpty()) {
		MessageBox(TEXT("書式ファイルが指定されていません"), TEXT("リスト出力エラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
		return(false);
	}
	// 書式ファイルの読み込み
	if (LoadFormatFile(pFormat->strFileName, &strHead, &strBody, &strFoot)) {
		// ファイル選択ダイアログを開く
		CString	strFilter;
		strFilter.Format(TEXT("%s(*%s)|*%s|All Files(*.*)|*.*|"), pFormat->strName, pFormat->strExtName, pFormat->strExtName);
		CMyFileDialog	dialog(FALSE, pFormat->strExtName, pFormat->strCurrentFile,
							   OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							   strFilter, NULL);
		if (dialog.DoModal() == IDOK) {
			CWaitCursor	wait;
			pFormat->strCurrentFile = dialog.GetPathName();

			// リストファイル出力
			bool	bIsAppend = dialog.m_bIsWriteAppend ? true : false;
			return(m_List.WriteFormatFile(pFormat->strCurrentFile, strHead, strBody, strFoot, pFormat->bWriteSelected, pFormat->bIsHtml, bIsAppend, pFormat->bWriteHtml));
		}
	}
	return(false);
}

void CSuperTagEditorView::OnUpdateWriteList(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnWriteList1() 
{
	CWaitCursor	wait;
	ExecWriteList(&g_writeFormat[0]);
}
void CSuperTagEditorView::OnWriteList2() 
{
	CWaitCursor	wait;
	ExecWriteList(&g_writeFormat[1]);
}
void CSuperTagEditorView::OnWriteList3() 
{
	CWaitCursor	wait;
	ExecWriteList(&g_writeFormat[2]);
}
void CSuperTagEditorView::OnWriteList4() 
{
	CWaitCursor	wait;
	ExecWriteList(&g_writeFormat[3]);
}
void CSuperTagEditorView::OnWriteList5() 
{
	CWaitCursor	wait;
	ExecWriteList(&g_writeFormat[4]);
}

void CSuperTagEditorView::OnUpdateSetListFont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
void CSuperTagEditorView::OnSetListFont() 
{
	CFontDialog	dialog;
	if (g_fontReport.lfFaceName[0] != '\0') {
		// 現在のフォントで初期化する
		dialog.m_cf.lpLogFont = &g_fontReport;
		dialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	}

	// 文字装飾の指定を使用不可にする
	dialog.m_cf.Flags &= ~CF_EFFECTS;
	if (dialog.DoModal() == IDOK) {
		g_fontReport = *dialog.m_cf.lpLogFont;
		// フォントの設定変更を反映させる
		UpdateFontListView();
	}
}

//static
HDDEDATA CALLBACK CSuperTagEditorView::DdemlCallback(UINT uType, UINT uFmt,
    HCONV hConv, HSZ hszTpc1, HSZ hszTpc2, HDDEDATA hData,
    DWORD dwData1, DWORD dwData2)
{
    if(!g_DdeServer)return NULL;
    TCHAR szBuffer[512];
    switch (uType) {
        case XTYP_CONNECT:
            //hszTpc2:サービス名 hszTpc1:トピック名
            g_DdeServer->QueryString(hszTpc2, szBuffer, sizeof(szBuffer));
            if(lstrcmp(szBuffer, DDE_SERVICE_NAME) != 0)
                return NULL;
            g_DdeServer->QueryString(hszTpc1, szBuffer, sizeof(szBuffer));
            if(lstrcmp(szBuffer, DDE_TOPIC_NAME) != 0)
                return NULL;
            return (HDDEDATA)TRUE;
        case XTYP_REQUEST:
            //hszTpc2:項目名 hsz1:トピック名
            return NULL;
        case XTYP_POKE:
            return NULL;
        case XTYP_EXECUTE:{
            TCHAR sz[2048];
            sz[0] = 0;
            DdeGetData(hData, (LPBYTE)sz, 2048, 0);
            g_SteView->OnDDE(sz);
            return (HDDEDATA)DDE_FACK;
        }
        default:
            return NULL;
    }
}

void   CSuperTagEditorView::OnDDE(LPTSTR sFileName)
{   //多重起動でファイル名が渡されたときの処理
    //ほとんど CSuperTagEditorView::OnDropFiles をコピペしただけ(^^ゞ

	CSuperTagEditorDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if(pDoc->IsTagUpdating()){
    //タグを更新中は何もしない
        return;
    }

    //実際は sFileName は "" で括られているので、取り除く必要がある
    //（KbDDEClient::Execute の仕様）
	std::vector<TCHAR> tmpFileName(lstrlen(sFileName));
    lstrcpyn(&tmpFileName[0], sFileName+1, lstrlen(sFileName)-2);

    int		nFileCount = 1;

	CWaitCursor	wait;

	// ファイルが１個の場合は、プレイリストの読み込みに対応
	if (nFileCount == 1) {
		CString strFileName = tmpFileName.data();
        strFileName.MakeLower();
		if (strFileName.Find(TEXT(".m3u")) != -1) {
			// プレイリスト追加
			LoadPlayList(tmpFileName.data());
			return;
		}
	}

	// プログレスバー初期化
	pDoc->StartLoadFile(TEXT("ＭＰ３ファイル読み込み中..."));

	// ドロップされたファイルを処理する
	int i; for (i = 0; i < nFileCount; i++) {
		CFileStatus	status;
		if (CFile::GetStatus(tmpFileName.data(), status) == TRUE && status.m_mtime != -1) {
			if (status.m_attribute & CFile::directory) {
				// ディレクトリの場合
				pDoc->OpenFolder(tmpFileName.data());
			} else {
				// ファイルの場合
				pDoc->AddRequestFile(tmpFileName.data(), NULL);
			}
		} else if (lstrlen(tmpFileName.data()) == 3 && tmpFileName[1] == ':' && tmpFileName[2] == '\\') { /* STEP 027 */
				// ドライブの場合
				pDoc->OpenFolder(tmpFileName.data());
		}
	}

	// 追加リクエストのあったファイルのタグ情報を読み込む
#ifdef FLICKERFREE
	//m_List.SetRedraw(FALSE);
#endif
	pDoc->ExecRequestFiles(true, g_bOptLoadFileChecked);
#ifdef FLICKERFREE
	//m_List.SetRedraw(TRUE);
#endif

	// プログレスバー終了
	pDoc->EndLoadFile();

	pDoc->UpdateAllViews(NULL);
}

void CSuperTagEditorView::OnDeleteChar() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DlgDeleteChar dialog;
	dialog.m_nPos = 0;
	if (dialog.DoModal() == IDOK) {
		CWaitCursor	wait;
		m_List.DeleteChar(dialog.m_nDelCount, dialog.m_nPos);	
	}
}

void CSuperTagEditorView::OnUpdateDeleteChar(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (m_List.IsRangeSelected()) {
		// 範囲選択あり
		pCmdUI->Enable(TRUE);
	} else if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		pCmdUI->Enable(m_List.IsCurrentCellEditOK() ? TRUE : FALSE);
	}	
}

void CSuperTagEditorView::OnUpdateSetNumberAdd(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (m_List.IsRangeSelected() == false) {
		// 範囲選択無し
		pCmdUI->Enable(FALSE);
	} else {
		// 範囲選択あり
		pCmdUI->Enable(((m_List.GetRangeSelect())[0].y == (m_List.GetRangeSelect())[1].y) ? FALSE : TRUE);
	}	
}

void CSuperTagEditorView::OnSetNumberAdd() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DlgAddNumber dialog;
	CWinApp	*pApp = AfxGetApp();
	dialog.m_nInitNumber = 1; /* Conspiracy 194 */
	dialog.m_nAddNumber = 1; /* Conspiracy 194 */
	dialog.m_nWidth = g_nAddNumberWidth; /* Baja 159 */
	dialog.m_nAddPosition = g_nAddNumberPos; /* Baja 159 */
	dialog.m_strSeparator = g_strAddNumberSep; /* Baja 159 */
	dialog.m_strBefore = g_strAddNumberBef; /* Conspiracy 194 */
	dialog.m_strAfter = g_strAddNumberAft; /* Conspiracy 194 */
	if (dialog.DoModal() == IDOK) {
		CWaitCursor	wait;
		if (dialog.m_nAddPosition != 2) { /* Conspiracy 194 */
			m_List.AddFillNumber(dialog.m_nInitNumber/* Conspiracy 194 */, dialog.m_nAddNumber/* Conspiracy 194 */, dialog.m_nWidth, dialog.m_strSeparator, "" /* Conspiracy 194 */, dialog.m_nAddPosition, dialog.m_bSpaceInitNumber ? true : false/* Conspiracy 194 */);
			g_strAddNumberSep = dialog.m_strSeparator; /* Baja 159 */
		} else { /* Conspiracy 194 */
			// 置き換えの時
			//m_List.SetFillNumber(dialog.m_nInitNumber, dialog.m_nAddNumber, dialog.m_bSpaceInitNumber ? true : false);
			m_List.AddFillNumber(dialog.m_nInitNumber, dialog.m_nAddNumber, dialog.m_nWidth, dialog.m_strBefore, dialog.m_strAfter, dialog.m_nAddPosition, dialog.m_bSpaceInitNumber ? true : false);
			g_strAddNumberBef = dialog.m_strBefore;
			g_strAddNumberAft = dialog.m_strAfter;
		}
		g_nAddNumberWidth = dialog.m_nWidth; /* Baja 159 *//* Conspiracy 194 */
		g_nAddNumberPos = dialog.m_nAddPosition; /* Baja 159 *//* Conspiracy 194 */
	}
}

void CSuperTagEditorView::OnUpdateMoveFolder(CCmdUI* pCmdUI) 
{
	if (m_List.GetSelectedCount() == 1 && m_List.GetSelectFileName() == NULL) {
		// フォルダの単一選択
		pCmdUI->Enable(TRUE); /* SeaKnows 038 */
		return;
	} else {
		// ファイルの単一選択／複数選択
		pCmdUI->Enable(m_List.GetSelectedCount() ? TRUE : FALSE);
		return;
	}
	pCmdUI->Enable(FALSE);
}

void CSuperTagEditorView::OnMoveFolder01() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DoMoveFolder(0); /* STEP 022 */
}

void CSuperTagEditorView::OnMoveFolder02() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DoMoveFolder(1); /* STEP 022 */
}

void CSuperTagEditorView::OnMoveFolder03() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DoMoveFolder(2); /* STEP 022 */
}

void CSuperTagEditorView::OnMoveFolder04() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DoMoveFolder(3); /* STEP 022 */
}

void CSuperTagEditorView::OnMoveFolder05() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DoMoveFolder(4); /* STEP 022 */
}

BOOL CSuperTagEditorView::SelectDirectory(LPTSTR sLocal, bool bCopy)
{
	bool	bResult;
	CSHBrowseForFolder	browse(true, /*g_bEnableMoveFolderCopy*/bCopy /* WildCherry 064 */);
	browse.SetCheckBoxTitle(TEXT("コピーする"));
	bResult = browse.Exec(sLocal);
	g_bEnableMoveFolderCopy = browse.GetSearchSubDirState();
	return(bResult);
}

BOOL CSuperTagEditorView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
    NMHDR* hdr = (NMHDR*)lParam;
	if (hdr->idFrom != IDR_MAINFRAME) {
		if (hdr->code == TTN_NEEDTEXT) {
			m_List.SendMessage(WM_NOTIFY, (WPARAM) hdr->idFrom, (LPARAM)lParam);
			return TRUE;
		}
		return CView::OnNotify(wParam, lParam, pResult);
	}
	switch (hdr->code) {
	case 2222:
		m_List.SendMessage(WM_NOTIFY, (WPARAM) hdr->idFrom, (LPARAM)lParam);
		return TRUE;
		break;
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

void CSuperTagEditorView::OnSelectTreeColum() /* TyphoonSwell 025 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.SelectTreeColumn();
}

void CSuperTagEditorView::OnUpdateSelectTreeColum(CCmdUI* pCmdUI) /* TyphoonSwell 025 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	int nIndex = m_List.GetSelectedItem();
	if (nIndex < 0) {
		pCmdUI->Enable(FALSE);
		return;
	}

//	if (m_List.ItemHasChildren(m_List.GetTreeItem(nIndex)) == FALSE) {
//		pCmdUI->Enable(FALSE);
//		return;
//	}
	/* SeaKnows 035 *//*
	m_List.SelectRangeStart();
	if (m_List.m_posMultiSelect[0].x == 0) return;*/
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorView::OnConvFormatUser01() /* TyphoonSwell 027 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	g_nUserConvFormatType = 0;
	CMainFrame *pMainFrm;
	pMainFrm=((CMainFrame*)AfxGetMainWnd());
	pMainFrm->UpdateToolBar();
}

void CSuperTagEditorView::OnUpdateConvFormatUser01(CCmdUI* pCmdUI) /* TyphoonSwell 027 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_nUserConvFormatType == 0 ? TRUE : FALSE);
}

void CSuperTagEditorView::OnConvFormatUser02() /* TyphoonSwell 027 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	g_nUserConvFormatType = 1;	
	CMainFrame *pMainFrm;
	pMainFrm=((CMainFrame*)AfxGetMainWnd());
	pMainFrm->UpdateToolBar();
}

void CSuperTagEditorView::OnUpdateConvFormatUser02(CCmdUI* pCmdUI) /* TyphoonSwell 027 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_nUserConvFormatType == 1 ? TRUE : FALSE);
}

void CSuperTagEditorView::OnConvFormatUser03() /* TyphoonSwell 027 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	g_nUserConvFormatType = 2;
	CMainFrame *pMainFrm;
	pMainFrm=((CMainFrame*)AfxGetMainWnd());
	pMainFrm->UpdateToolBar();
}

void CSuperTagEditorView::OnUpdateConvFormatUser03(CCmdUI* pCmdUI) /* TyphoonSwell 027 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_nUserConvFormatType == 2 ? TRUE : FALSE);
}

void CSuperTagEditorView::OnConvFormatUser04() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	g_nUserConvFormatType = 3;
	CMainFrame *pMainFrm;
	pMainFrm=((CMainFrame*)AfxGetMainWnd());
	pMainFrm->UpdateToolBar();	
}

void CSuperTagEditorView::OnUpdateConvFormatUser04(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_nUserConvFormatType == 3 ? TRUE : FALSE);	
}

void CSuperTagEditorView::OnConvFormatUser05() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	g_nUserConvFormatType = 4;
	CMainFrame *pMainFrm;
	pMainFrm=((CMainFrame*)AfxGetMainWnd());
	pMainFrm->UpdateToolBar();
}

void CSuperTagEditorView::OnUpdateConvFormatUser05(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(g_nUserConvFormatType == 4 ? TRUE : FALSE);
}

void CSuperTagEditorView::OnUpdateTeikei01(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 0);
}

void CSuperTagEditorView::OnTeikei01() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(0);
}

void CSuperTagEditorView::OnUpdateTeikei(CCmdUI *pCmdUI) /* SeaKnows 030 */
{
	if (m_List.IsRangeSelected()) {
		// 範囲選択あり
		pCmdUI->Enable(TRUE);
	} else if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		pCmdUI->Enable(m_List.IsCurrentCellEditOK() ? TRUE : FALSE);
	}
}

void CSuperTagEditorView::OnTeikei02() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(1);
}

void CSuperTagEditorView::OnUpdateTeikei02(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 1);
}

void CSuperTagEditorView::OnTeikei03() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(2);
}

void CSuperTagEditorView::OnUpdateTeikei03(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 2);
}

void CSuperTagEditorView::OnTeikei04() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(3);
}

void CSuperTagEditorView::OnUpdateTeikei04(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 3);
}

void CSuperTagEditorView::OnTeikei05() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(4);
}

void CSuperTagEditorView::OnUpdateTeikei05(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 4);
}

void CSuperTagEditorView::OnTeikei06() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(5);
}

void CSuperTagEditorView::OnUpdateTeikei06(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 5);
}

void CSuperTagEditorView::OnTeikei07() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(6);
}

void CSuperTagEditorView::OnUpdateTeikei07(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 6);
}

void CSuperTagEditorView::OnTeikei08() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(7);
}

void CSuperTagEditorView::OnUpdateTeikei08(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 7);
}

void CSuperTagEditorView::OnTeikei09() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(8);
}

void CSuperTagEditorView::OnUpdateTeikei09(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 8);
}

void CSuperTagEditorView::OnTeikei10() /* SeaKnows 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(9);
}

void CSuperTagEditorView::OnUpdateTeikei10(CCmdUI* pCmdUI) /* SeaKnows 030 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 9);
}

void CSuperTagEditorView::OnCheckFilenameMax() /* SeaKnows 037 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.CheckFileNameMax();
}

void CSuperTagEditorView::OnUpdateCheckFilenameMax(CCmdUI* pCmdUI) /* SeaKnows 037 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (g_bConfFileNameMaxCheck) {
		int nIndex = m_List.GetSelectedItem();
		if (nIndex < 0) {
			pCmdUI->Enable(FALSE);
			return;
		}
		pCmdUI->Enable(TRUE);
	} else {
		pCmdUI->Enable(FALSE);
	}
}

void CSuperTagEditorView::OnConvFormatUser() /* AstralCircle 041 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	COptionSheet prop;
	CDlgUserConvFormat	pageConv;
	COptionListBox	listBox;

	prop.SetListControl(&listBox);
	// プロパティーシートのスタイルを変更
	prop.m_psh.dwFlags |= PSH_NOAPPLYNOW;	// [適用]ボタン無し

	pageConv.m_nFormatType	= g_nUserConvFormatType;
	int i; for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
		pageConv.m_userFormat[i] = g_userConvFormat[i];
	}
	prop.SetTitle(TEXT("ユーザ変換書式設定"));
	prop.AddGroup(&pageConv);
	if (prop.DoModal() == IDOK) {
		g_nUserConvFormatType	= pageConv.m_nFormatType;
		for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
			g_userConvFormat[i] = pageConv.m_userFormat[i];
		}
		CMainFrame *pMainFrm;
		pMainFrm=((CMainFrame*)AfxGetMainWnd());
		pMainFrm->UpdateToolBar();
		((CSuperTagEditorApp *)AfxGetApp())->UpdateAccelerator();
	}
}

void CSuperTagEditorView::OnUpdateConvFormatUser(CCmdUI* pCmdUI) /* AstralCircle 041 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorView::OnTeikei_ShowSubMenu(){
	((CMainFrame*)AfxGetMainWnd())->ToolbarDropDown(ID_TEIKEI_TOOL);
	//this->GetParent()->ToolbarDropDown(ID_TEIKEI_TOOL);
}

void CSuperTagEditorView::OnTeikeiConfig() /* AstralCircle 041 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CPropertySheet prop;
	CDlgTeikei	pageTeikei[3];

	for (int k=0;k<3;k++) {
		pageTeikei[k].m_nGroupNumber = k+1;
		pageTeikei[k].m_strGroupName = g_strTeikeiGroupName[k];
		for (int i=0;i<10;i++) { /* STEP 035 */
			pageTeikei[k].m_teikeiInfo[i] = g_teikeiInfo[i+k*10];
		}
	}
	prop.SetTitle(TEXT("定型文設定"));
	prop.AddPage(&pageTeikei[0]);
	prop.AddPage(&pageTeikei[1]);
	prop.AddPage(&pageTeikei[2]);
	
	if (prop.DoModal() == IDOK) {
		for (int k=0;k<3;k++) {
			g_strTeikeiGroupName[k] = pageTeikei[k].m_strGroupName;
			for (int i=0;i<10;i++) { /* STEP 035 */
				g_teikeiInfo[i+k*10] = pageTeikei[k].m_teikeiInfo[i];
			}
		}
		((CSuperTagEditorApp *)AfxGetApp())->UpdateAccelerator();
	}
}

void CSuperTagEditorView::OnUpdateDlgTeikei(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);		
}

void CSuperTagEditorView::OnTeikei(int nIndex)
{
	CDlgTeikeiPaste dlgTeikei;
	/* STEP 035 */
	dlgTeikei.m_nTeikeiPaste = g_teikeiInfo[nIndex].nTeikeiPaste;
	dlgTeikei.m_bAddSpace = g_teikeiInfo[nIndex].bAddSpace;
	dlgTeikei.m_bAddChar = g_teikeiInfo[nIndex].bAddChar;
	dlgTeikei.m_strFront = g_teikeiInfo[nIndex].strFront;
	dlgTeikei.m_strBack = g_teikeiInfo[nIndex].strBack;
	dlgTeikei.m_bShowDialog = g_teikeiInfo[nIndex].bShowDialog;
	if (!g_teikeiInfo[nIndex].bShowDialog/* STEP 035 */ || dlgTeikei.DoModal() == IDOK) { /* FreeFall 052 */
		CWaitCursor	wait;
		/* STEP 035 */
		g_teikeiInfo[nIndex].nTeikeiPaste = dlgTeikei.m_nTeikeiPaste;
		g_teikeiInfo[nIndex].bAddSpace = dlgTeikei.m_bAddSpace;
		g_teikeiInfo[nIndex].bAddChar = dlgTeikei.m_bAddChar;
		g_teikeiInfo[nIndex].strFront = dlgTeikei.m_strFront;
		g_teikeiInfo[nIndex].strBack = dlgTeikei.m_strBack;
		g_teikeiInfo[nIndex].bShowDialog = dlgTeikei.m_bShowDialog;

		CString strPaste = g_teikeiInfo[nIndex].strTeikei;
		if (dlgTeikei.m_bAddChar == FALSE) { /* FunnyCorn 187 */
			dlgTeikei.m_strFront = "";
			dlgTeikei.m_strBack = "";
		}
		strPaste = dlgTeikei.m_strFront + strPaste + dlgTeikei.m_strBack; /* FunnyCorn 187 */
		switch (dlgTeikei.m_nTeikeiPaste) {
		case 0:
			break;
		case 1:
			strPaste = strPaste + (dlgTeikei.m_bAddSpace ? " " : "");
			break;
		case 2:
			strPaste = (dlgTeikei.m_bAddSpace ? " " : "") + strPaste;
			break;
		}
#ifdef FLICKERFREE
		//m_List.SetRedraw(FALSE);
#endif
		m_List.PasteString(strPaste.GetBuffer(0), dlgTeikei.m_nTeikeiPaste);
		strPaste.ReleaseBuffer();
		//g_strTeikei[nIndex].ReleaseBuffer();
		GetDocument()->UpdateAllViews(NULL);
#ifdef FLICKERFREE
		//m_List.SetRedraw(TRUE);
#endif
	}
}

void CSuperTagEditorView::OnUpdateTeikei(CCmdUI *pCmdUI, int nIndex)
{
	if (g_teikeiInfo[nIndex].strTeikei.IsEmpty()) {
		pCmdUI->Enable(FALSE);
	} else {
		OnUpdateTeikei(pCmdUI);
	}
}

void CSuperTagEditorView::OnTeikei201() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(10);
}

void CSuperTagEditorView::OnUpdateTeikei201(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 10);
}

void CSuperTagEditorView::OnTeikei202() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(11);	
}

void CSuperTagEditorView::OnUpdateTeikei202(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 11);
}

void CSuperTagEditorView::OnTeikei203() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(12);
}

void CSuperTagEditorView::OnUpdateTeikei203(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 12);
}

void CSuperTagEditorView::OnTeikei204() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(13);
}

void CSuperTagEditorView::OnUpdateTeikei204(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 13);
}

void CSuperTagEditorView::OnTeikei205() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(14);
}

void CSuperTagEditorView::OnUpdateTeikei205(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 14);
}

void CSuperTagEditorView::OnTeikei206() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(15);
}

void CSuperTagEditorView::OnUpdateTeikei206(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 15);
}

void CSuperTagEditorView::OnTeikei207() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(16);
}

void CSuperTagEditorView::OnUpdateTeikei207(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 16);
}

void CSuperTagEditorView::OnTeikei208() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(17);
}

void CSuperTagEditorView::OnUpdateTeikei208(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 17);
}

void CSuperTagEditorView::OnTeikei209() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(18);
}

void CSuperTagEditorView::OnUpdateTeikei209(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 18);
}

void CSuperTagEditorView::OnTeikei210() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(19);
}

void CSuperTagEditorView::OnUpdateTeikei210(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 19);
}

void CSuperTagEditorView::OnTeikei301() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(20);
}

void CSuperTagEditorView::OnUpdateTeikei301(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 20);
}

void CSuperTagEditorView::OnTeikei302() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(21);
}

void CSuperTagEditorView::OnUpdateTeikei302(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 21);
}

void CSuperTagEditorView::OnTeikei303() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(22);
}

void CSuperTagEditorView::OnUpdateTeikei303(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 22);
}

void CSuperTagEditorView::OnTeikei304() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(23);
}

void CSuperTagEditorView::OnUpdateTeikei304(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 23);
}

void CSuperTagEditorView::OnTeikei305() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(24);
}

void CSuperTagEditorView::OnUpdateTeikei305(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 24);
}

void CSuperTagEditorView::OnTeikei306() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(25);
}

void CSuperTagEditorView::OnUpdateTeikei306(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 25);
}

void CSuperTagEditorView::OnTeikei307() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(26);
}

void CSuperTagEditorView::OnUpdateTeikei307(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 26);
}

void CSuperTagEditorView::OnTeikei308() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(27);
}

void CSuperTagEditorView::OnUpdateTeikei308(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 27);
}

void CSuperTagEditorView::OnTeikei309() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(28);
}

void CSuperTagEditorView::OnUpdateTeikei309(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 28);
}

void CSuperTagEditorView::OnTeikei310() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnTeikei(29);
}

void CSuperTagEditorView::OnUpdateTeikei310(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateTeikei(pCmdUI, 29);
}

void CSuperTagEditorView::OnUnifyChar() /* StartInaction 054 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CDlgUnifyChar dlgUnify;

	dlgUnify.m_nConvAlpha = g_nUnifyAlpha;
	dlgUnify.m_nConvHiraKata = g_nUnifyHiraKata;
	dlgUnify.m_nConvKata = g_nUnifyKata;
	dlgUnify.m_nConvKigou = g_nUnifyKigou;
	dlgUnify.m_nConvSuji = g_nUnifySuji;
	dlgUnify.m_nConvUpLow = g_nUnifyUpLow;
	dlgUnify.m_nConvFixedUpLow = g_nUnifyFixedUpLow; /* STEP 040 */
	if (dlgUnify.DoModal() == IDOK) {
#ifdef FLICKERFREE
		//m_List.SetRedraw(FALSE);
#endif
		CWaitCursor	wait;
		g_nUnifyAlpha = dlgUnify.m_nConvAlpha;
		g_nUnifyHiraKata = dlgUnify.m_nConvHiraKata;
		g_nUnifyKata = dlgUnify.m_nConvKata;
		g_nUnifyKigou = dlgUnify.m_nConvKigou;
		g_nUnifySuji = dlgUnify.m_nConvSuji;
		g_nUnifyUpLow = dlgUnify.m_nConvUpLow;
		g_nUnifyFixedUpLow = dlgUnify.m_nConvFixedUpLow; /* STEP 040 */
		switch (g_nUnifyHiraKata) { // 平仮名片仮名
		case 1:// ひらがな
			m_List.ConvString(CONV_STR_KATA2HIRA);
			break;
		case 2:// カタカナ
			m_List.ConvString(CONV_STR_HIRA2KATA);
			break;
		}
		switch (g_nUnifyKigou /* BeachMonster 103 */) { // 記号
		case 1:// 全角
			m_List.ConvString(CONV_STR_ZEN_KIGOU);
			break;
		case 2:// 半角
			m_List.ConvString(CONV_STR_HAN_KIGOU);
			break;
		}
		switch (g_nUnifySuji) { // 数字
		case 1:// 全角
			m_List.ConvString(CONV_STR_ZEN_SUJI);
			break;
		case 2:// 半角
			m_List.ConvString(CONV_STR_HAN_SUJI);
			break;
		}
		switch (g_nUnifyKata) { // カタカナ
		case 1:// 全角
			m_List.ConvString(CONV_STR_ZEN_KATA);
			break;
		case 2:// 半角
			m_List.ConvString(CONV_STR_HAN_KATA);
			break;
		}
		switch (g_nUnifyAlpha) { // アルファベット
		case 1:// 全角
			m_List.ConvString(CONV_STR_ZEN_ALPHA);
			break;
		case 2:// 半角
			m_List.ConvString(CONV_STR_HAN_ALPHA);
			break;
		}
		switch (g_nUnifyUpLow) { // 大文字小文字
		case 1:// 大文字
			m_List.ConvString(CONV_STR_TO_UPPER);
			break;
		case 2:// 小文字
			m_List.ConvString(CONV_STR_TO_LOWER);
			break;
		case 3:// 単語の１文字目のみ大文字
			m_List.ConvString(CONV_STR_FIRST_UPPER);
			break;
		}
		switch (g_nUnifyFixedUpLow) { // 大文字小文字固定 /* STEP 040 */
		case 1:
			m_List.ConvString(CONV_STR_FIXED_UP_LOW);
			break;
		}
#ifdef FLICKERFREE
		//m_List.SetRedraw(TRUE);
#endif
	}
}

void CSuperTagEditorView::OnUpdateUnifyChar(CCmdUI* pCmdUI) /* StartInaction 054 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (m_List.IsRangeSelected()) {
		// 範囲選択あり
		pCmdUI->Enable(TRUE);
	} else if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		pCmdUI->Enable(m_List.IsCurrentCellEditOK() ? TRUE : FALSE);
	}
}

void CSuperTagEditorView::OnEndEditRight() /* BeachMonster 091 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	g_bEndEditMoveRightCell = g_bEndEditMoveRightCell ? false : true;
}

void CSuperTagEditorView::OnUpdateEndEditRight(CCmdUI* pCmdUI) /* BeachMonster 091 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (g_bOptEditOkDown == false) {
		pCmdUI->Enable(FALSE);
	} else {
		pCmdUI->SetCheck(g_bEndEditMoveRightCell ? TRUE : FALSE);
		pCmdUI->Enable(TRUE);
	}
}

void CSuperTagEditorView::OnDlgFavorites() /* RockDance 129 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CDlgFavorites dlg;
	for (int i=0;i<10;i++) {
		dlg.m_strFavirites[i] = g_strFavorite[i];
	}
	if (dlg.DoModal() == IDOK) {
		for (int i=0;i<10;i++) {
			g_strFavorite[i] = dlg.m_strFavirites[i];
		}
		((CSuperTagEditorApp *)AfxGetApp())->UpdateAccelerator(); // メニューの更新のために呼ぶ
	}
}

void CSuperTagEditorView::OnUpdateDlgFavorites(CCmdUI* pCmdUI) /* RockDance 129 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);	
}

void CSuperTagEditorView::OnFavorites() /* RockDance 129 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnDlgFavorites();
}

void CSuperTagEditorView::OnUpdateFavorites(CCmdUI* pCmdUI) /* RockDance 129 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);	
}

void CSuperTagEditorView::OnCalcFolderTotal()  /* RockDance 128 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.CalcSum(m_List.GetSelectedItem());	
}

void CSuperTagEditorView::OnUpdateCalcFolderTotal(CCmdUI* pCmdUI) /* RockDance 128 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (m_List.IsRangeSelected() || g_bOptShowTotalParent) {
		// 範囲選択あり
		pCmdUI->Enable(FALSE);
	} else if (m_List.GetSelectedCount() != 1) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		// 変更可能な項目か？
		pCmdUI->Enable(TRUE);
	}
	
}

void CSuperTagEditorView::OnUpdateFileConvAutoId3(CCmdUI* pCmdUI) /* RockDance 136 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable((m_List.GetSelectedItem() < 0) ? FALSE : TRUE);
}

void CSuperTagEditorView::OnEditPasteAdd()  /* Baja 171 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CDlgTeikeiPaste dlgTeikei;
	dlgTeikei.m_nTeikeiPaste = 0;
	dlgTeikei.m_bAddSpace = TRUE;
	dlgTeikei.m_strTitle = "追加で貼り付け";
	if (dlgTeikei.DoModal() == IDOK) { /* FreeFall 052 */
		CWaitCursor	wait;
		if (dlgTeikei.m_bAddChar == FALSE) { /* FunnyCorn 187 */
			dlgTeikei.m_strFront = "";
			dlgTeikei.m_strBack = "";
		}
		m_List.ClipboardPaste(dlgTeikei.m_nTeikeiPaste, dlgTeikei.m_bAddSpace ? true : false,
			dlgTeikei.m_strFront, dlgTeikei.m_strBack/* FunnyCorn 187 */);
#ifndef FLICKERFREE
		GetDocument()->UpdateAllViews(NULL);
#endif
	}
}

void CSuperTagEditorView::OnUpdateEditPasteAdd(CCmdUI* pCmdUI) /* Baja 171 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateEditCopy(pCmdUI);	
}

void CSuperTagEditorView::OnEditCopyFormat01() /* FunnyCorn 175 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.ClipboardCopyFormat(&g_userCopyFormat[0]);
}

void CSuperTagEditorView::OnEditCopyFormat02() /* FunnyCorn 175 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.ClipboardCopyFormat(&g_userCopyFormat[1]);	
}

void CSuperTagEditorView::OnEditCopyFormat03() /* FunnyCorn 175 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.ClipboardCopyFormat(&g_userCopyFormat[2]);
}

void CSuperTagEditorView::OnEditCopyFormat04() /* FunnyCorn 175 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.ClipboardCopyFormat(&g_userCopyFormat[3]);
}

void CSuperTagEditorView::OnEditCopyFormat05() /* FunnyCorn 175 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.ClipboardCopyFormat(&g_userCopyFormat[4]);	
}

void CSuperTagEditorView::OnUpdateEditCopyFormat(CCmdUI* pCmdUI) /* FunnyCorn 175 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (m_List.GetSelectedCount() == 1 && m_List.GetSelectFileName() == NULL) {
		// フォルダの単一選択
		pCmdUI->Enable(TRUE);
		return;
	} else {
		// ファイルの単一選択／複数選択
		//pCmdUI->Enable(m_List.GetSelectedCount() ? TRUE : FALSE);
		pCmdUI->Enable(TRUE);
		return;
	}
	pCmdUI->Enable(FALSE);	
}

void CSuperTagEditorView::OnDeleteCharSpace() /* FunnyCorn 177 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.DeleteCharSpace();
}

void CSuperTagEditorView::OnUpdateDeleteCharSpace(CCmdUI* pCmdUI) /* FunnyCorn 177 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	OnUpdateDeleteChar(pCmdUI);	
}

void CSuperTagEditorView::OnUpdatePluginCommand(CCmdUI* pCmdUI)
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	extern BOOL OnUpdatePluginCommand(UINT nID);
	pCmdUI->Enable(OnUpdatePluginCommand(pCmdUI->m_nID));
}

void CSuperTagEditorView::OnPluginCommand(UINT nID)
{
	extern void OnPluginCommand(UINT nID);
	OnPluginCommand(nID);
}

/* STEP 009 追加開始 */
void CSuperTagEditorView::OnConvExSetup() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	COptionSheet prop;
	CDlgConvFormatEx	pageConvEx;
	COptionListBox	listBox;

	prop.SetListControl(&listBox);
	// プロパティーシートのスタイルを変更
	prop.m_psh.dwFlags |= PSH_NOAPPLYNOW;	// [適用]ボタン無し

	pageConvEx.m_nFormatType	= 0;
	int i; for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
		pageConvEx.m_userFormatEx[i] = g_userConvFormatEx[i];
	}
	prop.SetTitle(TEXT("拡張書式変換書式設定"));
	prop.AddGroup(&pageConvEx);
	if (prop.DoModal() == IDOK) {
		for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
			g_userConvFormatEx[i] = pageConvEx.m_userFormatEx[i];
		}
		((CSuperTagEditorApp *)AfxGetApp())->UpdateAccelerator();
	}
}

void CSuperTagEditorView::OnUpdateConvExSetup(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorView::OnConvUserSetup() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnConvFormatUser();	
}

void CSuperTagEditorView::OnUpdateConvUserSetup(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
}
/* STEP 009 追加終了 */

void CSuperTagEditorView::OnSelectTreeFile()  /* STEP 013 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.SelectTreeFile();
}

void CSuperTagEditorView::OnMoveToParent() /* STEP 014 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.MoveToParent();
}

void CSuperTagEditorView::OnUpdateMoveToParent(CCmdUI* pCmdUI) /* STEP 014 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorView::DoMoveFolder(UINT index)  /* STEP 022 */
{
	TCHAR	sFolderName[_MAX_PATH] = {'\0'};

	// フォルダ選択ダイアログを開く
	if (g_userMoveFolder[index].strInitFolder.IsEmpty() && g_userMoveFolder[index].strCurrentMoveDirectory.IsEmpty()) {
		_tcscpy(sFolderName, g_strCurrentMoveDirectory);
	} else {
		if ( g_userMoveFolder[index].strCurrentMoveDirectory.IsEmpty()) {
			_tcscpy(sFolderName, g_userMoveFolder[index].strInitFolder);
		} else {
			_tcscpy(sFolderName, g_userMoveFolder[index].strCurrentMoveDirectory);
		}
	}
	if (SelectDirectory(sFolderName, g_userMoveFolder[index].bCopy) == TRUE) {
		g_strCurrentMoveDirectory = sFolderName;
		g_userMoveFolder[index].strCurrentMoveDirectory = sFolderName;

		// 移動
		m_List.MoveFolderFormat(&g_userMoveFolder[index], g_strCurrentMoveDirectory, g_bEnableMoveFolderCopy);
	}
}

void CSuperTagEditorView::OnMoveToNext() /* STEP 014 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.MoveToNext();	
}

void CSuperTagEditorView::OnUpdateMoveToNext(CCmdUI* pCmdUI) /* STEP 014 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);
}

void CSuperTagEditorView::OnMoveToPrevious() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_List.MoveToPrevious();
}

void CSuperTagEditorView::OnUpdateMoveToPrevious(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(TRUE);	
}

void CSuperTagEditorView::OnConvFormatUserT2f01() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(3, g_userConvFormat[0].strTag2File);

}

void CSuperTagEditorView::OnConvFormatUserT2f02() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(3, g_userConvFormat[1].strTag2File);
}

void CSuperTagEditorView::OnConvFormatUserT2f03() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(3, g_userConvFormat[2].strTag2File);
}

void CSuperTagEditorView::OnConvFormatUserT2f04() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(3, g_userConvFormat[3].strTag2File);
}

void CSuperTagEditorView::OnConvFormatUserT2f05() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(3, g_userConvFormat[4].strTag2File);
}

void CSuperTagEditorView::OnConvFormatUserF2t01() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(4, g_userConvFormat[0].strFile2Tag);
}

void CSuperTagEditorView::OnConvFormatUserF2t02() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(4, g_userConvFormat[1].strFile2Tag);
}

void CSuperTagEditorView::OnConvFormatUserF2t03() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(4, g_userConvFormat[2].strFile2Tag);
}

void CSuperTagEditorView::OnConvFormatUserF2t04() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(4, g_userConvFormat[3].strFile2Tag);
}

void CSuperTagEditorView::OnConvFormatUserF2t05() /* STEP 030 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvTagInfoSelected(4, g_userConvFormat[4].strFile2Tag);
}

void CSuperTagEditorView::OnConvTagToTag01() /* STEP 034 */
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvUserTag2Tag(&g_userConvFormatTag2Tag[0]);
}

void CSuperTagEditorView::OnUpdateConvTagToTag(CCmdUI* pCmdUI) /* STEP 034 */
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if (m_List.GetSelectedCount() >= 2) {
		// 複数行選択
		pCmdUI->Enable(FALSE);
	} else {
		pCmdUI->Enable(FALSE);
		// 範囲選択
		int		sx, sy, ex, ey;
		if (m_List.GetSelectedRange(sx, sy, ex, ey) == true) {
			if (sx == ex || (sx == ex && sy == ey)) {
				pCmdUI->Enable(TRUE);
			}
		}
	}
}

void CSuperTagEditorView::OnConvTagToTag02() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvUserTag2Tag(&g_userConvFormatTag2Tag[1]);
}

void CSuperTagEditorView::OnConvTagToTag03() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvUserTag2Tag(&g_userConvFormatTag2Tag[2]);
}

void CSuperTagEditorView::OnConvTagToTag04() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvUserTag2Tag(&g_userConvFormatTag2Tag[3]);
}

void CSuperTagEditorView::OnConvTagToTag05() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWaitCursor	wait;
	m_List.ConvUserTag2Tag(&g_userConvFormatTag2Tag[4]);
}

void CSuperTagEditorView::OnConvTag2tagSetup() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	COptionSheet prop;
	CDlgUserConvFormartTag2Tag	pageConvTag2Tag;
	COptionListBox	listBox;

	prop.SetListControl(&listBox);
	// プロパティーシートのスタイルを変更
	prop.m_psh.dwFlags |= PSH_NOAPPLYNOW;	// [適用]ボタン無し

	pageConvTag2Tag.m_nFormatType	= 0;
	int i; for (i = 0; i < USER_CONV_FORMAT_TAG2TAG_MAX; i++) {
		pageConvTag2Tag.m_userFormatTag2Tag[i] = g_userConvFormatTag2Tag[i];
	}
	prop.SetTitle(TEXT("タグ情報変換書式設定"));
	prop.AddGroup(&pageConvTag2Tag);
	if (prop.DoModal() == IDOK) {
		for (i = 0; i < USER_CONV_FORMAT_TAG2TAG_MAX; i++) {
			g_userConvFormatTag2Tag[i] = pageConvTag2Tag.m_userFormatTag2Tag[i];
		}
		((CSuperTagEditorApp *)AfxGetApp())->UpdateAccelerator();
	}
}

void CSuperTagEditorView::OnUpdateConvTag2tagSetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// SIフィールドから ID3 tag への変換 /* Version 1.01で再び追加 */
void CSuperTagEditorView::OnUpdateConvSiFieldToId3tag(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_List.GetSelectedItem() < 0) ? FALSE : TRUE);
}

// SIフィールドから ID3 tag への変換 /* Version 1.01で再び追加 */
void CSuperTagEditorView::OnConvSiFieldToId3tag() 
{
	CWaitCursor	wait;
	m_List.ConvSiFieldToId3tag();
	GetDocument()->UpdateAllFiles(true);
}

void CSuperTagEditorView::OnConvStrFixedUpperLower() /* STEP 040 */
{
	CWaitCursor	wait;
	m_List.ConvString(CONV_STR_FIXED_UP_LOW);
}
