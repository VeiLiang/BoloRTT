#include "nes_mapper.h"
//////////////////////////////////////////////////////////////////////////
// Mapper074  Nintendo MMC3                                             //
//////////////////////////////////////////////////////////////////////////

void MAP74_MMC3_set_PPU_banks()
{
  if(num_1k_VROM_banks)
  {
    if(chr_swap())
    {
      set_PPU_banks(MAPx->chr4,MAPx->chr5,MAPx->chr6,MAPx->chr7,MAPx->chr01,MAPx->chr01+1,MAPx->chr23,MAPx->chr23+1);
    }
    else
    {
      set_PPU_banks(MAPx->chr01,MAPx->chr01+1,MAPx->chr23,MAPx->chr23+1,MAPx->chr4,MAPx->chr5,MAPx->chr6,MAPx->chr7);
    }
  }
  else
  {
    if(chr_swap())
    {
      set_VRAM_bank(0, MAPx->chr4);
      set_VRAM_bank(1, MAPx->chr5);
      set_VRAM_bank(2, MAPx->chr6);
      set_VRAM_bank(3, MAPx->chr7);
      set_VRAM_bank(4, MAPx->chr01+0);
      set_VRAM_bank(5, MAPx->chr01+1);
      set_VRAM_bank(6, MAPx->chr23+0);
      set_VRAM_bank(7, MAPx->chr23+1);
    }
    else
    {
      set_VRAM_bank(0, MAPx->chr01+0);
      set_VRAM_bank(1, MAPx->chr01+1);
      set_VRAM_bank(2, MAPx->chr23+0);
      set_VRAM_bank(3, MAPx->chr23+1);
      set_VRAM_bank(4, MAPx->chr4);
      set_VRAM_bank(5, MAPx->chr5);
      set_VRAM_bank(6, MAPx->chr6);
      set_VRAM_bank(7, MAPx->chr7);
    }
  }
}
void MAP74_MMC3_set_CPU_banks()
{
  if(prg_swap())
  {
    set_CPU_banks(-2,MAPx->prg1,MAPx->prg0,-1);
  }
  else
  {
    set_CPU_banks(MAPx->prg0,MAPx->prg1,-2,-1);
  }
}
void MAP74_Reset()
{
	// clear registers FIRST!!!
    int i;
  for( i = 0; i < 8; i++) MAPx->regs[i] = 0x00;
  // set CPU bank pointers
  MAPx->prg0 = 0;
  MAPx->prg1 = 1;
  
  MAP74_MMC3_set_CPU_banks();

	MAPx->chr01 = 0;
    MAPx->chr23 = 2;
    MAPx->chr4  = 4;
    MAPx->chr5  = 5;
    MAPx->chr6  = 6;
    MAPx->chr7  = 7;
	MAP4_MMC3_set_PPU_banks();

	MAPx->irq_enabled = 0;
    MAPx->irq_counter = 0;
    MAPx->irq_latch = 0;

	// DWORD	crc = nes->rom->GetPROM_CRC();

	// patch = 0;
	// if( crc == 0x37ae04a8 ) {
	// 	patch = 1;
	// 	nes->SetRenderMethod( NES::TILE_RENDER );
	// }
}

void	MAP74_MemoryWrite( uint16 addr, uint8 data )
{
//DEBUGOUT( "MPRWR A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );

	switch( addr & 0xE001 ) {
		case	0x8000:
			MAPx->regs[0] = data;
			MAP74_MMC3_set_CPU_banks();
			MAP4_MMC3_set_PPU_banks();
			break;
		case	0x8001:
			MAPx->regs[1] = data;

			switch( MAPx->regs[0] & 0x07 ) {
				case	0x00:
					MAPx->chr01 = data & 0xFE;
					MAP4_MMC3_set_PPU_banks();
					break;
				case	0x01:
					MAPx->chr23 = data & 0xFE;
					MAP4_MMC3_set_PPU_banks();
					break;
				case	0x02:
					MAPx->chr4 = data;
					MAP4_MMC3_set_PPU_banks();
					break;
				case	0x03:
					MAPx->chr5 = data;
					MAP4_MMC3_set_PPU_banks();
					break;
				case	0x04:
					MAPx->chr6 = data;
					MAP4_MMC3_set_PPU_banks();
					break;
				case	0x05:
					MAPx->chr7 = data;
					MAP4_MMC3_set_PPU_banks();
					break;
				case	0x06:
					MAPx->prg0 = data;
					MAP4_MMC3_set_CPU_banks();
					break;
				case	0x07:
					MAPx->prg1 = data;
					MAP4_MMC3_set_CPU_banks();
					break;
			}
			break;
		case	0xA000:
			MAPx->regs[2] = data;
			if ( !(RomHeader->flags_1&0x8 ))
            {
            if(data & 0x01)
            {
                    set_mirroring(0,0,1,1);//Ë®Æ½¾µÏñ
                    //set_mirroring(NES_PPU::MIRROR_HORIZ);
            }
            else
            {
                    set_mirroring(0,1,0,1);//´¹Ö±¾µÏñ
                    //set_mirroring(NES_PPU::MIRROR_VERT);
            }
            }
			break;
		case	0xA001:
			MAPx->regs[3] = data;
			break;
		case	0xC000:
			MAPx->regs[4] = data;
            MAPx->irq_counter = MAPx->regs[4];
			break;
		case	0xC001:
			MAPx->regs[5] = data;
            MAPx->irq_latch = MAPx->regs[5];
			break;
		case	0xE000:
			MAPx->regs[6] = data;
            MAPx->irq_enabled = 0;
			break;
		case	0xE001:
			MAPx->regs[7] = data;
            MAPx->irq_enabled = 1;
			break;
	}	
	
}

void	MAP74_HSync( int scanline )
{
  if(MAPx->irq_enabled)
  {
    if((scanline >= 0) && (scanline <= 239)&&( spr_enabled() || bg_enabled()))
    {
      //if(parent_NES->ppu->spr_enabled() || parent_NES->ppu->bg_enabled())
      //{
//        if(patch == 1)
//        {
//          if(!(--MAPx->irq_counter))
//          {
//            MAPx->irq_counter = MAPx->irq_latch;
//            parent_NES->cpu->DoIRQ();
//          }
//        }
//        else if(patch == 2)
//        {
//          if(--MAPx->irq_counter == 0x01)
//          {
//            MAPx->irq_counter = MAPx->irq_latch;
//            parent_NES->cpu->DoIRQ();
//          }
//        }
//        else
        {
          if(!(MAPx->irq_counter--))
          {
            MAPx->irq_counter = MAPx->irq_latch;
            CPU_IRQ;
          }
        }
      }
    }
}


void MAP74_Init()
{
	NES_Mapper->Reset = MAP74_Reset;
	NES_Mapper->Write = MAP74_MemoryWrite;
	NES_Mapper->HSync = MAP74_HSync;   
}