
/////////////////////////////////////////////////////////////////////
// Mapper 189
void NES_mapper189::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }

  irq_counter = 0;
  irq_latch = 0;
  irq_enabled = 0;
}

void NES_mapper189::MemoryWriteLow(uint32 addr, uint8 data)
{
  if(addr >= 0x4100 && addr <= 0x41FF)
  {
    data = (data & 0x30) >> 4;
    set_CPU_banks(data*4,data*4+1,data*4+2,data*4+3);
  }
}

void NES_mapper189::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x8000:
      {
        regs[0] = data;
      }
      break;

    case 0x8001:
      {
        switch(regs[0])
        {
          case 0x40:
            {
              set_PPU_bank0(data+0);
              set_PPU_bank1(data+1);
            }
            break;

          case 0x41:
            {
              set_PPU_bank2(data+0);
              set_PPU_bank3(data+1);
            }
            break;

          case 0x42:
            {
              set_PPU_bank4(data);
            }
            break;

          case 0x43:
            {
              set_PPU_bank5(data);
            }
            break;

          case 0x44:
            {
              set_PPU_bank6(data);
            }
            break;

          case 0x45:
            {
              set_PPU_bank7(data);
            }
            break;

          case 0x46:
            {
              set_CPU_bank6(data);
            }
            break;

          case 0x47:
            {
              set_CPU_bank5(data);
            }
            break;
        }
      }
      break;

    case 0xA000:
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
      break;

    case 0xC000:
      {
        irq_counter = data;
      }
      break;

    case 0xC001:
      {
        irq_latch = data;
      }
      break;

    case 0xE000:
      {
        irq_enabled = 0;
      }
      break;

    case 0xE001:
      {
        irq_enabled = 1;
      }
      break;
  }
}

void NES_mapper189::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if((scanline >= 0) && (scanline <= 239))
    {
      if(parent_NES->ppu->spr_enabled() || parent_NES->ppu->bg_enabled())
      {
        if(!(--irq_counter))
        {
          irq_counter = irq_latch;
          parent_NES->cpu->DoIRQ();
        }
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////

