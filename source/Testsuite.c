/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : Testsuite.h                                                *
 *  Description : Sample testsuite                                           *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */

/*********************************************************
 *
 *  File was edited to test a Circular Buffer
 *
 *  Kyle Bryan
 *  November 2019
 *  Version 1.0
 *
 *
 */

#include "System.h"
#include "uCUnit-v1.0.h"
#include "Testsuite.h"
#include "circularbuffer.h"

void Testsuite_RunTests(void)
{
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();
  /* Init FSL debug console. */
  BOARD_InitDebugConsole();
  BUFF_ERROR error;
  UCUNIT_TestcaseBegin("Test Begin");
  CIRCBUFF * tx_buffer = (CIRCBUFF *)malloc(20);
  uint8_t * transmit = (uint8_t *)malloc(256);
//Check that pointers are not null
  UCUNIT_CheckIsNotNull(tx_buffer);
  UCUNIT_CheckIsNotNull(transmit);
//test data, empty and full
  memset(transmit, 0, 4);
  initCircBuffer(tx_buffer, transmit, 4);
  UCUNIT_CheckIsEqual(EMPTY, tx_buffer->status); //test it is empty
  error = add(tx_buffer, 'a');
  UCUNIT_CheckIsEqual(BUFFER_PASS, error);
  UCUNIT_CheckIsEqual(GOOD, tx_buffer->status);
  error = add(tx_buffer, 'b');
  error = add(tx_buffer, 'c');
  error = add(tx_buffer, 'd');
  UCUNIT_CheckIsEqual(FULL, tx_buffer->status);
  error = add(tx_buffer, 'e');
  UCUNIT_CheckIsEqual(BUFFER_FULL, error);
  error = removeItem(tx_buffer);
  error = removeItem(tx_buffer);
  error = removeItem(tx_buffer);
  error = removeItem(tx_buffer);
  UCUNIT_CheckIsEqual(EMPTY, tx_buffer->status);
//check wraparound works
  error = add(tx_buffer, 'b');
  error = add(tx_buffer, 'c');
  UCUNIT_CheckIsEqual('c', tx_buffer->buffer[0]);

  UCUNIT_TestcaseEnd();
  UCUNIT_WriteSummary();
  UCUNIT_Shutdown();
}
