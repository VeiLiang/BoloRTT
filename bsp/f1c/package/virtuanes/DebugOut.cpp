//
// Debug output
//
#include "DebugOut.h"

CDebugOut	Dbg;

// static const CHAR szClassName[] = "DebugWindow_wndclass";

CDebugOut::CDebugOut()
{
// #if	defined(_DEBUG) || defined(_DEBUGOUT)
// 	hWndDebugOutput = ::FindWindow( szClassName, NULL );
// 	if( !hWndDebugOutput ) {
// 		::OutputDebugString( "DebugWindow ‚ª‚ ‚è‚Ü‚¹‚ñ\n" );
// 	}
// #endif
}

void CDebugOut::Clear()
{
// #if	defined(_DEBUG) || defined(_DEBUGOUT)
// 	if( hWndDebugOutput ) {
// 		if( ::IsWindow( hWndDebugOutput ) ) {
// 			::SendMessage( hWndDebugOutput, WM_APP+1, (WPARAM)NULL, (LPARAM)NULL );
// 		}
// 	}
// #endif
}

void __cdecl CDebugOut::Out( LPSTR fmt, ... )
{
// #if	defined(_DEBUG) || defined(_DEBUGOUT)
	CHAR	buf[1000];
	va_list	va;
	va_start( va, fmt );
	// ::vsprintf( buf, fmt, va );
    rt_vsnprintf( buf,1000, fmt, va );
	rt_kprintf("%s",buf);
// #endif
}

void CDebugOut::Out( const string& str )
{
    rt_kprintf("%s",(LPSTR)str.c_str());
// #if	defined(_DEBUG) || defined(_DEBUGOUT)
// 	Out( (LPSTR)str.c_str() );
// #endif
}

