#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

volatile unsigned int * LED     = (unsigned int *)0x40000000;
volatile unsigned int * DDRbase = (unsigned int *)0x90000000;

int main()
{
    init_platform();
    * DDRbase = 3;
    * LED     = * DDRbase;
    print("Hello World\n\r");
    cleanup_platform();
    return 0;
}
