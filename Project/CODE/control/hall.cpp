#include "car.hpp"
#include "communication.hpp"
#include "peripherals.h"

/* GPIO3_Combined_0_15_IRQn interrupt handler */
extern "C" void GPIO3_GPIO_COMB_0_15_IRQHANDLER(void) {
    /* Get pins flags */
    uint32_t pins_flags = GPIO_GetPinsInterruptFlags(GPIO3);

    Car.Machine.Halt();
    com_log("gpio3\n\r");

    /* Clear ins flags */
    GPIO_ClearPinsInterruptFlags(GPIO3, pins_flags);
}

/* GPIO1_Combined_16_31_IRQn interrupt handler */
extern "C" void GPIO1_GPIO_COMB_16_31_IRQHANDLER(void) {
    /* Get pins flags */
    uint32_t pins_flags = GPIO_GetPinsInterruptFlags(GPIO1);

    Car.Machine.Halt();
    com_log("gpio1\n\r");

    /* Clear ins flags */
    GPIO_ClearPinsInterruptFlags(GPIO1, pins_flags);
}
