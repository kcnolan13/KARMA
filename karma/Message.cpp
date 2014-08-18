//////////////////////////////////////////////////////////////////////////////
//++
//
// Message.h
//
// Description:
//	  Contains implementation of CMessage class, members and methods.  This class
//	  provides the base class for all interprocess messages.
//
// Notes:
//    This class is intended to be used by several processes within AAMPS.  
//
// Copyright 2008 - Remote Sensing Solutions Inc.
//					Barnstable, MA
//
// Creation Author: Dan Robinson
// Creation Date:   02/05/2009
//
//--
//////////////////////////////////////////////////////////////////////////////

#include "Message.h"
// ************** IMPORTANT *******************
// This list MUST correspond to the TLV tags
//  defined in Message.h.
// ********************************************

int g_nTLVDataType[TLVTagMAX] = {
	ESYSDataTypeOTHER,		// TLVTagUNDEFINED
	ESYSDataTypeINT32,		// TLVTagUNIQUE_ID,						
	ESYSDataTypeSTRING,		// TLVTagUNIQUE_NAME,						
	ESYSDataTypeDOUBLE,		// TLVTagMINIMUM_VALUE,						
	ESYSDataTypeDOUBLE,		// TLVTagMAXIMUM_VALUE,						
	ESYSDataTypeDOUBLE,		// TLVTagSAMPLE_RATE,						
	ESYSDataTypeINT32,		// TLVTagDATA_TYPE,							
	ESYSDataTypeSTRING,		// TLVTagUNITS,								
	ESYSDataTypeSTRING,		// TLVTagDESCRIPTION,						
	ESYSDataTypeINT32,		// TLVTagHISTORICAL_DATA_REQUEST,			
	ESYSDataTypeINT32,		// TLVTagLIVE_DATA_REQUEST,					
	ESYSDataTypeINT32,		// TLVTagDEVICE_TYPE,						
	ESYSDataTypeINT32,		// TLVTagSTRING_DATA_LENGTH,				
	ESYSDataTypeSTRING,		// TLVTagFILE_NAME,							
 	ESYSDataTypeINT32,		// TLVTagSTART_TIME,						
	ESYSDataTypeINT32,		// TLVTagSEQUENCE_NUMBER,			
	ESYSDataTypeSTRING,		// TLVTagPARAM_NAME,				 
	ESYSDataTypeINT32,		// TLVTagUNLIMITED_INDEX,			
	ESYSDataTypeINT32,		// TLVTagBWN_SECONDS,				
	ESYSDataTypeINT32,		// TLVTagBWN_NANOSECONDS,			
	ESYSDataTypeOTHER,		// TLVTagDATA_VALUES,				
	ESYSDataTypeINT32,		// TLVTagDEVICE_INSTANCE,			
	ESYSDataTypeINT32,		// TLVTagDESCRIPTOR_COUNT,			
	ESYSDataTypeINT32,		// TLVTagWORD,						
	ESYSDataTypeDOUBLE,		// TLVTagREFRESH_RATE,				
	ESYSDataTypeDOUBLE,		// TLVTagRESOLUTION,				
	ESYSDataTypeINT32,		// TLVTagOCTAL_LABEL,				
	ESYSDataTypeINT32,		// TLVTagREMOTE_TERMINAL,			
	ESYSDataTypeINT32,		// TLVTagSUB_ADDRESS,				
	ESYSDataTypeINT32,		// TLVTagWORD_COUNT,				
	ESYSDataTypeDOUBLE,		// TLVTagC1,						
	ESYSDataTypeDOUBLE,		// TLVTagC2,						
	ESYSDataTypeDOUBLE,		// TLVTagC3,						
	ESYSDataTypeDOUBLE,		// TLVTagC4,						
	ESYSDataTypeSTRING,		// TLVTagAIRCRAFT,					
	ESYSDataTypeDOUBLE,		// TLVTagCONVERSION,				
	ESYSDataTypeSTRING,		// TLVTagROUTINE,
	ESYSDataTypeINT32,		// TLVTagRTDP_INPUT_ID,
	ESYSDataTypeINT32,		// TLVTagRTDP_INPUT_COUNT,
	ESYSDataTypeSTRING,		// TLVTagRTDP_INPUT_NAME,			
	ESYSDataTypeINT32,		// TLVTagINSTANCE,					
	ESYSDataTypeSTRING,		// TLVTagQUANTITY,					
	ESYSDataTypeSTRING,		// TLVTagAPPLICATION,				
	ESYSDataTypeSTRING,		// TLVTagLOCATION,					
	ESYSDataTypeSTRING,		// TLVTagSENSOR,					
	ESYSDataTypeSTRING,		// TLVTagAMPLIFIER,				
	ESYSDataTypeSTRING,		// TLVTagDATE,						
	ESYSDataTypeSTRING,		// TLVTagMANUFACTURER,				
	ESYSDataTypeSTRING,		// TLVTagMODEL,					
	ESYSDataTypeSTRING,		// TLVTagSERIAL					
	ESYSDataTypeSTRING,		// TLVTagNETCDF_NAME				
	ESYSDataTypeDOUBLE,		// TLVTagPLOT_MIN,				
	ESYSDataTypeDOUBLE,		// TLVTagPLOT_MAX,				
	ESYSDataTypeDOUBLE,		// TLVTagRANGE,				
	ESYSDataTypeDOUBLE,		// TLVTagIMAGE_FILTER,				
	ESYSDataTypeINT32,		// TLVTagCONFIG_LEVEL,				
	ESYSDataTypeINT32,		// TLVTagERROR_SEVERITY,			
	ESYSDataTypeSTRING,		// TLVTagERROR_MESSAGE,			
	ESYSDataTypeSTRING,		// TLVTagERROR_COMPONENT,			
	ESYSDataTypeSTRING,		// TLVTagERROR_REMEDY,		
	ESYSDataTypeINT32,		// TLVTagPLOT_COMPONENT	
	ESYSDataTypeSTRING,		// TLVTagFLID
	ESYSDataTypeSTRING,		// TLVTagFLIGHT_NUMBER		
	ESYSDataTypeSTRING,		// TLVTagFILE_OF_DAY	
	ESYSDataTypeINT32,		// TLVTagPROCESS_ID	
	ESYSDataTypeSTRING,		// TLVTagSVN_LIB_REV
	ESYSDataTypeINT32,		// TLVTagDSN_FILTER
	ESYSDataTypeINT32,		// TLVTagDATA_WIDTH,				
	ESYSDataTypeSTRING,		// TLVTagDATA_FORMAT,	
	ESYSDataTypeINT32,		// TLVTagVCO_FREQUENCY			
	ESYSDataTypeINT32,		// TLVTagTRANSFER_TYPE,			
	ESYSDataTypeSTRING,		// TLVTagSTATION_ID,			
	ESYSDataTypeSTRING,		// TLVTagMAXFL_WIND,			
	ESYSDataTypeSTRING,		// TLVTagUSER_NAME,				
	ESYSDataTypeSTRING,		// TLVTagPASSWORD,
	ESYSDataTypeSTRING,		// TLVTagMISSION_ID,					
	ESYSDataTypeSTRING,		// TLVTagSTORM_ID									
	ESYSDataTypeSTRING,		// TLVTagFLID_NUMBER
	ESYSDataTypeSTRING		// TLVTagWMO_AREA
	
};
						

//////////////////////////////////////////////////////////////////////
//++
//
// Method: CMessage
//
// Description:
//	  Class constructor.  Sets private member default values, allocates memory.
//
// Inputs:
//    Message Type
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

CMessage::CMessage( int32_t nMsgType )
{
	if (TRACE_CALLS) printf("CMessage::CMessage\n");
	
	InitClassMembers( );
	
	m_nMessageType = nMsgType;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: CMessage
//
// Description:
//	  Class constructor.  Sets private member variables to values passed.
//
// Inputs:
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

CMessage::CMessage( char *szMsgBuffer, bool bNetworkByteOrder )
{
	if (TRACE_CALLS) printf("CMessage::CMessage(char *)\n");
	
	InitClassMembers( );
	
	Unpack( szMsgBuffer, bNetworkByteOrder );
}


//////////////////////////////////////////////////////////////////////
//++
//
// Method: ~CMessage
//
// Description:
//	  Class destructor.  De-allocates memory.
//
// Inputs:
//    None
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

CMessage::~CMessage()
{
	if (TRACE_CALLS) printf("CMessage::~CMessage\n");
	
	// Remove TLVs from list
	if ( m_pTLVList != NULL )
	{
		while ( !( m_pTLVList->empty() ) )
		{
			CTLV *pTemp = m_pTLVList->PopHead( );
			delete pTemp;
		}
		
		delete m_pTLVList;
	} 
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: InitClassMembers
//
// Description:
//	  Initializes all class members to default values.
//
// Inputs:
//    None
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturn
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn CMessage::InitClassMembers( )
{
	if (TRACE_CALLS) printf("CMessage::InitClassMembers\n");
	
	m_nMessageType = 0;
	m_nTLVCount = 0;
	m_nMsgLength = sizeof(m_nMessageType) + sizeof(m_nTLVCount);
	
	m_pTLVList = new CLinkedList<CTLV *>( );

	return ( ESYSReturnSUCCESS );
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: Unpack
//
// Description:
//	  Unpacks a raw message buffer and populates object variables.
//
// Inputs:
//    szRawMsg - Raw message buffer
//
// Outputs:
//    None
//
// Return Value:
//    Message Length
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

int32_t CMessage::Unpack( char *szRawMsg, bool bNetworkByteOrder )
{
	if (TRACE_CALLS) printf("CMessage::Unpack\n");
	int32_t nTemp;
	int16_t nsTemp;
	
	m_nMessageType = ((IPCMessage *)szRawMsg)->nMessageType;
	int32_t nTLVCount = ((IPCMessage *)szRawMsg)->nTLVCount;
	if ( bNetworkByteOrder )
	{
		m_nMessageType = ntohl( m_nMessageType );
		nTLVCount = ntohl( nTLVCount );
	}
	m_nMsgLength = sizeof( m_nMessageType ) + sizeof( nTLVCount );
	if ( nTLVCount > 0 )
	{
		char *index = &((IPCMessage *)szRawMsg)->cTLVData;
		for (int i = 0; i < nTLVCount; i++ )
		{
			int32_t tag = ((TLV *)index)->lTag;
			int32_t len = ((TLV *)index)->lLength;
			char *pValue = &((TLV *)index)->cValue;

			if ( bNetworkByteOrder )
			{
				tag = ntohl( tag );
				len = ntohl( len );
				
				switch( g_nTLVDataType[tag & 0x0000FFFF] )
				{
					case ESYSDataTypeINT32:
					case ESYSDataTypeFLOAT:
						// For 32-bit integers
						memcpy( &nTemp, pValue, sizeof(nTemp) );
						nTemp = htonl( nTemp );
						memcpy( pValue, &nTemp, sizeof(nTemp) );
						break;
	
					case ESYSDataTypeINT16:
						nsTemp = 0;
						memcpy( &nsTemp, pValue, sizeof(nsTemp) );
						nTemp = htons( nsTemp );
						memcpy( pValue, &nsTemp, sizeof(nsTemp) );
						break;
	
					case ESYSDataTypeSTRING:
					case ESYSDataTypeBYTE:
						// Nothing to do 
						break;
						
					case ESYSDataTypeDOUBLE:					
					    SetNetworkByteOrder( pValue, len, ESYSDataTypeDOUBLE );
						break;
					
					case ESYSDataTypeOTHER:		
						if ( tag == TLVTagDATA_VALUES )
						{		
					    	SetNetworkByteOrder( pValue, len, ESYSDataTypeINT32 );
						}
						break;
						
					default:
						printf("Received unrecognized TLV Tag: %d\n", tag);
						break;
						
				} // end switch
			}
			AddTLV( tag, len, pValue );
			index += sizeof(int32_t) + sizeof(int32_t) + len;
		}
		
	}

	return m_nMsgLength;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: PackToSend
//
// Description:
//	  Packs a message object into a buffer suitable
//    for TCP transport (includes length prefix).
//
// Inputs:
//    None
//
// Outputs:
//    None
//
// Return Value:
//    number of bytes packed in the buffer
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

int32_t CMessage::PackToSend( char *buffer, bool bNetworkByteOrder, int32_t lDataType )
{
	if (TRACE_CALLS) printf("CMessage::PackToSend\n");
	
	// Move buffer index to account for message length prefix
	char *index = buffer;
	index += sizeof( m_nMsgLength );
	
	// Pack the rest of the message fields into the buffer
	Pack( index, bNetworkByteOrder, lDataType );
	
	// Copy the final message length value into the buffer prefix
	int32_t length;
	if ( bNetworkByteOrder )
	{
		length = htonl( m_nMsgLength );
	}
	else
	{
		length = m_nMsgLength;
	}
	
	memcpy(buffer, &length, sizeof(length));
	
	return m_nMsgLength + sizeof(m_nMsgLength);
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: Pack
//
// Description:
//	  Packs a message object into a buffer.
//
// Inputs:
//    None
//
// Outputs:
//    None
//
// Return Value:
//    number of bytes packed in the buffer
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

int32_t CMessage::Pack( char *buffer, bool bNetworkByteOrder, int32_t lDataType )
{
	if (TRACE_CALLS) printf("CMessage::Pack\n");
	
	char *index = buffer;
	if ( bNetworkByteOrder )
	{
		((IPCMessage *)index)->nMessageType = htonl(m_nMessageType);
		((IPCMessage *)index)->nTLVCount = htonl(m_nTLVCount);
	}
	else
	{
		((IPCMessage *)index)->nMessageType = m_nMessageType;
		((IPCMessage *)index)->nTLVCount = m_nTLVCount;
	}
	index = index + sizeof(int32_t) + sizeof(int32_t);
	// Make sure there's something in the list
	if ( m_nTLVCount > 0 )
	{
		m_pTLVList->PointToHead( );
		CTLV *pTLV = NULL;
		do
		{
			pTLV = m_pTLVList->GetCurrentData( );
			if ( bNetworkByteOrder )
			{
				((TLV *)index)->lTag = htonl( pTLV->GetTag( ) );
				((TLV *)index)->lLength = htonl( pTLV->GetLength( ) );
			}
			else
			{
				((TLV *)index)->lTag = pTLV->GetTag( );
				((TLV *)index)->lLength = pTLV->GetLength( );
			}
			index = index + sizeof(int32_t) + sizeof(int32_t);
			
			if ( bNetworkByteOrder )
			{
				// Strip off first two bytes, which may contain indexing values
				int32_t nTag = pTLV->GetTag( ) & 0x0000FFFF;
				switch( g_nTLVDataType[nTag] )
				{
					case ESYSDataTypeINT32:		
					case ESYSDataTypeFLOAT:						
						// For 32-bit values
						int32_t nTemp;
						memcpy( &nTemp, pTLV->GetValue(), sizeof(nTemp) );
						int32_t nTemp2;
						nTemp2 = htonl( nTemp );
						memcpy( index, &nTemp2, sizeof(nTemp2) );
						break;
	
					case ESYSDataTypeINT16:		
						// For 16-bit integers
						int16_t nsTemp;
						memcpy( &nsTemp, pTLV->GetValue(), sizeof(nsTemp) );
						int16_t nsTemp2;
						nsTemp2 = htonl( nsTemp );
						memcpy( index, &nsTemp2, sizeof(nsTemp2) );
						break;
	
					// No byte swapping for ASCII and Byte
					case ESYSDataTypeSTRING:								
					case ESYSDataTypeBYTE:      
						memcpy( index, pTLV->GetValue(), pTLV->GetLength() );
						break;
					
					case ESYSDataTypeDOUBLE:					
						memcpy( index, pTLV->GetValue(), pTLV->GetLength() );
					    SetNetworkByteOrder( index, pTLV->GetLength(), ESYSDataTypeDOUBLE );
						break;
					
					case ESYSDataTypeOTHER:	
						memcpy( index, pTLV->GetValue(), pTLV->GetLength() );
					    SetNetworkByteOrder( index, pTLV->GetLength(), lDataType );
						break;
						
					default:
						printf("Received unrecognized TLV Tag: %d\n", pTLV->GetTag( ));
						break;
						
				} // end switch
				
			} // end if network byte order
			else
			{
				memcpy( index, pTLV->GetValue(), pTLV->GetLength() );
			}

			index += pTLV->GetLength( );
			
		} while ( m_pTLVList->PointToNext( ) );
		
	} // end if not empty	
	
	return m_nMsgLength;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: AddTLV
//
// Description:
//	  Add a TLV to the message object.
//
// Inputs:
//    nTag - TLV Tag
//    nValue - integer value
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturn
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn CMessage::AddTLV( int nTag, int nValue )
{
	if (TRACE_CALLS) printf("CMessage::AddTLV(int, int)\n");
	
	ESYSReturn rtn = ESYSReturnSUCCESS;
	// Check to make sure we have room for this TLV
	int len = m_nMsgLength + sizeof(int32_t) + sizeof(int32_t) + sizeof(int32_t);
	if ( len <= MAX_PACKET_LEN )
	{
		CTLV *pTLV = new CTLV( nTag, sizeof(int32_t), (char *)&nValue );
		m_pTLVList->Append( pTLV );
		++m_nTLVCount;
		m_nMsgLength = len;
	}
	else
	{
		rtn = ESYSReturnMSG_TOO_LONG;
	}
	return rtn;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: AddTLV
//
// Description:
//	  Add a TLV to the message object.
//
// Inputs:
//    nTag - TLV Tag
//    fValue - floating point value
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturn
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn CMessage::AddTLV( int nTag, float fValue )
{
	if (TRACE_CALLS) printf("CMessage::AddTLV(int, float)\n");
	
	ESYSReturn rtn = ESYSReturnSUCCESS;
	
	// Check to make sure we have room for this TLV
	int len = m_nMsgLength + sizeof(int32_t) + sizeof(int32_t) + sizeof(float);
	if ( len <= MAX_PACKET_LEN )
	{
		CTLV *pTLV = new CTLV( nTag, sizeof(float), (char *)&fValue );
		m_pTLVList->Append( pTLV );
		++m_nTLVCount;
		m_nMsgLength = len;
	}
	else
	{
		rtn = ESYSReturnMSG_TOO_LONG;
	}
	
	return rtn;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: AddTLV
//
// Description:
//	  Add a TLV to the message object.
//
// Inputs:
//    nTag - TLV Tag
//    fValue - double precision value
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturn
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn CMessage::AddTLV( int nTag, double fValue )
{
	if (TRACE_CALLS) printf("CMessage::AddTLV(int, double)\n");
	
	ESYSReturn rtn = ESYSReturnSUCCESS;
	
	// Check to make sure we have room for this TLV
	int len = m_nMsgLength + sizeof(int32_t) + sizeof(int32_t) + sizeof(double);
	if ( len <= MAX_PACKET_LEN )
	{
		CTLV *pTLV = new CTLV( nTag, sizeof(double), (char *)&fValue );
		m_pTLVList->Append( pTLV );
		++m_nTLVCount;
		m_nMsgLength = len;
	}
	else
	{
		rtn = ESYSReturnMSG_TOO_LONG;
	}
	
	return rtn;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: AddTLV
//
// Description:
//	  Add a TLV to the message object.
//
// Inputs:
//    nTag - TLV Tag
//    nLen - Length of value
//    pValue - pointer to character buffer containing TLV value
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturn
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn CMessage::AddTLV( int nTag, int nLen, const char *pValue )
{
	if (TRACE_CALLS) printf("CMessage::AddTLV(int, int, char*)\n");
	
	ESYSReturn rtn = ESYSReturnSUCCESS;
	
	// Check to make sure we have room for this TLV
	int len = m_nMsgLength + sizeof(int32_t) + sizeof(int32_t) + nLen;
	if ( len <= MAX_PACKET_LEN )
	{
		CTLV *pTLV = new CTLV( nTag, nLen, pValue );
		m_pTLVList->Append( pTLV );
		++m_nTLVCount;
		m_nMsgLength = len;
	}
	else
	{
		rtn = ESYSReturnMSG_TOO_LONG;
	}
	
	
	return rtn;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: AddTLV
//
// Description:
//	  Add a TLV to the message object.
//
// Inputs:
//    CTLV *pTLV - pointer to a CTLV object
//
// Outputs:
//    None
//
// Return Value:
//    ESYSReturn
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn CMessage::AddTLV( CTLV *pTLV )
{
	if (TRACE_CALLS) printf("CMessage::AddTLV\n");
	
	ESYSReturn rtn = ESYSReturnSUCCESS;
	
	// Check to make sure we have room for this TLV
	int len = m_nMsgLength + sizeof(int32_t) + sizeof(int32_t) + pTLV->GetLength( );
	if ( len <= MAX_PACKET_LEN )
	{
		m_pTLVList->Append( pTLV );
		++m_nTLVCount;
		m_nMsgLength = len;
	}
	else
	{
		rtn = ESYSReturnMSG_TOO_LONG;
	}
	
	
	return rtn;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: GetTLVByTag
//
// Description:
//	  Retrieves a pointer to the data portion of a 
//    CTLV object based on the tag provided.
//
// Inputs:
//    lTag - Tag number to search for
//
// Outputs:
//    None
//
// Return Value:
//    pointer to TLV data, NULL if not found
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

CTLV * CMessage::GetTLVByTag( int nTag )
{
	if (TRACE_CALLS) printf("CMessage::GetTLVByTag\n");
	
	CTLV *pTLV = NULL;
	
	// Make sure there's something in the list
	if ( m_nTLVCount > 0 )
	{
		m_pTLVList->PointToHead( );
		do
		{
			pTLV = m_pTLVList->GetCurrentData( );
			
		} while ( m_pTLVList->PointToNext( ) && pTLV->GetTag() != nTag );
		
	} // end if not empty	
	
	if ( pTLV->GetTag() != nTag )
	{
		pTLV = NULL;
	}
	
	return pTLV;
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: SetNetworkByteOrder
//
// Description:
//	  Converts a data buffer into network byte order.  Determines the
//    width of the data and uses the appropriate conversion method.
//
// Inputs:
//    lTag - Tag number to search for
//
// Outputs:
//    None
//
// Return Value:
//    pointer to TLV, NULL if not found
// 
// Exceptions:
//    None
//
//--
//////////////////////////////////////////////////////////////////////

ESYSReturn CMessage::SetNetworkByteOrder( char *pBuffer, int32_t buflen, int32_t lDataType )
{
	if (TRACE_CALLS) printf("CMessage::SetNetworkByteOrder\n");

	ESYSReturn rtn = ESYSReturnSUCCESS;

	// strings and bytes don't need to be converted
	if ( lDataType != ESYSDataTypeSTRING &&
	     lDataType != ESYSDataTypeBYTE ) 
	{
		int32_t lDataWidth = 0;
		switch ( lDataType )
		{
			case ESYSDataTypeINT32:
				lDataWidth = sizeof(int32_t);
				break;
			case ESYSDataTypeFLOAT:
				lDataWidth = sizeof(float);
				break;
			case ESYSDataTypeINT16:
				lDataWidth = sizeof(int16_t);
				break;
			case ESYSDataTypeDOUBLE:
				lDataWidth = sizeof(double);
				break;
			default:
				lDataWidth = -1;
				rtn = ESYSReturnINVALID_DATA_TYPE;
				break;
		} // end switch
		
		// Loop through the buffer
		char *index = pBuffer;
		int iterations = buflen / lDataWidth;
		for ( int i = 0; i < iterations; i++ )
		{
			index = pBuffer + (lDataWidth * i);
			
			// Reverse the order of the bytes
			sreverse( index, lDataWidth );

		} // end for i
		
	} // end if not string or byte
	
	return rtn;
}
