#include <msp430.h> 
#include <stdint.h>
#include "library.h"
#include "project_settings.h"
#include "hal_general.h"
#include "task.h"
#include "subsystem.h"
#include "uart.h"
#include "terminal.h"
#include "lonnie_game.h"


void GPIO_Init();

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
    GPIO_Init();
    UART_Init(SUBSYSTEM_UART);
    /* Increase the baud rate for faster response */
    UART_ReconfigureBaud(SUBSYSTEM_UART, 115200);
    EnableInterrupts();

    Game_ClearScreen();
    Game_SetColor(ForegroundWhite);
    UART_printf(SUBSYSTEM_UART, "System Initialized\r\n");
    UART_printf(SUBSYSTEM_UART, "Type '$game scrollyboy play' to begin...\r\n");
    LonnieGame_Init();
    Log_EchoOn();


    while(1)
    {
        SystemTick();
    }

    return 0;
}

void GPIO_Init()
{
    // Init pins for UART on UCA3
    // P6.0 = TX
    // P6.1 = RX
    P6SEL0 |= BIT0 | BIT1;
    P6SEL1 &= ~(BIT0 | BIT1);
}
