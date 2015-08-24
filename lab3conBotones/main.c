#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define LEFT_BUTTON             GPIO_PIN_4
#define RIGHT_BUTTON            GPIO_PIN_0
#define BUTTONS_GPIO_BASE       GPIO_PORTF_BASE
#define ALL_BUTTONS             (LEFT_BUTTON | RIGHT_BUTTON)

#define	FALSE	0
#define	TRUE	1

uint8_t ui8PinData=2;
uint32_t ui32Buttons=0, ui32ButtonsAnterior=0;
bool onOff = FALSE;
bool BotonEncender=FALSE, BotonEncenderAnterior=FALSE;
bool BotonPosicion=FALSE, BotonPosicionAnterior=FALSE;

uint8_t ui8PinAnterior=0;

bool AntiRebote (uint32_t, bool);

int main(void)
{
	// Selecciona un cristal de 16 Mhz.
	// Adem�s selecciona el PLL de 400Mhz. Dividida por 2 (esto es por defecto), esta es la frecuencia de uso.
	// Adem�s divide por 5 la salida del PLL.
	// La frecuencia resultante es de 40 Mhz.
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	// Antes de realizar llamadas a alguna funci�n definida
	// en la librer�a "driverlib", debemos habilitar el reloj
	// para el GPIO que vayamos a utilizar.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	// Define los tres pines del puerto F conectados a los leds, como salidas
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	// Define los tres pines del puerto F conectados a los switch, como salida
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, LEFT_BUTTON|RIGHT_BUTTON);


	while(1)
	{
		// Almaceno el valor de los botones
//		ui32Buttons = GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS);
		BotonEncender = AntiRebote(LEFT_BUTTON, BotonEncenderAnterior);

		// Bot�n izquierdo prende y apaga leds
	    if (!BotonEncenderAnterior && BotonEncender) {
	    	if (onOff) {
	    		// Apaga el led
	    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
	    		onOff = 0;
	    	} else {
	    		// Prende el led
	    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, ui8PinData);
	    		onOff = 1;
	    	}

	    }

		BotonPosicion = AntiRebote(RIGHT_BUTTON, BotonPosicionAnterior);
	    // Bot�n derecho cambia de color
	    if (!BotonPosicionAnterior && BotonPosicion) {
	    	if(ui8PinData==8) {
	    		ui8PinData=2;
	    	} else {
	    		ui8PinData=ui8PinData*2;
	    	}
	    }
	    BotonEncenderAnterior=BotonEncender;
	    BotonPosicionAnterior=BotonPosicion;
	}
}

bool AntiRebote (uint32_t BitPosition, bool Anterior)
{
	uint32_t ui32Buttons=0;
	bool Actual=FALSE;

	ui32Buttons = GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS);
    if (ui32Buttons & BitPosition)
    	Actual=TRUE;
    else
    	Actual=FALSE;
    if (Anterior != Actual) {
    	SysCtlDelay(100000);
    	ui32Buttons = GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS);
        if (ui32Buttons & BitPosition)
        	Actual=TRUE;
        else
        	Actual=FALSE;
    }

    return Actual;
}
