//
// レジストリサポートクラス
//

#include "DebugOut.h"
#include "Registry.h"

#include "App.h"
#include "Pathlib.h"

#define	INI_USE

CHAR	CRegistry::m_szRegistryKey[MAX_PATH] = { "App" };

// void	CRegistry::SetRegistryKey( LPCTSTR lpszKey )
// {
// #ifndef	INI_USE
// 	if( lpszKey ) {
// 		::_tcscpy( m_szRegistryKey, lpszKey );
// 	}
// #else
// 	string	str;
// 	str = CApp::GetModulePath();
// 	str += lpszKey;

// 	strcpy( m_szRegistryKey, str.c_str() );
// #endif
// }

// HKEY	CRegistry::GetRegistryKey()
// {
// 	HKEY	hAppKey = NULL;
// 	HKEY	hSoftKey = NULL;
// 	if( ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE|KEY_READ, &hSoftKey) == ERROR_SUCCESS ) {
// 		DWORD dw;
// 		::RegCreateKeyEx( hSoftKey, m_szRegistryKey, 0, REG_NONE,
// 			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hAppKey, &dw);
// 	}
// 	if( hSoftKey )
// 		::RegCloseKey( hSoftKey );
// 	return	hAppKey;
// }

// HKEY	CRegistry::GetSectionKey( LPCTSTR lpszSection )
// {
// 	HKEY	hAppKey = GetRegistryKey();
// 	if( !hAppKey )
// 		return	NULL;
// 	HKEY	hSectionKey = NULL;
// 	DWORD	dw;
// 	::RegCreateKeyEx( hAppKey, lpszSection, 0, REG_NONE,
// 		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hSectionKey, &dw );
// 	::RegCloseKey( hAppKey );

// 	return	hSectionKey;
// }

UINT	CRegistry::GetProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, INT nDefault )
{
#ifndef	INI_USE
	HKEY	hSecKey = GetSectionKey( lpszSection );
	if( !hSecKey )
		return	nDefault;
	DWORD	dwValue;
	DWORD	dwType;
	DWORD	dwCount = sizeof(DWORD);
	LONG	lResult = ::RegQueryValueEx( hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)&dwValue, &dwCount );
	::RegCloseKey( hSecKey );

	return	(lResult==ERROR_SUCCESS)?(UINT)dwValue:nDefault;
#else
	//return ::GetPrivateProfileInt( lpszSection, lpszEntry, nDefault, m_szRegistryKey );
	return 0;
#endif
}

BOOL	CRegistry::GetProfileString( LPCSTR lpszSection, LPCSTR lpszEntry, LPVOID lpData, UINT nBytes )
{
#ifndef	INI_USE
	HKEY	hSecKey = GetSectionKey( lpszSection );
	if( !hSecKey )
		return	lpszDefault;

	DWORD	dwType, dwCount;
	LONG	lResult = ::RegQueryValueEx( hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount );
	if( dwCount > nBytes ) {
		::RegCloseKey( hSecKey );
		return	FALSE;
	}

	if( lResult == ERROR_SUCCESS ) {
		lResult = ::RegQueryValueEx( hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, lpData, &dwCount );
	}
	::RegCloseKey( hSecKey );

	return	(lResult==ERROR_SUCCESS)?TRUE:FALSE;
#else
	// DWORD dw = ::GetPrivateProfileString( lpszSection, lpszEntry, "", (CHAR*)lpData, nBytes, m_szRegistryKey );
	// return	(dw&&(dw<nBytes))?TRUE:FALSE;
	return 0;
#endif
}

BOOL	CRegistry::GetProfileBinary( LPCSTR lpszSection, LPCSTR lpszEntry, LPVOID lpData, UINT nBytes )
{
#ifndef	INI_USE
	HKEY	hSecKey = GetSectionKey(lpszSection);
	if( !hSecKey )
		return	FALSE;

	DWORD	dwType, dwCount;
	LONG	lResult = ::RegQueryValueEx( hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount );
	if( dwCount > nBytes ) {
		::RegCloseKey( hSecKey );
		return	FALSE;
	}
	if( lResult == ERROR_SUCCESS ) {
		lResult = ::RegQueryValueEx( hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, lpData, &dwCount );
	}
	::RegCloseKey( hSecKey );

	return	(lResult==ERROR_SUCCESS)?TRUE:FALSE;
#else
	//return	::GetPrivateProfileStruct( lpszSection, lpszEntry, lpData, nBytes, m_szRegistryKey );
	return 0;
#endif
}

BOOL	CRegistry::WriteProfileInt( LPCSTR lpszSection, LPCSTR lpszEntry, INT nValue )
{
#ifndef	INI_USE
	HKEY	hSecKey = GetSectionKey( lpszSection );
	if( !hSecKey )
		return	FALSE;
	LONG	lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD,
			(LPBYTE)&nValue, sizeof(nValue));
	::RegCloseKey( hSecKey );
	return	(lResult == ERROR_SUCCESS);
#else
	// CHAR	szTemp[16];
	// wsprintf( szTemp, "%d", nValue );
	// return	::WritePrivateProfileString( lpszSection, lpszEntry, szTemp, m_szRegistryKey );
	return 0;
#endif
}

BOOL	CRegistry::WriteProfileString( LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszValue )
{
#ifndef	INI_USE
	LONG	lResult;
	if( !lpszEntry ) {
		HKEY	hAppKey = GetRegistryKey();
		if( !hAppKey )
			return	FALSE;
		lResult = ::RegDeleteKey( hAppKey, lpszSection );
		::RegCloseKey( hAppKey );
	} else if( !lpszValue ) {
		HKEY	hSecKey = GetSectionKey( lpszSection );
		if( !hSecKey )
			return	FALSE;
		lResult = ::RegDeleteValue( hSecKey, (LPTSTR)lpszEntry );
		::RegCloseKey( hSecKey );
	} else {
		HKEY	hSecKey = GetSectionKey( lpszSection );
		if( !hSecKey )
			return	FALSE;
		lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
			(LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
		::RegCloseKey( hSecKey );
	}
	return	(lResult == ERROR_SUCCESS);
#else
	//return	::WritePrivateProfileString( lpszSection, lpszEntry, lpszValue, m_szRegistryKey );
	return 0;
#endif
}

BOOL	CRegistry::WriteProfileBinary( LPCSTR lpszSection, LPCSTR lpszEntry, LPVOID lpData, UINT nBytes )
{
#ifndef	INI_USE
	HKEY	hSecKey = GetSectionKey( lpszSection );
	if( !hSecKey )
		return	FALSE;
	LONG	lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY, lpData, nBytes );
	::RegCloseKey( hSecKey );
	return	(lResult == ERROR_SUCCESS);
#else
	//return	::WritePrivateProfileStruct( lpszSection, lpszEntry, lpData, nBytes, m_szRegistryKey );
	return 0;
#endif
}

