
/////////////////////////////////////////////////////////////////////
// Mapper 69
void NES_mapper69::Reset()
{
  patch = 0;

  // for Dynamite Batman
  if(parent_NES->crc32() == 0xad28aef6) // Dynamite Batman
  {
    patch = 1;
  }

  // Init ExSound
  parent_NES->apu->SelectExSound(32);

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

void NES_mapper69::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xE000)
  {
    case 0x8000:
      {
        regs[0] = data;
      }
      break;

    case 0xA000:
      {
        switch(regs[0] & 0x0F)
        {
          case 0x00:
            {
              set_PPU_bank0(data);
            }
            break;

          case 0x01:
            {
              set_PPU_bank1(data);
            }
            break;

          case 0x02:
            {
              set_PPU_bank2(data);
            }
            break;

          case 0x03:
            {
              set_PPU_bank3(data);
            }
            break;

          case 0x04:
            {
              set_PPU_bank4(data);
            }
            break;

          case 0x05:
            {
              set_PPU_bank5(data);
            }
            break;

          case 0x06:
            {
              set_PPU_bank6(data);
            }
            break;

          case 0x07:
            {
              set_PPU_bank7(data);
            }
            break;

          case 0x08:
            {
              if(!patch)
              {
                if(!(data & 0x40))
                {
                  set_CPU_bank3(data);
                }
              }
            }
            break;

          case 0x09:
            {
              set_CPU_bank4(data);
            }
            break;

          case 0x0A:
            {
              set_CPU_bank5(data);
            }
            break;

          case 0x0B:
            {
              set_CPU_bank6(data);
            }
            break;

          case 0x0C:
            {
              data &= 0x03;
              if(data == 0)
              {
                set_mirroring(NES_PPU::MIRROR_VERT);
              }
              else if(data == 1)
              {
                set_mirroring(NES_PPU::MIRROR_HORIZ);
              }
              else if(data == 2)
              {
                set_mirroring(0,0,0,0);
              }
              else
              {
                set_mirroring(1,1,1,1);
              }
            }
            break;

          case 0x0D:
            {
              irq_enabled = data;
            }
            break;

          case 0x0E:
            {
              irq_counter = (irq_counter & 0xFF00) | data;
            }
            break;

          case 0x0F:
            {
              irq_counter = (irq_counter & 0x00FF) | (data << 8);
            }
            break;
        }
      }
      break;

    case 0xC000:
    case 0xE000:
      {
        parent_NES->apu->ExWrite(addr, data);
      }
      break;
  }
}

void NES_mapper69::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if(irq_counter <= 113)
    {
      parent_NES->cpu->DoIRQ();
      irq_counter = 0;
    }
    else
    {
      irq_counter -= 113;
    }
  }
}
/////////////////////////////////////////////////////////////////////

