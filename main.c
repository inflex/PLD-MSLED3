#define F_CPU 3300000
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>



int main(void) {

	CLKCTRL.MCLKCTRLB = 0;

	PORTA.DIR |= (1 << PIN3_bp);  // PWM output
	PORTA.DIR &= ~(1 << PIN1_bp); // input 1
	PORTA.DIR &= ~(1 << PIN2_bp); // input 2

	PORTA.PIN1CTRL |= (1 << PORT_PULLUPEN_bp);
	PORTA.PIN2CTRL |= (1 << PORT_PULLUPEN_bp);

	PORTA.OUT &= ~(1<<PIN3_bp);
//	PORTA.OUT |= (1<<PIN3_bp);

//	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTA_gc;

	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_DSBOTTOM_gc;
	TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
	TCA0.SINGLE.PERBUF = 0x0080; // half of 16-bit?
	TCA0.SINGLE.CMP0BUF = 0x0040;

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;

	while(1){
		if ( (PORTA.IN & (1<<PIN1_bp)) == 0) {
			if (TCA0.SINGLE.CMP0BUF >= 1) TCA0.SINGLE.CMP0BUF -= 1;
			if (TCA0.SINGLE.CMP0BUF < 30) TCA0.SINGLE.CMP0BUF = 0;
//			PORTA.OUT &= ~(1<<PIN3_bp);
		}
		if ( (PORTA.IN & (1<<PIN2_bp)) == 0) {
			if (TCA0.SINGLE.CMP0BUF <= 0x0080) TCA0.SINGLE.CMP0BUF += 1;
			if (TCA0.SINGLE.CMP0BUF < 30) TCA0.SINGLE.CMP0BUF = 30;
//			PORTA.OUT |= (1<<PIN3_bp);
		}
		_delay_ms(10);

	}

	return 0;
}
