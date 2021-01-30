
/////////////////////////////////////////////////////////////////////
// Mapper 33
void NES_mapper33::Reset()
{
  patch = 0;

  if(parent_NES->crc32() == 0x5e9bc161 || // Akira
     parent_NES->crc32() == 0xecdbafa4 || // Bakushou!! Jinsei Gekijou
     parent_NES->crc32() == 0x59cd0c31 || // Don Doko Don
     parent_NES->crc32() == 0x837c1342 || // Golf Ko Open
     parent_NES->crc32() == 0x42d893e4 || // Operation Wolf
     parent_NES->crc32() == 0x07ee6d8f || // Power Blazer
     parent_NES->crc32() == 0x5193fb54)   // Takeshi no Sengoku Fuuunji
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

void NES_mapper33::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x8000:
      {
        if(patch) // old #33 games
        {
          if(data & 0x40)
          {
            set_mirroring(NES_PPU::MIRROR_HORIZ);
          }
          else
          {
            set_mirroring(NES_PPU::MIRROR_VERT);
          }
          set_CPU_bank4(data & 0x1F);
        }
        else
        {
          set_CPU_bank4(data);
        }
      }
      break;

    case 0x8001:
      {
        if(patch) // old #33 games
        {
          set_CPU_bank5(data & 0x1F);
        }
        else
        {
          set_CPU_bank5(data);
        }
      }
      break;

    case 0x8002:
      {
        set_PPU_bank0(data*2+0);
        set_PPU_bank1(data*2+1);
      }
      break;

    case 0x8003:
      {
        set_PPU_bank2(data*2+0);
        set_PPU_bank3(data*2+1);
      }
      break;

    case 0xA000:
      {
        set_PPU_bank4(data);
      }
      break;

    case 0xA001:
      {
        set_PPU_bank5(data);
      }
      break;

    case 0xA002:
      {
        set_PPU_bank6(data);
      }
      break;

    case 0xA003:
      {
        set_PPU_bank7(data);
      }
      break;

    case 0xC000:
      {
        irq_counter = data;
      }
      break;

    case 0xC001:
    case 0xC002:
    case 0xE001:
    case 0xE002:
      {
        irq_enabled = data;
      }
      break;

    case 0xE000:
      {
        if (data & 0x40)
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
        else
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
      }
      break;
  }
}

void NES_mapper33::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if(parent_NES->ppu->spr_enabled() || parent_NES->ppu->bg_enabled())
    {
      if (scanline >= 0 && scanline <= 239)
      {
        if(irq_counter == 0x00)
        {
          parent_NES->cpu->DoIRQ();
          irq_counter = 0;
          irq_enabled = 0;
        }
        else
        {
          irq_counter++;
        }
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////

