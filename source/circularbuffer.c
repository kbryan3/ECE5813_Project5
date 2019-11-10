 /********************************************************************
*
* @file circularbuffer.c
* @brief Interfaces for a circular bugger that holds characters
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
* wrap around using modulo was found via the following website as well as
* algorithms for add and remove and advancing/decreminting the indexes properly
* https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/
* the modulo function is used whenever the head/tail indexes are changed to make
* sure the buffer wraps around and does not go past the total index count
***********************************************************************/
#include "circularbuffer.h"



BUFF_ERROR initCircBuffer(CIRCBUFF* buffstruct, uint8_t* buff, uint16_t len)
{
	if(buffstruct == NULL || buff == NULL)
	{
		return BUFFER_FAIL;
	}
	else
	{
		buffstruct->buffer = buff;
		buffstruct->head = 0;
		buffstruct->tail = 0;
		buffstruct->length = len;
		buffstruct->count = len - 1;
	}
	return BUFFER_PASS;
}

BUFF_ERROR add(CIRCBUFF* buffstruct, uint8_t item)
{
	//check if pointers are not null
	if(buffIsPointerValid(buffstruct) != BUFFER_PASS)
	{
		return BUFFER_INVALID;
	}
	//check is buffer is full(tail = head + 1)
	if(buffIsFull(buffstruct) == BUFFER_FULL)
	{
		return BUFFER_FULL;
	}
	else
	{
		buffstruct->buffer[buffstruct->head] = item; //add item to buffer
		buffstruct->head = (buffstruct->head + 1) % buffstruct->count; //move head
		return BUFFER_PASS;
	}

}

BUFF_ERROR removeItem(CIRCBUFF* buffstruct)
{
	//check if pointers are not null
	if(buffIsPointerValid(buffstruct) != BUFFER_PASS)
	{
		return BUFFER_INVALID;
	}
	//check if buffer is empty
	if(buffIsEmpty(buffstruct) == BUFFER_EMPTY)
	{
		return BUFFER_EMPTY;
	}
	else
	{
		PRINTF("%c", buffstruct->buffer[buffstruct->tail]);
		buffstruct->tail= (buffstruct->tail+1) % buffstruct->count;
		return BUFFER_PASS;
	}
}

BUFF_ERROR buffIsFull(CIRCBUFF* buffstruct)
{
	if((buffstruct->head + 1) == buffstruct->tail)
	{
		return BUFFER_FULL;
	}
	else
	{
		return BUFFER_PASS;
	}
}

BUFF_ERROR buffIsEmpty(CIRCBUFF* buffstruct)
{
	if(buffstruct->head == 0 && buffstruct->tail == 0)
	{
		return BUFFER_EMPTY;
	}
	else
	{
		return BUFFER_PASS;
	}
}
BUFF_ERROR buffIsInit(CIRCBUFF* buffstruct)
{
	if(buffstruct == NULL)
	{
		return BUFFER_INVALID;
	}
	else
	{
		return BUFFER_PASS;
	}
}

BUFF_ERROR buffIsPointerValid(CIRCBUFF* buffstruct)
{

	if(buffstruct == NULL)
	{
		return BUFFER_INVALID;
	}
	else if(buffstruct->buffer == NULL)
	{
		return BUFFER_INVALID;
	}
	else
	{
		return BUFFER_PASS;
	}

}

BUFF_ERROR destroyCircBuffer(CIRCBUFF* buffstruct)
{
	if(buffIsInit(buffstruct) == BUFFER_INVALID)
	{
		return BUFFER_INVALID;
	}
	else
	{
		free(buffstruct);
		return BUFFER_PASS;
	}
}

