#include "sys_clock.h"
#include "delay.h"
#include "stepper.h"

int main()
{
    setup_system_clk();
    setup_stepper_motor();
    
    while (1){
        //stepper_wave_drive();
        // stepper_full_drive();
        stepper_half_drive();
    }
}