
/////////////////////////////////////////////////////////////////////
// Mapper 4
// much of this is based on the DarcNES source. thanks, nyef :)
void NES_mapper4::Reset()
{
  patch = 0;

  if(parent_NES->crc32() == 0xdebea5a6 || // Ninja Ryukenden 2 - Ankoku no Jashin Ken
     parent_NES->crc32() == 0xc5fea9f2)   // Dai2Ji - Super Robot Taisen
  {
    patch = 1;
  }

  if(parent_NES->crc32() == 0xd7a97b38)   // Chou Jinrou Senki - Warwolf
  {
    patch = 2;
  }

  if(parent_NES->crc32() == 0xeb2dba63)   // VS TKO Boxing
  {
    patch = 3;
    vs_index = 0;
  }

  if(parent_NES->crc32() == 0x135adf7c)   // VS Atari RBI Baseball
  {
    patch = 4;
    vs_index = 0;
  }

  // clear registers FIRST!!!
  for(int i = 0; i < 8; i++) regs[i] = 0x00;

  // set CPU bank pointers
  prg0 = 0;
  prg1 = 1;
  MMC3_set_CPU_banks();

  // set VROM banks
  if(num_1k_VROM_banks)
  {
    chr01 = 0;
    chr23 = 2;
    chr4  = 4;
    chr5  = 5;
    chr6  = 6;
    chr7  = 7;
    MMC3_set_PPU_banks();
  }
  else
  {
    chr01 = chr23 = chr4 = chr5 = chr6 = chr7 = 0;
  }

  irq_enabled = 0;
  irq_counter = 0;
  irq_latch = 0;
}

uint8 NES_mapper4::MemoryReadLow(uint32 addr)
{
  if(patch == 3)
  {
    // VS TKO Boxing security
    if(addr == 0x5E00)
    {
      vs_index = 0;
      return 0x00;
    }
    else if(addr == 0x5E01)
    {
      uint8 security_data[32] = 
      {
        0xff, 0xbf, 0xb7, 0x97, 0x97, 0x17, 0x57, 0x4f,
        0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90, 0x94, 0x14,
        0x56, 0x4e, 0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90,
        0xd4, 0x5c, 0x3e, 0x26, 0x87, 0x83, 0x13, 0x00
      };
      return security_data[(vs_index++) & 0x1F];
    }
  }
  else if(patch == 4)
  {
    // VS Atari RBI Baseball security
    if(addr == 0x5E00)
    {
      vs_index = 0;
      return 0xFF;
    }
    else if(addr == 0x5E01)
    {
      switch(vs_index++)
      {
        case 0x09:
          return 0x6F;
        default:
          return 0xB4;
      }
    }
  }
  return  (uint8)(addr >> 8);
}

void NES_mapper4::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xE001)
  {
    case 0x8000:
      {
/*
        regs[0] = data;
        MMC3_set_PPU_banks();
        MMC3_set_CPU_banks();
*/
		uint8 old = regs[0];
		regs[0] = data;
		
		if ((old & 0x40) != (data & 0x40)) {
			MMC3_set_CPU_banks();
		}
		if ((old & 0x80) != (data & 0x80)) {
			MMC3_set_PPU_banks();
		}
      }
      break;

    case 0x8001:
      {
        uint32 bank_num;

        regs[1] = data;
        bank_num = regs[1];

        switch(regs[0] & 0x07)
        {
          case 0x00:
            {
              //if(num_1k_VROM_banks)
              {
                bank_num &= 0xfe;
				if (chr01 != bank_num) {
					chr01 = bank_num;
					MMC3_set_PPU_banks();
				}
              }
            }
            break;

          case 0x01:
            {
              //if(num_1k_VROM_banks)
              {
                bank_num &= 0xfe;
				if (chr23 != bank_num) {
					chr23 = bank_num;
					MMC3_set_PPU_banks();
				}
              }
            }
            break;

          case 0x02:
            {
              //if(num_1k_VROM_banks)
              {
				if (chr4 != bank_num) {
					chr4 = bank_num;
					MMC3_set_PPU_banks();
				}
              }
            }
            break;

          case 0x03:
            {
              //if(num_1k_VROM_banks)
              {
				if (chr5 != bank_num) {
					chr5 = bank_num;
					MMC3_set_PPU_banks();
				}
              }
            }
            break;

          case 0x04:
            {
              //if(num_1k_VROM_banks)
              {
				if (chr6 != bank_num) {
					chr6 = bank_num;
					MMC3_set_PPU_banks();
				}
              }
            }
            break;

          case 0x05:
            {
              //if(num_1k_VROM_banks)
              {
				if (chr7 != bank_num) {
					chr7 = bank_num;
					MMC3_set_PPU_banks();
				}
              }
            }
            break;

          case 0x06:
            {
			  if (prg0 != bank_num) {
				  prg0 = bank_num;
				  MMC3_set_CPU_banks();
			  }
            }
            break;

          case 0x07:
            {
			  if (prg1 != bank_num) {
				  prg1 = bank_num;
				  MMC3_set_CPU_banks();
			  }
            }
            break;
        }
      }
      break;

    case 0xA000:
      {
        regs[2] = data;

        if(data & 0x40)
        {
          LOG("MAP4 MIRRORING: 0x40 ???" << endl);
        }

        if(parent_NES->ROM->get_mirroring() != NES_PPU::MIRROR_FOUR_SCREEN)
        {
          if(data & 0x01)
          {
            set_mirroring(NES_PPU::MIRROR_HORIZ);
          }
          else
          {
            set_mirroring(NES_PPU::MIRROR_VERT);
          }
        }
      }
      break;

    case 0xA001:
      {
        regs[3] = data;

        if(data & 0x80)
        {
          // enable save RAM $6000-$7FFF
        }
        else
        {
          // disable save RAM $6000-$7FFF
        }
      }
      break;

    case 0xC000:
      regs[4] = data;
      irq_counter = regs[4];
      break;

    case 0xC001:
      regs[5] = data;
      irq_latch = regs[5];
      break;

    case 0xE000:
      regs[6] = data;
      irq_enabled = 0;
      break;

    case 0xE001:
      regs[7] = data;
      irq_enabled = 1;
      break;

    default:
      LOG("MAP4: UNKNOWN: " << HEX(addr,4) << " = " << HEX(data) << endl);
      break;

  }
}

void NES_mapper4::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if((scanline >= 0) && (scanline <= 239))
    {
      if(parent_NES->ppu->spr_enabled() || parent_NES->ppu->bg_enabled())
      {
        if(patch == 1)
        {
          if(!(--irq_counter))
          {
            irq_counter = irq_latch;
            parent_NES->cpu->DoIRQ();
          }
        }
        else if(patch == 2)
        {
          if(--irq_counter == 0x01)
          {
            irq_counter = irq_latch;
            parent_NES->cpu->DoIRQ();
          }
        }
        else
        {
          if(!(irq_counter--))
          {
            irq_counter = irq_latch;
            parent_NES->cpu->DoIRQ();
          }
        }
      }
    }
  }
}

void NES_mapper4::MMC3_set_CPU_banks()
{
  if(prg_swap())
  {
    set_CPU_banks(num_8k_ROM_banks-2,prg1,prg0,num_8k_ROM_banks-1);
  }
  else
  {
    set_CPU_banks(prg0,prg1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);
  }
}

void NES_mapper4::MMC3_set_PPU_banks()
{
  if(num_1k_VROM_banks)
  {
    if(chr_swap())
    {
      set_PPU_banks(chr4,chr5,chr6,chr7,chr01,chr01+1,chr23,chr23+1);
    }
    else
    {
      set_PPU_banks(chr01,chr01+1,chr23,chr23+1,chr4,chr5,chr6,chr7);
    }
  }
  else
  {
    if(chr_swap())
    {
      set_VRAM_bank(0, chr4);
      set_VRAM_bank(1, chr5);
      set_VRAM_bank(2, chr6);
      set_VRAM_bank(3, chr7);
      set_VRAM_bank(4, chr01+0);
      set_VRAM_bank(5, chr01+1);
      set_VRAM_bank(6, chr23+0);
      set_VRAM_bank(7, chr23+1);
    }
    else
    {
      set_VRAM_bank(0, chr01+0);
      set_VRAM_bank(1, chr01+1);
      set_VRAM_bank(2, chr23+0);
      set_VRAM_bank(3, chr23+1);
      set_VRAM_bank(4, chr4);
      set_VRAM_bank(5, chr5);
      set_VRAM_bank(6, chr6);
      set_VRAM_bank(7, chr7);
    }
  }
}

#define MAP4_ROM(ptr)  (((ptr)-parent_NES->ROM->get_ROM_banks())  >> 13)
#define MAP4_VROM(ptr) (((ptr)-parent_NES->ROM->get_VROM_banks()) >> 10)
#define MAP4_VRAM(ptr) (((ptr)-parent_NES->ppu->get_patt()) >> 10)

void NES_mapper4::SNSS_fixup() // HACK HACK HACK HACK
{
  NES_6502::Context context;
  parent_NES->cpu->GetContext(&context);

  prg0 = MAP4_ROM(context.mem_page[prg_swap() ? 6 : 4]);
  prg1 = MAP4_ROM(context.mem_page[5]);
  if(num_1k_VROM_banks)
  {
    if(chr_swap())
    {
      chr01 = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[4]);
      chr23 = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[6]);
      chr4  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[0]);
      chr5  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[1]);
      chr6  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[2]);
      chr7  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[3]);
    }
    else
    {
      chr01 = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[0]);
      chr23 = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[2]);
      chr4  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[4]);
      chr5  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[5]);
      chr6  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[6]);
      chr7  = MAP4_VROM(parent_NES->ppu->PPU_VRAM_banks[7]);
    }
  }
  else
  {
    if(chr_swap())
    {
      chr01 = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[4]);
      chr23 = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[6]);
      chr4  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[0]);
      chr5  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[1]);
      chr6  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[2]);
      chr7  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[3]);
    }
    else
    {
      chr01 = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[0]);
      chr23 = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[2]);
      chr4  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[4]);
      chr5  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[5]);
      chr6  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[6]);
      chr7  = MAP4_VRAM(parent_NES->ppu->PPU_VRAM_banks[7]);
    }
  }
}
/////////////////////////////////////////////////////////////////////

