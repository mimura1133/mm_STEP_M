#define WM_SMX_IPC WM_USER
/* SCMPXウィンドウに下記のような形式でメッセージを送ってください */

// result = SendMessage( hwnd_scmpx, WM_SMX_IPC, command_data, command );






#define IPC_GETVERSION 0
/* IPC_GETVERSION を送ると次の値を返します */
/* Version 0.0.7e = 0x000007e0 */
/* Version 0.0.8j = 0x00000800 */
/* Version 0.0.9e = 0x000009e0 */
/* Version 0.0.9j = 0x00000900 */
/* Version 0.0.9A = 0x000009a0 */
/* Version 1.00e = 0x000010e0 */
/* Version 1.00j = 0x00001000 */
/* Version 1.10j = 0x00001100 */
/* Version 1.11e = 0x000011e0 */
/* Version 1.11j = 0x00001110 */
/* Version 1.20e = 0x000012e0 */
/* Version 1.20j = 0x00001200 */
/* Version 1.30e = 0x000013e0 */
/* Version 1.30j = 0x00001300 */
/* Version 1.40e = 0x000014e0 */
/* Version 1.40j = 0x00001400 */
/* Version 1.41e = 0x000014e1 */
/* Version 1.41j = 0x00001401 */
/* Version 1.42e = 0x000014e2 */
/* Version 1.42j = 0x00001402 */
/* Version 1.50e = 0x000015e0 */
/* Version 1.50j = 0x00001500 */

// if ( SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_GETVERSION ) != 0x00000800 )
// MessageBox( NULL, "Error, SCMPX 0.0.8(Japanese) not found", "Warning", MB_OK );




#define IPC_PLAYFILE 100
/* IPC_PLAYFILE はプレイリストにファイルを追加します */
/* (拡張子が '.m3u' の場合はプレイリストファイルとして読み込まれます) */

// char file[] = "C:\\Song\\mkr.mp3";
// for( i = 0; i <= strlen( file ); i++ )
// SendMessage( hwnd_scmpx, WM_SMX_IPC, file[i], IPC_PLAYFILE );




#define IPC_DELETE 101
/* IPC_DELETE はプレイリストをクリアします */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_DELETE );




#define IPC_STARTPLAY 102
/* IPC_STARTPLAY は再生コマンドを処理します */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_STARTPLAY );




#define IPC_CHDIR 103
/* IPC_CHDIR はSCMPXのカレントディレクトリを変更します */

// char dir[] = "C:\\Song";
// for( i = 0; i <= strlen( dir ); i++ )
// SendMessage( hwnd_scmpx, WM_SMX_IPC, dir[i], IPC_PLAYFILE );




#define IPC_ISPLAYING 104
/* IPC_ISPLAYING は下記の再生ステータスを返します */
/* 戻り値=0 : 停止中 */
/* 戻り値=1 : 再生中 */
/* 戻り値=3 : 一時停止 */

// status = SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_ISPLAYING );




#define IPC_GETOUTPUTTIME 105
/* IPC_GETOUTPUTTIME は再生時間を返します */
/* wParam=0 にすると現在の再生時間をms単位で返します */
/* wParam=1 にすると読み込まれている曲の長さが秒単位で戻ってきます */
/* 戻り値=-1 は再生していないか、エラーです */
/* ※再生時間は実質秒単位のかなりいいかげんな値が戻されるので注意してください */

// song_pos = SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_GETOUTPUTTIME );
// song_len = SendMessage( hwnd_scmpx, WM_SMX_IPC, 1, IPC_GETOUTPUTTIME );




#define IPC_JUMPTOTIME 106
/* IPC_JUMPTOTIME はms単位で再生中の曲の位置を変更します */
/* -1は停止中、1はEOF、0が戻ってくると成功を意味します */

// status = SendMessage( hwnd_scmpx, WM_SMX_IPC, new_song_pos, IPC_JUMPTOTIME );




#define IPC_WRITEPLAYLIST 120
/* IPC_WRITEPLAYLIST はプレイリストを書き出します */
/* 標準のプレイリストは絶対パスで、指定したリストには相対パスで書き込まれます */

// char favorite[] = "C:\\mp3z\\favorite.m3u";
// for( i = 0; i <= strlen( favorite ); i++ )
// SendMessage( hwnd_scmpx, WM_SMX_IPC, favorite[i], IPC_WRITEPLAYLIST );

/*----
　lParam=121以降はWinAMP 2.0(Pre2 9/2版)で追加されたフロントエンドです。
　SCMPXでは version 0.0.9a/e から対応しています。
　lParam=211はWinAMP 2.04、lParam=125,126はWinAMP 2.05のフロントエンドで
　SCMPXではそれぞれ version 1.10、1.11 から対応しています。
----*/





#define IPC_SETPLAYLISTPOS 121
/* プレイリストの位置を変更します */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, new_pl_pos, IPC_SETPLAYLISTPOS );




#define IPC_SETVOLUME 122
/* ボリュームを変更します(値は0-255で、255が最大です) */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, new_volume, IPC_SETVOLUME );




#define IPC_SETPANNING 123
/* パンを変更します(値は0-255、数値の低いほうが左よりで中央は128です) */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, new_pan, IPC_SETPANNING );




#define IPC_GETLISTLENGTH 124
/* プレイリストの長さを返します */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_GETLISTLENGTH );




#define IPC_GETLISTPOS 125
/* 選択されているプレイリスト番号を返します */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_GETLISTPOS );




#define IPC_GETINFO 126
/* 現在読み込まれているファイルのフォーマットを返します */
/* wParam=0 のときはサンプリングレート */
/* wParam=1 のときはビットレート */
/* wParam=2 のときはチャネル数 */

// sfreq = SendMessage( hwnd_scmpx, WM_SMX_IPC, 0, IPC_GETINFO );
// brate = SendMessage( hwnd_scmpx, WM_SMX_IPC, 1, IPC_GETINFO );
// nch = SendMessage( hwnd_scmpx, WM_SMX_IPC, 2, IPC_GETINFO );




#define IPC_GETPLAYLISTFILE 211
/* プレイリストに登録されているファイル名の文字列ポインタを返します */
/* 戻り値がNULLの場合はエラーです */

// SendMessage( hwnd_scmpx, WM_SMX_IPC, index, IPC_GETPLAYLISTFILE );
