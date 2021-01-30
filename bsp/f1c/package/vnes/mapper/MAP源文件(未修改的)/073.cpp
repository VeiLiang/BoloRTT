
/////////////////////////////////////////////////////////////////////
// Mapper 73
void NES_mapper73::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  irq_counter = 0;
  irq_enabled = 0;
}

void NES_mapper73::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x8000:
      {
        irq_counter = (irq_counter & 0xFFF0) | (data & 0x0F);
      }
      break;

    case 0x9000:
      {
        irq_counter = (irq_counter & 0xFF0F) | ((data & 0x0F) << 4);
      }
      break;

    case 0xA000:
      {
        irq_counter = (irq_counter & 0xF0FF) | ((data & 0x0F) << 8);
      }
      break;

    case 0xB000:
      {
        irq_counter = (irq_counter & 0x0FFF) | ((data & 0x0F) << 12);
      }
      break;

    case 0xC000:
      {
        irq_enabled = data;
      }
      break;

    case 0xF000:
      {
        set_CPU_bank4(data*2+0);
        set_CPU_bank5(data*2+1);
      }
      break;
  }
}

void NES_mapper73::HSync(uint32 scanline)
{
  if(irq_enabled & 0x02)
  {
    if(irq_counter > 0xFFFF - 114)
    {
      parent_NES->cpu->DoIRQ();
      irq_enabled = 0;
    }
    else
    {
      irq_counter += 114;
    }
  }
}
/////////////////////////////////////////////////////////////////////

