#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 17
void NES_mapper17::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }

  irq_enabled = 0;
  irq_counter = 0;
  irq_latch = 0;
}

void NES_mapper17::MemoryWriteLow(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x42FE:
      {
        if((data & 0x10) == 0)
        {
          set_mirroring(0,0,0,0);
        }
        else
        {
          set_mirroring(1,1,1,1);
        }
      }
      break;

    case 0x42FF:
      {
        if((data & 0x10) == 0)
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
        else
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
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
        irq_latch = (irq_latch & 0xFF00) | data;
      }
      break;

    case 0x4503:
      {
        irq_latch = (irq_latch & 0x00FF) | ((uint32)data << 8);
        irq_counter = irq_latch;
        irq_enabled = 1;
      }
      break;

    case 0x4504:
      {
        set_CPU_bank4(data);
      }
      break;

    case 0x4505:
      {
        set_CPU_bank5(data);
      }
      break;

    case 0x4506:
      {
        set_CPU_bank6(data);
      }
      break;

    case 0x4507:
      {
        set_CPU_bank7(data);
      }
      break;

    case 0x4510:
      {
        set_PPU_bank0(data);
      }
      break;

    case 0x4511:
      {
        set_PPU_bank1(data);
      }
      break;

    case 0x4512:
      {
        set_PPU_bank2(data);
      }
      break;

    case 0x4513:
      {
        set_PPU_bank3(data);
      }
      break;

    case 0x4514:
      {
        set_PPU_bank4(data);
      }
      break;

    case 0x4515:
      {
        set_PPU_bank5(data);
      }
      break;

    case 0x4516:
      {
        set_PPU_bank6(data);
      }
      break;

    case 0x4517:
      {
        set_PPU_bank7(data);
      }
      break;
  }
}

void NES_mapper17::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if(irq_counter >= 0xFFFF - 113)
    {
      parent_NES->cpu->DoIRQ();
      irq_counter = 0;
      irq_enabled = 0;
    }
    else
    {
      irq_counter += 113;
    }
  }
}
/////////////////////////////////////////////////////////////////////

#endif
