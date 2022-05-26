//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : �H�VAV����`                                   //
//////////////////////////////////////////////////////////////////////////
class	Mapper116 : public Mapper
{
public:
	Mapper116( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	WriteLow( WORD addr, BYTE data );
	void	Write( WORD addr, BYTE data );

	void	HSync( INT scanline );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	reg[8];
	BYTE	prg0, prg1, prg2, prg3;
	BYTE	prg0L, prg1L;
	BYTE	chr0, chr1, chr2, chr3, chr4, chr5, chr6, chr7;

	BYTE	irq_enable;
	INT	irq_counter;
	BYTE	irq_latch;

	BYTE	ExPrgSwitch;
	BYTE	ExChrSwitch;

private:
	void	SetBank_CPU();
	void	SetBank_PPU();
};
