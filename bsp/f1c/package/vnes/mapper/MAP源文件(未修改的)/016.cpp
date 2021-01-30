#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 16
void NES_mapper16::Reset()
{
  patch = 0;

  if(parent_NES->crc32() == 0x3f15d20d) // Famicom Jump 2 - Saikyou no 7 Nin
  {
    patch = 1;
  }

  if(!patch)
  {
    NES_6502::Context context;
    parent_NES->cpu->GetContext(&context);
    context.mem_page[3] = serial_out;
    parent_NES->cpu->SetContext(&context);
    eeprom_status = 0x00;
    barcode_status = 0x00;
    barcode_enabled = 0;
    //memset(serial_out, eeprom_status | barcode_status, 0x2000);
    serial_out[0] = eeprom_status | barcode_status;
  }

  // set CPU bank pointers
  set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

  regs[0] = 0;
  regs[1] = 0;
  regs[2] = 0;

  irq_enabled = 0;
  irq_counter = 0;
  irq_latch = 0;
}

void NES_mapper16::MemoryWrite(uint32 addr, uint8 data)
{
  if(patch)
  {
    MemoryWrite3(addr, data);
  }
  else
  {
    MemoryWrite2(addr, data);
  }
}

void NES_mapper16::MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
  if(patch)
  {
    //MemoryWrite3(addr, data);
  }
  else
  {
    MemoryWrite2(addr, data);
  }
}

void NES_mapper16::MemoryReadSaveRAM(uint32 addr)
{
  if(barcode_enabled && addr == 0x6000)
  {
    if(barcode_phase == 1)
    {
      if(++barcode_wait == 10)
      {
        barcode_wait = 0;
        barcode_status = barcode[barcode_pt];
        barcode_pt++;
        if(barcode_pt == barcode_pt_max)
        {
          barcode_status = 0x08;
          barcode_phase = 2;
        }
        serial_out[0] = eeprom_status | barcode_status;
      }
    }
  }
}

void NES_mapper16::HSync(uint32 scanline)
{
  if(barcode_enabled)
  {
    if(barcode_phase == 0)
    {
      barcode_status = 0x08;
      if(++barcode_wait > 600)
      {
        barcode_wait = 0;
        barcode_phase = 1;
      }
      serial_out[0] = eeprom_status | barcode_status;
    }
    else if(barcode_phase == 2)
    {
      barcode_status = 0x08;
      if(++barcode_wait > 600)
      {
        barcode_status = 0x00;
        barcode_enabled = 0;
      }
      serial_out[0] = eeprom_status | barcode_status;
    }
  }

  if(irq_enabled)
  {
    if(irq_counter <= 114)
    {
      parent_NES->cpu->DoIRQ();
      irq_counter = 0;
      irq_enabled = 0;
    }
    else
    {
      irq_counter -= 114;
    }
  }
}

void NES_mapper16::MemoryWrite2(uint32 addr, uint8 data)
{
  switch(addr & 0x000F)
  {
    case 0x0000:
      {
        set_PPU_bank0(data);
      }
      break;

    case 0x0001:
      {
        set_PPU_bank1(data);
      }
      break;

    case 0x0002:
      {
        set_PPU_bank2(data);
      }
      break;

    case 0x0003:
      {
        set_PPU_bank3(data);
      }
      break;

    case 0x0004:
      {
        set_PPU_bank4(data);
      }
      break;

    case 0x0005:
      {
        set_PPU_bank5(data);
      }
      break;

    case 0x0006:
      {
        set_PPU_bank6(data);
      }
      break;

    case 0x0007:
      {
        set_PPU_bank7(data);
      }
      break;

    case 0x0008:
      {
        set_CPU_bank4(data*2+0);
        set_CPU_bank5(data*2+1);
      }
      break;

    case 0x0009:
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

    case 0x000A:
      {
        irq_enabled = data & 0x01;
        irq_counter = irq_latch;
      }
      break;

    case 0x000B:
      {
        irq_latch = (irq_latch & 0xFF00) | data;
      }
      break;

    case 0x000C:
      {
         irq_latch = ((uint32)data << 8) | (irq_latch & 0x00FF);
      }
      break;

    case 0x000D:
      {
        if(data == 0x80)
        {
          // reset
          eeprom_addr = 0x00;
          eeprom_mode = 0x00;
          eeprom_wbit = 0x00;
          eeprom_rbit = 0x00;
        }
        else if(eeprom_cmd[3] == 0x00 && eeprom_cmd[2] == 0x20 &&
                eeprom_cmd[1] == 0x60 && eeprom_cmd[0] == 0x40 && data == 0x00)
        {
          // reset ?
        }
        else if(eeprom_cmd[3] == 0x00 && eeprom_cmd[2] == 0x40 &&
                eeprom_cmd[1] == 0x60 && eeprom_cmd[0] == 0x20 && data == 0x00)
        {
          // start
          eeprom_wbit = 0x01;
          eeprom_rbit = 0x01;
          eeprom_data = 0;
          eeprom_mode = 0;
        }
        else if (eeprom_cmd[0] == 0x60 && data == 0xE0)
        {
          if(!eeprom_mode)
          {
            // sync
            eeprom_wbit = 0x01;
            eeprom_rbit = 0x01;
            eeprom_data = 0;
            eeprom_mode = 1;
            eeprom_status = 0x00;
            //memset(serial_out, eeprom_status | barcode_status, 0x2000);
            serial_out[0] = eeprom_status | barcode_status;
          }
          else
          {
            // read
            eeprom_data = parent_NES->ReadSaveRAM(eeprom_addr);
            if(eeprom_data & eeprom_rbit)
            {
              eeprom_status = 0x10;
              //memset(serial_out, eeprom_status | barcode_status, 0x2000);
              serial_out[0] = eeprom_status | barcode_status;
            }
            else
            {
              eeprom_status = 0x00;
              //memset(serial_out, eeprom_status | barcode_status, 0x2000);
              serial_out[0] = eeprom_status | barcode_status;
            }
            eeprom_rbit <<= 1;
            eeprom_wbit = 0x00;
          }
        }
        else if(eeprom_cmd[1] == 0x00 && eeprom_cmd[0] == 0x20 && data == 0x00)
        {
          // write 0
          eeprom_data &= 0xFF - eeprom_wbit;
          if(eeprom_wbit == 0x80)
          {
            if(eeprom_mode)
            {
              parent_NES->WriteSaveRAM(eeprom_addr, eeprom_data);
            }
            else
            {
              eeprom_addr = eeprom_data & 0x7F;
            }
            eeprom_wbit = 0x00;
          }
          else
          {
            eeprom_wbit <<= 1;
          }
          eeprom_rbit = 0x00;
        }
        else if(eeprom_cmd[3] == 0x00 && eeprom_cmd[2] == 0x40 &&
                eeprom_cmd[1] == 0x60 && eeprom_cmd[0] == 0x40 && data == 0x00)
        {
          // write 1
          eeprom_data |= eeprom_wbit;
          if(eeprom_wbit == 0x80)
          {
            if(eeprom_mode)
            {
              parent_NES->WriteSaveRAM(eeprom_addr, eeprom_data);
            }
            else
            {
              eeprom_addr = eeprom_data & 0x7F;
            }
            eeprom_wbit = 0x00;
          }
          else
          {
            eeprom_wbit <<= 1;
          }
          eeprom_rbit = 0x00;
        }
        eeprom_cmd[3] = eeprom_cmd[2];
        eeprom_cmd[2] = eeprom_cmd[1];
        eeprom_cmd[1] = eeprom_cmd[0];
        eeprom_cmd[0] = data;
      }
      break;
  }
}

void NES_mapper16::MemoryWrite3(uint32 addr, uint8 data)
{
  switch(addr)
  {
    case 0x8000:
    case 0x8001:
    case 0x8002:
    case 0x8003:
      {
        regs[0] = data & 0x01;
        set_CPU_bank4(regs[0]*0x20+regs[2]*2+0);
        set_CPU_bank5(regs[0]*0x20+regs[2]*2+1);
      }
      break;

    case 0x8004:
    case 0x8005:
    case 0x8006:
    case 0x8007:
      {
        regs[1] = data & 0x01;
        set_CPU_bank6(regs[1]*0x20+0x1E);
        set_CPU_bank7(regs[1]*0x20+0x1F);
      }
      break;

    case 0x8008:
      {
        regs[2] = data;
        set_CPU_bank4(regs[0]*0x20+regs[2]*2+0);
        set_CPU_bank5(regs[0]*0x20+regs[2]*2+1);
        set_CPU_bank6(regs[1]*0x20+0x1E);
        set_CPU_bank7(regs[1]*0x20+0x1F);
      }
      break;

    case 0x8009:
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

    case 0x800A:
      {
        irq_enabled = data & 0x01;
        irq_counter = irq_latch;
      }
      break;

    case 0x800B:
      {
        irq_latch = (irq_latch & 0xFF00) | data;
      }
      break;

    case 0x800C:
      {
         irq_latch = ((uint32)data << 8) | (irq_latch & 0x00FF);
      }
      break;

    case 0x800D:
      {
        //write protect
      }
      break;
  }
}

void NES_mapper16::SetBarcodeValue(uint32 value_low, uint32 value_high)
{
  boolean prefix_parity_type[10][6] =
  {
    {0,0,0,0,0,0}, {0,0,1,0,1,1},
    {0,0,1,1,0,1}, {0,0,1,1,1,0},
    {0,1,0,0,1,1}, {0,1,1,0,0,1},
    {0,1,1,1,0,0}, {0,1,0,1,0,1},
    {0,1,0,1,1,0}, {0,1,1,0,1,0}
  };
  boolean data_left_odd[10][7] =
  {
    {0,0,0,1,1,0,1}, {0,0,1,1,0,0,1},
    {0,0,1,0,0,1,1}, {0,1,1,1,1,0,1},
    {0,1,0,0,0,1,1}, {0,1,1,0,0,0,1},
    {0,1,0,1,1,1,1}, {0,1,1,1,0,1,1},
    {0,1,1,0,1,1,1}, {0,0,0,1,0,1,1}
  };
  boolean data_left_even[10][7] =
  {
    {0,1,0,0,1,1,1}, {0,1,1,0,0,1,1},
    {0,0,1,1,0,1,1}, {0,1,0,0,0,0,1},
    {0,0,1,1,1,0,1}, {0,1,1,1,0,0,1},
    {0,0,0,0,1,0,1}, {0,0,1,0,0,0,1},
    {0,0,0,1,0,0,1}, {0,0,1,0,1,1,1}
  };
  boolean data_right[10][7] =
  {
    {1,1,1,0,0,1,0}, {1,1,0,0,1,1,0},
    {1,1,0,1,1,0,0}, {1,0,0,0,0,1,0},
    {1,0,1,1,1,0,0}, {1,0,0,1,1,1,0},
    {1,0,1,0,0,0,0}, {1,0,0,0,1,0,0},
    {1,0,0,1,0,0,0}, {1,1,1,0,1,0,0}
  };
  uint32 i, j, pt = 0;

  if(value_high != 0) // normal barcode (13 characters)
  {
    // set numbers
    uint8 prefix, num[12];
    prefix = (uint8)(value_high / 10000); value_high %= 10000;
    num[0] = (uint8)(value_high / 1000); value_high %= 1000;
    num[1] = (uint8)(value_high / 100); value_high %= 100;
    num[2] = (uint8)(value_high / 10); value_high %= 10;
    num[3] = (uint8)value_high;
    num[4] = (uint8)(value_low / 10000000)%10; value_low %= 10000000;
    num[5] = (uint8)(value_low / 1000000); value_low %= 1000000;
    num[6] = (uint8)(value_low / 100000); value_low %= 100000;
    num[7] = (uint8)(value_low / 10000); value_low %= 10000;
    num[8] = (uint8)(value_low / 1000); value_low %= 1000;
    num[9] = (uint8)(value_low / 100); value_low %= 100;
    num[10] = (uint8)(value_low / 10); value_low %= 10;
    // check character
    uint32 odd = (num[10]+num[8]+num[6]+num[4]+num[2]+num[0])*3;
    uint32 even = num[9]+num[7]+num[5]+num[3]+num[1]+prefix;
    num[11] = (10 - ((odd + even) % 10)) % 10;
    // left guard  bar
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    // 6 left characters
    for(i = 0; i < 6; i++)
    {
      if(prefix_parity_type[prefix][i])
      {
        // prity is even
        for(j = 0; j < 7; j++)
        {
          barcode[pt] = (data_left_even[num[i]][j] == 1) ? 0x00 : 0x08;
          pt++;
        }
      }
      else
      {
        // parity is odd
        for(j = 0; j < 7; j++)
        {
          barcode[pt] = (data_left_odd[num[i]][j] == 1) ? 0x00 : 0x08;
          pt++;
        }
      }
    }
    // center bar
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    // 5 right characters and 1 check character
    for(i = 0; i < 6; i++)
    {
      for(j = 0; j < 7; j++)
      {
        barcode[pt] = (data_right[num[i+6]][j] == 1) ? 0x00 : 0x08;
        pt++;
      }
    }
    // right guard bar
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    // right margin
    barcode[pt] = 0x08; pt++;
  }
  else // short barcode (8 characters)
  {
    // set numbers
    uint8 num[8];
    num[0] = (uint8)(value_low / 10000000)%10; value_low %= 10000000;
    num[1] = (uint8)(value_low / 1000000); value_low %= 1000000;
    num[2] = (uint8)(value_low / 100000); value_low %= 100000;
    num[3] = (uint8)(value_low / 10000); value_low %= 10000;
    num[4] = (uint8)(value_low / 1000); value_low %= 1000;
    num[5] = (uint8)(value_low / 100); value_low %= 100;
    num[6] = (uint8)(value_low / 10); value_low %= 10;
    // check character
    uint32 odd = (num[6]+num[4]+num[2]+num[0])*3;
    uint32 even = num[5]+num[3]+num[1];
    num[7] = (10 - ((odd + even) % 10)) % 10;
    // left guard  bar
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    // 4 left characters
    for(i = 0; i < 4; i++)
    {
      for(j = 0; j < 7; j++)
      {
        barcode[pt] = (data_left_odd[num[i]][j] == 1) ? 0x00 : 0x08;
        pt++;
      }
    }
    // center bar
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    // 4 right characters and 1 check character
    for(i = 0; i < 4; i++)
    {
      for(j = 0; j < 7; j++)
      {
        barcode[pt] = (data_right[num[i+4]][j] == 1) ? 0x00 : 0x08;
        pt++;
      }
    }
    // right guard bar
    barcode[pt] = 0x00; pt++;
    barcode[pt] = 0x08; pt++;
    barcode[pt] = 0x00; pt++;
    // right margin
    barcode[pt] = 0x08; pt++;
  }
  barcode_pt_max = pt;

  barcode_pt = 0;
  barcode_phase = 0;
  barcode_wait = 0;
  barcode_enabled = 1;
}
/////////////////////////////////////////////////////////////////////

#endif
