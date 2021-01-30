#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 71
void NES_mapper71::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);
}

void NES_mapper71::MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
  set_CPU_bank4(data*2+0);
  set_CPU_bank5(data*2+1);
}

void NES_mapper71::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xF000)
  {
    case 0x9000:
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

    case 0xC000:
    case 0xD000:
    case 0xE000:
    case 0xF000:
      {
        set_CPU_bank4(data*2+0);
        set_CPU_bank5(data*2+1);
      }
      break;
  }
}
/////////////////////////////////////////////////////////////////////

#endif
