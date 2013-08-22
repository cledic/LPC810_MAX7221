#include "LPC8xx.h"
#include "power_mode.h"

void sleep ( void) {

	LPC_PMU->PCON = 0;
    SCB->SCR &= ~(1<<2);

    __WFI();
}

void deep_sleep ( void) {
    LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;
    LPC_PMU->PCON = (1<<0);
    SCB->SCR |= (1<<2);
    __WFI();
}

void power_down ( void) {
    LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;
    LPC_PMU->PCON = (2<<0);
    SCB->SCR |= (1<<2);
    __WFI();
}
