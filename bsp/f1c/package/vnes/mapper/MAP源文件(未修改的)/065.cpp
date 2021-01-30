
/////////////////////////////////////////////////////////////////////
// Mapper 65
void NES_mapper65::Reset()
{
  patch = 0;

  // for Kaiketsu Yanchamaru 3
  if(parent_NES->crc32() == 0xe30b7f64) // Kaiketsu Yanchamaru 3 - Taiketsu! Zouringen
  {
    patch = 1;
  }

  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }

  irq_enabled = 0;
  irq_counter = 0;
}

void NES_mapper65::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x8000:
      {
        set_CPU_bank4(data);
      }
      break;

    case 0x9000:
      {
        if(!patch)
        {
          if(data & 0x40)
          {
            set_mirroring(NES_PPU::MIRROR_VERT);
          }
          else
          {
            set_mirroring(NES_PPU::MIRROR_HORIZ);
          }
        }
      }
      break;

    case 0x9001:
      {
        if(patch)
        {
          if(data & 0x80)
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

    case 0x9003:
      {
        if(!patch)
        {
          irq_enabled = data & 0x80;
        }
      }
      break;

    case 0x9004:
      {
        if(!patch)
        {
          irq_counter = irq_latch;
        }
      }
      break;

    case 0x9005:
      {
        if(patch)
        {
          irq_counter = (uint8)(data << 1);
          irq_enabled = data;
        }
        else
        {
          irq_latch = (irq_latch & 0x00FF) | ((uint32)data << 8);
        }
      }
      break;

    case 0x9006:
      {
        if(patch)
        {
          irq_enabled = 1;
        }
        else
        {
          irq_latch = (irq_latch & 0xFF00) | data;
        }
      }
      break;

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
        set_PPU_bank6(data);
      }
      break;

    case 0xB007:
      {
        set_PPU_bank7(data);
      }
      break;

    case 0xA000:
      {
        set_CPU_bank5(data);
      }
      break;

    case 0xC000:
      {
        set_CPU_bank6(data);
      }
      break;
  }
}

void NES_mapper65::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if(patch)
    {
      if(irq_counter == 0)
      {
        parent_NES->cpu->DoIRQ();
        irq_enabled = 0;
      }
      else
      {
        irq_counter--;
      }
    }
    else
    {
      if(irq_counter <= 113)
      {
        parent_NES->cpu->DoIRQ();
        irq_enabled = 0;
        irq_counter = 0xFFFF;
      }
      else
      {
        irq_counter -= 113;
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////

