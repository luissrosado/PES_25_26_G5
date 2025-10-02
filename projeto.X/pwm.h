#define PWM_FREQ 31300UL            // PWM frequency defined as 31.3 kHz

void PWM_init(uint16_t channel, uint16_t dcyc);
void PWM_updDcyc(uint16_t channel, uint16_t dcyc);
