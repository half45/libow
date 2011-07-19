#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "onewire.h"
#include "avr.h"

uint8_t ow_reset(void)
{
	OW_OUT_LOW();
	OW_DIR_OUT();
	_delay_us(480);
	OW_DIR_IN();
	_delay_us(64);
	for(int i;i<25;i++)
	{
		if(OW_GET_IN()) return 0;
		_delay_us(2);
	}
	_delay_us(500);
	if( OW_GET_IN() == 0 )
	{
		return 0;
	}
	return 1;
}

uint8_t ow_write(uint8_t b)
{
	uint8_t i = 8, j;

	do {
		j = ow_bit( b & 1 );
		b >>= 1;
		if( j ) {
			b |= 0x80;
		}
	} while( --i );

	return b;
}

uint8_t ow_read(void)
{
	return ow_write( 0xFF ); 
}

uint8_t ow_bit(uint8_t b)
{
	OW_DIR_OUT();
	_delay_us(2);
	if( b ) OW_DIR_IN();
	_delay_us(15-2-OW_CONF_DELAYOFFSET);
	b = OW_GET_IN();
	_delay_us(60-15-2+OW_CONF_DELAYOFFSET);
	OW_DIR_IN();
	_delay_us(OW_RECOVERY_TIME);
	return b;
}
