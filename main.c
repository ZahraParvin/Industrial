/*
 * GccApplication1.c
 *
 * Created: 10/5/2023 10:18:12 AM
 * Author : zpash
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU 4915200
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void uart_init(unsigned int baud_rate){

	/* Set baud rate */
	UBRR0H = (unsigned char)(MYUBRR >> 8);
	UBRR0L = (unsigned char)(MYUBRR);

	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);


	UCSR0C = (1 << URSEL0) | (1 << UCSZ00) | (1 << UCSZ00);
	//UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

void uart_send(unsigned char data) {
	while (((1<<UDRE0) & UCSR0A) == 0 );

	/* Copy 9th bit to TXB8 */
	UCSR0B &= ~(1<<TXB80); //?
	if ( data & 0x0100 )   //?
	UCSR0B |= (1<<TXB80); //?

	UDR0 = data;
}

unsigned char uart_receive(){
	while (((1<<RXCIE0) & UCSR0A) == 0 );
	return UDRE0;	
}

ISR(USART0_RXC_vect) {
	// Read the received character
	unsigned char received = UDR0;
	
	// Echo the received character back to the terminal
	uart_send(received);
}
//App_ReceiveCallback(received);

static ch uart_printf(char c, FILE* stream ) {

	if (c== '\n')
	{
		uart_send('\r');
	}
	uart_send(c);
	return 0;
}


main(void)
{
	uart_init(BAUD);

	fdevopen(uart_printf, NULL);
	
	DDRB |= (1 << DDB1);
	
	
    while (1) {
		
		uart_send('a');
		unsigned char received = uart_receive();
		
		
        //PORTB ^= (1 << PORTB1);
        _delay_ms(100);
    }
}

