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
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "onewire.h"
#include "device/common.h"
#include "device/ds18b20.h"

#define ON(port, pin) (port |= _BV(pin))
#define OFF(port, pin) (port &= ~_BV(pin))

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init( unsigned int ubrr)
{
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXEN)|(1<<TXEN);
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}

void USART_Send( char *data )
{
	for(int i=0;i<strlen(data);i++) USART_Transmit(data[i]);
}

void USART_Send_hex( char *data, uint8_t len )
{
	char buf[7];
	for(int i=0;i<len;i++)
	{
		itoa(data[i], buf, 16);
		USART_Send(buf);
		USART_Transmit('-');
	}
}

void USART_Send_bin( uint8_t *data, uint8_t len )
{
	for(int i=0;i<len;i++) USART_Transmit(data[i]);
}

unsigned char USART_Receive( void )
{
	while ( !(UCSRA & (1<<RXC)) );
	return UDR;
}

void ok()
{
	ON(PORTB, PB0);
	_delay_ms(500);
	OFF(PORTB, PB0);
}

int main(void) {

	USART_Init ( MYUBRR );
	ON(DDRB, PB0);

	uint8_t id[OW_ROMCODE_SIZE], diff, buf[7], sp[DS18X20_SP_SIZE];

	for(;;)
	{
		ds18b20_convert_all();
		ds18b20_wait();
		for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE; )
		{
			diff = ds18b20_search_read(diff, sp);
			itoa(ds18b20_raw_to_celsius(sp), buf, 10);
			USART_Send( buf );
			USART_Transmit('\n');
		}
		USART_Send( "---" );
		USART_Transmit('\n');
		ok();
	}
	return 0;
}
