#ifndef EXTI_H
#define EXTI_H
void setup_external_interrupt(void);
void clear_exti_pending_flag(int8_t interrupt_line);
#endif 