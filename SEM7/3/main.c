#include <msp430.h>

int VoltageMode = 0;
int PowerMode = 0;

void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}

void SetVcoreDown(unsigned int level)
{
  PMMCTL0_H = PMMPW_H;                                                  //enter password to change settings
  SVSMLCTL = SVMLE | SVSLE | SVSMLRRL0 * level | SVSLRVL0 * level;      //new level for SVS high and SVM high
  while(!(PMMIFG & SVSMLDLYIFG));                                       //delay
  PMMCTL0_L = PMMCOREV0 * level;                                        //new level for core voltage
  PMMCTL0_H = 0x00;
}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	VoltageMode += 1;

	if(VoltageMode == 4)
	{
		VoltageMode = 0;
	}
	if(VoltageMode == 0)
	{
		SetVcoreUp (0x02);
		SetVcoreUp (0x03);

		UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
		UCSCTL4 |= SELM_2 + SELS_2;                        // Set MCLK = REFO

		  __bis_SR_register(SCG0);                  // Disable the FLL control loop
		  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
		  UCSCTL1 = DCORSEL_2;                      // Select DCO range 50MHz operation
		  UCSCTL2 = 1 + 20;                   // Set DCO Multiplier for 25MHz
		                                            // (N + 1) * FLLRef = Fdco
		                                            // (20 + 1) * 32768 = 0.7MHz
		                                            // Set FLL Div = fDCOCLK/2
		  __bic_SR_register(SCG0);                  // Enable the FLL control loop

		  // Worst-case settling time for the DCO when the DCO range bits have been
		  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
		  // UG for optimization.
		  // 32 x 32 x 0.7MHz / 32,768 Hz ~ 22k MCLK cycles for DCO to settle
		  __delay_cycles(22937);

			P1DIR |= BIT1;
			P1OUT |= BIT1;
			P1OUT &= ~BIT2;

	}
	if(VoltageMode == 1)
	{
		  P7DIR |= BIT7;                            // ACLK set out to pin
		  P7SEL |= BIT7;

		  P5SEL |= BIT4+BIT5;                       // Select XT1
		  UCSCTL6 &= ~(XT1OFF);                     // XT1 On
		  UCSCTL6 |= XCAP_3;                        // Internal load cap
		  UCSCTL3 = SELREF__XT1CLK;                              // FLL Reference Clock = XT1

		  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive strength

		  UCSCTL4 |= SELM__XT1CLK;                        // ACLK = LFTX1 (by default)

			P1DIR |= BIT1;
			P1OUT |= BIT1;
			P1OUT &= ~BIT2;
	}

	if(VoltageMode == 2)
	{
		SetVcoreDown (0x03);
		SetVcoreDown (0x02);

		UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
		UCSCTL4 |= SELM_2 + SELS_2;                        // Set MCLK = REFO

		  __bis_SR_register(SCG0);                  // Disable the FLL control loop
		  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
		  UCSCTL1 = DCORSEL_2;                      // Select DCO range 50MHz operation
		  UCSCTL2 = 1 + 20;                   // Set DCO Multiplier for 25MHz
													// (N + 1) * FLLRef = Fdco
													// (20 + 1) * 32768 = 0.7MHz
													// Set FLL Div = fDCOCLK/2
		  __bic_SR_register(SCG0);                  // Enable the FLL control loop

		  // Worst-case settling time for the DCO when the DCO range bits have been
		  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
		  // UG for optimization.
		  // 32 x 32 x 0.7MHz / 32,768 Hz ~ 22k MCLK cycles for DCO to settle
		  __delay_cycles(22937);

			P1DIR |= BIT2;
			P1OUT |= BIT2;
			P1OUT &= ~BIT1;
	}

	if(VoltageMode == 3)
	{
		  P7DIR |= BIT7;                            // ACLK set out to pin
		  P7SEL |= BIT7;

		  P5SEL |= BIT4+BIT5;                       // Select XT1
		  UCSCTL6 &= ~(XT1OFF);                     // XT1 On
		  UCSCTL6 |= XCAP_3;                        // Internal load cap
		  UCSCTL3 = SELREF__XT1CLK;                              // FLL Reference Clock = XT1

		  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive strength

		  UCSCTL4 |= SELM__XT1CLK;                        // ACLK = LFTX1 (by default)

			P1DIR |= BIT2;
			P1OUT |= BIT2;
			P1OUT &= ~BIT1;
	}

	P1IFG = 0;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	PowerMode ^= 1;
	if(PowerMode == 0)
	{
		__bic_SR_register_on_exit(LPM1_bits + GIE);
		P8OUT &= ~BIT2;
		P1OUT |= BIT0;
		__bis_SR_register_on_exit(GIE);

		P1DIR |= BIT0;
		P1OUT |= BIT0;
		P8OUT &= ~BIT2;
	}

	if(PowerMode == 1)
	{
		__bic_SR_register_on_exit(GIE);
		P1OUT &= ~BIT0;
		P8OUT |= BIT2;
		__bis_SR_register_on_exit(LPM1_bits + GIE);


		P1OUT &= ~BIT0;
		P8DIR |= BIT2;
		P8OUT |= BIT2;
	}

	P2IFG = 0;
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	P1DIR |= BIT5;
	P1OUT ^= BIT5;        // Turn on/off LED2
}

int main(void)
{
	P7DIR |= BIT7;                        //MCLK set out to pins
	P7SEL |= BIT7;

	WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer

	P1DIR |= BIT0;
	P1OUT |= BIT0;

	P1DIR |= BIT1;
	P1OUT |= BIT1;

	P8DIR |= BIT2;
	P8OUT &= ~BIT2;

	P1DIR |= BIT7;                // Set P1.7 to output direction
	P1OUT |= BIT7;                // To connect P1REN
	P1REN |= BIT7;
	P1DIR &= ~BIT7;               // Set P1.7 to input direction

	P1IE |= BIT7;
	P1IES |= BIT7;

	P2DIR |= BIT2;                // Set P2.2 to output direction
	P2OUT |= BIT2;                // To connect P2REN
	P2REN |= BIT2;
	P2DIR &= !BIT2;               // Set P1.7 to input direction

	P2IE |= BIT2;
	P2IES |= BIT2;

	SFRIE1 |= BIT0;

	SetVcoreUp (0x01);
	SetVcoreUp (0x02);
	SetVcoreUp (0x03);

	UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	UCSCTL4 |= SELM_2;                        // Set MCLK = REFO

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_2;                      // Select DCO range 50MHz operation
	UCSCTL2 = 1 + 20;                   // Set DCO Multiplier for 25MHz
											// (N + 1) * FLLRef = Fdco
											// (20 + 1) * 32768 = 0.7MHz
										// Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 0.7MHz / 32,768 Hz ~ 22k MCLK cycles for DCO to settle
	__delay_cycles(22937);

	// Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize


	WDTCTL = WDTPW + WDTTMSEL + WDTCNTCL + WDTIS1 + WDTIS0;  // Toggle to interval mode, timer counter value set to 0, set timer to 1s

	__bis_SR_register(GIE);
	for(;;)
	{
		do
		{
			UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
													// Clear XT2,XT1,DCO fault flags
			SFRIFG1 &= ~OFIFG;                      // Clear fault flags
		}while (SFRIFG1&OFIFG);
	}
	//__no_operation();

	return 0;
}
