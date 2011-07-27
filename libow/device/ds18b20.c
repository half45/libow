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
#include "device/ds18b20.h"

uint8_t ds18b20_convert_all(void)
{
	if(!ow_reset()) return 0;
	ow_write(0xCC);
	ow_write(0x44);
	return 1;
}

uint8_t ds18b20_search_read(uint8_t diff, uint8_t *sp)
{
	uint8_t id[OW_ROMCODE_SIZE];

	diff = ow_rom_search( diff, id );
	ow_rom_match( id );
	ow_write(0xBE);
	for(int i=0; i<DS18X20_SP_SIZE; i++) sp[i] = ow_read();

	return diff;
}

int8_t ds18b20_raw_to_celsius( uint8_t *sp )
{
	int8_t celsius;

	celsius = (sp[1] << 4) | (sp[0] >> 4);

	if ( celsius & 0x8000 ) 
	{
		celsius ^= 0xffff;
		celsius = -celsius;
	}

	return celsius;
}
