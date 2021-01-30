
/////////////////////////////////////////////////////////////////////
// Mapper 3
void NES_mapper3::Reset()
{
  // set CPU bank pointers
  if(num_8k_ROM_banks > 2)
  {
    set_CPU_banks(0,1,2,3);
  }
  else
  {
    set_CPU_banks(0,1,0,1);
  }

  // set VROM banks
  set_PPU_banks(0,1,2,3,4,5,6,7);
}

void NES_mapper3::MemoryWrite(uint32 addr, uint8 data)
{
  uint32 base;

  data &= (num_1k_VROM_banks>>1)-1;

  base = ((uint32)data) << 3;
  set_PPU_banks(base+0,base+1,base+2,base+3,base+4,base+5,base+6,base+7);
}
/////////////////////////////////////////////////////////////////////

