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
		Send_String_Poll("Null Pointer!");
	}
	if(log_level==level)
	{
#ifndef PC
		printLevel(level);
		printFunction(func);
		//Send_String_Poll("%s", str);
		Send_String_Poll(str);
		Send_String_Poll("\n\r");
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

void printLevel(logger_level level)
{
	if(level == TEST)
	{
		Send_String_Poll("TEST: ");
	}
	else if(level == DBUG)
	{
		Send_String_Poll("DBUG: ");
	}
	else
	{
		Send_String_Poll("STATUS: ");
	}
}

void printFunction(function_called func)
{
	if(func == TOGGLELED)
	{
		Send_String_Poll("toggleLED(): ");
	}
	else if(func == SETALERTLOW)
	{
		Send_String_Poll("setAlertLow: ");
	}
	else if(func == PRINTTEMPERATURE)
	{
		Send_String_Poll("printTemperature: ");
	}
	else if(func == PRINTAVERAGETEMPERATURE)
	{
		Send_String_Poll("printAverageTemperature(): ");
	}
	else if(func == GETTEMPERATURE)
	{
		Send_String_Poll("getTemperature(): ");
	}
	else if(func == TEST_POINTERS)//Test pointers() called
	{
		Send_String_Poll("testPointers(): ");
	}
	else if(func == SYSTEMSHUTDOWN)
	{
		Send_String_Poll("systemShutdown(): ");
	}
	else if(func == TESTSUITE)
	{
		Send_String_Poll("testSuite(): ");
	}
	else if(func == RUNBIT)
	{
		Send_String_Poll("runBIT(): ");
	}
	else if(func == STATESTATEMACHINE)
	{
		Send_String_Poll("stateStateMachine(): ");
	}
	else if(func == STATETABLEMACHINE)
	{
		Send_String_Poll("stateTableMachine(): ");
	}
	else if(func == TEST_BASICCHECKS)
	{
		Send_String_Poll("Test_BasicChecks: ");
	}

}
