#include <msp430.h>

int enable = 0;

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	if(!(P1IN & BIT7))
	{
		enable ^= 1;	      // Change flag's value to opposite value
		P8OUT ^= BIT1;        // Turn on/off LED2
	}
	if(enable == 0)
	{
		WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer
	}
	else
	{
		WDTCTL = WDTPW + WDTTMSEL + WDTCNTCL + WDTIS1 + WDTIS0;  // Toggle to interval mode, timer counter value set to 0, set timer to 1s
	}
	P1IFG = 0;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	if(!(P2IN & BIT2))
	{
		enable ^= 1;	      // Change flag's value to opposite value
		P8OUT ^= BIT1;        // Turn on/off LED2
	}
	if(enable == 0)
	{
		WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer
	}
	else
	{
		WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTIS1 | WDTIS0;  // Toggle to interval mode, timer counter value set to 0, set timer to 1s
	}
	P2IFG = 0;
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	P8OUT ^= BIT1;        // Turn on/off LED2
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer

	P8DIR |= BIT1;                // Set P8.1 to output direction
	P8OUT &= !BIT1;               // Turn off LED2

	P1DIR |= BIT7;                // Set P1.7 to output direction
	P1OUT |= BIT7;                // To connect P1REN
	P1REN |= BIT7;
	P1DIR &= !BIT7;               // Set P1.7 to input direction

	P1IE |= BIT7;
	P1IES |= BIT7;

	P2DIR |= BIT2;                // Set P2.2 to output direction
	P2OUT |= BIT2;                // To connect P2REN
	P2REN |= BIT2;
	P2DIR &= !BIT2;               // Set P1.7 to input direction

	P2IE |= BIT2;
	P2IES |= BIT2;

	SFRIE1 |= BIT0;

	__bis_SR_register(LPM0_bits + GIE);
	__no_operation();

	return 0;
}
