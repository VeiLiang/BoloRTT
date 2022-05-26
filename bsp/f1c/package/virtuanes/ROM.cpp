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
// ?������
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
			// xxx �ٖ@��?����
			
			DEBUGOUT( "open file %s failed!errno:%d\n", fname,errno);
			
			LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
			sprintf( szErrorString, szErrStr, fname );
			throw	szErrorString;
		}
		DEBUGOUT( "open file %s ok\n", fname);
		// ?�敶���召
		::fseek( fp, 0, SEEK_END );
		FileSize = ::ftell( fp );
		::fseek( fp, 0, SEEK_SET );
		DEBUGOUT( "file size%d\n", FileSize);
		// ??�����召�iNES?+1��?�ȏ�?�H)
		if( FileSize < 17 ) 
		{
			// ��������
			throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
		}

		// �\???����
		if( !(temp = (LPBYTE)::malloc( FileSize )) ) 
		{
			// �ٖ@�\?����
			throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
		}

		// ?�敶��
		if( ::fread( temp, FileSize, 1, fp ) != 1 ) 
		{
			// ?�敶����?
			throw	CApp::GetErrorString( IDS_ERROR_READ );
		}

		FCLOSE( fp );
#else

		fp = open( fname, O_RDONLY );
		if( fp <= 0 )
		{
			// xxx �ٖ@��?����
			
			DEBUGOUT( "open file %s failed!errno:%d\n", fname,errno);
			
			LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
			sprintf( szErrorString, szErrStr, fname );
			throw	szErrorString;
		}
		DEBUGOUT( "open file %s ok\n", fname);
		// ?�敶���召
		int FileSize = lseek( fp, 0, SEEK_END );
		// = ::ftell( fp );
		lseek( fp, 0, SEEK_SET );
		DEBUGOUT( "file size%d\n", FileSize);
		// ??�����召�iNES?+1��?�ȏ�?�H)
		if( FileSize < 17 ) 
		{
			// ��������
			throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
		}

		// �\???����
		if( !(temp = (LPBYTE)::malloc( FileSize )) ) 
		{
			// �ٖ@�\?����
			throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
		}

		// ?�敶��
		if( read(fp, temp, FileSize  ) != FileSize ) 
		{
			// ?�敶����?
			throw	CApp::GetErrorString( IDS_ERROR_READ );
		}

		close( fp );
#endif

		DEBUGOUT( "copy file header\n");
		// ?������?���{
		::memcpy( &header, temp, sizeof(NESHEADER) );
		DEBUGOUT( "ok\n");
		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) 
		 {
			// ?������?���{
			memcpy( &header, temp, sizeof(NESHEADER) );
		} 
		else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) 
		{
			// ?������?���{
			memcpy( &header, temp, sizeof(NESHEADER) );
		}
		else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') 
		{
			// ?������?���{
			memcpy( &header, temp, sizeof(NESHEADER) );
		} 
		else 
		{
			// FREE( temp );
			DEBUGOUT( "not suppost file type\n");
			// if( !UnCompress( fname, &temp, (LPDWORD)&FileSize ) ) {
			// 	// �i���s?
			// 	throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTFORMAT );
			// }
			// // ?������?���{
			// ::memcpy( &header, temp, sizeof(NESHEADER) );
		}

		// Since the zip/fds/nes is defrosted and raw, now apply the patch
		// if( Config.emulator.bAutoIPS ) 
		// {
		// 	LPBYTE	ipstemp = NULL;
		// 	if( !(ipstemp = (LPBYTE)::malloc( FileSize )) ) 
		// 	{
		// 		//�ٖ@�\?����
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
		// ����NES����
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
				// NES����??��
				throw	CApp::GetErrorString( IDS_ERROR_INVALIDNESHEADER );
			}
			
			// PRG BANK
			if( !(lpPRG = (LPBYTE)malloc( PRGsize )) ) {
				// �ٖ@�\?����
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
					// CHR bank �k���c
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
			// ��?�召
			diskno = header.PRG_PAGE_SIZE;

			if( FileSize < (16+65500*diskno) ) 
			{
				// ��?�召?��
				throw	CApp::GetErrorString( IDS_ERROR_ILLEGALDISKSIZE );
			}
			if( diskno > 8 ) 
			{
				// 8�ʈȏ�I��?�s�x��
				throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTDISK );
			}

			ZEROMEMORY( &header, sizeof(NESHEADER) );

			// �����M?
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
			// ����??�p
			if( !(lpDisk = (LPBYTE)malloc( PRGsize )) ) {
				
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			// CHR BANK
			lpCHR = NULL;

			::memcpy( lpPRG, &header, sizeof(NESHEADER) );
			::memcpy( lpPRG+sizeof(NESHEADER), temp+sizeof(NESHEADER), 65500*(LONG)diskno );
			// �w�萔���I�d�ʈʒu
			ZEROMEMORY( lpDisk, PRGsize );
//			memcpy( lpDisk, &header, sizeof(NESHEADER) );
//			memcpy( lpDisk+sizeof(NESHEADER), temp+sizeof(NESHEADER), PRGsize-sizeof(NESHEADER) );

			lpPRG[0] = 'F';
			lpPRG[1] = 'D';
			lpPRG[2] = 'S';
			lpPRG[3] = 0x1A;
			lpPRG[4] = (BYTE)diskno;

			//DISKSYSTEM BIOS�I��?
			string	Path = CPathlib::MakePathExt( CApp::GetModulePath(), "DISKSYS", "ROM" );
#ifndef READ_WRITE_USE_POSIX
			if( !(fp = fopen( Path.c_str(), "rb" )) ) {
				// DISKSYS.ROM�v�L
				throw	CApp::GetErrorString( IDS_ERROR_NODISKBIOS );
			}

			::fseek( fp, 0, SEEK_END );
			FileSize = ::ftell( fp );
			::fseek( fp, 0, SEEK_SET );
			if( FileSize < 17 ) {
				// ��������
				throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
			}
			if( !(bios = (LPBYTE)malloc( FileSize )) ) 
			{
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			if( fread( bios, FileSize, 1, fp ) != 1 ) 
			{
				// ����?�掸?
				throw	CApp::GetErrorString( IDS_ERROR_READ );
			}
			FCLOSE( fp );
#else
			fp = open( Path.c_str(), O_RDONLY );
			if( fp <= 0 ) 
			{
				DEBUGOUT( "err no:%d\n",errno);
				// DISKSYS.ROM�v�L
				throw	CApp::GetErrorString( IDS_ERROR_NODISKBIOS );
			}

			FileSize = lseek( fp, 0, SEEK_END );
			//FileSize = ::ftell( fp );
			lseek( fp, 0, SEEK_SET );
			if( FileSize < 17 ) {
				// ��������
				throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
			}
			if( !(bios = (LPBYTE)malloc( FileSize )) ) 
			{
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			if( read(fp, bios, FileSize ) != FileSize ) 
			{
				// ����?�掸?
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
			// NES�`��BIOS
				::memcpy( lpDiskBios, bios+0x6010, 8*1024 );
			} 
			else 
			{
			// ��BIOS
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

			// ����?
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
			// �i���s�x��
			throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTFORMAT );
		}

		// ?��H�a/������
		{
			string	tempstr;
			tempstr = CPathlib::SplitPath( fname );
			::strcpy( path, tempstr.c_str() );
			DEBUGOUT( "path:%s\n",path);
			tempstr = CPathlib::SplitFname( fname );
			::strcpy( name, tempstr.c_str() );
			DEBUGOUT( "name:%s\n",name);
			//���n�������i�����H�a�j
			::strcpy( fullpath, fname );
		}
		DEBUGOUT( "read settting ...\n");
		// �f��?�u
		if( !bNSF ) 
		{
			mapper = (header.control1>>4)|(header.control2&0xF0);
			crc = crcall = crcvrom = 0;
			DEBUGOUT( "mapper:%d\n",mapper);
			if( mapper != 20 ) {
				// PRG crc�I?�Z�i�^NesToy�IPRG CRC�����j
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
		// ?�팴���I???��
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
		// ��?�s�z�o?��ʕ�???�c�i^^�G
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

		//?�������m??
		throw	CApp::GetErrorString( IDS_ERROR_UNKNOWN );
#endif	// !_DEBUG
	}
}

//
// ��?��
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
// ROM����??
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
// ROM�������I??�i��?��?PAL�j
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

