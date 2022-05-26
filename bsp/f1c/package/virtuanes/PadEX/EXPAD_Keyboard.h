//////////////////////////////////////////////////////////////////////////
// Family Basic Keyboard                                                //
//////////////////////////////////////////////////////////////////////////
class	EXPAD_Keyboard : public EXPAD
{
public:
	EXPAD_Keyboard( NES* parent ) : EXPAD( parent ) {}

	void	Reset();

	BYTE	Read4016();
	BYTE	Read4017();
	void	Write4016( BYTE data );

protected:
	BOOL	bGraph;
	BOOL	bOut;
	BYTE	ScanNo;

private:
};
