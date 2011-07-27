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

#include <avr/io.h>
#include <util/delay.h>

#define OW_PIN  PD7
#define OW_IN   PIND
#define OW_OUT  PORTD
#define OW_DDR  DDRD

#define OW_GET_IN()   ( OW_IN & (1<<OW_PIN))
#define OW_OUT_LOW()  ( OW_OUT &= (~(1 << OW_PIN)) )
#define OW_OUT_HIGH() ( OW_OUT |= (1 << OW_PIN) )
#define OW_DIR_IN()   ( OW_DDR &= (~(1 << OW_PIN )) )
#define OW_DIR_OUT()  ( OW_DDR |= (1 << OW_PIN) )

//#define OW_CONF_CYCLESPERACCESS 13
//#define OW_CONF_DELAYOFFSET ( (uint16_t)( ((OW_CONF_CYCLESPERACCESS) * 1000000L) / F_CPU ) )

//#define delay_us(us)  for(;us>0;us--) asm("nop")
#define delay_us(us)  _delay_us(us)
