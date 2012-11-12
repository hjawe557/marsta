#ifndef LMP_H
#define LMP_H

#define ADC_DONE 0x18
#define ADC_AUXCN 0x12
#define BGCALCN 0x10
#define CH0_CONFIG 0x21
#define CH0_INPUTCN 0x20
#define CH_STS 0x1E
#define CH_SCAN 0x1F
#define SENDIAG_FLAGS 0x19
#define SCALCN 0x17
#define ADC_DOUT 0x1A
#define ADC_DOUTH 0x1A
#define ADC_DOUTM 0x1B
#define ADC_DOUTL 0x1C
#define ADC_RESTART 0x0B
#define RESETCN 0x00

void lmp_init(void);
uint32_t lmp_get_status(void);
uint32_t lmp_get_adc(void);
uint32_t lmp_read_data(uint8_t adr, uint8_t nr);
void lmp_write_data(uint8_t adr, uint8_t data);
void lmp_setup(void);



#endif
