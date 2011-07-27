/*  This file is part of libow.

*   libow is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.

*   libow is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.

*   You should have received a copy of the GNU General Public License
*   along with libow.  If not, see <http://www.gnu.org/licenses/>.*/

#include "conf.h"
#include "onewire.h"
#include "device/common.h"

uint8_t ow_reset(void)
{
	OW_OUT_LOW();
	OW_DIR_OUT();
	delay_us(480);
	OW_DIR_IN();
	delay_us(64);
	for(int i;i<25;i++)
	{
		if(OW_GET_IN()) return 0;
		delay_us(2);
	}
	delay_us(500);
	if( OW_GET_IN() == 0 ) return 0;
	return 1;
}

uint8_t ow_write(uint8_t byte)
{
	uint8_t r;

	for(int i=0;i<8;i++)
	{
		r = ow_bit( byte & 1 );
		byte >>= 1;
		if( r ) byte |= 0x80;
	}

	return byte;
}

uint8_t ow_read(void)
{
	return ow_write( 0xFF );
}

uint8_t ow_bit(uint8_t b)
{
	OW_DIR_OUT();
	delay_us(2);
	if( b ) OW_DIR_IN();
	delay_us(13);
	b = OW_GET_IN();
	delay_us(43);
	OW_DIR_IN();
	delay_us(OW_RECOVERY_TIME);
	return b;
}

//AN187
//don't work
uint8_t ow_rom_search_an187( uint8_t LastDiscrepancy, uint8_t *id )
{
	uint8_t id_bit, cmp_id_bit, id_bit_number, LastFamilyDiscrepancy, last_zero=0, search_direction;

	ow_reset();
	ow_write( OW_SEARCH_ROM );

	for(id_bit_number=0;id_bit_number<64;id_bit_number++)
	{
		id_bit = ow_bit( 1 );
		cmp_id_bit = ow_bit( 1 );

		if( (id_bit && cmp_id_bit) == 1 ) return 0;

		if( (id_bit && cmp_id_bit) == 0 )
		{
			if( id_bit_number == LastDiscrepancy ) search_direction = 1;
			else 
			{
				if( id_bit_number > LastDiscrepancy ) search_direction = 0;
				else search_direction = id[id_bit_number];
			}
			if( search_direction == 0 )
			{
				last_zero = id_bit_number;
				if( last_zero < 8 ) LastFamilyDiscrepancy = last_zero;
			}
		}
		else search_direction = id_bit;
		*id >>= 1;
		if( search_direction ) *id |= 0x80;
		id++;
		ow_bit( search_direction );
	}
	LastDiscrepancy = last_zero;
	return LastDiscrepancy;
}

//http://gandalf.arubi.uni-kl.de/avr_projects/tempsensor/index.html
uint8_t ow_rom_search( uint8_t diff, uint8_t *id )
{
	uint8_t i, j, next_diff;
	uint8_t b;

	if( !ow_reset() ) {
		return 0;         // error, no device found <--- early exit!
	}

	ow_write( OW_SEARCH_ROM );        // ROM search command
	next_diff = OW_LAST_DEVICE;         // unchanged on last device

	i = OW_ROMCODE_SIZE * 8;            // 8 bytes

	do {
		j = 8;                          // 8 bits
		do {
			b = ow_bit( 1 );         // read bit
			if( ow_bit( 1 ) ) {      // read complement bit
				if( b ) {               // 0b11
					return -1; // data error <--- early exit!
				}
			}
			else {
				if( !b ) {              // 0b00 = 2 devices
					if( diff > i || ((*id & 1) && diff != i) ) {
						b = 1;          // now 1
						next_diff = i;  // next pass 0
					}
				}
			}
			ow_bit( b );             // write bit
			*id >>= 1;
			if( b ) {
				*id |= 0x80;            // store bit
			}
			
			i--;
			
		} while( --j );
		
		id++;                           // next byte
	
	} while( i );

	return next_diff;                   // to continue search
}

void ow_rom_match( uint8_t *id )
{
	ow_reset();
	ow_write( OW_MATCH_ROM );
	for(int i=0;i<OW_ROMCODE_SIZE;i++) ow_write( id[i] );
}
