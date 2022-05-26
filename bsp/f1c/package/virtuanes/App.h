//
// アプリケーションサポートクラス
//

#ifndef	__CAPP_INCLUDED__
#define	__CAPP_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Typedef.h"
#define	ERRORSTRING_MAX	32

class CApp {
public:
	// // 起動時に設定する
	// static	void	SetInstance( HINSTANCE hInstance )	{ m_hInstance = hInstance; }
	// static	void	SetPrevInstance( HINSTANCE hInstance )	{ m_hPrevInstance = hInstance; }
	// static	void	SetCmdLine( LPSTR lpCmdLine )		{ m_lpCmdLine = lpCmdLine; }
	// static	void	SetCmdShow( INT nCmdShow )		{ m_nCmdShow = nCmdShow; }

	// static	void	SetHWnd( HWND hWnd )			{ m_hWnd = hWnd; }
	// static	void	SetMenu( HMENU hMenu )			{ m_hMenu = hMenu; }
	// static	void	SetPlugin( HINSTANCE hPlugin )		{ m_hPlugin = hPlugin; LoadErrorString(); }

	static	void	SetModulePath( LPCSTR lpModulePath )	{ ::strcpy( m_szModulePath, lpModulePath ); }

	// // アプリ内部で使用する
	// static	HINSTANCE	GetInstance()		{ return m_hInstance; }
	// static	HINSTANCE	GetPrevInstance()	{ return m_hPrevInstance; }
	// static	LPTSTR		GetCmdLine()		{ return m_lpCmdLine; }
	// static	INT		GetCmdShow()		{ return m_nCmdShow; }

	// static	HWND		GetHWnd()		{ return m_hWnd; }
	// static	HMENU		GetMenu()		{ return m_hMenu; }
	// static	HINSTANCE	GetPlugin()		{ return m_hPlugin; }

	static	LPCSTR		GetModulePath()		{ return m_szModulePath; }

	// static	HMENU		LoadMenu( UINT uID )	{ return ::LoadMenu( m_hPlugin, MAKEINTRESOURCE(uID) ); }
	// static	HICON		LoadIcon( UINT uID )	{ return ::LoadIcon( m_hInstance, MAKEINTRESOURCE(uID) ); }
	// static	INT		LoadString( UINT uID, LPTSTR lpBuffer, INT nBufferMax ) {
	// 				return	::LoadString( m_hPlugin, uID, lpBuffer, nBufferMax ); }

	// // エラー処理用
	static	void	LoadErrorString();
	static	CHAR*	GetErrorString( INT nID );

protected:
	// // WinMain からの引数そのまま
	// static	HINSTANCE	m_hInstance;
	// static	HINSTANCE	m_hPrevInstance;
	// static	LPSTR		m_lpCmdLine;
	// static	INT		m_nCmdShow;

	// // 言語リソースプラグインのインスタンスハンドル
	// static	HINSTANCE	m_hPlugin;
	// // メインウインドウのウインドウハンドル
	// static	HWND		m_hWnd;
	// // メインウインドウのメニューハンドル
	// static	HMENU		m_hMenu;

	// // プログラムのパス(起動時のパス)
	static	CHAR		m_szModulePath[_MAX_PATH];

	// エラー処理用
	static	INT	m_ErrorStringTableID[ERRORSTRING_MAX];
	static	CHAR*	m_ErrorStringTable[ERRORSTRING_MAX];
	static	CHAR	m_ErrorString[ERRORSTRING_MAX][256];
private:
};

// エラーメッセージ用テンポラリ
extern	CHAR	szErrorString[256];

#endif	// !__CAPP_INCLUDED__
