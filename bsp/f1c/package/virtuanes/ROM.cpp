//////////////////////////////////////////////////////////////////////////
//                                                                      //
//      NES ROM Cartridge class                                         //
//                                                           Norix      //
//                                               written     2001/02/20 //
//                                               last modify ----/--/-- //
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>

#include "errno.h"
#include "VirtuaNESres.h"

#include "DebugOut.h"
#include "App.h"
// #include "Plugin.h"
#include "Pathlib.h"
#include "Crclib.h"
#include "Config.h"

// #include "Archive.h"

#include "rom.h"
#include "romdb.h"
#include "mmu.h"

#include "ips.h"

#include "rtthread.h"
// #define READ_WRITE_USE_POSIX
#ifdef READ_WRITE_USE_POSIX
#include <dfs_posix.h>
#endif


//
// ?造函数
//
ROM::ROM( const char* fname )
{
#ifndef READ_WRITE_USE_POSIX
	FILE	*fp = NULL;
#else
	int	fp = 0;
#endif
	LPBYTE	temp = NULL;
	LPBYTE	bios = NULL;
	LONG	FileSize;
	DEBUGOUT( "memset some values...\n" );
	ZEROMEMORY( &header, sizeof(header) );
	ZEROMEMORY( path, sizeof(path) );
	ZEROMEMORY( name, sizeof(name) );

	bPAL = FALSE;
	bNSF = FALSE;
	NSF_PAGE_SIZE = 0;

	lpPRG = lpCHR = lpTrainer = lpDiskBios = lpDisk = NULL;

	crc = crcall = 0;
	mapper = 0;
	diskno = 0;
	DEBUGOUT( "open file %s\n", fname);
	try 
	{
#ifndef READ_WRITE_USE_POSIX
		if( !(fp = ::fopen( fname, "rb" )) ) 
		{
			// xxx 无法打?文件
			
			DEBUGOUT( "open file %s failed!errno:%d\n", fname,errno);
			
			LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
			sprintf( szErrorString, szErrStr, fname );
			throw	szErrorString;
		}
		DEBUGOUT( "open file %s ok\n", fname);
		// ?取文件大小
		::fseek( fp, 0, SEEK_END );
		FileSize = ::ftell( fp );
		::fseek( fp, 0, SEEK_SET );
		DEBUGOUT( "file size%d\n", FileSize);
		// ??文件大小（NES?+1字?以上?？)
		if( FileSize < 17 ) 
		{
			// 文件太小
			throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
		}

		// 申???内存
		if( !(temp = (LPBYTE)::malloc( FileSize )) ) 
		{
			// 无法申?内存
			throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
		}

		// ?取文件
		if( ::fread( temp, FileSize, 1, fp ) != 1 ) 
		{
			// ?取文件失?
			throw	CApp::GetErrorString( IDS_ERROR_READ );
		}

		FCLOSE( fp );
#else

		fp = open( fname, O_RDONLY );
		if( fp <= 0 )
		{
			// xxx 无法打?文件
			
			DEBUGOUT( "open file %s failed!errno:%d\n", fname,errno);
			
			LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
			sprintf( szErrorString, szErrStr, fname );
			throw	szErrorString;
		}
		DEBUGOUT( "open file %s ok\n", fname);
		// ?取文件大小
		int FileSize = lseek( fp, 0, SEEK_END );
		// = ::ftell( fp );
		lseek( fp, 0, SEEK_SET );
		DEBUGOUT( "file size%d\n", FileSize);
		// ??文件大小（NES?+1字?以上?？)
		if( FileSize < 17 ) 
		{
			// 文件太小
			throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
		}

		// 申???内存
		if( !(temp = (LPBYTE)::malloc( FileSize )) ) 
		{
			// 无法申?内存
			throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
		}

		// ?取文件
		if( read(fp, temp, FileSize  ) != FileSize ) 
		{
			// ?取文件失?
			throw	CApp::GetErrorString( IDS_ERROR_READ );
		}

		close( fp );
#endif

		DEBUGOUT( "copy file header\n");
		// ?制文件?副本
		::memcpy( &header, temp, sizeof(NESHEADER) );
		DEBUGOUT( "ok\n");
		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) 
		 {
			// ?制文件?副本
			memcpy( &header, temp, sizeof(NESHEADER) );
		} 
		else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) 
		{
			// ?制文件?副本
			memcpy( &header, temp, sizeof(NESHEADER) );
		}
		else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') 
		{
			// ?制文件?副本
			memcpy( &header, temp, sizeof(NESHEADER) );
		} 
		else 
		{
			// FREE( temp );
			DEBUGOUT( "not suppost file type\n");
			// if( !UnCompress( fname, &temp, (LPDWORD)&FileSize ) ) {
			// 	// 格式不?
			// 	throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTFORMAT );
			// }
			// // ?制文件?副本
			// ::memcpy( &header, temp, sizeof(NESHEADER) );
		}

		// Since the zip/fds/nes is defrosted and raw, now apply the patch
		// if( Config.emulator.bAutoIPS ) 
		// {
		// 	LPBYTE	ipstemp = NULL;
		// 	if( !(ipstemp = (LPBYTE)::malloc( FileSize )) ) 
		// 	{
		// 		//无法申?内存
		// 		throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
		// 	}
		// 	::memcpy( ipstemp, temp, FileSize );
		// 	if( ApplyIPS( fname, ipstemp, FileSize ) ) {
		// 		::memcpy( &header, ipstemp, sizeof(NESHEADER) );
		// 		::memcpy( temp, ipstemp, FileSize );
		// 	}

		// 	FREE( ipstemp );
		// }

		DWORD	PRGoffset, CHRoffset;
		LONG	PRGsize, CHRsize;

		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) 
		 {
		// 普通NES文件
			PRGsize = (LONG)header.PRG_PAGE_SIZE*0x4000;
			CHRsize = (LONG)header.CHR_PAGE_SIZE*0x2000;
			PRGoffset = sizeof(NESHEADER);
			CHRoffset = PRGoffset + PRGsize;
			DEBUGOUT( "nes file check...\n");
			if( IsTRAINER() ) {
				PRGoffset += 512;
				CHRoffset += 512;
			}
			DEBUGOUT( "PRG BANK\n");
			if( PRGsize <= 0 || (PRGsize+CHRsize) > FileSize ) {
				// NES文件??常
				throw	CApp::GetErrorString( IDS_ERROR_INVALIDNESHEADER );
			}
			
			// PRG BANK
			if( !(lpPRG = (LPBYTE)malloc( PRGsize )) ) {
				// 无法申?内存
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			
			::memcpy( lpPRG, temp+PRGoffset, PRGsize );
			
			DEBUGOUT( "CHR BANK\n");
			// CHR BANK
			if( CHRsize > 0 ) 
			{
				if( !(lpCHR = (LPBYTE)malloc( CHRsize )) ) 
				{
					
					throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
				}

				if( FileSize >= CHRoffset+CHRsize ) 
				{
					memcpy( lpCHR, temp+CHRoffset, CHRsize );
				} 
				else 
				{
					// CHR bank 很少…
					CHRsize -= (CHRoffset+CHRsize - FileSize);
					memcpy( lpCHR, temp+CHRoffset, CHRsize );
				}
			} 
			else 
			{
				lpCHR = NULL;
			}
			DEBUGOUT( "Trainer\n");
			// Trainer
			if( IsTRAINER() ) 
			{
				if( !(lpTrainer = (LPBYTE)malloc( 512 )) ) 
				{
					
					throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
				}

				memcpy( lpTrainer, temp+sizeof(NESHEADER), 512 );
			} 
			else 
			{
				lpTrainer = NULL;
			}
			DEBUGOUT( "ok\n");
		} 
		else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) 
		{
		// FDS(Nintendo Disk System)
			// 磁?大小
			diskno = header.PRG_PAGE_SIZE;

			if( FileSize < (16+65500*diskno) ) 
			{
				// 磁?大小?常
				throw	CApp::GetErrorString( IDS_ERROR_ILLEGALDISKSIZE );
			}
			if( diskno > 8 ) 
			{
				// 8面以上的磁?不支持
				throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTDISK );
			}

			ZEROMEMORY( &header, sizeof(NESHEADER) );

			// 制作假信?
			header.ID[0] = 'N';
			header.ID[1] = 'E';
			header.ID[2] = 'S';
			header.ID[3] = 0x1A;
			header.PRG_PAGE_SIZE = (BYTE)diskno*4;
			header.CHR_PAGE_SIZE = 0;
			header.control1 = 0x40;
			header.control2 = 0x10;

			PRGsize = sizeof(NESHEADER)+65500*(LONG)diskno;
			// PRG BANK
			if( !(lpPRG = (LPBYTE)malloc( PRGsize )) ) {
				
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			// 数据??用
			if( !(lpDisk = (LPBYTE)malloc( PRGsize )) ) {
				
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			// CHR BANK
			lpCHR = NULL;

			::memcpy( lpPRG, &header, sizeof(NESHEADER) );
			::memcpy( lpPRG+sizeof(NESHEADER), temp+sizeof(NESHEADER), 65500*(LONG)diskno );
			// 指定数据的重写位置
			ZEROMEMORY( lpDisk, PRGsize );
//			memcpy( lpDisk, &header, sizeof(NESHEADER) );
//			memcpy( lpDisk+sizeof(NESHEADER), temp+sizeof(NESHEADER), PRGsize-sizeof(NESHEADER) );

			lpPRG[0] = 'F';
			lpPRG[1] = 'D';
			lpPRG[2] = 'S';
			lpPRG[3] = 0x1A;
			lpPRG[4] = (BYTE)diskno;

			//DISKSYSTEM BIOS的加?
			string	Path = CPathlib::MakePathExt( CApp::GetModulePath(), "DISKSYS", "ROM" );
#ifndef READ_WRITE_USE_POSIX
			if( !(fp = fopen( Path.c_str(), "rb" )) ) {
				// DISKSYS.ROM没有
				throw	CApp::GetErrorString( IDS_ERROR_NODISKBIOS );
			}

			::fseek( fp, 0, SEEK_END );
			FileSize = ::ftell( fp );
			::fseek( fp, 0, SEEK_SET );
			if( FileSize < 17 ) {
				// 文件太小
				throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
			}
			if( !(bios = (LPBYTE)malloc( FileSize )) ) 
			{
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			if( fread( bios, FileSize, 1, fp ) != 1 ) 
			{
				// 文件?取失?
				throw	CApp::GetErrorString( IDS_ERROR_READ );
			}
			FCLOSE( fp );
#else
			fp = open( Path.c_str(), O_RDONLY );
			if( fp <= 0 ) 
			{
				DEBUGOUT( "err no:%d\n",errno);
				// DISKSYS.ROM没有
				throw	CApp::GetErrorString( IDS_ERROR_NODISKBIOS );
			}

			FileSize = lseek( fp, 0, SEEK_END );
			//FileSize = ::ftell( fp );
			lseek( fp, 0, SEEK_SET );
			if( FileSize < 17 ) {
				// 文件太小
				throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
			}
			if( !(bios = (LPBYTE)malloc( FileSize )) ) 
			{
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			if( read(fp, bios, FileSize ) != FileSize ) 
			{
				// 文件?取失?
				throw	CApp::GetErrorString( IDS_ERROR_READ );
			}
			close( fp );
#endif

			if( !(lpDiskBios = (LPBYTE)malloc( 8*1024 )) ) 
			{
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}

			if( bios[0] == 'N' && bios[1] == 'E' && bios[2] == 'S' && bios[3] == 0x1A ) 
			{
			// NES形式BIOS
				::memcpy( lpDiskBios, bios+0x6010, 8*1024 );
			} 
			else 
			{
			// 生BIOS
				::memcpy( lpDiskBios, bios, 8*1024 );
			}
			FREE( bios );
		} 
		else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') 
		{
		// NSF
			bNSF = TRUE;
			ZEROMEMORY( &header, sizeof(NESHEADER) );

			// 文件?
			memcpy( &nsfheader, temp, sizeof(NSFHEADER) );

			PRGsize = FileSize-sizeof(NSFHEADER);
DEBUGOUT( "PRGSIZE:%d\n", PRGsize );
			PRGsize = (PRGsize+0x0FFF)&~0x0FFF;
DEBUGOUT( "PRGSIZE:%d\n", PRGsize );
			if( !(lpPRG = (LPBYTE)malloc( PRGsize )) ) 
			{
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
				throw	szErrorString;
			}
			ZEROMEMORY( lpPRG, PRGsize );
			memcpy( lpPRG, temp+sizeof(NSFHEADER), FileSize-sizeof(NSFHEADER) );

			NSF_PAGE_SIZE = PRGsize>>12;
DEBUGOUT( "PAGESIZE:%d\n", NSF_PAGE_SIZE );
		} 
		else 
		{
			// 格式不支持
			throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTFORMAT );
		}

		// ?取路径/文件名
		{
			string	tempstr;
			tempstr = CPathlib::SplitPath( fname );
			::strcpy( path, tempstr.c_str() );
			DEBUGOUT( "path:%s\n",path);
			tempstr = CPathlib::SplitFname( fname );
			::strcpy( name, tempstr.c_str() );
			DEBUGOUT( "name:%s\n",name);
			//原始文件名（完整路径）
			::strcpy( fullpath, fname );
		}
		DEBUGOUT( "read settting ...\n");
		// 映射?置
		if( !bNSF ) 
		{
			mapper = (header.control1>>4)|(header.control2&0xF0);
			crc = crcall = crcvrom = 0;
			DEBUGOUT( "mapper:%d\n",mapper);
			if( mapper != 20 ) {
				// PRG crc的?算（与NesToy的PRG CRC相同）
				if( IsTRAINER() ) 
				{
					crcall  = CRC::CrcRev( 512+PRGsize+CHRsize, temp+sizeof(NESHEADER) );
					crc     = CRC::CrcRev( 512+PRGsize, temp+sizeof(NESHEADER) );
					if( CHRsize )
						crcvrom = CRC::CrcRev( CHRsize, temp+PRGsize+512+sizeof(NESHEADER) );
				} 
				else 
				{
					crcall  = CRC::CrcRev( PRGsize+CHRsize, temp+sizeof(NESHEADER) );
					crc     = CRC::CrcRev( PRGsize, temp+sizeof(NESHEADER) );
					if( CHRsize )
						crcvrom = CRC::CrcRev( CHRsize, temp+PRGsize+sizeof(NESHEADER) );
				}
				DEBUGOUT( "crc:0x%08x\n",crc);
				FilenameCheck( name );
				romdatabase.HeaderCorrect( header, crcall, crc );
				DEBUGOUT( "HeaderCorrect ok\n");
#include "ROM_Patch.pacth"
				fdsmakerID = fdsgameID = 0;
			} else {
				crc = crcall = crcvrom = 0;

				fdsmakerID = lpPRG[0x1F];
				fdsgameID  = (lpPRG[0x20]<<24)|(lpPRG[0x21]<<16)|(lpPRG[0x22]<<8)|(lpPRG[0x23]<<0);
			}
		} 
		else 
		{
		// NSF
			mapper = 0x0100;	// Private mapper
			crc = crcall = crcvrom = 0;
			fdsmakerID = fdsgameID = 0;
		}

		FREE( temp );
	} 
	catch( CHAR* str ) 
	{
		DEBUGOUT( "exc :%s\n",str);
		// ?常原因的???理
#ifndef READ_WRITE_USE_POSIX
		FCLOSE( fp );
#else
		close(fp);
#endif
		FREE( temp );
		FREE( bios );

		FREE( lpPRG );
		FREE( lpCHR );
		FREE( lpTrainer );
		FREE( lpDiskBios );
		FREE( lpDisk );

		throw	str;
#ifndef	_DEBUG
	} catch(...) {
		// 因?不想出?一般保???…（^^；
#ifndef READ_WRITE_USE_POSIX
		FCLOSE( fp );
#else
		close(fp);
#endif
		FREE( temp );
		FREE( bios );

		FREE( lpPRG );
		FREE( lpCHR );
		FREE( lpTrainer );
		FREE( lpDiskBios );
		FREE( lpDisk );

#ifdef	_DATATRACE
		// For dis...
		FREE( PROM_ACCESS );
#endif

		//?生了未知??
		throw	CApp::GetErrorString( IDS_ERROR_UNKNOWN );
#endif	// !_DEBUG
	}
}

//
// 析?器
//
ROM::~ROM()
{
	FREE( lpPRG );
	FREE( lpCHR );
	FREE( lpTrainer );
	FREE( lpDiskBios );
	FREE( lpDisk );
}

//
// ROM文件??
//
INT	ROM::IsRomFile( const char* fname )
{

NESHEADER	header;
#ifndef READ_WRITE_USE_POSIX
	FILE*	fp = NULL;
	if( !(fp = fopen( fname, "rb" )) )
		return	IDS_ERROR_OPEN;

	if( fread( &header, sizeof(header), 1, fp ) != 1 ) 
	{
		FCLOSE( fp );
		return	IDS_ERROR_READ;
	}
	FCLOSE( fp );
#else
	int fp;
	fp = open( fname, O_RDONLY );
	if( fp <= 0 )
	{
		DEBUGOUT( "err no:%d\n",errno);
		return	IDS_ERROR_OPEN;
	}
	if( read( fp,&header, sizeof(header) ) != sizeof(header) ) 
	{
		close( fp );
		return	IDS_ERROR_READ;
	}
	close( fp );
#endif
	if( header.ID[0] == 'N' && header.ID[1] == 'E'
	 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		for( INT i = 0; i < 8; i++ ) {
			if( header.reserved[i] )
				return	IDS_ERROR_ILLEGALHEADER;
		}
		return	0;
	} else if( header.ID[0] == 'F' && header.ID[1] == 'D'
		&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		return	0;
	} else if( header.ID[0] == 'N' && header.ID[1] == 'E'
		&& header.ID[2] == 'S' && header.ID[3] == 'M') {
		return	0;
	} 
	else 
	{
		return	IDS_ERROR_UNSUPPORTFORMAT;
		// LPBYTE	temp = NULL;
		// LONG	size;
		// if( !UnCompress( fname, &temp, (LPDWORD)&size ) )
		// 	return	IDS_ERROR_UNSUPPORTFORMAT;

		// memcpy( &header, temp, sizeof(NESHEADER) );
		// FREE( temp );
		// if( header.ID[0] == 'N' && header.ID[1] == 'E'
		//  && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		// 	for( INT i = 0; i < 8; i++ ) {
		// 		if( header.reserved[i] )
		// 			return	IDS_ERROR_ILLEGALHEADER;
		// 	}
		// 	return	0;
		// } else if( header.ID[0] == 'F' && header.ID[1] == 'D'
		// 	&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		// 	return	0;
		// } else if( header.ID[0] == 'N' && header.ID[1] == 'E'
		// 	&& header.ID[2] == 'S' && header.ID[3] == 'M') {
		// 	return	0;
		// }
	}

	return	IDS_ERROR_UNSUPPORTFORMAT;
}

//
// ROM文件名的??（自?判?PAL）
//
void	ROM::FilenameCheck( const char* fname )
{
	// unsigned char*	p = (unsigned char*)fname;

	// while( *p != (unsigned char)'\0' ) 
	// {
	// 	if( *p == (unsigned char)'(' ) 
	// 	{
	// 		if( _mbsnbicmp( p, (unsigned char*)"(E)", 3 ) == 0 ) 
	// 		{
				bPAL = TRUE;
	// 			return;
	// 		}
	// 	}

	// 	p = _mbsinc(p);
	// }
}

