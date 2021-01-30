
/////////////////////////////////////////////////////////////////////
// Mapper 18
void NES_mapper18::Reset()
{
  if(parent_NES->crc32() == 0x424ec0a6)   // Jajamaru Gekimaden - Maboroshi no Kinmajou (J)
  {
    patch = 1;
  }

  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  regs[0] = 0;
  regs[1] = 1;
  regs[2] = num_8k_ROM_banks-2;
  regs[3] = num_8k_ROM_banks-1;
  regs[4] = 0;
  regs[5] = 0;
  regs[6] = 0;
  regs[7] = 0;
  regs[8] = 0;
  regs[9] = 0;
  regs[10] = 0;

  irq_enabled = 0;
  irq_latch = 0;
  irq_counter = 0;
}

void NES_mapper18::MemoryWrite(uint32 addr, uint8 data)
{
  // regs[ 0] ... 8K PROM bank at CPU $8000
  // regs[ 1] ... 8K PROM bank at CPU $A000
  // regs[ 2] ... 8K PROM bank at CPU $C000
  // regs[ 3] ... 1K VROM bank at PPU $0000
  // regs[ 4] ... 1K VROM bank at PPU $0400
  // regs[ 5] ... 1K VROM bank at PPU $0800
  // regs[ 6] ... 1K VROM bank at PPU $0C00
  // regs[ 7] ... 1K VROM bank at PPU $1000
  // regs[ 8] ... 1K VROM bank at PPU $1400
  // regs[ 9] ... 1K VROM bank at PPU $1800
  // regs[10] ... 1K VROM bank at PPU $1C00

  switch(addr)
  {
    case 0x8000:
      {
        regs[0] = (regs[0] & 0xF0) | (data & 0x0F);
        set_CPU_bank4(regs[0]);
      }
      break;

    case 0x8001:
      {
        regs[0] = (regs[0] & 0x0F) | ((data & 0x0F) << 4);
        set_CPU_bank4(regs[0]);
      }
      break;

    case 0x8002:
      {
        regs[1] = (regs[1] & 0xF0) | (data & 0x0F);
        set_CPU_bank5(regs[1]);
      }
      break;

    case 0x8003:
      {
        regs[1] = (regs[1] & 0x0F) | ((data & 0x0F) << 4);
        set_CPU_bank5(regs[1]);
      }
      break;

    case 0x9000:
      {
        regs[2] = (regs[2] & 0xF0) | (data & 0x0F);
        set_CPU_bank6(regs[2]);
      }
      break;

    case 0x9001:
      {
        regs[2] = (regs[2] & 0x0F) | ((data & 0x0F) << 4);
        set_CPU_bank6(regs[2]);
      }
      break;

    case 0xA000:
      {
        regs[3] = (regs[3] & 0xF0) | (data & 0x0F);
        set_PPU_bank0(regs[3]);
      }
      break;

    case 0xA001:
      {
        regs[3] = (regs[3] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank0(regs[3]);
      }
      break;

    case 0xA002:
      {
        regs[4] = (regs[4] & 0xF0) | (data & 0x0F);
        set_PPU_bank1(regs[4]);
      }
      break;

    case 0xA003:
      {
        regs[4] = (regs[4] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank1(regs[4]);
      }
      break;

    case 0xB000:
      {
        regs[5] = (regs[5] & 0xF0) | (data & 0x0F);
        set_PPU_bank2(regs[5]);
      }
      break;

    case 0xB001:
      {
        regs[5] = (regs[5] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank2(regs[5]);
      }
      break;

    case 0xB002:
      {
        regs[6] = (regs[6] & 0xF0) | (data & 0x0F);
        set_PPU_bank3(regs[6]);
      }
      break;

    case 0xB003:
      {
        regs[6] = (regs[6] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank3(regs[6]);
      }
      break;

    case 0xC000:
      {
        regs[7] = (regs[7] & 0xF0) | (data & 0x0F);
        set_PPU_bank4(regs[7]);
      }
      break;

    case 0xC001:
      {
        regs[7] = (regs[7] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank4(regs[7]);
      }
      break;

    case 0xC002:
      {
        regs[8] = (regs[8] & 0xF0) | (data & 0x0F);
        set_PPU_bank5(regs[8]);
      }
      break;

    case 0xC003:
      {
        regs[8] = (regs[8] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank5(regs[8]);
      }
      break;

    case 0xD000:
      {
        regs[9] = (regs[9] & 0xF0) | (data & 0x0F);
        set_PPU_bank6(regs[9]);
      }
      break;

    case 0xD001:
      {
        regs[9] = (regs[9] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank6(regs[9]);
      }
      break;

    case 0xD002:
      {
        regs[10] = (regs[10] & 0xF0) | (data & 0x0F);
        set_PPU_bank7(regs[10]);
      }
      break;

    case 0xD003:
      {
        regs[10] = (regs[10] & 0x0F) | ((data & 0x0F) << 4);
        set_PPU_bank7(regs[10]);
      }
      break;

    case 0xE000:
      {
        irq_latch = (irq_latch & 0xFFF0) | (data & 0x0F);
      }
      break;

    case 0xE001:
      {
        irq_latch = (irq_latch & 0xFF0F) | ((data & 0x0F) << 4);
      }
      break;

    case 0xE002:
      {
        irq_latch = (irq_latch & 0xF0FF) | ((data & 0x0F) << 8);
      }
      break;

    case 0xE003:
      {
        irq_latch = (irq_latch & 0x0FFF) | ((data & 0x0F) << 12);
      }
      break;

    case 0xF000:
      {
        irq_counter = irq_latch;
      }
      break;

    case 0xF001:
      {
        irq_enabled = data & 0x01;
      }
      break;

    case 0xF002:
      {
        data &= 0x03;
        if(data == 0)
        {
          set_mirroring(NES_PPU::MIRROR_HORIZ);
        }
        else if(data == 1)
        {
          set_mirroring(NES_PPU::MIRROR_VERT);
        }
        else
        {
          set_mirroring(0,0,0,0);
        }
      }
      break;
  }
}

void NES_mapper18::HSync(uint32 scanline)
{
  if(irq_enabled)
  {
    if(irq_counter <= 113)
    {
      parent_NES->cpu->DoIRQ();
      irq_counter = (patch == 1) ? 114 : 0;
      irq_enabled = 0;
    }
    else
    {
      irq_counter -= 113;
    }
  }
}

#define MAP18_ROM(ptr)  (((ptr)-parent_NES->ROM->get_ROM_banks())  >> 13)
#define MAP18_VROM(ptr) (((ptr)-parent_NES->ROM->get_VROM_banks()) >> 10)

void NES_mapper18::SNSS_fixup()
{
  NES_6502::Context context;
  parent_NES->cpu->GetContext(&context);
  regs[0] = MAP18_ROM(context.mem_page[4]);
  regs[1] = MAP18_ROM(context.mem_page[5]);
  regs[2] = MAP18_ROM(context.mem_page[6]);
  regs[3] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[0]);
  regs[4] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[1]);
  regs[5] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[2]);
  regs[6] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[3]);
  regs[7] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[4]);
  regs[8] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[5]);
  regs[9] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[6]);
  regs[10] = MAP18_VROM(parent_NES->ppu->PPU_VRAM_banks[7]);
}
/////////////////////////////////////////////////////////////////////

