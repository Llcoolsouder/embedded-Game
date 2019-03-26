#include <msp430.h> 
#include <stdint.h>
#include "library.h"
#include "project_settings.h"
#include "hal_general.h"
#include "task.h"
#include "subsystem.h"
#include "uart.h"
#include "lonnie_game.h"


void CallbackTest()
{
    UART_printf(SUBSYSTEM_UART, "Printing stuff \n");
    printf("printf");
}

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    DisableInterrupts();
    Timing_Init();
    Task_Init();
    UART_Init(SUBSYSTEM_UART);
    /* Increase the baud rate for faster response */
    UART_ReconfigureBaud(SUBSYSTEM_UART, 115200);
    EnableInterrupts();

    UART_printf(SUBSYSTEM_UART, "System Initialized\r\n");
    UART_printf(SUBSYSTEM_UART, "Type '$game scrollyboy play' to begin...\r\n");
    LonnieGame_Init();
    Log_EchoOn();

    //Task_Schedule(CallbackTest, 0, 0, 100);

    while(1)
    {
        SystemTick();
    }

    return 0;
}
