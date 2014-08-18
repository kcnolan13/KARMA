//////////////////////////////////////////////////////////////////////////////
//++
//
// SocketHelpers.h
//
// Description:
//	  Contains definitions of functions and other utilities designed to 
//    add robustness to our socket implementation.
//
// Notes:
//    These functions and utilities have been adapted, with
//    permission, from the book:
//    UNIX Network Programming, The Sockets Networking API
//    Volume 1, Third Edition
//    by W. Richard Stevens, Bill Fenner, and Andrew M Rudoff
//
// Copyright 2009 - Remote Sensing Solutions Inc.
//					Barnstable, MA
//
// Creation Author: Dan Robinson
// Creation Date:   04/08/2009
//
//--
//////////////////////////////////////////////////////////////////////////////

#ifndef SOCKETHELPERS_H_
#define SOCKETHELPERS_H_

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>

#include "SystemConstants.h"

// Function prototypes
ssize_t	readn(int fd, void *vptr, size_t n);
ssize_t	writen(int fd, const void *vptr, size_t n);
void sreverse(char *s, int buflen);
int32_t  CreateNewTCPListenSocket( uint32_t port, uint32_t reuse = 0, char *szPrimaryIP=NULL, char *szSecondaryIP=NULL  );
ESYSReturn WriteToSysLog( int32_t nSeverity, char *pMessage, char *pComponent, char *pRemedy=NULL );
void DropTrailingSlash( const char *pInString, char *pOutString );
bool DoesFileExist( const char *szFileName );
char *GetLibraryVersion( void );
ESYSReturn SubscribeForSystemMessages( ESYSProcess processID, int32_t sockfd,
									   IPCMsgType msgType, int32_t nSubscriptionFlag );
ESYSReturn SendProcessInfo( ESYSProcess processID, int32_t sockfd );
ESYSReturn SubscribeForDSNs( ESYSProcess processID, int32_t sockfd, 
							int32_t nDSNFilter=DSN_FILTER_ALL_DATA );

/* Signal message to real-time side to apply change. RT side
   will take data and attempt to acknowledge. If it is ok,
   the data will be written out to shm (the running config). */
int cam_signal(struct cam_signal *signal, struct cam_signal_resp *resp);

#endif /*SOCKETHELPERS_H_*/
