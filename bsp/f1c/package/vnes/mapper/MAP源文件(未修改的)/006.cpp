
/////////////////////////////////////////////////////////////////////
// Mapper 6
void NES_mapper6::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,14,15);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }
  else
  {
    set_VRAM_bank(0, 0);
    set_VRAM_bank(0, 1);
    set_VRAM_bank(0, 2);
    set_VRAM_bank(0, 3);
    set_VRAM_bank(0, 4);
    set_VRAM_bank(0, 5);
    set_VRAM_bank(0, 6);
    set_VRAM_bank(0, 7);
  }
  parent_NES->ppu->vram_size = 0x8000;
}

void NES_mapper6::MemoryWriteLow(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x42FE:
      {
        if(data & 0x10)
        {
          set_mirroring(1,1,1,1);
        }
        else
        {
          set_mirroring(0,0,0,0);
        }
      }
      break;

    case 0x42FF:
      {
        if(data & 0x10)
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
        else
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
      }
      break;

    case 0x4501:
      {
        irq_enabled = 0;
      }
      break;

    case 0x4502:
      {
        irq_counter = (irq_counter & 0xFF00) | (uint32)data;
      }
      break;

    case 0x4503:
      {
        irq_counter = (irq_counter & 0x00FF) | ((uint32)data << 8);
        irq_enabled = 1;
      }
      break;
  }
}

void NES_mapper6::MemoryWrite(uint32 addr, uint8 data)
{
  uint8 prg_bank = (data & 0x3C) >> 2;
  uint8 chr_bank = data & 0x03;

  set_CPU_bank4(prg_bank*2+0);
  set_CPU_bank5(prg_bank*2+1);

  set_VRAM_bank(0, chr_bank * 8 + 0);
  set_VRAM_bank(1, chr_bank * 8 + 1);
  set_VRAM_bank(2, chr_bank * 8 + 2);
  set_VRAM_bank(3, chr_bank * 8 + 3);
  set_VRAM_bank(4, chr_bank * 8 + 4);
  set_VRAM_bank(5, chr_bank * 8 + 5);
  set_VRAM_bank(6, chr_bank * 8 + 6);
  set_VRAM_bank(7, chr_bank * 8 + 7);
}

void NES_mapper6::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    irq_counter += 133;
    if(irq_counter >= 0xFFFF)
    {
      parent_NES->cpu->DoIRQ();
      irq_counter = 0;
    }
  }
}
/////////////////////////////////////////////////////////////////////

