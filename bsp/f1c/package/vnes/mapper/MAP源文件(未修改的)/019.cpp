
/////////////////////////////////////////////////////////////////////
// Mapper 19
void NES_mapper19::Reset()
{
  patch = 0;

  if(parent_NES->crc32() == 0x761ccfb5)   // Digital Devil Story - Megami Tensei 2
  {
    patch = 1;
  }

  if(parent_NES->crc32() == 0x96533999 || // Dokuganryuu Masamune
     parent_NES->crc32() == 0x429fd177 || // Famista '90
     parent_NES->crc32() == 0xdd454208 || // Hydlide 3 - Yami Kara no Houmonsha (J).nes
     parent_NES->crc32() == 0xb1b9e187 || // Kaijuu Monogatari
     parent_NES->crc32() == 0xaf15338f)   // Mindseeker
  {
    patch = 2;
  }

  // Init ExSound
  //parent_NES->apu->SelectExSound(16);

  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  if(num_1k_VROM_banks >= 8)
  {
    set_PPU_bank0(num_1k_VROM_banks-8);
    set_PPU_bank1(num_1k_VROM_banks-7);
    set_PPU_bank2(num_1k_VROM_banks-6);
    set_PPU_bank3(num_1k_VROM_banks-5);
    set_PPU_bank4(num_1k_VROM_banks-4);
    set_PPU_bank5(num_1k_VROM_banks-3);
    set_PPU_bank6(num_1k_VROM_banks-2);
    set_PPU_bank7(num_1k_VROM_banks-1);
  }

  regs[0] = 0;
  regs[1] = 0;
  regs[2] = 0;

  // Rick
  irq_counter = irq_enabled = 0;
}

uint8 NES_mapper19::MemoryReadLow(uint32 addr)
{
  if(addr == 0x4800)
  {
    if(patch == 2)
    {
      uint8 retval =  parent_NES->ReadSaveRAM(regs[2] & 0x7F);
      if(regs[2] & 0x80) regs[2] = ((regs[2] & 0x7F)+1) | 0x80;
      return retval;
    }
    else
    {
      //return parent_NES->apu->ExRead(addr);
    }
  }
  else if((addr & 0xF800) == 0x5000) // addr $5000-$57FF
  {
    return (uint8)(irq_counter & 0x00FF);
  }
  else if ((addr & 0xF800) == 0x5800) // addr $5800-$5FFF
  {
    return (uint8)((irq_counter & 0x7F00) >> 8);
  }
  else
  {
    return (uint8)(addr >> 8);
  }
}

void NES_mapper19::MemoryWriteLow(uint32 addr, uint8 data)
{
  switch(addr & 0xF800)
  {
    case 0x4800:
      {
        if(addr == 0x4800)
        {
          if(patch == 2)
          {
            parent_NES->WriteSaveRAM(regs[2] & 0x7F, data);
            if(regs[2] & 0x80) regs[2] = ((regs[2] & 0x7F)+1) | 0x80;
          }
          else
          {
            //parent_NES->apu->ExWrite(addr, data);
          }
        }
      }
      break;

    case 0x5000: // addr $5000-$57FF
      {
        irq_counter = (irq_counter & 0xFF00) | data;
      }
      break;

    case 0x5800: // addr $5000-$57FF
      {
        irq_counter = (irq_counter & 0x00FF) | ((uint32)(data & 0x7F) << 8);
        irq_enabled = (data & 0x80) >> 7;
        if(patch)
        {
          irq_counter ++;
        }
      }
      break;
  }
}

void NES_mapper19::MemoryWrite(uint32 addr, uint8 data)
{
  switch(addr & 0xF800)
  {
    case 0x8000: // addr $8000-$87FF
      {
        if(data < 0xE0 || regs[0] == 1)
        {
          set_PPU_bank0(data);
        }
        else
        {
          set_VRAM_bank(0, 0);
        }
      }
      break;

    case 0x8800: // addr $8800-$8FFF
      {
        if(data < 0xE0 || regs[0] == 1)
        {
          set_PPU_bank1(data);
        }
        else
        {
          set_VRAM_bank(1, 1);
        }
      }
      break;

    case 0x9000: // addr $9000-$97FF
      {
        if(data < 0xE0 || regs[0] == 1)
        {
          set_PPU_bank2(data);
        }
        else
        {
          set_VRAM_bank(2, 2);
        }
      }
      break;

    case 0x9800: // addr $9800-$9FFF
      {
        if(data < 0xE0 || regs[0] == 1)
        {
          set_PPU_bank3(data);
        }
        else
        {
          set_VRAM_bank(3, 3);
        }
      }
      break;

    case 0xA000: // addr $A000-$A7FF
      {
        if(data < 0xE0 || regs[1] == 1)
        {
          set_PPU_bank4(data);
        }
        else
        {
          set_VRAM_bank(4, 4);
        }
      }
      break;

    case 0xA800: // addr $A800-$AFFF
      {
        if(data < 0xE0 || regs[1] == 1)
        {
          set_PPU_bank5(data);
        }
        else
        {
          set_VRAM_bank(5, 5);
        }
      }
      break;

    case 0xB000: // addr $B000-$B7FF
      {
        if(data < 0xE0 || regs[1] == 1)
        {
          set_PPU_bank6(data);
        }
        else
        {
          set_VRAM_bank(6, 6);
        }
      }
      break;

    case 0xB800: // addr $B800-$BFFF
      {
        if(data < 0xE0 || regs[1] == 1)
        {
          set_PPU_bank7(data);
        }
        else
        {
          set_VRAM_bank(7, 7);
        }
      }
      break;

    case 0xC000: // addr $C000-$C7FF
      {
        if(data <= 0xDF)
        {
          set_PPU_bank8(data);
        }
        else
        {
          set_VRAM_bank(8, data & 0x01);
        }
      }
      break;

    case 0xC800: // addr $C800-$CFFF
      {
        if(data <= 0xDF)
        {
          set_PPU_bank9(data);
        }
        else
        {
          set_VRAM_bank(9, data & 0x01);
        }
      }
      break;

    case 0xD000: // addr $D000-$D7FF
      {
        if(data <= 0xDF)
        {
          set_PPU_bank10(data);
        }
        else
        {
          set_VRAM_bank(10, data & 0x01);
        }
      }
      break;

    case 0xD800: // addr $D800-$DFFF
      {
        if(data <= 0xDF)
        {
          set_PPU_bank11(data);
        }
        else
        {
          set_VRAM_bank(11, data & 0x01);
        }
      }
      break;

    case 0xE000: // addr $E000-$E7FF
      {
        set_CPU_bank4(data & 0x3F);
      }
      break;

    case 0xE800: // addr $E800-$EFFF
      {
        set_CPU_bank5(data & 0x3F);
        regs[0] = (data & 0x40) >> 6;
        regs[1] = (data & 0x80) >> 7;
      }
      break;

    case 0xF000: // addr $F000-$F7FF
      {
        set_CPU_bank6(data & 0x3F);
      }
      break;

    case 0xF800:
      {
        if(addr == 0xF800)
        {
          if(patch == 2)
          {
            regs[2] = data;
          }
          else
          {
            //parent_NES->apu->ExWrite(addr, data);
          }
        }
      }
      break;
  }
}

void NES_mapper19::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if(irq_counter >= (uint32)(0x7FFF - ((patch == 1) ? 112 : 113)))
    {
      irq_counter = 0x7FFF;
      parent_NES->cpu->DoIRQ();
    }
    else
    {
      irq_counter += ((patch == 1) ? 112 : 113);
    }
  }
}
/////////////////////////////////////////////////////////////////////

