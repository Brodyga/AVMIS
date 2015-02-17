#include <msp430.h> 

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	P1OUT &= ~(BIT1 + BIT3);
	if((CBCTL1 & CBOUT) == 0)
	{
		P1OUT |= BIT1;
	}
	else
	{
		P1OUT |= BIT3;
	}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
	P1OUT &= ~(BIT1 + BIT3);
	if((CBCTL1 & CBOUT) == 0)
	{
		P1OUT |= BIT1;
	}
	else
	{
		P1OUT |= BIT3;
	}
}

#pragma vector=COMP_B_VECTOR
__interrupt void Comp_B_ISR (void)
{
	P1OUT &= ~(BIT1 + BIT3);
	//CBCTL1 ^= CBIES;              // Toggles interrupt edge
	CBINT = ~CBIFG;              // Clear Interrupt flag

	P1OUT &= ~(BIT1 + BIT3);
	if((CBCTL1 & CBOUT) == 0)
	{
		P1OUT |= BIT1;
	}
	else
	{
		P1OUT |= BIT3;
	}
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P6DIR &= ~BIT0;
    P6DIR &= ~BIT2;
    P6SEL |= BIT0;
    P6SEL |= BIT2;

    P1DIR |= BIT6;
    P1SEL |= BIT6;

    P1DIR |= BIT1 | BIT3;
    P1SEL &= ~(BIT1 | BIT3);
    P1OUT |= (BIT1 | BIT3);

    CBCTL0 = CBIMEN | CBIPEN | CBIPSEL_2 | CBIMSEL_0;

    CBINT &= ~(CBIFG + CBIIFG);   // Clear any errant interrupts
    CBINT  |= CBIE;               // Enable CompB Interrupt on rising edge of CBIFG (CBIES=0)

    CBCTL1 |= CBON | CBF | CBFDLY_3;

    //TA0CCTL0 = CCIE;
    //TA0CTL = TASSEL_2 | ID_0 | MC_2 | TACLR;
    //SFRIE1 |= BIT0;
    //WDTCTL = WDTPW + WDTTMSEL + WDTCNTCL + WDTIS1 + WDTIS0;  // Toggle to interval mode, timer counter value set to 0, set timer to 1s

    __bis_SR_register(LPM0_bits + GIE);
    __no_operation();

	return 0;
}
