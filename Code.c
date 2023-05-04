#include <msp430.h>
#include <stdint.h>
#define BAUDRATE 115200    // desired baud rate
#define SMCLK_FREQ 1000000 // SMCLK frequency
int m=0;
volatile unsigned int temp[50];
volatile unsigned int diff[50];
volatile unsigned int i=0;
volatile unsigned int j=0;
volatile unsigned char hh = 0;
volatile unsigned char hl = 0;
volatile unsigned char th = 0;
volatile unsigned char tl = 0;
volatile unsigned char checksum = 0;
volatile unsigned char check = 0;
volatile unsigned char dataok = 0 ;
volatile uint16_t adcResult;
float moisturePercent;
float DRY_VALUE = 890;
float WET_VALUE = 311;

void UART_init() {
  // Set pins 1.6 and 1.7 as UART pins
  P1SEL0 |= BIT6 + BIT7;
  P1SEL1 &= ~(BIT6 + BIT7);

  // Set the baud rate and other UART parameters
  UCA0CTLW0 |= UCSWRST;
  UCA0CTLW0 |= UCSSEL__SMCLK;  // Use SMCLK as clock source
  UCA0BRW = SMCLK_FREQ / BAUDRATE;  // Set the baud rate
  UCA0CTLW0 &= ~UCSWRST;  // Release the UART from reset
}

void UART_send_char(char c) {
  while (!(UCA0IFG & UCTXIFG));  // Wait for TX buffer to be empty
  UCA0TXBUF = c;  // Send the character
}

void UART_send_string(const char* str) {
  while (*str) {  // Loop until null-terminator is reached
    UART_send_char(*str);  // Send the current character
    str++;  // Move to the next character
  }
}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    UART_init();
    P1DIR |= BIT5;
    P1OUT &= ~BIT5;
    P5DIR |= BIT0;
    P5OUT &= ~BIT0;

    __delay_cycles(20000);

    P5OUT |= BIT0;

    __delay_cycles(5);

    P5DIR &= ~BIT0;
    P5SEL0 |= BIT0;
    P5SEL1 &= ~BIT0;


    ADCCTL0 |= ADCSHT_2 | ADCON; // 16 ADCCLK cycles for sample and hold time, ADC ON
    ADCCTL1 |= ADCSHP; // ADCCLK is used as the sampling timer
    ADCCTL2 |= ADCRES; // 12-bit resolution
    ADCMCTL0 |= ADCINCH_9; // Select channel A5 for ADC input
    ADCCTL0 |= ADCENC; // Enable conversion



    _enable_interrupts();

   while(1){

  if (m==0)
  {
         TB2CTL = TBSSEL_2|MC_2|TBCLR;
         TB2CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;

        if (i >= 40){
            for (j = 1; j <= 8; j++){
                if (diff[j] >= 100)
                    hh |= (0x01 << (8-j));
            }
            for (j = 9; j <= 16; j++){
                if (diff[j] >= 100)
                    hl |= (0x01 << (16-j));
            }
            for (j = 17; j <= 24; j++){
                if (diff[j] >= 100)
                    th |= (0x01 << (24-j));
            }
            for (j = 25; j <= 32; j++){
                if (diff[j] >= 100)
                    tl |= (0x01 << (32-j));
            }
            for (j = 33; j <= 40; j++){
                if (diff[j] >= 110)
                    checksum |= (0x01 << (40-j));
            }
            check = hh+hl+th+tl;
            if (check == checksum)
                dataok = 1;
            else
                dataok = 0;
        }
        if (th<16, th>30,hh<30, hh>50)
        {
            if (th < 16)
               { UART_send_string("It's too cold!\r\n"); }
            else if (th > 30)
               {UART_send_string("It's too hot!\r\n");}
            else if (hh > 30)
               {UART_send_string("I need more humidity!\r\n");}
            else if (hh < 50)
               {{UART_send_string("Please lower the humidity!\r\n");}
            P1OUT ^= BIT5;
           __delay_cycles(50);
           P1OUT &= ~BIT5;
           __delay_cycles(500);
        }
        m=1;
  }
  else
  {
              TBCTL = TBSSEL__ACLK | MC__CONTINUOUS; // Select ACLK as clock source, continuous mode
              TBCCR0 = 32768; // Set period to 1 second
              TBCTL |= TBCLR; // Clear timer
              ADCCTL0 |= ADCSC; // Start conversion
              __delay_cycles(1000); // Wait for ADC result to be ready
              adcResult = ADCMEM0; // Store ADC result
              moisturePercent = ((DRY_VALUE - adcResult ) * 100.0) / (DRY_VALUE - WET_VALUE);
              if (moisturePercent < 20, moisturePercent > 40)
              {        if (moisturePercent < 20)
                          { UART_send_string("Please Water Me!\r\n"); }
                       else if (moisturePercent > 40)
                       {UART_send_string("Please Drain My Water!\r\n");
                       }
                       P1OUT ^= BIT5;
                       __delay_cycles(50);
                       P1OUT &= ~BIT5;
                       __delay_cycles(500);
                    }
              m=0;
    }
   }
}
}
#pragma vector = TIMER2_B1_VECTOR
__interrupt void Timer_B2(void){
    temp[i] = TB2CCR1;
    i += 1;
    if (i>=2) diff[i-1]=temp[i-1]-temp[i-2];
    TB2CCTL1 &= ~CCIFG ;
}
