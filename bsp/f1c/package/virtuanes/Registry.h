//
// レジストリサポートクラス
//
#ifndef	__CREGISTRY_INCLUDED__
#define	__CREGISTRY_INCLUDED__

// #define	WIN32_LEAN_AND_MEAN
// #include <windows.h>
// #include <windowsx.h>
// #include <TCHAR.h>

#include <string>
using namespace std;

class	CRegistry
{
public:
	static	void	SetRegistryKey( LPCSTR	lpszKey );

	static	UINT	GetProfileInt   ( LPCSTR lpszSection, LPCSTR lpszEntry, INT nDefault );
	static	BOOL	GetProfileString( LPCSTR lpszSection, LPCSTR lpszEntry, LPVOID lpData, UINT nBytes );
	static	BOOL	GetProfileBinary( LPCSTR lpszSection, LPCSTR lpszEntry, LPVOID lpData, UINT nBytes );

	static	BOOL	WriteProfileInt( LPCSTR lpszSection, LPCSTR lpszEntry, INT nValue );
	static	BOOL	WriteProfileString( LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszValue );
	static	BOOL	WriteProfileBinary( LPCSTR lpszSection, LPCSTR lpszEntry, LPVOID pData, UINT nBytes );

protected:
	static	CHAR	m_szRegistryKey[MAX_PATH];

	// static	HKEY	GetRegistryKey();
	// static	HKEY	GetSectionKey( LPCTSTR lpszSection );

private:
};

#endif	// !__CREGISTRY_INCLUDED__
