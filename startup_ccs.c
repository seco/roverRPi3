//*****************************************************************************
//
// startup_ccs.c - Startup code for use with TI's Code Composer Studio.
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"


//*****************************************************************************
//
// fault_decoder.c - A fault handler with decoder, to assist with debug.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"

#ifdef __FAULT_STARTUP_DEBUG__
#include "utils/uartstdio.h"

//*****************************************************************************
//
// A structure to map fault bit IDs with a human readable text string.
//
//*****************************************************************************
typedef struct
{
    unsigned long ulFaultBits;
    char *cFaultText;
}
tFaultMap;

//*****************************************************************************
//
// Text mapping for the usage, bus, and memory faults.
//
//*****************************************************************************
tFaultMap sUsageFaultMap[6] =
{
    { NVIC_FAULT_STAT_DIV0,     "Divide by 0"           },
    { NVIC_FAULT_STAT_UNALIGN,  "Unaligned access"      },
    { NVIC_FAULT_STAT_NOCP,     "No coprocessor"        },
    { NVIC_FAULT_STAT_INVPC,    "Invalid PC"            },
    { NVIC_FAULT_STAT_INVSTAT,  "Invalid state (EPSR)"  },
    { NVIC_FAULT_STAT_UNDEF,    "Undefined instruction" }
};
tFaultMap sBusFaultMap[5] =
{
    { NVIC_FAULT_STAT_BSTKE,    "Exception stacking bus error"      },
    { NVIC_FAULT_STAT_BUSTKE,   "Exception return unstacking error" },
    { NVIC_FAULT_STAT_IMPRE,    "Imprecise error"                   },
    { NVIC_FAULT_STAT_PRECISE,  "Precise error"                     },
    { NVIC_FAULT_STAT_IBUS,     "Instruction bus error"             }
};
tFaultMap sMemFaultMap[4] =
{
    { NVIC_FAULT_STAT_MSTKE,    "Exception stacking memory access error"   },
    { NVIC_FAULT_STAT_MUSTKE,   "Exception return unstacking access error" },
    { NVIC_FAULT_STAT_DERR,     "Data access violation"                    },
    { NVIC_FAULT_STAT_IERR,     "Instruction access violation"             }
};
//*****************************************************************************
//
// Reads the NVIC fault status register and prints human readable strings
// for the bits that are set.  Also dumps the exception frame.
//
// Assumes that UART0 is already configured for use with the UARTStdio module
// and that UARTStdio is NOT configured in it's interrupt-driven mode (that is,
// UART_BUFFERED is not defined).
//
//*****************************************************************************
//void FaultDecoder(unsigned long *pulExceptionFrame)
void FaultDecoder()
{
    unsigned long ulFaultStatus;
    unsigned int uIdx;

    //
    // Read the fault status register.
    //
    ulFaultStatus = HWREG(NVIC_FAULT_STAT);

    //
    // Check for any bits set in the usage fault field.  Print a human
    // readable string for any bits that are set.
    //
    if(ulFaultStatus & 0xffff0000)
    {
        UARTprintf("\nUSAGE FAULT:\n");
        for(uIdx = 0; uIdx < 6; uIdx++)
        {
            if(ulFaultStatus & sUsageFaultMap[uIdx].ulFaultBits)
            {
                UARTprintf(" %s\n", sUsageFaultMap[uIdx].cFaultText);
            }
        }
    }

    //
    // Check for any bits set in the bus fault field.  Print a human
    // readable string for any bits that are set.
    //
    if(ulFaultStatus & 0x0000ff00)
    {
        UARTprintf("\nBUS FAULT:\n");
        for(uIdx = 0; uIdx < 5; uIdx++)
        {
            if(ulFaultStatus & sBusFaultMap[uIdx].ulFaultBits)
            {
                UARTprintf(" %s\n", sBusFaultMap[uIdx].cFaultText);
            }
        }

        //
        // Also print the faulting address if it is available.
        //
        if(ulFaultStatus & NVIC_FAULT_STAT_BFARV)
        {
            UARTprintf("BFAR = %08X\n", HWREG(NVIC_FAULT_ADDR));
        }
    }

    //
    // Check for any bits set in the memory fault field.  Print a human
    // readable string for any bits that are set.
    //
    if(ulFaultStatus & 0x000000ff)
    {
        UARTprintf("\nMEMORY MANAGE FAULT:\n");
        for(uIdx = 0; uIdx < 4; uIdx++)
        {
            if(ulFaultStatus & sMemFaultMap[uIdx].ulFaultBits)
            {
                UARTprintf(" %s\n", sMemFaultMap[uIdx].cFaultText);
            }
        }

        //
        // Also print the faulting address if it is available.
        //
        if(ulFaultStatus & NVIC_FAULT_STAT_MMARV)
        {
            UARTprintf("MMAR = %08X\n", HWREG(NVIC_MM_ADDR));
        }
    }

    //
    // Print the context of the exception stack frame.
    //
//    UARTprintf("\nException Frame\n---------------\n");
//    UARTprintf("R0   = %08X\n", pulExceptionFrame[0]);
//    UARTprintf("R1   = %08X\n", pulExceptionFrame[1]);
//    UARTprintf("R2   = %08X\n", pulExceptionFrame[2]);
//    UARTprintf("R3   = %08X\n", pulExceptionFrame[3]);
//    UARTprintf("R12  = %08X\n", pulExceptionFrame[4]);
//    UARTprintf("LR   = %08X\n", pulExceptionFrame[5]);
//    UARTprintf("PC   = %08X\n", pulExceptionFrame[6]);
//    UARTprintf("xPSR = %08X\n", pulExceptionFrame[7]);

//Reboot
    //SysCtlReset();
}

//
// For CCS implement this function in pure assembly.  This prevents the TI
// compiler from doing funny things with the optimizer.
//
//#if defined(ccs)
//    __asm("    .sect \".text:FaultHandlerDecoder\"\n"
//          "    .clink\n"
//          "    .thumbfunc FaultHandlerDecoder\n"
//          "    .thumb\n"
//          "    .global FaultHandlerDecoder\n"
//          "    .global FaultDecoder\n"
//          "FaultHandlerDecoder:\n"
//          "    bl   FaultDecoder\n"
//          "    bx lr\n");
//#endif
#endif
//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);
extern void PP0ISR(void);
extern void PP1ISR(void);

//*****************************************************************************
//
// External declaration for the reset handler that is to be called when the
// processor is started
//
//*****************************************************************************
extern void _c_int00(void);

//*****************************************************************************
//
// Linker variable that marks the top of the stack.
//
//*****************************************************************************
extern uint32_t __STACK_TOP;

//*****************************************************************************
//
// External declarations for the interrupt handlers used by the application.
//
//*****************************************************************************


//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000 or at the start of
// the program if located at a start address other than 0.
//
//*****************************************************************************
#pragma DATA_SECTION(g_pfnVectors, ".intvecs")
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((uint32_t)&__STACK_TOP),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
    FaultISR,                               // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    IntDefaultHandler,                      // The PendSV handler
    IntDefaultHandler,                      // The SysTick handler
    IntDefaultHandler,                      // GPIO Port A
    IntDefaultHandler,                      // GPIO Port B
    IntDefaultHandler,                      // GPIO Port C
    IntDefaultHandler,                      // GPIO Port D
    IntDefaultHandler,                      // GPIO Port E
    IntDefaultHandler,                      // UART0 Rx and Tx
    IntDefaultHandler,                      // UART1 Rx and Tx
    IntDefaultHandler,                      // SSI0 Rx and Tx
    IntDefaultHandler,                      // I2C0 Master and Slave
    IntDefaultHandler,                      // PWM Fault
    IntDefaultHandler,                      // PWM Generator 0
    IntDefaultHandler,                      // PWM Generator 1
    IntDefaultHandler,                      // PWM Generator 2
    IntDefaultHandler,                      // Quadrature Encoder 0
    IntDefaultHandler,                      // ADC Sequence 0
    IntDefaultHandler,                      // ADC Sequence 1
    IntDefaultHandler,                      // ADC Sequence 2
    IntDefaultHandler,                      // ADC Sequence 3
    IntDefaultHandler,                      // Watchdog timer
    IntDefaultHandler,                       // Timer 0 subtimer A
    IntDefaultHandler,                      // Timer 0 subtimer B
    IntDefaultHandler,                       // Timer 1 subtimer A
    IntDefaultHandler,                      // Timer 1 subtimer B
    IntDefaultHandler,                      // Timer 2 subtimer A
    IntDefaultHandler,                      // Timer 2 subtimer B
    IntDefaultHandler,                      // Analog Comparator 0
    IntDefaultHandler,                      // Analog Comparator 1
    IntDefaultHandler,                      // Analog Comparator 2
    IntDefaultHandler,                      // System Control (PLL, OSC, BO)
    IntDefaultHandler,                      // FLASH Control
    IntDefaultHandler,                      // GPIO Port F
    IntDefaultHandler,                      // GPIO Port G
    IntDefaultHandler,                      // GPIO Port H
    IntDefaultHandler,                      // UART2 Rx and Tx
    IntDefaultHandler,                      // SSI1 Rx and Tx
    IntDefaultHandler,                      // Timer 3 subtimer A
    IntDefaultHandler,                      // Timer 3 subtimer B
    IntDefaultHandler,                      // I2C1 Master and Slave
    IntDefaultHandler,                      // CAN0
    IntDefaultHandler,                      // CAN1
    IntDefaultHandler,                      // Ethernet
    IntDefaultHandler,                      // Hibernate
    IntDefaultHandler,                      // USB0
    IntDefaultHandler,                      // PWM Generator 3
    IntDefaultHandler,                      // uDMA Software Transfer
    IntDefaultHandler,                      // uDMA Error
    IntDefaultHandler,                      // ADC1 Sequence 0
    IntDefaultHandler,                      // ADC1 Sequence 1
    IntDefaultHandler,                      // ADC1 Sequence 2
    IntDefaultHandler,                      // ADC1 Sequence 3
    IntDefaultHandler,                      // External Bus Interface 0
    IntDefaultHandler,                      // GPIO Port J
    IntDefaultHandler,                      // GPIO Port K
    IntDefaultHandler,                      // GPIO Port L
    IntDefaultHandler,                      // SSI2 Rx and Tx
    IntDefaultHandler,                      // SSI3 Rx and Tx
    IntDefaultHandler,                      // UART3 Rx and Tx
    IntDefaultHandler,                      // UART4 Rx and Tx
    IntDefaultHandler,                      // UART5 Rx and Tx
    IntDefaultHandler,                      // UART6 Rx and Tx
    IntDefaultHandler,                      // UART7 Rx and Tx
    IntDefaultHandler,                      // I2C2 Master and Slave
    IntDefaultHandler,                      // I2C3 Master and Slave
    IntDefaultHandler,                      // Timer 4 subtimer A
    IntDefaultHandler,                      // Timer 4 subtimer B
    IntDefaultHandler,                      // Timer 5 subtimer A
    IntDefaultHandler,                      // Timer 5 subtimer B
    IntDefaultHandler,                      // FPU
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // I2C4 Master and Slave
    IntDefaultHandler,                      // I2C5 Master and Slave
    IntDefaultHandler,                      // GPIO Port M
    IntDefaultHandler,                      // GPIO Port N
    0,                                      // Reserved
    IntDefaultHandler,                      // Tamper
    PP0ISR,                      			// GPIO Port P (Summary or P0)
    PP1ISR,                      			// GPIO Port P1
    IntDefaultHandler,                      // GPIO Port P2
    IntDefaultHandler,                      // GPIO Port P3
    IntDefaultHandler,                      // GPIO Port P4
    IntDefaultHandler,                      // GPIO Port P5
    IntDefaultHandler,                      // GPIO Port P6
    IntDefaultHandler,                      // GPIO Port P7
    IntDefaultHandler,                      // GPIO Port Q (Summary or Q0)
    IntDefaultHandler,                      // GPIO Port Q1
    IntDefaultHandler,                      // GPIO Port Q2
    IntDefaultHandler,                      // GPIO Port Q3
    IntDefaultHandler,                      // GPIO Port Q4
    IntDefaultHandler,                      // GPIO Port Q5
    IntDefaultHandler,                      // GPIO Port Q6
    IntDefaultHandler,                      // GPIO Port Q7
    IntDefaultHandler,                      // GPIO Port R
    IntDefaultHandler,                      // GPIO Port S
    IntDefaultHandler,                      // SHA/MD5 0
    IntDefaultHandler,                      // AES 0
    IntDefaultHandler,                      // DES3DES 0
    IntDefaultHandler,                      // LCD Controller 0
    IntDefaultHandler,                      // Timer 6 subtimer A
    IntDefaultHandler,                      // Timer 6 subtimer B
    IntDefaultHandler,                      // Timer 7 subtimer A
    IntDefaultHandler,                      // Timer 7 subtimer B
    IntDefaultHandler,                      // I2C6 Master and Slave
    IntDefaultHandler,                      // I2C7 Master and Slave
    IntDefaultHandler,                      // HIM Scan Matrix Keyboard 0
    IntDefaultHandler,                      // One Wire 0
    IntDefaultHandler,                      // HIM PS/2 0
    IntDefaultHandler,                      // HIM LED Sequencer 0
    IntDefaultHandler,                      // HIM Consumer IR 0
    IntDefaultHandler,                      // I2C8 Master and Slave
    IntDefaultHandler,                      // I2C9 Master and Slave
    IntDefaultHandler                       // GPIO Port T
};

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    //
    // Jump to the CCS C initialization routine.  This will enable the
    // floating-point unit as well, so that does not need to be done here.
    //
    __asm("    .global _c_int00\n"
          "    b.w     _c_int00");
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
#ifdef __FAULT_STARTUP_DEBUG__
    uint32_t i = 0;
    FaultDecoder();
    //
    // Enter an infinite loop.
    //
//    while(1)
//    {
//    }
    i++;
    UARTprintf("Faults occurred: %u\n", i);
#else
    while(1);
#endif
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}
