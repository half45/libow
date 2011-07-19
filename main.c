#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "onewire.h"

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
	
	int Temp;
	uint8_t data[12];
char buf[10] = {0};
	//if(ow_reset()) ok();
	//_delay_ms(500);

	//uint8_t answ = 0, recv = 0;
	for(;;)
	{
		//USART_Receive();
		//USART_Receive();
		ow_reset();
		/*ow_write(0xCC);//Master issues Skip ROM command.
		ow_write(0x4E);//Master issues Write Scratchpad command.
		ow_write(0x10);//Master sends three data bytes to scratchpad (TH, TL, and config).
		ow_write(0x22);
		ow_write(0xD2);
		ow_reset();
		ow_write(0xCC);//Master issues Skip ROM command.
		ow_write(0xBE);//Master issues Read Scratchpad command.
		//00000000: 50 05 10 22 5f ff 0c 10 b4 */
		ow_write(0xCC);//Master issues Skip ROM command.
		ow_write(0x44);//Master issues Convert T command.
		_delay_ms(1000);
		ow_reset();
		ow_write(0xCC);//Master issues Skip ROM command.
		ow_write(0xBE);//Master issues Read Scratchpad command.
		//00000000: fa 01 10 22 5f ff 06 10 31 
		for(int i=0; i<9; i++)
		{
			data[i] = ow_read();
			//USART_Transmit(data[i]);
			//USART_Transmit('\n');
		}
		Temp=(data[1]<<8)+data[0];//take the two bytes from the response relating to temperature

  Temp=Temp>>4;//divide by 16 to get pure celcius readout
  itoa(Temp, buf, 10);
  for(int i=0;i<strlen(buf);i++)
  USART_Transmit(buf[i]);
		ok();
		USART_Transmit('\n');
		_delay_ms(500);
	}
	return 0;
}
