//
// NES ROMDB class
//

#include <stdio.h>
#include <stdlib.h>
// #include <mbstring.h>

#include "typedef.h"
#include "macro.h"

#include "DebugOut.h"
#include "App.h"
// #include "Plugin.h"
#include "Pathlib.h"
#include "Crclib.h"

// #include "Archive.h"

#include "romdb.h"
void * __dso_handle = 0 ;
ROMDATABASE	romdatabase;

//�����������^??strtok()����
char* myStrtok_origin(char* str_arr,const char* delimiters,char**temp_str)
{
    //��?�꘢�w?���w���ҕ�����
    char*b_temp;
    /*
    * 1�A���f�Q��str_arr����?��C�@�ʐ�NULL�A��???���Itemp_str��?�N�n�ʒu�G
    * ��s��NULL�C?��str?�N�n�ʒu?�n�ؕ��B
    */
    if(str_arr == NULL)
    {
        str_arr =*temp_str;
    }
    //2�A��?�ҕ���������
    //?�`delimiters����?�n�I���L����
    str_arr += strspn(str_arr, delimiters);
    //3�A���f���O�ҕ���I�ʒu����?'\0'�C�ᐥ?�ԉ�NULL�C��???
    if(*str_arr =='\0')
    {
        return NULL;
    }
    /*
    * 4�A�ۑ����O�I�ҕ������I�w?b_temp�C?�pstrpbrk()��b_temp���Q���𕄁C
    * �@�ʝQ�s���C?��temp_str???�ҕ���������������'\0'�I�ʒu�C
    * b_temp�v�L?��?���G��Q��?�����𕄏��݈ʒu???'\0',
    * b_temp��������B�f���Ctemp_str�w�����𕄓I����ʒu�B
    */
    b_temp = str_arr;
    str_arr = strpbrk(str_arr, delimiters);
    if(str_arr == NULL)
    {
        *temp_str = strchr(b_temp,'\0');
    }
    else
    {
        *str_arr ='\0';
        *temp_str = str_arr +1;
    }
    //5�A�����ō@������?�Q�v�Q�����𕄁C�s��b_temp�ԉ�B
    return b_temp;
}
//�g�pmyStrtok��?��myStrtok_origin����
char* myStrtok(char* str_arr,const char* delimiters)
{
    static char*last;
    return myStrtok_origin(str_arr, delimiters,&last);
}

//
// ROM DATABASE (NESToy&NNNesterJ database)
//
INT	ROMDATABASE::HeaderCheck( NESHEADER& hdr, DWORD crcall, DWORD crc, ROMDB& data )
{
	if( m_DataBaseList.empty() ) {
		LoadDatabase();
	}

	if( m_DataBaseList.empty() )
		return	-2;	// �v�L����?

	map<DWORD, ROMDB, ROMDBCMP>::iterator it = m_DataBaseList.find( crcall );

	if( it == m_DataBaseList.end() )
		return	-1;	//�v�L����?


	data = (*it).second;

	// ���??�ꉺ
	if( data.crcall == crcall || (data.crc == crc && data.crc) ) {
		if( hdr.control1 == data.control1 && hdr.control2 == data.control2 ) {
			return	0;	// ���S�C�z
		}
	}
	return	1;	// CRC����v�I�C�A��?�s��?
}

BOOL	ROMDATABASE::HeaderCorrect( NESHEADER& hdr, DWORD crcall, DWORD crc )
{
	if( m_DataBaseList.empty() ) 
	{
		LoadDatabase();
	}

	if( m_DataBaseList.empty() )
		return	FALSE;	// �v�L����?

	map<DWORD, ROMDB, ROMDBCMP>::iterator it = m_DataBaseList.find( crcall );

	if( it == m_DataBaseList.end() )
		return	FALSE;	// �v�L����?

	ROMDB	data = (*it).second;

	// ���??�ꉺ
	if( data.crcall == crcall || (data.crc == crc && data.crc) ) {
		hdr.control1 = data.control1;
		hdr.control2 = data.control2;
		for( INT i = 0; i < 8; i++ ) {
			hdr.reserved[i] = 0;
		}
		return	TRUE;
	}
	return	FALSE;
}

void	ROMDATABASE::LoadDatabase()
{
	FILE*	fp = NULL;
	CHAR	buf[512];
	const CHAR seps[] = ";\n\0";	// ���u��
	ROMDB	db;

	DEBUGOUT( "Database loading...\n" );

	string	Path = CPathlib::MakePathExt( CApp::GetModulePath(), "nesromdb", "dat" );

	DEBUGOUT( "File:%s\n", Path.c_str() );

	m_DataBaseList.clear();

	if( (fp = fopen( Path.c_str(), "r" )) ) 
	{
		while( fgets( buf, 512, fp ) ) 
		{
			if( buf[0] == ';' ) 
			{	// ���쒍?���i
				continue;
			}

			CHAR*	pToken;

			// ALL CRC
			if( !(pToken = (CHAR*)myStrtok( (CHAR*)buf, seps )) )
				continue;
			db.crcall = strtoul( pToken, NULL, 16 );
			// PRG CRC
			if( !(pToken = (CHAR*)myStrtok( NULL, seps )) )
				continue;
			db.crc = strtoul( pToken, NULL, 16 );

			// Title
			if( !(pToken = (CHAR*)myStrtok( NULL, seps )) )
				continue;
			db.title = pToken;

			// Control 1
			if( !(pToken = (CHAR*)myStrtok( NULL, seps )) )
				continue;
			db.control1 = atoi( pToken );
			// Control 2
			if( !(pToken = (CHAR*)myStrtok( NULL, seps )) )
				continue;
			db.control2 = atoi( pToken );

			// PRG SIZE
			if( !(pToken = (CHAR*)myStrtok( NULL, seps )) )
				continue;
			db.prg_size = atoi( pToken );
			// CHR SIZE
			if( !(pToken = (CHAR*)myStrtok( NULL, seps )) )
				continue;
			db.chr_size = atoi( pToken );

			// Country
			if( !(pToken = (CHAR*)myStrtok( NULL, seps )) )
				continue;
			db.country = pToken;

			db.bNTSC = TRUE;
			// Europe (PAL???)
			if( strcmp( pToken, "E"   ) == 0
			 || strcmp( pToken, "Fra" ) == 0
			 || strcmp( pToken, "Ger" ) == 0
			 || strcmp( pToken, "Spa" ) == 0
			 || strcmp( pToken, "Swe" ) == 0
			 || strcmp( pToken, "Ita" ) == 0
			 || strcmp( pToken, "Aus" ) == 0 ) {
				db.bNTSC = FALSE;
			}

			// Manufacturer
			if( pToken = (CHAR*)myStrtok( NULL, seps ) ) {
				db.manufacturer = pToken;
			} else {
				db.manufacturer.erase( db.manufacturer.begin(), db.manufacturer.end() );
			}

			// Sale date
			if( pToken = (CHAR*)myStrtok( NULL, seps ) ) {
				db.saledate = pToken;
			} else {
				db.saledate.erase( db.saledate.begin(), db.saledate.end() );
			}

			// Price
			if( pToken = (CHAR*)myStrtok( NULL, seps ) ) {
				db.price = pToken;
			} else {
				db.price.erase( db.price.begin(), db.price.end() );
			}

			// Genre
			if( pToken = (CHAR*)myStrtok( NULL, seps ) ) {
				db.genre = pToken;
			} else {
				db.genre.erase( db.genre.begin(), db.genre.end() );
			}

			m_DataBaseList[db.crcall] = db;
		}
		FCLOSE( fp );
	} 
	else 
	{
		DEBUGOUT( "Database file not found.\n" );
	}
}

