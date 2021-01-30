
/////////////////////////////////////////////////////////////////////
// Mapper 7
void NES_mapper7::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,2,3);
}

void NES_mapper7::MemoryWrite(uint32 addr, uint8 data)
{
  uint32 bank;

  bank = (data & 0x07) << 2;
  set_CPU_banks(bank+0,bank+1,bank+2,bank+3);

  if(data & 0x10)
  {
    set_mirroring(1,1,1,1);
  }
  else
  {
    set_mirroring(0,0,0,0);
  }
}
/////////////////////////////////////////////////////////////////////

