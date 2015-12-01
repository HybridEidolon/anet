/* 
Copyright (C) 1995-2001 Activision, Inc.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef mstcp_h
#define mstcp_h

#include <OpenTransport.h>
#include <OpenTptInternet.h>

#ifdef __MWERKS__
	#include <OpenTransport.h>
	#include <OpenTptInternet.h>
#else
	#include <windows.h> 
	#include <winsock.h>
#endif

#ifdef __MWERKS__
	typedef unsigned long u_long;
	typedef unsigned short u_short;
	typedef unsigned int BOOL;
	typedef unsigned long ULONG;
	typedef unsigned short USHORT;
	
	#ifndef TRUE
		#define TRUE		1
	#endif
	#ifndef FALSE
		#define FALSE		1
	#endif
#endif

#include "dictset.h"


/**
* Constants
*/

/* Magic values for port numbers */
#define TCP_SOCKET_ANY		(0)		/* Let OS choose the port number */
#define TCP_SOCKET_DEFAULT	(0x52A1+commapi_NET_PORTOFFSET) /* +1) MAD */ /* +1 for uudps */

/* Longest allowed packet */
#define TCP_MAX_USERDATALEN	512

/* Predefined TCPHANDLEs */
#define TCP_HDL_NONE	dcst_INVALID_KEY

/* Result Codes */
#include "commerr.h"
#define TCP_RES_OK				comm_STATUS_OK
#define TCP_RES_FULL			comm_STATUS_FULL
#define TCP_RES_EMPTY			comm_STATUS_EMPTY
#define TCP_RES_BAD				comm_STATUS_BAD
#define TCP_RES_NETWORK			comm_STATUS_NETWORK_NOT_PRESENT
#define TCP_RES_UNIMPLEMENTED	comm_STATUS_UNIMPLEMENTED

/**
* Types
*/

/* Address of a peer (used to specify destination/source info) */
typedef struct TCPPEER_s {
	u_long addr;	/* in network byte order */
	u_short port;	/* in network byte order */
} TCPPEER;

/* Handle for a peer (used to identify a node in the package) */
typedef dcst_key_t TCPHANDLE;

/* TCP Instance Struct (like C++ object's data) */
typedef struct tcpInstance_s
{
#ifdef __MWERKS__
	EndpointRef			udpEndpoint;
	InetSvcRef			inetService;
	BOOL				isValid;
#else
  SOCKET socket;				/* WinSock socket */
#endif
  dcst_t handles;				/* TCPHANDLE<->TCPPEER dictionary set */
  TCPHANDLE myHandle;			/* Handle for this node's address */
  TCPHANDLE broadcastHandle;	/* Handle for broadcast address */
} 
TCPINSTANCE;


/**
* Methods
*/

/* Constructor: Start a run of TCPWIN, communicating using
   the specified socket.  If socket is TCP_SOCKET_ANY, have
   OS choose an unused socket.  TCPWIN will support a maximum
   of maxHandles connections at a time. */
TCPINSTANCE * TCPWIN_Create(unsigned short maxHandles, unsigned short* socket, int *status);

/* Destructor: End a run of TCPWIN. */
void TCPWIN_Destroy(TCPINSTANCE *TCP);

/* Convert a TCPPEER to its corresponding TCPHANDLE.  If the
   TCPPEER has not yet been assigned a handle, assign one if
   and only if insert is TRUE.  If insert is false and no
   handle has been assigned, return TCP_HDL_NONE. */
TCPHANDLE TCPWIN_Address2Handle(TCPINSTANCE *TCP, TCPPEER *addr, BOOL insert);

/* Convert a TCPHANDLE to its corresponding TCPPEER.  Return
   a status code to indicate success or failure. */
int TCPWIN_Handle2Address(TCPINSTANCE *TCP, TCPHANDLE handle, TCPPEER *addr);

/* Send a packet to a node identified by a TCPHANDLE. */
int TCPWIN_PutPacket(TCPINSTANCE *TCP, void *bufptr, ULONG len, TCPHANDLE hdest);

/* Receive a packet.  The packet will be copied into bufptr, and
   the source handle will be copied into phSrc.  If pSrcAddr is
   not NULL, the source address will be copied into it. */
int TCPWIN_GetPacket(TCPINSTANCE *TCP, void *bufptr, ULONG *len, TCPHANDLE *phSrc, TCPPEER *pSrcAddr);

#endif
