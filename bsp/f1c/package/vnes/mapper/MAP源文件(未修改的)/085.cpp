
/////////////////////////////////////////////////////////////////////
// Mapper 85
void NES_mapper85::Reset()
{
  // Init ExSound
  parent_NES->apu->SelectExSound(2);

  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }
  else
  {
    set_VRAM_bank(0, 0);
    set_VRAM_bank(1, 0);
    set_VRAM_bank(2, 0);
    set_VRAM_bank(3, 0);
    set_VRAM_bank(4, 0);
    set_VRAM_bank(5, 0);
    set_VRAM_bank(6, 0);
    set_VRAM_bank(7, 0);
  }

  irq_enabled = 0;
  irq_counter = 0;
  irq_latch = 0;
}

void NES_mapper85::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xF038)
  {
    case 0x8000:
      {
        set_CPU_bank4(data);
      }
      break;

    case 0x8008:
    case 0x8010:
      {
        set_CPU_bank5(data);
      }
      break;

    case 0x9000:
      {
        set_CPU_bank6(data);
      }
      break;

    case 0x9010:
    case 0x9030:
      {
        parent_NES->apu->ExWrite(addr, data);
      }
      break;

    case 0xA000:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank0(data);
        }
        else
        {
          set_VRAM_bank(0, data);
        }
      }
      break;

    case 0xA008:
    case 0xA010:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank1(data);
        }
        else
        {
          set_VRAM_bank(1, data);
        }
      }
      break;

    case 0xB000:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank2(data);
        }
        else
        {
          set_VRAM_bank(2, data);
        }
      }
      break;

    case 0xB008:
    case 0xB010:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank3(data);
        }
        else
        {
          set_VRAM_bank(3, data);
        }
      }
      break;

    case 0xC000:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank4(data);
        }
        else
        {
          set_VRAM_bank(4, data);
        }
      }
      break;

    case 0xC008:
    case 0xC010:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank5(data);
        }
        else
        {
          set_VRAM_bank(5, data);
        }
      }
      break;

    case 0xD000:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank6(data);
        }
        else
        {
          set_VRAM_bank(6, data);
        }
      }
      break;

    case 0xD008:
    case 0xD010:
      {
        if(num_1k_VROM_banks)
        {
          set_PPU_bank7(data);
        }
        else
        {
          set_VRAM_bank(7, data);
        }
      }
      break;

    case 0xE000:
      {
        data &= 0x03;
        if(data == 0x00)
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
        else if(data == 0x01)
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
        else if(data == 0x02)
        {
          set_mirroring(0,0,0,0);
        }
        else
        {
          set_mirroring(1,1,1,1);
        }
      }
      break;

    case 0xE008:
    case 0xE010:
      {
        irq_latch = data;
      }
      break;

    case 0xF000:
      {
        irq_enabled = data & 0x03;
        if(irq_enabled & 0x02)
        {
          irq_counter = irq_latch;
        }
      }
      break;

    case 0xF008:
    case 0xF010:
      {
        irq_enabled = (irq_enabled & 0x01) * 3;
      }
      break;
  }
}

void NES_mapper85::HSync(uint32 scanline)
{
  if(irq_enabled & 0x02)
  {
    if(irq_counter == 0xFF)
    {
      parent_NES->cpu->DoIRQ();
      irq_counter = irq_latch;
    }
    else
    {
      irq_counter++;
    }
  }
}
/////////////////////////////////////////////////////////////////////

