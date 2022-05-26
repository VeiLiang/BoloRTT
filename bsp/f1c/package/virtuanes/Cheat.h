#ifndef	__CEHAT_INCLUDED__
#define	__CEHAT_INCLUDED__

#include "typedef.h"

#include <string>
using namespace std;

// 以下の２つはORマスク
#define	CHEAT_ENABLE		(1<<0)
#define	CHEAT_KEYDISABLE	(1<<1)

// 書き込み種類
#define	CHEAT_TYPE_ALWAYS	0	// 常に書き込み
#define	CHEAT_TYPE_ONCE		1	// １回だけ書き込み
#define	CHEAT_TYPE_GREATER	2	// データより大きい時
#define	CHEAT_TYPE_LESS		3	// データより小さい時

// データ長
#define	CHEAT_LENGTH_1BYTE	0
#define	CHEAT_LENGTH_2BYTE	1
#define	CHEAT_LENGTH_3BYTE	2
#define	CHEAT_LENGTH_4BYTE	3

class	CHEATCODE {
public:
	BYTE	enable;
	BYTE	type;
	BYTE	length;
	WORD	address;
	DWORD	data;

	string	comment;
};

class	GENIECODE {
public:
	WORD	address;
	BYTE	data;
	BYTE	cmp;
};

#endif	// !__CEHAT_INCLUDED__

