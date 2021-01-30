
/////////////////////////////////////////////////////////////////////
// Mapper 1
void NES_mapper1::Reset()
{
  patch = 0;

  /*
  if(parent_NES->crc32() == 0xb8747abf) // Best Play - Pro Yakyuu Special
  {
    patch = 1;
    wram_bank = 0;
    wram_flag = 0;
    wram_count = 0;
    NES_6502::Context context;
    parent_NES->cpu->GetContext(&context);
    context.mem_page[3] = wram;
    parent_NES->cpu->SetContext(&context);
    for(uint32 i = 0; i < 0x4000; i++)
    {
      wram[i] = parent_NES->ReadSaveRAM(i);
    }
  }

  if(parent_NES->crc32() == 0x9b565541) // Triathron, The
  {
    parent_NES->frame_irq_disenabled = 1;
  }
  */

  write_count = 0;
  bits = 0x00;
  regs[0] = 0x0C; // reflects initial ROM state
  regs[1] = 0x00;
  regs[2] = 0x00;
  regs[3] = 0x00;
  {
    uint32 size_in_K = num_8k_ROM_banks * 8;

    if(size_in_K == 1024)
    {
      MMC1_Size = MMC1_1024K;
    }
    else if(size_in_K == 512)
    {
      MMC1_Size = MMC1_512K;
    }
    else
    {
      MMC1_Size = MMC1_SMALL;
    }
  }
  MMC1_256K_base = 0; // use first 256K
  MMC1_swap = 0;

  if(MMC1_Size == MMC1_SMALL)
  {
    // set two high pages to last two banks
    MMC1_HI1 = num_8k_ROM_banks-2;
    MMC1_HI2 = num_8k_ROM_banks-1;
  }
  else
  {
    // set two high pages to last two banks of current 256K region
    MMC1_HI1 = (256/8)-2;
    MMC1_HI2 = (256/8)-1;
  }

  // set CPU bank pointers
  MMC1_bank1 = 0;
  MMC1_bank2 = 1;
  MMC1_bank3 = MMC1_HI1;
  MMC1_bank4 = MMC1_HI2;

  MMC1_set_CPU_banks();
}

void NES_mapper1::MMC1_set_CPU_banks()
{
  set_CPU_banks((MMC1_256K_base << 5) + (MMC1_bank1 & ((256/8)-1)),
                (MMC1_256K_base << 5) + (MMC1_bank2 & ((256/8)-1)),
                (MMC1_256K_base << 5) + (MMC1_bank3 & ((256/8)-1)),
                (MMC1_256K_base << 5) + (MMC1_bank4 & ((256/8)-1)));
}

void NES_mapper1::MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
  // Best Play - Pro Yakyuu Special
  /*
  if(patch)
  {
    if(wram_bank)
    {
      wram[addr-0x4000] = data;
    }
    else
    {
      wram[addr-0x6000] = data;
    }
    parent_NES->WriteSaveRAM(addr-0x6000, wram[addr-0x6000]);
    parent_NES->WriteSaveRAM(addr-0x4000, wram[addr-0x4000]);
  }
  */
}

void NES_mapper1::MemoryWrite(uint32 addr, uint8 data)
{
  if(patch && addr == 0xBFFF)
  {
    wram_count++;
    wram_flag += data & 0x01;
    if(wram_count == 5)
    {
      if(wram_flag)
      {
        wram_bank = 1;
        NES_6502::Context context;
        parent_NES->cpu->GetContext(&context);
        context.mem_page[3] = wram + 0x2000;
        parent_NES->cpu->SetContext(&context);
      }
      else
      {
        wram_bank = 0;
        NES_6502::Context context;
        parent_NES->cpu->GetContext(&context);
        context.mem_page[3] = wram;
        parent_NES->cpu->SetContext(&context);
      }
      wram_flag = 0;
      wram_count = 0;
    }
  }

  uint32 reg_num;

  // if write is to a different reg, reset
  if((addr & 0x6000) != (last_write_addr & 0x6000))
  {
    write_count = 0;
    bits = 0x00;
  }
  last_write_addr = addr;

  // if bit 7 set, reset and return
  if(data & 0x80)
  {
    write_count = 0;
    bits = 0x00;
    return;
  }

  if(data & 0x01) bits |= (1 << write_count);
  write_count++;
  if(write_count < 5) return;

  reg_num = (addr & 0x7FFF) >> 13;
  regs[reg_num] = bits;

  write_count = 0;
  bits = 0x00;

//  LOG("MAP1 REG" << reg_num << ": " << HEX(regs[reg_num],2) << endl);

  switch(reg_num)
  {
    case 0:
      {
//        LOG("REG0: " << HEX(reg[0],2) << endl);

        // set mirroring
        if(regs[0] & 0x02)
        {
          if(regs[0] & 0x01)
          {
            set_mirroring(NES_PPU::MIRROR_HORIZ);
          }
          else
          {
            set_mirroring(NES_PPU::MIRROR_VERT);
          }
        }
        else
        {
          // one-screen mirroring
          if(regs[0] & 0x01)
          {
            set_mirroring(1,1,1,1);
          }
          else
          {
            set_mirroring(0,0,0,0);
          }
        }
      }
      break;

    case 1:
      {
        uint8 bank_num = regs[1];

//        LOG("REG1: " << HEX(reg[1],2) << endl);

        if(MMC1_Size == MMC1_1024K)
        {
          if(regs[0] & 0x10)
          {
            if(MMC1_swap)
            {
              MMC1_256K_base = (regs[1] & 0x10) >> 4;
              if(regs[0] & 0x08)
              {
                MMC1_256K_base |= ((regs[2] & 0x10) >> 3);
              }
              MMC1_set_CPU_banks();
              MMC1_swap = 0;
            }
            else
            {
              MMC1_swap = 1;
            }
          }
          else
          {
            // use 1st or 4th 256K banks
            MMC1_256K_base = (regs[1] & 0x10) ? 3 : 0;
            MMC1_set_CPU_banks();
          }
        }
        else if((MMC1_Size == MMC1_512K) && (!num_1k_VROM_banks))
        {
          MMC1_256K_base = (regs[1] & 0x10) >> 4;
          MMC1_set_CPU_banks();
        }
        else if(num_1k_VROM_banks)
        {
          // set VROM bank at $0000
          if(regs[0] & 0x10)
          {
            // swap 4K
            bank_num <<= 2;
            set_PPU_bank0(bank_num+0);
            set_PPU_bank1(bank_num+1);
            set_PPU_bank2(bank_num+2);
            set_PPU_bank3(bank_num+3);
          }
          else
          {
            // swap 8K
            bank_num <<= 2;
            set_PPU_banks(bank_num+0,bank_num+1,bank_num+2,bank_num+3,
                          bank_num+4,bank_num+5,bank_num+6,bank_num+7);
          }
        }
        else
        {
          if(regs[0] & 0x10)
          {
            bank_num <<= 2;
            set_VRAM_bank(0, bank_num+0);
            set_VRAM_bank(1, bank_num+1);
            set_VRAM_bank(2, bank_num+2);
            set_VRAM_bank(3, bank_num+3);
          }
        }
      }
      break;

    case 2:
      {
        uint8 bank_num = regs[2];

//        LOG("REG2: " << HEX(reg[2],2) << endl);

        if((MMC1_Size == MMC1_1024K) && (regs[0] & 0x08))
        {
          if(MMC1_swap)
          {
            MMC1_256K_base =  (regs[1] & 0x10) >> 4;
            MMC1_256K_base |= ((regs[2] & 0x10) >> 3);
            MMC1_set_CPU_banks();
            MMC1_swap = 0;
          }
          else
          {
            MMC1_swap = 1;
          }
        }

        if(!num_1k_VROM_banks)
        {
          if(regs[0] & 0x10)
          {
            bank_num <<= 2;
            set_VRAM_bank(4, bank_num+0);
            set_VRAM_bank(5, bank_num+1);
            set_VRAM_bank(6, bank_num+2);
            set_VRAM_bank(7, bank_num+3);
            break;
          }
        }

        // set 4K VROM bank at $1000
        if(regs[0] & 0x10)
        {
          // swap 4K
          bank_num <<= 2;
          set_PPU_bank4(bank_num+0);
          set_PPU_bank5(bank_num+1);
          set_PPU_bank6(bank_num+2);
          set_PPU_bank7(bank_num+3);
        }
      }
      break;

    case 3:
      {
        uint8 bank_num = regs[3];

//        LOG("REG3: " << HEX(reg[3],2) << endl);

        // set ROM bank
        if(regs[0] & 0x08)
        {
          // 16K of ROM
          bank_num <<= 1;

          if(regs[0] & 0x04)
          {
            // 16K of ROM at $8000
            MMC1_bank1 = bank_num;
            MMC1_bank2 = bank_num+1;
            MMC1_bank3 = MMC1_HI1;
            MMC1_bank4 = MMC1_HI2;
          }
          else
          {
            // 16K of ROM at $C000
            if(MMC1_Size == MMC1_SMALL)
            {
              MMC1_bank1 = 0;
              MMC1_bank2 = 1;
              MMC1_bank3 = bank_num;
              MMC1_bank4 = bank_num+1;
            }
          }
        }
        else
        {
          // 32K of ROM at $8000
          bank_num <<= 1; // thanx rinao

          MMC1_bank1 = bank_num;
          MMC1_bank2 = bank_num+1;
          if(MMC1_Size == MMC1_SMALL)
          {
            MMC1_bank3 = bank_num+2;
            MMC1_bank4 = bank_num+3;
          }
        }

        MMC1_set_CPU_banks();
      }
      break;
  }
}
/////////////////////////////////////////////////////////////////////

