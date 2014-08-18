//////////////////////////////////////////////////////////////////////////////
//++
//
// SocketHelpers.cpp
//
// Description:
//	  Contains implementation of functions and other utilities designed to 
//    add robustness to our socket implementation.
//
// Notes:
//
// Copyright 2009 - Remote Sensing Solutions Inc.
//					Barnstable, MA
//
// Creation Author: Dan Robinson
// Creation Date:   04/08/2009
//
//--
//////////////////////////////////////////////////////////////////////////////
using namespace std;

#include "Helpers.h"
#include "SystemConstants.h"
#include "SystemTextStrings.h"
#include "Message.h"

//////////////////////////////////////////////////////////////////////
//++
//
// Method: readn
//
// Description:
//	  Reads a given number of bytes from a socket into a buffer.
//
//    Adapted with permission from the book:
//    UNIX Network Programming, The Sockets Networking API
//    Volume 1, Third Edition
//    by W. Richard Stevens, Bill Fenner, and Andrew M Rudoff
//
// Inputs:
//    fd - socket file descriptor
//    vptr - buffer to read data into
//    n - number of bytes to read from socket
//
// Outputs:
//    None
//
// Return Value:
//    number of bytes read from the socket
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ssize_t	readn(int fd, void *vptr, size_t n)
{
	if (TRACE_CALLS) printf("Helpers - readn()\n");
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else
				return(-1);
		} else if (nread == 0)
			break;				/* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return( n - nleft );		/* return >= 0 */
}
/* end readn */

//////////////////////////////////////////////////////////////////////
//++
//
// Method: writen
//
// Description:
//	  Writes a given number of bytes to a socket from a buffer.
//
//    Adapted with permission from the book:
//    UNIX Network Programming, The Sockets Networking API
//    Volume 1, Third Edition
//    by W. Richard Stevens, Bill Fenner, and Andrew M Rudoff
//
// Inputs:
//    fd - socket file descriptor
//    vptr - buffer to write from
//    n - number of bytes to write to socket
//
// Outputs:
//    None
//
// Return Value:
//    number of bytes written to the socket
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ssize_t	writen(int fd, const void *vptr, size_t n)
{
	if (TRACE_CALLS) printf("Helpers - writen()\n");
	int		nleft = n;
	int		nwritten = 0;
	const char	*ptr = (char *)vptr;
	
	while(nleft > 0)
	 {
 		nwritten = write(fd, ptr, nleft);

		if( nwritten <= 0)
		{
			if (nwritten < 0 && errno == EINTR)
			{
				nwritten = 0;		/* and call write() again */
			}
			else
			{
				return(-1);			/* error */
			}
		}

		nleft -= nwritten;
		ptr   += nwritten;
		
	}
	
	return( n );
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: sreverse
//
// Description:
//	  Swaps bytes in a buffer.
//
// Inputs:
//    s - buffer containing bytes to swap
//    buflen - length of the buffer
//
// Outputs:
//    None
//
// Return Value:
//    None
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

void sreverse(char *s, int buflen)
{
	if (TRACE_CALLS) printf("Helpers - sreverse()\n");
  size_t begin = 0;
  size_t end = buflen-1;

  while (begin < end)
    swap(s[begin++], s[end--]);
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: CreateNewTCPListenSocket
//
// Description:
//	  Creates a new TCP socket to listen for connections.
//
// Inputs:
//    None
//
// Outputs:
//    None
//
// Return Value:
//    sockfd = socket number for success, -1 for failure
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

int32_t CreateNewTCPListenSocket( uint32_t port, uint32_t nReuseAddr, char *szPrimaryIP, char *szSecondaryIP )
{
	if (TRACE_CALLS) printf("Helpers - CreateNewTCPListenSocket()\n");
	struct sockaddr_in server;
	int32_t sockfd = -1;
	
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sockfd < 0 ) 
	{
		printf( "CreateNewTCPListenSocket: FATAL: Could not create socket\n" );
		sockfd = -1;
	}
	else
	{
		if ( nReuseAddr )
		{
		    u_int yes = 1;  
		    if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0 ) 
		    {
		    	printf("setsockopt SO_REUSEADDR failed\n");
		    	sockfd = -1;
		    }
		}
	    
		if ( sockfd != -1 )
	    {
			bzero( &server, sizeof(server) );
			server.sin_family      = AF_INET;
			if ( szPrimaryIP == NULL )
			{
				server.sin_addr.s_addr = htonl( INADDR_ANY );
			}
			else
			{
				inet_pton(AF_INET, szPrimaryIP, &server.sin_addr.s_addr);  
			}
			server.sin_port        = htons( port );
		
		   	int ret = bind( sockfd, (struct sockaddr *)&server, sizeof(server) );
		    if ( ret < 0 ) 
		    { 
		    	if ( errno == EADDRNOTAVAIL && szSecondaryIP != NULL )
		    	{
					bzero( &server, sizeof(server) );
					server.sin_family      = AF_INET;
					inet_pton(AF_INET, szSecondaryIP, &server.sin_addr.s_addr);  
					server.sin_port        = htons( port );
		   			int ret = bind( sockfd, (struct sockaddr *)&server, sizeof(server) );
		   			if ( ret < 0 )
		   			{
						printf( "CreateNewTCPListenSocket: FATAL: Unable to bind secondary socket\n" );
		   				sockfd = -1;
		   			}
		    	}
		    	else
		    	{
					printf( "CreateNewTCPListenSocket: FATAL: Unable to bind primary socket\n" );
					sockfd = -1;
		    	}
			}
			
			if ( sockfd != -1 )
			{	
				listen( sockfd, LISTENQ );
			}
	    }
	}
	
	return( sockfd );
}

//////////////////////////////////////////////////////////////////////
//++
//
// Function: WriteToSysLog
//
// Description:
//	  Writes errors to the syslog
//
// Inputs:
//    pMsg - Error message to write
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturnSUCCESS - Successful completion
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn WriteToSysLog( int32_t nSeverity, char *pMessage, char *pComponent, char *pRemedy )
{
	if (TRACE_CALLS) printf("Helpers - WriteToSysLog\n");
	
	char  *szErrorDescriptor[] = { 
		"EMERGENCY", 			// LOG_EMERG	/* system is unusable */
		"ALERT",	 			// LOG_ALERT	/* action must be taken immediately */
		"CRITICAL",				// LOG_CRIT		/* critical conditions */
		"ERROR", 				// LOG_ERR		/* error conditions */
		"WARNING",	 			// LOG_WARNING	/* warning conditions */
		"NOTICE",	 			// LOG_NOTICE	/* normal but significant condition */
		"INFO",					// LOG_INFO		/* informational */
		"DEBUG" };				// LOG_DEBUG	/* debug-level messages */
		
	ESYSReturn eRtn = ESYSReturnSUCCESS;
	int nErrBuffLen = (MAX_ERROR_LEN*2) + 25;
	char szErrBuffer[nErrBuffLen];
	memset( szErrBuffer, 0, nErrBuffLen );
	if ( pRemedy != NULL )
	{
		sprintf( szErrBuffer, "%s: %s REMEDY: %s", 
				 szErrorDescriptor[nSeverity], pMessage, pRemedy );
	}
	else
	{
		sprintf( szErrBuffer, "%s: %s", 
				 szErrorDescriptor[nSeverity], pMessage );
	}
	
	openlog( pComponent, LOG_PERROR, LOG_USER );
	syslog( nSeverity, szErrBuffer );
	closelog();

	return( eRtn );
}

//////////////////////////////////////////////////////////////////////
//++
//
// Function: DropTrailingSlash
//
// Description:
//	  Removes the trailing slash (if any) from a string
//
// Inputs:
//    pInString - input string with or without trailing slash
//
// Outputs:
//    pOutString - output string with no trailing slash
//
// Return Value:
//    None
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

void DropTrailingSlash( const char *pInString, char *pOutString )
{
	if (TRACE_CALLS) printf("Helpers - DropTrailingSlash\n");
	
	if( !strcmp(&(pInString[strlen(pInString)-1]), "/") )
	{
		strncpy(pOutString, pInString, strlen(pInString)-1);
	}
	else
	{
		strcpy( pOutString, pInString );
	}
}

//////////////////////////////////////////////////////////////////////
//++
//
// Function: DoesFileExist
//
// Description:
//	  Determines if a file exists without attempting to open it
//    This avoids any file permissions issues (but not folder permission issues)
//
// Inputs:
//    szFileName - path to file
//
// Outputs:
//
// Return Value:
//    true = file exists
//    false = file does not exist
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

bool DoesFileExist( const char *szFileName ) 
{
	struct stat stFileInfo;
	bool bReturn;
	int intStat;
	
	// Attempt to get the file attributes
	intStat = stat( szFileName, &stFileInfo );
	if( intStat == 0 ) 
	{
	  	// We were able to get the file attributes
	  	// so the file obviously exists.
	  	bReturn = true;
	} 
	else 
	{
	    // We were not able to get the file attributes.
	    bReturn = false;
  	}
  
  	return(bReturn);
}

//////////////////////////////////////////////////////////////////////
//++
//
// Function: GetLibraryVersion
//
// Description:
//	  Returns the repository revision number that was used to build the library
//
// Inputs:
//
// Outputs:
//
// Return Value:
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

char *GetLibraryVersion( void )
{
	return 0;
	//SHOULD BE THIS:
	//return SVN_LIB_REV;
}

//////////////////////////////////////////////////////////////////////////////
//++
//
// SubscribeForSystemMessages
//
// Description:
//	  Subscribes for system messages
//
// Inputs:
//    msgType - Message Type 
//	  nSubscriptionFlag - subscribe or unsubscribe
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturnSUCCESS - Successful completion
//    ESYSReturnSOCKET_ERROR - Socket closed prior to completion
// 
// Exceptions:
//    None
//
//
//--
//////////////////////////////////////////////////////////////////////////////

ESYSReturn SubscribeForSystemMessages( ESYSProcess processID, int32_t sockfd,
									   IPCMsgType msgType, int32_t nSubscriptionFlag )
{
	if (TRACE_CALLS) printf("%s::SubscribeForSystemMessages\n", ESYSProcessText[processID].c_str());
	ESYSReturn eRtn = ESYSReturnSUCCESS;
	char buf[MAX_PACKET_LEN];
	
	// Create Data Feed Subscribe 
	CMessage *pMsg = new CMessage( IPCMsgTypeDATA_FEED_SUBSCRIBE );
	pMsg->AddTLV( TLVTagLIVE_DATA_REQUEST, nSubscriptionFlag );
	pMsg->AddTLV( TLVTagUNIQUE_ID, (MSG_SUBSCRIPTION_PREFIX+msgType) );
	
	int32_t nXmitSize = pMsg->PackToSend( buf, false );
	int32_t nBytes = writen( sockfd, buf, nXmitSize );

	// Release allocated memory that is no longer needed
	delete pMsg;
	
	// check for errors
	if ( nBytes < 0 )
	{
		eRtn = ESYSReturnSOCKET_ERROR;
	}
	
	return( eRtn );
}

//////////////////////////////////////////////////////////////////////////////
//++
//
// SendProcessInfo
//
// Description:
//	  Send info about the process to the DDS
//
// Inputs:
//    None
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturnSUCCESS - Successful completion
//    ESYSReturnSOCKET_ERROR - Socket closed prior to completion
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////////////

ESYSReturn SendProcessInfo( ESYSProcess processID, int32_t sockfd )
{
	if (TRACE_CALLS) printf("%s::SendProcessInfo\n", 
							ESYSProcessText[processID].c_str());
	
	ESYSReturn eRtn = ESYSReturnSUCCESS;
	
	CMessage *pMsg = new CMessage( IPCMsgTypePROCESS_INFO );
	pMsg->AddTLV( TLVTagPROCESS_ID, processID );
	//pMsg->AddTLV( TLVTagSVN_LIB_REV, strlen(GetLibraryVersion()), GetLibraryVersion() );
	char buf[MAX_PACKET_LEN];
	memset( buf, 0, MAX_PACKET_LEN );
	int32_t nXmitSize = pMsg->PackToSend( buf, false );
	
	int nBytes = writen( sockfd, buf, nXmitSize );
	
	if ( nBytes == 0 )
	{
		eRtn = ESYSReturnSOCKET_ERROR;
	}

	delete pMsg;
	
	return( eRtn );
}
	
//////////////////////////////////////////////////////////////////////////////
//++
//
// SubscribeForDSNs
//
// Description:
//	  Subscribes for Data Source Notification messages
//
// Inputs:
//    Nones
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturnSUCCESS - Successful completion
//    ESYSReturnSOCKET_ERROR - Socket closed prior to completion
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////////////

ESYSReturn SubscribeForDSNs( ESYSProcess processID, int32_t sockfd, int32_t nDSNFilter )
{
	if (TRACE_CALLS) printf("%s::SubscribeForDSNs\n", 
							ESYSProcessText[processID].c_str());
	
	ESYSReturn 	eRtn = ESYSReturnSUCCESS;
	char 		buf[MAX_PACKET_LEN];
	
	// Subscribe for Config Updates
	CMessage *pMsg = new CMessage( IPCMsgTypeDATA_FEED_SUBSCRIBE );
	pMsg->AddTLV( TLVTagLIVE_DATA_REQUEST, SUBSCRIBE );
	pMsg->AddTLV( TLVTagUNIQUE_ID, 
				(MSG_SUBSCRIPTION_PREFIX+IPCMsgTypeDATA_SOURCE_NOTIFICATION) );
	pMsg->AddTLV( TLVTagDSN_FILTER, nDSNFilter );
	
	int32_t nXmitSize = pMsg->PackToSend( buf, false );
	int32_t nBytes = writen( sockfd, buf, nXmitSize );

	// Release allocated memory that is no longer needed
	delete pMsg;
	
	// check for errors
	if ( nBytes < 0 )
	{
		return( ESYSReturnSOCKET_ERROR );
	}
	
	return( eRtn );
}

/* Signal message to real-time side to apply change. RT side
   will take data and attempt to acknowledge. If it is ok,
   the data will be written out to shm (the running config). */
int cam_signal(struct cam_signal *signal, struct cam_signal_resp *resp) {
	int tries = 10;
	int fd; 
	int resp_fd;
	memcpy(&resp->signal,signal,sizeof(*signal));
	while (tries) {
		tries--;
		fd = open(CAM_SIGNAL_DEVICE,O_WRONLY);
		if (fd < 0) {
			usleep(1000*100);
			continue;
		}
		resp_fd = open(CAM_SIGNAL_RESP_DEVICE,O_RDONLY);
		if (resp_fd == -1) {
			close(fd);
			resp->result = -1;
			return -1;
		}
		write(fd,signal,sizeof(*signal));
		read(resp_fd,resp,sizeof(*resp));
		close(fd);
		close(resp_fd);
		break;
	}
	if (tries > 0) {
		return 0;
	} else {
		resp->result = -1;
		return -1;
	}
}

