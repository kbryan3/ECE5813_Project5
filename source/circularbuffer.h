 /********************************************************************
*
* @file circularbuffer.h
* @brief Interfaces for a circular bugger that holds characters
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
***********************************************************************/
#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "logger.h"

typedef enum
{
	BUFFER_PASS,
	BUFFER_FAIL,
	BUFFER_EMPTY,
	BUFFER_FULL,
	BUFFER_INVALID
}BUFF_ERROR;

typedef enum
{
	GOOD,
	EMPTY,
	FULL
}BUFF_STATUS;

typedef struct
{
	uint8_t * buffer;
	uint16_t  head;
	uint16_t  tail;
	uint16_t length;
	uint16_t count;
	BUFF_STATUS status;

}CIRCBUFF;

#define START_CRITICAL() (__disable_irq())
#define END_CRITICAL()   (__enable_irq())

/**
* @brief Initializes a circular buffer given a pointer and length in bytes
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*uint8_t* buff pointer to the buffer where the characters will be stored
*uint16_t len length of buffer in bytes
*
*@return BUFF_ERROR whether there was an error during function
*/
BUFF_ERROR initCircBuffer(CIRCBUFF* buffstruct, uint8_t* buff, uint16_t len);

/**
* @brief Adds an item to the buffer
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*uint16_t  item data being added to the buffer
*
*@return BUFF_ERROR whether buffer was full
*/
BUFF_ERROR add(CIRCBUFF* buffstruct, uint8_t item);

/**
* @brief Removes oldest item from buffer
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*
*@return BUFF_ERROR whether buffer was empty
*/
uint8_t removeItem(CIRCBUFF* buffstruct);

/**
* @brief Checks if buffer is full
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*
*@return BUFF_ERROR whether the buffer is full
*/
BUFF_ERROR buffIsFull(CIRCBUFF* buffstruct);

/**
* @brief Checks if buffer is empty
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*
*@return BUFF_ERROR whether the buffer is empty
*/
BUFF_ERROR buffIsEmpty(CIRCBUFF* buffstruct);

/**
* @brief Checks if buffer initialized
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*
*@return BUFF_ERROR whether the buffer is initialized
*/
BUFF_ERROR buffIsInit(CIRCBUFF* buffstruct);

/**
* @brief Checks if buffer pointer is valid
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*
*@return BUFF_ERROR whether the pointer is valid
*/
BUFF_ERROR buffIsPointerValid(CIRCBUFF* buffstruct);

/**
* @brief Clears a circular buffer
*
*CIRCBUFF* buffstruct pointer to CIRCBUFF structure defining a circ buffer
*
*@return BUFF_ERROR whether the buffer destroy was successful
*/
BUFF_ERROR destroyCircBuffer(CIRCBUFF* buffstruct);















#endif /* __CIRCULARBUFFER_H__ */
