
/////////////////////////////////////////////////////////////////////
// Mapper 75
void NES_mapper75::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }

  regs[0] = 0;
  regs[1] = 1;
}

void NES_mapper75::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xF000)
  {
    case 0x8000:
      {
        set_CPU_bank4(data);
      }
      break;

    case 0x9000:
      {
        if(data & 0x01)
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
        else
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
        regs[0] = (regs[0] & 0x0F) | ((data & 0x02) << 3);
        set_PPU_bank0(regs[0]*4+0);
        set_PPU_bank1(regs[0]*4+1);
        set_PPU_bank2(regs[0]*4+2);
        set_PPU_bank3(regs[0]*4+3);
        regs[1] = (regs[1] & 0x0F) | ((data & 0x04) << 2);
        set_PPU_bank4(regs[1]*4+0);
        set_PPU_bank5(regs[1]*4+1);
        set_PPU_bank6(regs[1]*4+2);
        set_PPU_bank7(regs[1]*4+3);
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

    case 0xE000:
      {
        regs[0] = (regs[0] & 0x10) | (data & 0x0F);
        set_PPU_bank0(regs[0]*4+0);
        set_PPU_bank1(regs[0]*4+1);
        set_PPU_bank2(regs[0]*4+2);
        set_PPU_bank3(regs[0]*4+3);
      }
      break;

    case 0xF000:
      {
        regs[1] = (regs[1] & 0x10) | (data & 0x0F);
        set_PPU_bank4(regs[1]*4+0);
        set_PPU_bank5(regs[1]*4+1);
        set_PPU_bank6(regs[1]*4+2);
        set_PPU_bank7(regs[1]*4+3);
      }
      break;
  }
}
/////////////////////////////////////////////////////////////////////

