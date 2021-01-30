
/////////////////////////////////////////////////////////////////////
// Mapper 15
void NES_mapper15::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,2,3);
}

void NES_mapper15::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x8000:
      {
        if(data & 0x80)
        {
          set_CPU_bank4((data & 0x3F)*2+1);
          set_CPU_bank5((data & 0x3F)*2+0);
          set_CPU_bank6((data & 0x3F)*2+3);
          set_CPU_bank7((data & 0x3F)*2+2);
        }
        else
        {
          set_CPU_bank4((data & 0x3F)*2+0);
          set_CPU_bank5((data & 0x3F)*2+1);
          set_CPU_bank6((data & 0x3F)*2+2);
          set_CPU_bank7((data & 0x3F)*2+3);
        }
        if(data & 0x40)
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
        else
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
      }
      break;

    case 0x8001:
      {
        if(data & 0x80)
        {
          set_CPU_bank6((data & 0x3F)*2+1);
          set_CPU_bank7((data & 0x3F)*2+0);
        }
        else
        {
          set_CPU_bank6((data & 0x3F)*2+0);
          set_CPU_bank7((data & 0x3F)*2+1);
        }
      }
      break;

    case 0x8002:
      {
        if(data & 0x80)
        {
          set_CPU_bank4((data & 0x3F)*2+1);
          set_CPU_bank5((data & 0x3F)*2+1);
          set_CPU_bank6((data & 0x3F)*2+1);
          set_CPU_bank7((data & 0x3F)*2+1);
        }
        else
        {
          set_CPU_bank4((data & 0x3F)*2);
          set_CPU_bank5((data & 0x3F)*2);
          set_CPU_bank6((data & 0x3F)*2);
          set_CPU_bank7((data & 0x3F)*2);
        }
      }
      break;

    case 0x8003:
      {
        if(data & 0x80)
        {
          set_CPU_bank6((data & 0x3F)*2+1);
          set_CPU_bank7((data & 0x3F)*2+0);
        }
        else
        {
          set_CPU_bank6((data & 0x3F)*2+0);
          set_CPU_bank7((data & 0x3F)*2+1);
        }
        if(data & 0x40)
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
/////////////////////////////////////////////////////////////////////

