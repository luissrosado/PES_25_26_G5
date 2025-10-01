
#define SPI_SS_ANS       ANSELBbits.ANSB3 
#define SPI_SS_TRIS      TRISBbits.TRISB3
#define SPI_SS_PORT      PORTBbits.RB3
#define LED_TRIS         TRISBbits.TRISB14
#define LED_LAT          LATBbits.LATB14


void SPI1Init(void);
void __attribute__((interrupt, no_auto_psv)) _SPI1RXInterrupt(void);


