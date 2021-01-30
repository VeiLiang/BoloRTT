
/////////////////////////////////////////////////////////////////////
// Mapper 21
void NES_mapper21::Reset()
{
  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

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

void NES_mapper21::MemoryWrite(uint32 addr, uint8 data)
{
  // regs[0] ... 1K VROM bank at PPU $0000
  // regs[1] ... 1K VROM bank at PPU $0400
  // regs[2] ... 1K VROM bank at PPU $0800
  // regs[3] ... 1K VROM bank at PPU $0C00
  // regs[4] ... 1K VROM bank at PPU $1000
  // regs[5] ... 1K VROM bank at PPU $1400
  // regs[6] ... 1K VROM bank at PPU $1800
  // regs[7] ... 1K VROM bank at PPU $1C00
  // regs[8] ... $8000 Switching Mode

  switch (addr & 0xF0CF)
  {
    case 0x8000:
      {
        if(regs[8] & 0x02)
        {
          set_CPU_bank6(data);
        }
        else
        {
          set_CPU_bank4(data);
        }
      }
      break;

    case 0xA000:
      {
        set_CPU_bank5(data);
      }
      break;

    case 0x9000:
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
      break;

    case 0x9002:
    case 0x9080:
      {
        regs[8] = data;
      }
      break;

    case 0xB000:
      {
        regs[0] = (regs[0] & 0xF0) | (data & 0x0F);
        set_PPU_bank0(regs[0]);
      }
      break;

    case 0xB002:
    case 0xB040:
      {
        regs[0] = (regs[0] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank0(regs[0]);
      }
      break;

    case 0xB001:
    case 0xB004:
    case 0xB080:
      {
        regs[1] = (regs[1] & 0xF0) | (data & 0x0F);
        set_PPU_bank1(regs[1]);
      }
      break;

    case 0xB003:
    case 0xB006:
    case 0xB0C0:
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

    case 0xC002:
    case 0xC040:
      {
        regs[2] = (regs[2] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank2(regs[2]);
      }
      break;

    case 0xC001:
    case 0xC004:
    case 0xC080:
      {
        regs[3] = (regs[3] & 0xF0) | (data & 0x0F);
        set_PPU_bank3(regs[3]);
      }
      break;

    case 0xC003:
    case 0xC006:
    case 0xC0C0:
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

    case 0xD002:
    case 0xD040:
      {
        regs[4] = (regs[4] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank4(regs[4]);
      }
      break;

    case 0xD001:
    case 0xD004:
    case 0xD080:
      {
        regs[5] = (regs[5] & 0xF0) | (data & 0x0F);
        set_PPU_bank5(regs[5]);
      }
      break;

    case 0xD003:
    case 0xD006:
    case 0xD0C0:
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

    case 0xE002:
    case 0xE040:
      {
        regs[6] = (regs[6] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank6(regs[6]);
      }
      break;

    case 0xE001:
    case 0xE004:
    case 0xE080:
      {
        regs[7] = (regs[7] & 0xF0) | (data & 0x0F);
        set_PPU_bank7(regs[7]);
      }
      break;

    case 0xE003:
    case 0xE006:
    case 0xE0C0:
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

    case 0xF002:
    case 0xF040:
      {
        irq_latch = (irq_latch & 0x0F) | ((data & 0x0F) << 4);
      }
      break;

    case 0xF003:
    case 0xF0C0:
      {
        irq_enabled = (irq_enabled & 0x01) * 3;
      }
      break;

    case 0xF004:
    case 0xF080:
      {
        irq_enabled = data & 0x03;
        if(irq_enabled & 0x02)
        {
          irq_counter = irq_latch;
        }
      }
      break;
  }
}

void NES_mapper21::HSync(uint32 scanline)
{
  if(irq_enabled & 0x02)
  {
    if(irq_counter == 0xFF)
    {
      irq_counter = irq_latch;
      irq_enabled = (irq_enabled & 0x01) * 3;
      parent_NES->cpu->DoIRQ();
    }
    else
    {
      irq_counter++;
    }
  }
}

#define MAP21_VROM(ptr) (((ptr)-parent_NES->ROM->get_VROM_banks()) >> 10)

void NES_mapper21::SNSS_fixup()
{
  regs[0] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[0]);
  regs[1] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[1]);
  regs[2] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[2]);
  regs[3] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[3]);
  regs[4] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[4]);
  regs[5] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[5]);
  regs[6] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[6]);
  regs[7] = MAP21_VROM(parent_NES->ppu->PPU_VRAM_banks[7]);
}
/////////////////////////////////////////////////////////////////////

