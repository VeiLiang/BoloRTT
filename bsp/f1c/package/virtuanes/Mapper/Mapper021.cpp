//////////////////////////////////////////////////////////////////////////
// Mapper021  Konami VRC4 (Address mask $F006 or $F0C0)                 //
//////////////////////////////////////////////////////////////////////////
void	Mapper021::Reset()
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = i;
	}
	reg[8] = 0;

	irq_enable = 0;
	irq_counter = 0;
	irq_latch = 0;
	irq_clock = 0;

	SetPROM_32K_Bank( 0, 1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
}

void	Mapper021::Write( WORD addr, BYTE data )
{
	switch( addr & 0xF0CF ) {
		case	0x8000:
			if(reg[8] & 0x02) {
				SetPROM_8K_Bank( 6, data );
			} else {
				SetPROM_8K_Bank( 4, data );
			}
			break;
		case	0xA000:
			SetPROM_8K_Bank( 5, data );
			break;

		case	0x9000:
			data &= 0x03;
			if( data == 0 )	     SetVRAM_Mirror( VRAM_VMIRROR );
			else if( data == 1 ) SetVRAM_Mirror( VRAM_HMIRROR );
			else if( data == 2 ) SetVRAM_Mirror( VRAM_MIRROR4L );
			else		     SetVRAM_Mirror( VRAM_MIRROR4H );
			break;

		case 0x9002:
		case 0x9080:
			reg[8] = data;
			break;

		case 0xB000:
			reg[0] = (reg[0] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 0, reg[0] );
			break;
		case 0xB002:
		case 0xB040:
			reg[0] = (reg[0] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 0, reg[0] );
			break;

		case 0xB001:
		case 0xB004:
		case 0xB080:
			reg[1] = (reg[1] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 1, reg[1] );
			break;
		case 0xB003:
		case 0xB006:
		case 0xB0C0:
			reg[1] = (reg[1] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 1, reg[1] );
			break;

		case 0xC000:
			reg[2] = (reg[2] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 2, reg[2] );
			break;
		case 0xC002:
		case 0xC040:
			reg[2] = (reg[2] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 2, reg[2] );
			break;

		case 0xC001:
		case 0xC004:
		case 0xC080:
			reg[3] = (reg[3] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 3, reg[3] );
			break;
		case 0xC003:
		case 0xC006:
		case 0xC0C0:
			reg[3] = (reg[3] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 3, reg[3] );
			break;

		case 0xD000:
			reg[4] = (reg[4] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 4, reg[4] );
			break;
		case 0xD002:
		case 0xD040:
			reg[4] = (reg[4] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 4, reg[4] );
			break;

		case 0xD001:
		case 0xD004:
		case 0xD080:
			reg[5] = (reg[5] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 5, reg[5] );
			break;
		case 0xD003:
		case 0xD006:
		case 0xD0C0:
			reg[5] = (reg[5] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 5, reg[5] );
			break;

		case 0xE000:
			reg[6] = (reg[6] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 6, reg[6] );
			break;
		case 0xE002:
		case 0xE040:
			reg[6] = (reg[6] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 6, reg[6] );
			break;

		case 0xE001:
		case 0xE004:
		case 0xE080:
			reg[7] = (reg[7] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 7, reg[7] );
			break;
		case 0xE003:
		case 0xE006:
		case 0xE0C0:
			reg[7] = (reg[7] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 7, reg[7] );
			break;

		case 0xF000:
			irq_latch = (irq_latch & 0xF0) | (data & 0x0F);
			break;
		case 0xF002:
		case 0xF040:
			irq_latch = (irq_latch & 0x0F) | ((data & 0x0F) << 4);
			break;

		case 0xF003:
		case 0xF0C0:
		case 0xF006:
			irq_enable = (irq_enable & 0x01) * 3;
			irq_clock = 0;

			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;

		case 0xF004:
		case 0xF080:
			irq_enable = data & 0x03;
			if( irq_enable & 0x02 ) {
				irq_counter = irq_latch;
				irq_clock = 0;
			}

			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;

//		case 0xF006:
//			nes->cpu->ClrIRQ( IRQ_MAPPER );
//			break;
	}
}

void	Mapper021::Clock( INT cycles )
{
	if( irq_enable & 0x02 ) {
		if( (irq_clock-=cycles) < 0 ) {
			irq_clock += 0x72;
			if( irq_counter == 0xFF ) {
				irq_counter = irq_latch;
//				irq_enable = (irq_enable & 0x01) * 3;
//				nes->cpu->IRQ_NotPending();
				nes->cpu->SetIRQ( IRQ_MAPPER );
			} else {
				irq_counter++;
			}
		}
	}
}

void	Mapper021::SaveState( LPBYTE p )
{
	for( INT i = 0; i < 9; i++ ) {
		p[i] = reg[i];
	}
	p[ 9] = irq_enable;
	p[10] = irq_counter;
	p[11] = irq_latch;
	*(INT*)&p[12] = irq_clock;
}

void	Mapper021::LoadState( LPBYTE p )
{
	for( INT i = 0; i < 9; i++ ) {
		reg[i] = p[i];
	}
	irq_enable  = p[ 9];
	irq_counter = p[10];
	irq_latch   = p[11];
	irq_clock   = *(INT*)&p[12];
}
