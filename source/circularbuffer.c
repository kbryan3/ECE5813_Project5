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
		buffstruct->status = EMPTY;
	}
	return BUFFER_PASS;
}

BUFF_ERROR add(CIRCBUFF* buffstruct, uint8_t item)
{
	//check if pointers are not null
	if(buffIsPointerValid(buffstruct) != BUFFER_PASS)
	{
		log_string((uint8_t*)"BUFFER INVALID", DBUG, ADD);
		return BUFFER_INVALID;
	}
	//check is buffer is already full so no more writes
	if(buffstruct->status == FULL)
	{
		log_string((uint8_t*)"BUFFER FULL", DBUG, ADD);
		return BUFFER_FULL;
	}
	else
	{
		buffstruct->buffer[buffstruct->head] = item; //add item to buffer
		buffstruct->status = GOOD;
		log_string((uint8_t*)"ADDING TO BUFFER: ", DBUG, ADD);
		START_CRITICAL();
		//check if buffer is full and set status if so and do not increment
		if(buffIsFull(buffstruct) == BUFFER_FULL)
		{
			buffstruct->status = FULL;
			return BUFFER_PASS;
		}
		buffstruct->head = (buffstruct->head + 1) % buffstruct->length; //move head
		END_CRITICAL();
		return BUFFER_PASS;
	}

}

uint8_t removeItem(CIRCBUFF* buffstruct)
{
	uint8_t c = 0;
	//check if pointers are not null
	if(buffIsPointerValid(buffstruct) != BUFFER_PASS)
	{
		log_string((uint8_t*)"BUFFER INVALID: ", DBUG, REMOVEITEM);
		return c;
	}
	//check if buffer is currently empty, if so exit
	if(buffstruct->status == EMPTY)
	{
		log_string((uint8_t*)"BUFFER EMPTY: ", DBUG, REMOVEITEM);
		return c;
	}
	else
	{
		buffstruct->status = GOOD;
		log_string((uint8_t*)"Removing Item: ", DBUG, REMOVEITEM);
		//check if buffer pointers are equal before incrementing tail
		if(buffIsEmpty(buffstruct) == BUFFER_EMPTY)
		{
			buffstruct->status = EMPTY;
			return buffstruct->buffer[buffstruct->tail];
		}
		c = buffstruct->buffer[buffstruct->tail];
		START_CRITICAL();
		buffstruct->tail= (buffstruct->tail+1) % buffstruct->length;
		if(buffIsEmpty(buffstruct) == BUFFER_EMPTY)
		{
			buffstruct->status = EMPTY;
		}
		END_CRITICAL();
		return c;
	}
}

BUFF_ERROR buffIsFull(CIRCBUFF* buffstruct)
{
	if(((buffstruct->head + 1) % (buffstruct->length)) == buffstruct->tail)
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
	if(buffstruct->tail == buffstruct->head)
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
		free(buffstruct->buffer);
		free(buffstruct);
		return BUFFER_PASS;
	}
}

