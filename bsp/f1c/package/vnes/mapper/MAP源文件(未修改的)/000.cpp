
/////////////////////////////////////////////////////////////////////
// Mapper 0
void NES_mapper0::Reset()
{
  // set CPU bank pointers
  if(num_8k_ROM_banks > 2)
  {
    set_CPU_banks(0,1,2,3);
  }
  else if(num_8k_ROM_banks > 1)
  {
    set_CPU_banks(0,1,0,1);
  }
  else
  {
    set_CPU_banks(0,0,0,0);
  }

  if(num_1k_VROM_banks)
  {
    set_PPU_banks(0,1,2,3,4,5,6,7);
  }
}
/////////////////////////////////////////////////////////////////////

