/**
\brief cc2538-specific definition of the "debugpins" bsp module.

\author Xavier Vilajosana <xvilajosana@eecs.berkeley.edu>, August 2013.
*/


#include "debugpins.h"
#include "gpio.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "board.h"
#if (DEBUG_VIA_SERIAL == 1)
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include "uart.h"
#include "uarthal.h"
#endif


//=========================== defines =========================================
// Board dbPINS defines
#define BSP_PINA_BASE           GPIO_A_BASE
#define BSP_PIND_BASE           GPIO_D_BASE

#define BSP_PINA_4              GPIO_PIN_4      //!< PA4 -- frame -RF1.5
#define BSP_PINA_5              GPIO_PIN_5      //!< PA5 -- isr   -RF1.11

#define BSP_PIND_3              GPIO_PIN_3      //!< PD3 -- slot  -RF1.6
#define BSP_PIND_2              GPIO_PIN_2      //!< PD2 -- fsm   -RF1.8
#define BSP_PIND_1              GPIO_PIN_1      //!< PD1 -- task  -RF1.10
#define BSP_PIND_0              GPIO_PIN_0      //!< PD0 -- radio -RF1-12


#if (DEBUG_VIA_SERIAL == 1)

#define DBG_MAX_REPLY_SIZE       20
//=========================== variables =======================================
// static char pucUdpLog[DBG_MAX_REPLY_SIZE + 1];
// static uint32_t g_ui32Base = UART0_BASE;
static const char * const g_pcHex = "0123456789abcdef";

extern void UARTprintf(const char *pcString, ...);
extern void UARTvprintf(const char *pcString, __VALIST vaArgP);
#endif
//=========================== prototypes ======================================

port_INLINE void bspDBpinToggle(uint32_t base,uint8_t ui8Pin);

//=========================== public ==========================================

void debugpins_init() {
   GPIOPinTypeGPIOOutput(BSP_PINA_BASE, BSP_PINA_4 | BSP_PINA_5);
   GPIOPinTypeGPIOOutput(BSP_PIND_BASE, BSP_PIND_3 | BSP_PIND_2 | BSP_PIND_1 | BSP_PIND_0);

   GPIOPinWrite(BSP_PINA_BASE, (BSP_PINA_4 | BSP_PINA_5), 0x00);
   GPIOPinWrite(BSP_PIND_BASE, (BSP_PIND_3 | BSP_PIND_2 | BSP_PIND_1 | BSP_PIND_0), 0);
}

// PA4
void debugpins_frame_toggle() {
   bspDBpinToggle(BSP_PINA_BASE, BSP_PINA_4);
}
void debugpins_frame_clr() {
    GPIOPinWrite(BSP_PINA_BASE, BSP_PINA_4, 0);
}
void debugpins_frame_set() {
   GPIOPinWrite(BSP_PINA_BASE, BSP_PINA_4, BSP_PINA_4);
}

// PD3
void debugpins_slot_toggle() {
	bspDBpinToggle(BSP_PIND_BASE, BSP_PIND_3);
}
void debugpins_slot_clr() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_3, 0);
}
void debugpins_slot_set() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_3, BSP_PIND_3);
}

// PD2
void debugpins_fsm_toggle() {
	bspDBpinToggle(BSP_PIND_BASE, BSP_PIND_2);
}
void debugpins_fsm_clr() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_2, 0);
}
void debugpins_fsm_set() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_2, BSP_PIND_2);
}

// PD1
void debugpins_task_toggle() {
	bspDBpinToggle(BSP_PIND_BASE,BSP_PIND_1);
}
void debugpins_task_clr() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_1, 0);
}
void debugpins_task_set() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_1, BSP_PIND_1);
}

// PA5
void debugpins_isr_toggle() {
	bspDBpinToggle(BSP_PINA_BASE, BSP_PINA_5);
}
void debugpins_isr_clr() {
	GPIOPinWrite(BSP_PINA_BASE, BSP_PINA_5, 0);
}
void debugpins_isr_set() {
	GPIOPinWrite(BSP_PINA_BASE, BSP_PINA_5, BSP_PINA_5);
}

// PD0
void debugpins_radio_toggle() {
	bspDBpinToggle(BSP_PIND_BASE, BSP_PIND_0);
}
void debugpins_radio_clr() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_0, 0);
}
void debugpins_radio_set() {
	GPIOPinWrite(BSP_PIND_BASE, BSP_PIND_0, BSP_PIND_0);
}

//------------ private ------------//

port_INLINE void bspDBpinToggle(uint32_t base, uint8_t ui8Pin)
{
    //
    // Get current pin values of selected bits
    //
    uint32_t ui32Toggle = GPIOPinRead(base, ui8Pin);

    //
    // Invert selected bits
    //
    ui32Toggle = (~ui32Toggle) & ui8Pin;

    //
    // Set GPIO
    //
    GPIOPinWrite(base, ui8Pin, ui32Toggle);
}



#if (DEBUG_VIA_SERIAL == 0)
void UARTprintf(const char *pcString, ...)
{

}

#else

/*
uint8_t debug_send_frame(char *frame, uint8_t size)
{
 	uint16_t ui16Length=size;
    register uint16_t ui16Idx = 0;

    DISABLE_INTERRUPTS();
    while((ui16Idx < ui16Length))
    {
    	uart_writeByte(frame[ui16Idx++]);
    }
    ENABLE_INTERRUPTS();

    return ((uint8_t) ui16Length);
}

void UdpLogMsg(char *pucMsg, ...)
{
  int iUdpLogSize;
  char * pch;

  pch=strchr(pucMsg,0x00);   //find the end of the string
  iUdpLogSize = pch - pucMsg;

  if (iUdpLogSize >= DBG_MAX_REPLY_SIZE)
    iUdpLogSize = DBG_MAX_REPLY_SIZE;

  memcpy(pucUdpLog, pucMsg, iUdpLogSize);

  strcat(pucUdpLog, "\r\n");
  debug_send_frame(pucUdpLog, iUdpLogSize+2);

}
*/


//*****************************************************************************
//
//! Writes a string of characters to the UART output.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//! \param ui32Len is the length of the string to transmit.
//!
//! This function will transmit the string to the UART output.  The number of
//! characters transmitted is determined by the \e ui32Len parameter.  This
//! function does no interpretation or translation of any characters.  Since
//! the output is sent to a UART, any LF (/n) characters encountered will be
//! replaced with a CRLF pair.
//!
//! Besides using the \e ui32Len parameter to stop transmitting the string, if a
//! null character (0) is encountered, then no more characters will be
//! transmitted and the function will return.
//!
//! In non-buffered mode, this function is blocking and will not return until
//! all the characters have been written to the output FIFO.  In buffered mode,
//! the characters are written to the UART transmit buffer and the call returns
//! immediately.  If insufficient space remains in the transmit buffer,
//! additional characters are discarded.
//!
//! \return Returns the count of characters written.
//
//*****************************************************************************
int
UARTwrite(const char *pcBuf, uint32_t ui32Len)
{
#ifdef UART_BUFFERED
    unsigned int uIdx;

    //
    // Check for valid arguments.
    //
    ASSERT(pcBuf != 0);
    ASSERT(g_ui32Base != 0);

    //
    // Send the characters
    //
    for(uIdx = 0; uIdx < ui32Len; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        //
        if(pcBuf[uIdx] == '\n')
        {
            if(!TX_BUFFER_FULL)
            {
                g_pcUARTTxBuffer[g_ui32UARTTxWriteIndex] = '\r';
                ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxWriteIndex);
            }
            else
            {
                //
                // Buffer is full - discard remaining characters and return.
                //
                break;
            }
        }

        //
        // Send the character to the UART output.
        //
        if(!TX_BUFFER_FULL)
        {
            g_pcUARTTxBuffer[g_ui32UARTTxWriteIndex] = pcBuf[uIdx];
            ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxWriteIndex);
        }
        else
        {
            //
            // Buffer is full - discard remaining characters and return.
            //
            break;
        }
    }

    //
    // If we have anything in the buffer, make sure that the UART is set
    // up to transmit it.
    //
    if(!TX_BUFFER_EMPTY)
    {
        UARTPrimeTransmit(g_ui32Base);
        UARTIntEnable(g_ui32Base, UART_INT_TX);
    }

    //
    // Return the number of characters written.
    //
    return(uIdx);
#else
    unsigned int uIdx;

    //
    // Check for valid UART base address, and valid arguments.
    //
    //ASSERT(g_ui32Base != 0);
    //ASSERT(pcBuf != 0);

    //
    // Send the characters
    //
    DISABLE_INTERRUPTS();
    for(uIdx = 0; uIdx < ui32Len; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        //
        if(pcBuf[uIdx] == '\n')
        {
        	uart_writeByte('\r');
        }

        //
        // Send the character to the UART output.
        //
        //UARTCharPut(g_ui32Base, );
		uart_writeByte(pcBuf[uIdx]);
    }
    ENABLE_INTERRUPTS();

    //
    // Return the number of characters written.
    //
    return(uIdx);
#endif
}

//*****************************************************************************
//
//! A simple UART based vprintf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param vaArgP is a variable argument list pointer whose content will depend
//! upon the format string passed in \e pcString.
//!
//! This function is very similar to the C library <tt>vprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments in the variable arguments list must match the
//! requirements of the format string.  For example, if an integer was passed
//! where a string was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void
UARTvprintf(const char *pcString, __VALIST vaArgP)
{
    uint32_t ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg;
    char *pcStr, pcBuf[16], cFill;

    //
    // Check the arguments.
    //
    //ASSERT(pcString != 0);

    //
    // Loop while there are more characters in the string.
    //
    while(*pcString)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for(ui32Idx = 0; (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0');
            ui32Idx++)
        {
        }

        //
        // Write this portion of the string.
        //
        UARTwrite(pcString, ui32Idx);

        //
        // Skip the portion of the string that was written.
        //
        pcString += ui32Idx;

        //
        // See if the next character is a %.
        //
        if(*pcString == '%')
        {
            //
            // Skip the %.
            //
            pcString++;

            //
            // Set the digit count to zero, and the fill character to space
            // (i.e. to the defaults).
            //
            ui32Count = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
again:

            //
            // Determine how to handle the next character.
            //
            switch(*pcString++)
            {
                //
                // Handle the digit characters.
                //
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    //
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    //
                    if((pcString[-1] == '0') && (ui32Count == 0))
                    {
                        cFill = '0';
                    }

                    //
                    // Update the digit count.
                    //
                    ui32Count *= 10;
                    ui32Count += pcString[-1] - '0';

                    //
                    // Get the next character.
                    //
                    goto again;
                }

                //
                // Handle the %c command.
                //
                case 'c':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Print out the character.
                    //
                    UARTwrite((char *)&ui32Value, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %d and %i commands.
                //
                case 'd':
                case 'i':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    //
                    if((int32_t)ui32Value < 0)
                    {
                        //
                        // Make the value positive.
                        //
                        ui32Value = -(int32_t)ui32Value;

                        //
                        // Indicate that the value is negative.
                        //
                        ui32Neg = 1;
                    }
                    else
                    {
                        //
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %s command.
                //
                case 's':
                {
                    //
                    // Get the string pointer from the varargs.
                    //
                    pcStr = va_arg(vaArgP, char *);

                    //
                    // Determine the length of the string.
                    //
                    for(ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++)
                    {
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcStr, ui32Idx);

                    //
                    // Write any required padding spaces
                    //
                    if(ui32Count > ui32Idx)
                    {
                        ui32Count -= ui32Idx;
                        while(ui32Count--)
                        {
                            UARTwrite(" ", 1);
                        }
                    }

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %u command.
                //
                case 'u':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; i.e. %X will use lower case letters for a-f
                // instead of the upper case letters is should use.  We also
                // alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 16.
                    //
                    ui32Base = 16;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Determine the number of digits in the string version of
                    // the value.
                    //
convert:
                    for(ui32Idx = 1;
                        (((ui32Idx * ui32Base) <= ui32Value) &&
                         (((ui32Idx * ui32Base) / ui32Base) == ui32Idx));
                        ui32Idx *= ui32Base, ui32Count--)
                    {
                    }

                    //
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    //
                    if(ui32Neg)
                    {
                        ui32Count--;
                    }

                    //
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    //
                    if(ui32Neg && (cFill == '0'))
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';

                        //
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    //
                    if((ui32Count > 1) && (ui32Count < 16))
                    {
                        for(ui32Count--; ui32Count; ui32Count--)
                        {
                            pcBuf[ui32Pos++] = cFill;
                        }
                    }

                    //
                    // If the value is negative, then place the minus sign
                    // before the number.
                    //
                    if(ui32Neg)
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';
                    }

                    //
                    // Convert the value into a string.
                    //
                    for(; ui32Idx; ui32Idx /= ui32Base)
                    {
                        pcBuf[ui32Pos++] = g_pcHex[(ui32Value / ui32Idx) % ui32Base];
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcBuf, ui32Pos);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %% command.
                //
                case '%':
                {
                    //
                    // Simply write a single %.
                    //
                    UARTwrite(pcString - 1, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle all other commands.
                //
                default:
                {
                    //
                    // Indicate an error.
                    //
                    UARTwrite("ERROR", 5);

                    //
                    // This command has been handled.
                    //
                    break;
                }
            }
        }
    }
}

//*****************************************************************************
//
//! A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param ... are the optional arguments, which depend on the contents of the
//! format string.
//!
//! This function is very similar to the C library <tt>fprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments after \e pcString must match the requirements of
//! the format string.  For example, if an integer was passed where a string
//! was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void
UARTprintf(const char *pcString, ...)
{
	__VALIST vaArgP;

    //
    // Start the varargs processing.
    //
    va_start(vaArgP, pcString);

    UARTvprintf(pcString, vaArgP);

    //
    // We're finished with the varargs now.
    //
    va_end(vaArgP);
}

#endif /* (DEBUG_VIA_SERIAL == 1)*/

