
/////////////////////////////////////////////////////////////////////
// Mapper 23
void NES_mapper23::Reset()
{
  patch = 0xFFFF;

  if(parent_NES->crc32() == 0x93794634) // Akumajou Special - Boku Dracula Kun
  {
    patch = 0xF00C;
  }

  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  // set PPU bank pointers
  set_PPU_banks(0,1,2,3,4,5,6,7);

  regs[0] = 0;
  regs[1] = 1;
  regs[2] = 2;
  regs[3] = 3;
  regs[4] = 4;
  regs[5] = 5;
  regs[6] = 6;
  regs[7] = 7;
  regs[8] = 0;

  irq_enabled = 0;
  irq_counter = 0;
  irq_latch = 0;
}

void NES_mapper23::MemoryWrite(uint32 addr, uint8 data)
{
  // regs[0] ... 1K VROM bank at PPU $0000
  // regs[1] ... 1K VROM bank at PPU $0400
  // regs[2] ... 1K VROM bank at PPU $0800
  // regs[3] ... 1K VROM bank at PPU $0C00
  // regs[4] ... 1K VROM bank at PPU $1000
  // regs[5] ... 1K VROM bank at PPU $1400
  // regs[6] ... 1K VROM bank at PPU $1800
  // regs[7] ... 1K VROM bank at PPU $1C00
  // regs[8] ... $9008 swap

  switch(addr & patch)
  {
    case 0x8000:
    case 0x8004:
    case 0x8008:
    case 0x800C:
      {
        if(regs[8])
        {
          set_CPU_bank6(data);
        }
        else
        {
          set_CPU_bank4(data);
        }
      }
      break;

    case 0x9000:
      {
        if(data != 0xFF)
        {
          data &= 0x03;
          if(data == 0)
          {
            set_mirroring(NES_PPU::MIRROR_VERT);
          }
          else if(data == 1)
          {
            set_mirroring(NES_PPU::MIRROR_HORIZ);
          }
          else if(data == 2)
          {
            set_mirroring(0,0,0,0);
          }
          else
          {
            set_mirroring(1,1,1,1);
          }
        }
      }
      break;

    case 0x9008:
      {
        regs[8] = data & 0x02;
      }
      break;

    case 0xA000:
    case 0xA004:
    case 0xA008:
    case 0xA00C:
      {
        set_CPU_bank5(data);
      }
      break;

    case 0xB000:
      {
        regs[0] = (regs[0] & 0xF0) | (data & 0x0F);
        set_PPU_bank0(regs[0]);
      }
      break;

    case 0xB001:
    case 0xB004:
      {
        regs[0] = (regs[0] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank0(regs[0]);
      }
      break;

    case 0xB002:
    case 0xB008:
      {
        regs[1] = (regs[1] & 0xF0) | (data & 0x0F);
        set_PPU_bank1(regs[1]);
      }
      break;

    case 0xB003:
    case 0xB00C:
      {
        regs[1] = (regs[1] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank1(regs[1]);
      }
      break;

    case 0xC000:
      {
        regs[2] = (regs[2] & 0xF0) | (data & 0x0F);
        set_PPU_bank2(regs[2]);
      }
      break;

    case 0xC001:
    case 0xC004:
      {
        regs[2] = (regs[2] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank2(regs[2]);
      }
      break;

    case 0xC002:
    case 0xC008:
      {
        regs[3] = (regs[3] & 0xF0) | (data & 0x0F);
        set_PPU_bank3(regs[3]);
      }
      break;

    case 0xC003:
    case 0xC00C:
      {
        regs[3] = (regs[3] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank3(regs[3]);
      }
      break;

    case 0xD000:
      {
        regs[4] = (regs[4] & 0xF0) | (data & 0x0F);
        set_PPU_bank4(regs[4]);
      }
      break;

    case 0xD001:
    case 0xD004:
      {
        regs[4] = (regs[4] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank4(regs[4]);
      }
      break;

    case 0xD002:
    case 0xD008:
      {
        regs[5] = (regs[5] & 0xF0) | (data & 0x0F);
        set_PPU_bank5(regs[5]);
      }
      break;

    case 0xD003:
    case 0xD00C:
      {
        regs[5] = (regs[5] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank5(regs[5]);
      }
      break;

    case 0xE000:
      {
        regs[6] = (regs[6] & 0xF0) | (data & 0x0F);
        set_PPU_bank6(regs[6]);
      }
      break;

    case 0xE001:
    case 0xE004:
      {
        regs[6] = (regs[6] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank6(regs[6]);
      }
      break;

    case 0xE002:
    case 0xE008:
      {
        regs[7] = (regs[7] & 0xF0) | (data & 0x0F);
        set_PPU_bank7(regs[7]);
      }
      break;

    case 0xE003:
    case 0xE00C:
      {
        regs[7] = (regs[7] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank7(regs[7]);
      }
      break;

    case 0xF000:
      {
        irq_latch = (irq_latch & 0xF0) | (data & 0x0F);
      }
      break;

    case 0xF004:
      {
        irq_latch = (irq_latch & 0x0F) | ((data & 0x0F) << 4);
      }
      break;

    case 0xF008:
      {
        irq_enabled = data & 0x03;
        if(irq_enabled & 0x02)
        {
          irq_counter = irq_latch;
        }
      }
      break;

    case 0xF00C:
      {
        irq_enabled = (irq_enabled & 0x01) * 3;
      }
      break;
  }
}

void NES_mapper23::HSync(uint32 scanline)
{
  if(irq_enabled & 0x02)
  {
    if(irq_counter == 0xFF)
    {
      parent_NES->cpu->DoIRQ();
      irq_counter = irq_latch;
      irq_enabled = (irq_enabled & 0x01) * 3;
    }
    else
    {
      irq_counter++;
    }
  }
}

#define MAP23_VROM(ptr) (((ptr)-parent_NES->ROM->get_VROM_banks()) >> 10)

void NES_mapper23::SNSS_fixup()
{
  regs[0] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[0]);
  regs[1] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[1]);
  regs[2] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[2]);
  regs[3] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[3]);
  regs[4] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[4]);
  regs[5] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[5]);
  regs[6] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[6]);
  regs[7] = MAP23_VROM(parent_NES->ppu->PPU_VRAM_banks[7]);
}
/////////////////////////////////////////////////////////////////////

