
/////////////////////////////////////////////////////////////////////
// Mapper 32
void NES_mapper32::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }

  patch = 0;

  if(parent_NES->crc32() == 0xc0fed437) // Major League
  {
    set_mirroring(0,0,0,0);
    patch = 1;
  }

  if(parent_NES->crc32() == 0xfd3fc292) // Ai Sensei no Oshiete - Watashi no Hoshi
  {
    set_CPU_banks(30,31,30,31);
  }

  regs[0] = 0;
}

void NES_mapper32::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xF000)
  {
    case 0x8000:
      {
        if(regs[0] & 0x02)
        {
          set_CPU_bank6(data);
        }
        else
        {
          set_CPU_bank4(data);
        }
      }
      break;

    case 0x9000:
      {
        if(data & 0x01)
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
        else
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
        regs[0] = data;
      }
      break;

    case 0xA000:
      {
        set_CPU_bank5(data);
      }
      break;
  }

  switch(addr & 0xF007)
  {
    case 0xB000:
      {
        set_PPU_bank0(data);
      }
      break;

    case 0xB001:
      {
        set_PPU_bank1(data);
      }
      break;

    case 0xB002:
      {
        set_PPU_bank2(data);
      }
      break;

    case 0xB003:
      {
        set_PPU_bank3(data);
      }
      break;

    case 0xB004:
      {
        set_PPU_bank4(data);
      }
      break;

    case 0xB005:
      {
        set_PPU_bank5(data);
      }
      break;

    case 0xB006:
      {
        if ((patch == 1) && (data & 0x40))
        {
          set_mirroring(0,0,0,1);
        }
        set_PPU_bank6(data);
      }
      break;

    case 0xB007:
      {
        if ((patch == 1) && (data & 0x40))
        {
          set_mirroring(0,0,0,0);
        }
        set_PPU_bank7(data);
      }
      break;
  }
}
/////////////////////////////////////////////////////////////////////

