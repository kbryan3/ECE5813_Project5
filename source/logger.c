 /********************************************************************
*
* @file logger.c
* @brief Contains functions to log and print data
*
* There are three modes the logger can be in TEST, DBUG, STATUS
*  TEST- will print ucUnit messages(only runs the uCUnit, not full program)
*  DBUG - will print program information as well as temperature readouts
*  STATUS - will only print out temperature readouts
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#ifndef PC
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#endif
#include "logger.h"
#include "uart.h"

void Init_SysTick(void)
{
	SysTick->LOAD = (4800000L/100);
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	__disable_irq();
	g_ticks++;
	__enable_irq();

}
void log_enable()
{
	log_a = 1;
}

void log_disable()
{
	log_a = 0;
}


_Bool log_status()
{
	if(log_a)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void log_data(uint32_t * loc, size_t length, logger_level level, function_called func)
{
	if(!loc)
	{
		PRINTF("Null Pointer!");
	}
	if(log_level == level)
	{
		uint8_t * bytes_ptr = (uint8_t *)loc;

		for(uint8_t i = 0; i<length; i++)
		{
#ifndef PC
			printLevel(level);
			printFunction(func);
			PRINTF("Address: 0x%X    Data: 0x%02X\n\r", bytes_ptr+i, bytes_ptr[i]);
		}

		PRINTF("\n\r");
#else
			printf("Address: %p    Data: 0x%02X\n\r", (void*)(bytes_ptr+i), bytes_ptr[i]);
		}

		printf("\n\r");
#endif
	}

}

void log_string(uint8_t * str, logger_level level, function_called func)
{
	if(!str)
	{
		Send_String_Poll((uint8_t *)"Null Pointer!");
	}
	if(log_level==level)
	{
#ifndef PC
		printLevel(level);
		printFunction(func);
		//Send_String_Poll("%s", str);
		Send_String_Poll(str);
		Send_String_Poll((uint8_t *)"\n\r");
#else
		printf("%s", str);
		printf("\n");
#endif
	}
}

void log_temp(int16_t * temperature,
		logger_level level, function_called func)
{
	if(!temperature)
	{
		PRINTF("Null Pointer!");
	}
	int32_t printTemp;
	printTemp = ((int32_t)*temperature) * .0625;
	if(log_level == level)
	{
		printLevel(level);
		printFunction(func);
		PRINTF("Temperature is: %d C", printTemp);
		PRINTF("\n\r");
	}
}

void log_int(uint32_t * integer, logger_level level, function_called func)
{
	if(!integer)
	{
		PRINTF("Null Pointer!");
	}
	if(log_level==level)
	{
#ifndef PC
		printLevel(level);
		printFunction(func);
		PRINTF("%d", integer);
		PRINTF("\n\r");
#else
	printf("%ls", integer);
	printf("\n");
#endif
	}
}

void log_time()
{
	uint32_t ticks = g_ticks;
	uint32_t hours = 36000 / ticks;
	ticks = ticks - 36000 * hours;
	uint32_t minutes = ticks / 600;
	ticks = ticks - 600 * minutes;
	uint32_t seconds = ticks / 10;
	ticks = ticks - 10 * seconds;

}

void printLevel(logger_level level)
{
	if(level == TEST)
	{
		Send_String_Poll((uint8_t *)"TEST: ");
	}
	else if(level == DBUG)
	{
		Send_String_Poll((uint8_t *)"DBUG: ");
	}
	else
	{
		Send_String_Poll((uint8_t *)"STATUS: ");
	}
}

void printFunction(function_called func)
{
	if(func == TOGGLELED)
	{
		Send_String_Poll((uint8_t *)"toggleLED(): ");
	}
	else if(func == SETALERTLOW)
	{
		Send_String_Poll((uint8_t *)"setAlertLow: ");
	}
	else if(func == PRINTTEMPERATURE)
	{
		Send_String_Poll((uint8_t *)"printTemperature: ");
	}
	else if(func == PRINTAVERAGETEMPERATURE)
	{
		Send_String_Poll((uint8_t *)"printAverageTemperature(): ");
	}
	else if(func == GETTEMPERATURE)
	{
		Send_String_Poll((uint8_t *)"getTemperature(): ");
	}
	else if(func == TEST_POINTERS)//Test pointers() called
	{
		Send_String_Poll((uint8_t *)"testPointers(): ");
	}
	else if(func == SYSTEMSHUTDOWN)
	{
		Send_String_Poll((uint8_t *)"systemShutdown(): ");
	}
	else if(func == TESTSUITE)
	{
		Send_String_Poll((uint8_t *)"testSuite(): ");
	}
	else if(func == RUNBIT)
	{
		Send_String_Poll((uint8_t *)"runBIT(): ");
	}
	else if(func == STATESTATEMACHINE)
	{
		Send_String_Poll((uint8_t *)"stateStateMachine(): ");
	}
	else if(func == STATETABLEMACHINE)
	{
		Send_String_Poll((uint8_t *)"stateTableMachine(): ");
	}
	else if(func == TEST_BASICCHECKS)
	{
		Send_String_Poll((uint8_t *)"Test_BasicChecks: ");
	}

}
