
/////////////////////////////////////////////////////////////////////
// Mapper 2
void NES_mapper2::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);
}

void NES_mapper2::MemoryWrite(uint32 addr, uint8 data)
{
  data &= num_8k_ROM_banks-1;
  set_CPU_banks(data*2,(data*2)+1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);
}
/////////////////////////////////////////////////////////////////////

