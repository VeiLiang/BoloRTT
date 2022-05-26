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

//根据函数原型??strtok()函数
char* myStrtok_origin(char* str_arr,const char* delimiters,char**temp_str)
{
    //定?一个指?来指向待分解串
    char*b_temp;
    /*
    * 1、判断参数str_arr是否?空，如果是NULL就以???来的temp_str作?起始位置；
    * 若不是NULL，?以str?起始位置?始切分。
    */
    if(str_arr == NULL)
    {
        str_arr =*temp_str;
    }
    //2、跳?待分解字符串
    //?描delimiters字符?始的所有分解符
    str_arr += strspn(str_arr, delimiters);
    //3、判断当前待分解的位置是否?'\0'，若是?返回NULL，否???
    if(*str_arr =='\0')
    {
        return NULL;
    }
    /*
    * 4、保存当前的待分解串的指?b_temp，?用strpbrk()在b_temp中找分解符，
    * 如果找不到，?将temp_str???待分解字符串末尾部'\0'的位置，
    * b_temp没有?生?化；若找到?将分解符所在位置???'\0',
    * b_temp相当于被截断了，temp_str指向分解符的下一位置。
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
    //5、函数最后部分无?找没找到分解符，都将b_temp返回。
    return b_temp;
}
//使用myStrtok来?化myStrtok_origin函数
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
		return	-2;	// 没有数据?

	map<DWORD, ROMDB, ROMDBCMP>::iterator it = m_DataBaseList.find( crcall );

	if( it == m_DataBaseList.end() )
		return	-1;	//没有数据?


	data = (*it).second;

	// 大体??一下
	if( data.crcall == crcall || (data.crc == crc && data.crc) ) {
		if( hdr.control1 == data.control1 && hdr.control2 == data.control2 ) {
			return	0;	// 完全匹配
		}
	}
	return	1;	// CRC是一致的，但是?不一?
}

BOOL	ROMDATABASE::HeaderCorrect( NESHEADER& hdr, DWORD crcall, DWORD crc )
{
	if( m_DataBaseList.empty() ) 
	{
		LoadDatabase();
	}

	if( m_DataBaseList.empty() )
		return	FALSE;	// 没有数据?

	map<DWORD, ROMDB, ROMDBCMP>::iterator it = m_DataBaseList.find( crcall );

	if( it == m_DataBaseList.end() )
		return	FALSE;	// 没有数据?

	ROMDB	data = (*it).second;

	// 大体??一下
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
	const CHAR seps[] = ";\n\0";	// 分隔符
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
			{	// 当作注?字段
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

