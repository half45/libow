#define OW_PIN  PD7
#define OW_IN   PIND
#define OW_OUT  PORTD
#define OW_DDR  DDRD

#define OW_RECOVERY_TIME 10 /* usec */

#define OW_CONF_CYCLESPERACCESS 13
#define OW_CONF_DELAYOFFSET ( (uint16_t)( ((OW_CONF_CYCLESPERACCESS) * 1000000L) / F_CPU ) )

extern uint8_t ow_reset(void);

extern uint8_t ow_write(uint8_t b);
extern uint8_t ow_read(void);
extern uint8_t ow_bit(uint8_t b);

extern uint8_t ow_search(void);
extern void ow_command( uint8_t command, uint8_t *id );
