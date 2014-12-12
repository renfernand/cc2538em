#include "board.h"
#include "stdio.h"
#include <string.h>
#include "hw_ioc.h"             // Access to IOC register defines
#include "hw_ssi.h"             // Access to SSI register defines
#include "hw_sys_ctrl.h"        // Clocking control
#include "hw_memmap.h"
#include "ioc.h"                // Access to driverlib ioc fns
#include "gpio.h"               // Access to driverlib gpio fns
#include "osens.h"
#include "osens_itf.h"
#include "opentimers.h"
#include "scheduler.h"
#include "board.h"
#include "sys_ctrl.h"
#include "mylinkxs.h"
#include "leds.h"
#include "hw_ints.h"
#include "interrupt.h"

extern osens_point_ctrl_t sensor_points;
static volatile bool g_bGPIOIntFlag = false;
static volatile uint8_t g_Lightchange;
void bspLedToggle(uint8_t ui8Leds);

void light_mote_sm(void)
{
	uint8_t ucAux,ucBotao;

	//pega o valor da lampada
	sensor_points.points[0].value.value.u8 = light_get_value();


	//PROVISORIO!!!!acende lampada
	ucBotao = GPIOPinRead(GPIO_A_BASE, GPIO_PIN_3);
	if ((ucBotao & GPIO_PIN_3) == 0) {
		osens_liga_lampada_local();
	}

}

//TODO!!!! MUDAR PARA O RESPECTIVOS PINOS DE IO.
void light_init(void) {

	g_Lightchange = 0;

    //config leds saida
    GPIOPinTypeGPIOOutput(BSP_LIGHT_LED_OUT_BASE, BSP_LIGHT_LED_OUT);

    //config leds retorno
    //GPIOPinTypeGPIOInput(BSP_LIGHT_LED_IN_BASE, BSP_LIGHT_LED_IN);
	//IOCPadConfigSet(BSP_LIGHT_LED_IN_BASE, BSP_LIGHT_LED_IN, IOC_OVERRIDE_DIS);

    //config botao
	GPIOPinTypeGPIOInput(BSP_LIGHT_BOTAO_BASE,BSP_LIGHT_BOTAO);
	IOCPadConfigSet(BSP_LIGHT_BOTAO_BASE, BSP_LIGHT_BOTAO, IOC_OVERRIDE_PUE);


#if 0
    //
    // Set the type of interrupt for Port A - Pin 3.  In this example we will
    // use a falling edge.  You have the option to do any of the following:
    // rising edge, falling edge, both edges, low level, or high level.
    //
    GPIOIntTypeSet(BSP_LIGHT_BOTAO_BASE, BSP_LIGHT_BOTAO, GPIO_FALLING_EDGE);

    //
    // Enable the GPIO interrupt.
    //
    GPIOPinIntEnable(BSP_LIGHT_BOTAO_BASE, BSP_LIGHT_BOTAO);

    //
    // Enable the GPIOA interrupt on the processor (NVIC).
    //
    IntEnable(BSP_LIGHT_INT_GPIO);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();
#endif

	light_off();
}

void light_timer(void) {
   // switch off the light pulse
   light_off();
}

void light_on(void) {

	g_Lightchange = 1;
    GPIOPinWrite(BSP_LIGHT_LED_OUT_BASE, BSP_LIGHT_LED_OUT, BSP_LIGHT_LED_OUT);

	//PROVISORIO!!!!acende lampada
	if (g_Lightchange)
	{
		g_Lightchange = 0;
		bspLedToggle(BSP_LIGHT_LED_IN);
	}

}

void light_off(void) {
    GPIOPinWrite(BSP_LIGHT_LED_OUT_BASE, BSP_LIGHT_LED_OUT, 0);
}


uint8_t light_get_value(void)
{

	if (GPIOPinRead(BSP_LIGHT_LED_IN_BASE, BSP_LIGHT_LED_IN) & BSP_LIGHT_LED_IN)
		return 1;
	else
		return 0;

}

//*****************************************************************************
//
// The interrupt handler for the for GPIO J interrupt.
//
//*****************************************************************************
void GPIOAIntHandler(void)
{
    uint32_t ui32GPIOIntStatus;

    //
    // Get the masked interrupt status.
    //
    ui32GPIOIntStatus = GPIOPinIntStatus(BSP_LIGHT_BOTAO_BASE, true);

    //
    // Simple debounce function wait for button de-press
    //
    while(!GPIOPinRead(BSP_LIGHT_BOTAO_BASE, BSP_LIGHT_BOTAO))
    {
    }

    //
    // Acknowledge the GPIO  - Pin n interrupt by clearing the interrupt flag.
    //
    GPIOPinIntClear(BSP_LIGHT_BOTAO_BASE, ui32GPIOIntStatus);

    //
    // Set an interrupt flag to indicate an interrupt has occurred.
    //
    g_bGPIOIntFlag = true;
}





