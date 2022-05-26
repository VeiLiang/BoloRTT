//
// CRC�`�F�b�N�p���C�u�����N���X
//
#include "Typedef.h"
#include <limits.h>

class	CRC
{
public:
	static	DWORD	Crc( INT size, LPBYTE c );
	static	DWORD	CrcRev( INT size, LPBYTE c );	// �t��

protected:
	static	void	MakeTable();
	static	void	MakeTableRev();	// �t��

	static	BOOL	m_Init;
	static	BOOL	m_InitRev;
	static	DWORD	m_CrcTable[ UCHAR_MAX+1 ];
	static	DWORD	m_CrcTableRev[ UCHAR_MAX+1 ];
private:
};

