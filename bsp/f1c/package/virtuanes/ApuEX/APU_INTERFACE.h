//////////////////////////////////////////////////////////////////////////
//                                                                      //
//      APU Interface class                                             //
//                                                           Norix      //
//                                               written     2001/09/18 //
//                                               last modify ----/--/-- //
//////////////////////////////////////////////////////////////////////////
#ifndef	__APU_INTERFACE_INCLUDED__
#define	__APU_INTERFACE_INCLUDED__



#include "typedef.h"
#include "macro.h"

#define	APU_CLOCK		1789772.5f

// Fixed point decimal macro
#define	INT2FIX(x)	((x)<<16)
#define	FIX2INT(x)	((x)>>16)

// APU�C���^�[�t�F�[�X���ۃN���X
class	APU_INTERFACE
{
public:
	// ��Ɏ�����v����������
	virtual	void	Reset( FLOAT fClock, INT nRate ) = 0;
	virtual	void	Setup( FLOAT fClock, INT nRate ) = 0;
	virtual	void	Write( WORD addr, BYTE data ) = 0;
	virtual	INT	Process( INT channel ) = 0;

	// �I�v�V�����Ŏ�������
	virtual	BYTE	Read ( WORD addr ) { return (BYTE)(addr>>8); }

	virtual	void	WriteSync( WORD addr, BYTE data ) {}
	virtual	BYTE	ReadSync ( WORD addr ) { return 0; }
	virtual	void	VSync() {}
	virtual	BOOL	Sync( INT cycles ) { return FALSE; }	// IRQ�𔭐����鎞TRUE��Ԃ�

	virtual	INT	GetFreq( INT channel ) { return 0; }

	// For State save
	virtual	INT	GetStateSize() { return 0; }
	virtual	void	SaveState( LPBYTE p ) {}
	virtual	void	LoadState( LPBYTE p ) {}
protected:
private:
};

#endif	// !__APU_INTERFACE_INCLUDED__

