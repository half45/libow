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

#define DS18X20_CONVERT_T         0x44
#define DS18X20_READ              0xBE
#define DS18X20_WRITE             0x4E
#define DS18X20_EE_WRITE          0x48
#define DS18X20_EE_RECALL         0xB8
#define DS18X20_READ_POWER_SUPPLY 0xB4

#define DS18X20_SP_SIZE           9

#define ds18b20_wait() while( !ow_bit(1) )

extern uint8_t ds18b20_search_read(uint8_t diff, uint8_t *sp);
extern uint8_t ds18b20_convert_all(void);

extern int8_t ds18b20_raw_to_celsius( uint8_t *sp );
