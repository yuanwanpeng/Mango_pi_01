/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "transport.h"
#include "GPRS.h"
extern uint8_t g_send_Server_buf[];
extern GPRS_TypeDef							*p_GPRS,GPRS;
int transport_sendPacketBuffer(unsigned char* buf, int buflen, unsigned char ack)
{
	if(GPRS_CIPSEND(p_GPRS,buf,buflen) != ERROR)//等待IDLE中断
	{
		//printf("GPRS_CIPSEND ret success\r\n");
		return SUCCESS;
	}
	else{
		printf("GPRS_CIPSEND ret error\r\n");
		return ERROR;
	}
//	return buflen;
}


int transport_getdata(unsigned char* buf, int count)
{
	memcpy(buf, (void*)&p_GPRS->DOWN_BUF[p_GPRS->DOWN_LEN], count);//(void*)强制why?
	p_GPRS->DOWN_LEN += count;
	return count;
//	int rc = recv(mysock, buf, count, 0);
//	//printf("received %d bytes count %d\n", rc, (int)count);
//	return rc;
}

