#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 64
void NES_mapper64::Reset()
{
  // set CPU bank pointers
  set_CPU_bank4(num_8k_ROM_banks-1);
  set_CPU_bank5(num_8k_ROM_banks-1);
  set_CPU_bank6(num_8k_ROM_banks-1);
  set_CPU_bank7(num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }

  irq_latch = 0;
  irq_counter = 0;
  irq_enabled = 0;

  regs[0] = 0;
  regs[1] = 0;
  regs[2] = 0;
}

void NES_mapper64::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xF003)
  {
    case 0x8000:
      {
        regs[0] = data & 0x0F;
        regs[1] = data & 0x40;
        regs[2] = data & 0x80;
      }
      break;

    case 0x8001:
      {
        switch(regs[0])
        {
          case 0x00:
            {
              if(regs[2])
              {
                set_PPU_bank4(data+0);
                set_PPU_bank5(data+1);
              }
              else
              {
                set_PPU_bank0(data+0);
                set_PPU_bank1(data+1);
              }
            }
            break;

          case 0x01:
            {
              if(regs[2])
              {
                set_PPU_bank6(data+0);
                set_PPU_bank7(data+1);
              }
              else
              {
                set_PPU_bank2(data+0);
                set_PPU_bank3(data+1);
              }
            }
            break;

          case 0x02:
            {
              if(regs[2])
              {
                set_PPU_bank0(data);
              }
              else
              {
                set_PPU_bank4(data);
              }
            }
            break;

          case 0x03:
            {
              if(regs[2])
              {
                set_PPU_bank1(data);
              }
              else
              {
                set_PPU_bank5(data);
              }
            }
            break;

          case 0x04:
            {
              if(regs[2])
              {
                set_PPU_bank2(data);
              }
              else
              {
                set_PPU_bank6(data);
              }
            }
            break;

          case 0x05:
            {
              if(regs[2])
              {
                set_PPU_bank3(data);
              }
              else
              {
                set_PPU_bank7(data);
              }
            }
            break;

          case 0x06:
            {
              if(regs[1])
              {
                set_CPU_bank5(data);
              }
              else
              {
                set_CPU_bank4(data);
              }
            }
            break;

          case 0x07:
            {
              if(regs[1])
              {
                set_CPU_bank6(data);
              }
              else
              {
                set_CPU_bank5(data);
              }
            }
            break;

          case 0x08:
            {
              //if(regs[2])
              //{
              //  set_PPU_bank5(data);
              //}
              //else
              {
                set_PPU_bank1(data);
              }
            }
            break;

          case 0x09:
            {
              //if(regs[2])
              //{
              //  set_PPU_bank7(data);
              //}
              //else
              {
                set_PPU_bank3(data);
              }
            }
            break;

          case 0x0F:
            {
              if(regs[1])
              {
                set_CPU_bank4(data);
              }
              else
              {
                set_CPU_bank6(data);
              }
            }
            break;
        }
      }
      break;

    case 0xA000:
      {
        if(!(data & 0x01))
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
        else
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
      }
      break;

    case 0xC000:
      {
        irq_latch = data;
        irq_counter = irq_latch;
      }
      break;

    case 0xC001:
      {
        irq_counter = irq_latch;
      }
      break;

    case 0xE000:
      {
        irq_enabled = 0;
        irq_counter = irq_latch;
      }
      break;

    case 0xE001:
      {
        irq_enabled = 1;
        irq_counter = irq_latch;
      }
      break;
  }
}

void NES_mapper64::HSync(uint32 scanline)
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

#endif
