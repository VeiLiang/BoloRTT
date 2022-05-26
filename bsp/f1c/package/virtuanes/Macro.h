//
// 便利かも知れないマクロたち
//
#ifndef	__MACRO_INCLUDED__
#define	__MACRO_INCLUDED__
#include <rtthread.h>
#include <string.h>
// 多重呼び出し対応マクロちゃん
#define	DELETEPTR(x)	if(x) { delete x; x = NULL; }
#define	DELETEARR(x)	if(x) { delete[] x; x = NULL; }
#define	RELEASE(x)	if(x) { x->Release(); x=NULL; }
#define	FREE(x)		if(x) { free(x); x=NULL; }
#define	FCLOSE(x)	if(x) { fclose(x); x=NULL; }
#define	GDIDELETE(x)	if(x) { DeleteObject(x); x=NULL; }
#define	CLOSEHANDLE(x)	if(x) { CloseHandle(x); x = NULL; }

// その他
#define	ZEROMEMORY(p,s)	memset( (p), 0, (s) )
#define	ZeroMemory(p,s) 	memset( (p), 0, (s) )
// RECT構造体用
#define	RCWIDTH(rc)	((rc).right-(rc).left)
#define	RCHEIGHT(rc)	((rc).bottom-(rc).top)

#endif	// !__MACRO_INCLUDED__
