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

#define OW_RECOVERY_TIME 10 /* usec */

uint8_t ow_reset(void);

uint8_t ow_write(uint8_t b);
uint8_t ow_read(void);
uint8_t ow_bit(uint8_t b);

uint8_t ow_rom_search( uint8_t diff, uint8_t *id );
uint8_t ow_rom_search_an187( uint8_t diff, uint8_t *id );
void ow_rom_match( uint8_t *id );
