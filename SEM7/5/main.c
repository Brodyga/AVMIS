#include <msp430.h>

int mirror = 0;

const char symb_0[8] = { 129, //10000001
						 129, //10000001
						 153, //10011001
						 153, //10011001
						 153, //10011001
						 153, //10011001
						 129, //10000001
						 129, //10000001
					  };

const char symb_1[8] = { 249, //11111001
						 241, //11110001
						 225, //11100001
						 201, //11001001
						 249, //11111001
						 249, //11111001
						 249, //11111001
						 249, //11111001
					  };

const char symb_2[8] = { 129, //10000001
						 129, //10000001
						 249, //11111001
						 129, //10000001
						 129, //10000001
						 159, //10011111
						 129, //10000001
						 129, //10000001
					  };

const char symb_3[8] = { 129, //10000001
						 129, //10000001
						 249, //11111001
						 129, //10000001
						 129, //10000001
						 249, //11111001
						 129, //10000001
						 129, //10000001
					  };

const char symb_4[8] = { 153, //10011001
		 	 	 	 	 153, //10011001
		 	 	 	 	 153, //10011001
						 129, //10000001
						 129, //10000001
						 249, //11111001
						 249, //11111001
						 249, //11111001
					  };

const char symb_5[8] = { 129, //10000001
						 129, //10000001
						 159, //10011111
						 129, //10000001
						 129, //10000001
						 249, //11111001
						 129, //10000001
						 129, //10000001
					  };

const char symb_6[8] = { 129, //10000001
						 129, //10000001
						 159, //10011111
						 159, //10011111
						 129, //10000001
						 153, //10011001
						 153, //10011001
						 129, //10000001
					  };

const char symb_7[8] = { 129, //10000001
						 129, //10000001
						 249, //11111001
						 249, //11111001
						 249, //11111001
						 249, //11111001
						 249, //11111001
						 249, //11111001
					  };

const char symb_8[8] = { 129, //10000001
						 153, //10011001
						 153, //10011001
						 129, //10000001
						 129, //10000001
						 153, //10011001
						 153, //10011001
						 129, //10000001
					  };

const char symb_9[8] = { 129, //10000001
						 153, //10011001
						 153, //10011001
						 129, //10000001
						 249, //11111001
						 249, //11111001
						 129, //10000001
						 129, //10000001
					  };

 /* reverse:  переворачиваем строку s на месте */
 void reverse(char s[])
 {
     int i, j;
     char c;

     int len = 0;
     for(len = 0; s[len] != '\0'; len++);
     for (i = 0, j = len - 1; i < j; i++, j--)
     {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     //reverse(s);
 }

char cma3000_SPI(unsigned char byte1, unsigned char byte2)
{
  char indata;
  P3OUT &= ~BIT5; // CSB = 0 to start SPI operation
  indata = UCA0RXBUF;
  // wait TXIFG == TXBUF is ready for new data
  while (!(UCA0IFG & UCTXIFG));
  UCA0TXBUF = byte1; // send first byte
  // wait RXIFG == RXBUF have new data
  while (!(UCA0IFG & UCRXIFG));
  indata = UCA0RXBUF;
  // wait TXIFG == TXBUF is ready for new data
  while (!(UCA0IFG & UCTXIFG));
  UCA0TXBUF = byte2; // send second byte
  // wait RXIFG == RXBUF have new data
  while (!(UCA0IFG & UCRXIFG));
  indata = UCA0RXBUF;
  // wait until USCI_A0 interface is no longer busy
  while(UCA0STAT & UCBUSY);
  P3OUT |= BIT5; // CSB = 1 to stop SPI operation
  return indata;
}

void DOGS102_SPI(unsigned char byte1)
{
  // wait TXIFG == TXBUF is ready for new data
  while(!(UCB1IFG & UCTXIFG));
  P7OUT &= ~BIT4; // CS = 0 to start SPI operation
  UCB1TXBUF = byte1; // SPI transmit
  // wait until USCI_B1 interface is no longer busy
  while (UCB1STAT & UCBUSY);
  P7OUT |= BIT4; // CS = 1 to stop SPI operation
}

void GetBitValue(char s[], char val)
{
	int i;
	switch((val - '0'))
	{
	case 0:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_0[i];
		}
		break;
	case 1:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_1[i];
		}
		break;
	case 2:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_2[i];
		}
		break;
	case 3:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_3[i];
		}
		break;
	case 4:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_4[i];
		}
		break;
	case 5:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_5[i];
		}
		break;
	case 6:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_6[i];
		}
		break;
	case 7:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_7[i];
		}
		break;
	case 8:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_8[i];
		}
		break;
	case 9:
		for(i = 0; i < 8; i++)
		{
			s[i] = symb_9[i];
		}
		break;
	}
}

void WriteToLCD(char dx)
{
	int temp = dx;
	char numb[8];
	itoa(temp, numb);

    int len = 0;
    for(len = 0; numb[len] != '\0'; len++);
	//write value to LCD
	int i = 0;
	for(i = 0; i < 8; i++) //cycle for all pages
	{
		P5OUT &= ~BIT6; // DOGS102 format: command
		DOGS102_SPI(176 + i); // set page address

		char bit[8];
		if(i < len)
		{
			GetBitValue(bit, numb[i]);
		}
		int count_l = 0;
		int count_m = 0;
		if(mirror == 0)
		{
			count_l = 0;
			count_m = 0;
		}
		else
		{
			count_l = 14;
			count_m = 1;
		}
		int j = 0;
		for(j = 0; j < 8; j++) // cycle for all columns
		{
			P5OUT &= ~BIT6; // DOGS102 format: command
			if(count_l == 16)
			{
				count_l = 0;
				count_m++;
			}

			DOGS102_SPI(0 + count_l); // set LSB column address
			DOGS102_SPI(16 + count_m); // set MSB column address

			P5OUT |= BIT6; // DOGS102 format: data
			if(i < len)
			{
				DOGS102_SPI(bit[j]); // 8 pixels set
			}
			else
			{
				DOGS102_SPI(0xFF); // 8 pixels set
			}

			count_l++;
		}
	}
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	if(mirror == 0)
	{
		  int i = 0;
		  for(i = 0; i < 8; i++) //cycle for all pages
		  {
			P5OUT &= ~BIT6; // DOGS102 format: command
			DOGS102_SPI(176 + i); // set page address

			int count_l = 0;
			int count_m = 0;
			int j = 0;
			for(j = 0; j < 8; j++) // cycle for all columns
			{
				P5OUT &= ~BIT6; // DOGS102 format: command
				if(count_l == 16)
				{
					count_l = 0;
					count_m++;
				}

				DOGS102_SPI(0 + count_l); // set LSB column address
				DOGS102_SPI(16 + count_m); // set MSB column address

				P5OUT |= BIT6; // DOGS102 format: data
				DOGS102_SPI(0xFF); // 8 pixels set

				count_l++;
			}
		  }
	}
	else
	{
		  int i = 0;
		  for(i = 0; i < 8; i++) //cycle for all pages
		  {
			P5OUT &= ~BIT6; // DOGS102 format: command
			DOGS102_SPI(176 + i); // set page address

			int count_l = 14;
			int count_m = 1;
			int j = 0;
			for(j = 0; j < 8; j++) // cycle for all columns
			{
				P5OUT &= ~BIT6; // DOGS102 format: command
				if(count_l == 16)
				{
					count_l = 0;
					count_m++;
				}

				DOGS102_SPI(0 + count_l); // set LSB column address
				DOGS102_SPI(16 + count_m); // set MSB column address

				P5OUT |= BIT6; // DOGS102 format: data
				DOGS102_SPI(0xFF); // 8 pixels set

				count_l++;
			}
		  }
	}

	mirror ^= 0x01;

	if(mirror == 0)
	{
		P5OUT &= ~BIT6; // DOGS102 format: command
		DOGS102_SPI(161); // set mirror
	}
	else
	{
		P5OUT &= ~BIT6; // DOGS102 format: command
		DOGS102_SPI(160); // set mirror
	}
	P1IFG = 0;
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
  char dx;
  if (P2IN & BIT5)
  {
    P1OUT &= ~BIT0; // LED1 off
    // 110 = DOUTX register, 0 - read, 0 - predefined
    dx = cma3000_SPI(0x18, 0);
    __delay_cycles(1250);
    if (dx > 32)
    {
    	P1OUT |= BIT0; // LED1 is on
    }
    WriteToLCD(dx);
  }
}

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD; //stop watchdog
  P1DIR |= BIT0;  //LED1 as output
  P1OUT &= ~BIT0; //LED1 off

  P1DIR |= BIT7;  // Set P1.7 to output direction
  P1OUT |= BIT7;  // To connect P1REN
  P1REN |= BIT7;
  P1DIR &= ~BIT7; // Set P1.7 to input direction

  P1IE |= BIT7;
  P1IES |= BIT7;

  // ========== CMA3000 ports ==========
  P2DIR &= ~BIT5; // cma3000 INT
  P2OUT |= BIT5; // cma3000 INT pull-up resistor
  P2REN |= BIT5; // cma3000 INT enable resistor
  P2IE |= BIT5; // cma3000 INT interrupt enable
  P2IE |= BIT5; // low-to-high
  P2IES &= ~BIT5;
  P2IFG &= ~BIT5; // cma3000 INT clear int flag
  P3DIR |= BIT5; // cma3000 CSB set as output
  P3OUT |= BIT5; // CSB = 1 for disabling cma3000
  P2DIR |= BIT7; // cma3000 SCK set as output
  P2SEL |= BIT7; // device mode: P2.7 is UCA0CLK
  P3DIR |= (BIT3 | BIT6); // cma3000 MOSI, PWR set as output
  P3DIR &= ~BIT4; // cma3000 MITO set as input
  P3SEL |= (BIT3 | BIT4); // device mod: P3.3 - UCA0SIMO, P3.4 - UCA0SOMI
  P3OUT |= BIT6; // enable PWR

  // ========== DOGS102 ports ==========
  P5DIR |= (BIT7 | BIT6); // CD & RST as output
  P7DIR |= (BIT4 | BIT6); // CS & ENA as output
  P4DIR |= (BIT1 | BIT3); // CDA/SIMO & SCK as output
  P7OUT |= (BIT4 | BIT6); // CS & ENA no select, on bkLED
  P4SEL |= (BIT1 | BIT3); // P4.1 & P4.3 is UCB1SIMO & UCB1CLK
  P5OUT &= ~BIT7; // RST = 0 is reset
  __delay_cycles(25000);
  P5OUT |= BIT7; // RST = 1 is no reset
  __delay_cycles(125000);

  // ========== ACCEL initialization ==========
  UCA0CTL1 |= UCSWRST; // SPI reset logic is on
  UCA0CTL0 |= UCSYNC | UCMST | UCMSB | UCCKPH; // master, MSB first, data latch on rising
  UCA0CTL1 |= UCSWRST | UCSSEL__SMCLK; // SMCLK for clock, keep reset
  UCA0BR0 = 0x30; // low byte division
  UCA0BR1 = 0; // high byte division
  UCA0MCTL = 0; // no modulation
  UCA0CTL1 &= ~UCSWRST; // start spi interface

  // start SPI transmit
  // 1 = REVID register, 0 - read, 0 - predefined
  // second byte can be any
  cma3000_SPI(0x4, 0);
  __delay_cycles(1250);

  // 10 = CTRL register, 1 - write, 0 - predefined
  // cma3000 CTRL register set: 2g, disable I2C, 400 Hz
  cma3000_SPI(0xA, BIT7 | BIT4 | BIT2);
  __delay_cycles(25000);

  // ========== LCD initialization ==========
  UCB1CTL1 |= UCSWRST; // SPI reset logic is on
  UCB1CTL0 |= UCSYNC | UCMST | UCMSB | UCCKPH; // master, MSB first, data latch on rising
  UCB1CTL1 |= UCSWRST | UCSSEL__SMCLK; // SMCLK for clock, keep reset
  UCB1BR0 = 0x30; // low byte division
  UCB1BR1 = 0; // high byte division
  UCB1CTL1 &= ~UCSWRST; // start spi interface

  P5OUT &= ~BIT6; // DOGS102 format: command
  DOGS102_SPI(161); // set mirror column
  DOGS102_SPI(192); // set mirror row

  DOGS102_SPI(167); // set inverse mode

  DOGS102_SPI(0x2F); // power on
  DOGS102_SPI(0xAF); // display on
  //clearing LCD(set 0xFF to all bits)
  int i = 0;
  for(i = 0; i < 8; i++) //cycle for all pages
  {
	P5OUT &= ~BIT6; // DOGS102 format: command
	DOGS102_SPI(176 + i); // set page address

	int count_l = 0;
	int count_m = 0;
	int j = 0;
	for(j = 0; j < 132; j++) // cycle for all columns
	{
		P5OUT &= ~BIT6; // DOGS102 format: command
		if(count_l == 16)
		{
			count_l = 0;
			count_m++;
		}

		DOGS102_SPI(0 + count_l); // set LSB column address
		DOGS102_SPI(16 + count_m); // set MSB column address

		P5OUT |= BIT6; // DOGS102 format: data
		DOGS102_SPI(0xFF); // 8 pixels set

		count_l++;
	}
  }

  // enter LPM0, enable interrupts
  __bis_SR_register(LPM0_bits + GIE);
  __no_operation();

  return 0;
}
