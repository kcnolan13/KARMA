//////////////////////////////////////////////////////////////////////////////
//++
//
// Message.h
//
// Description:
//	  Contains definition of CMessage class, members and methods.  This class
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

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdio.h>
#include <netinet/in.h>

#include "SystemConstants.h"
#include "LinkedList.h"
#include "TLV.h"
#include "Helpers.h"

// IPC Message TLV Tag Definitions
// ************** IMPORTANT *******************
// This list MUST correspond to the TLV data
// types defined in Message.cpp.
// ********************************************
enum TLVTag
{
	TLVTagUNDEFINED	= 0,
	TLVTagUNIQUE_ID,				/* 0x01 */		
	TLVTagUNIQUE_NAME,				/* 0x02 */		
	TLVTagMINIMUM_VALUE,				/* 0x03 */			
	TLVTagMAXIMUM_VALUE,				/* 0x04 */			
	TLVTagSAMPLE_RATE,				/* 0x05 */		
	TLVTagDATA_TYPE,				/* 0x06 */			
	TLVTagUNITS,					/* 0x07 */			
	TLVTagDESCRIPTION,				/* 0x08 */		
	TLVTagHISTORICAL_DATA_REQUEST,			/* 0x09 */		
	TLVTagLIVE_DATA_REQUEST,			/* 0x0a */			
	TLVTagDEVICE_TYPE,				/* 0x0b */		
	TLVTagSTRING_DATA_LENGTH,			/* 0x0c */		
	TLVTagFILE_NAME,				/* 0x0d */			
 	TLVTagSTART_TIME,				/* 0x0e */		
	TLVTagSEQUENCE_NUMBER,				/* 0x0f */
	TLVTagPARAM_NAME,				/* 0x10 */ 
	TLVTagUNLIMITED_INDEX,				/* 0x11 */
	TLVTagBWN_SECONDS,				/* 0x12 */
	TLVTagBWN_NANOSECONDS,				/* 0x13 */
	TLVTagDATA_VALUES,				/* 0x14 */
	TLVTagDEVICE_INSTANCE,				/* 0x15 */
	TLVTagDESCRIPTOR_COUNT,				/* 0x16 */
	TLVTagWORD,					/* 0x17 */
	TLVTagREFRESH_RATE,				/* 0x18 */
	TLVTagRESOLUTION,				/* 0x19 */
	TLVTagOCTAL_LABEL,				/* 0x1a */
	TLVTagREMOTE_TERMINAL,				/* 0x1b */
	TLVTagSUB_ADDRESS,				/* 0x1c */
	TLVTagWORD_COUNT,				/* 0x1d */
	TLVTagC1,					/* 0x1e */
	TLVTagC2,					/* 0x1f */
	TLVTagC3,					/* 0x20 */
	TLVTagC4,					/* 0x21 */
	TLVTagAIRCRAFT,					/* 0x22 */
	TLVTagCONVERSION,				/* 0x23 */
	TLVTagROUTINE,					/* 0x24 */
	TLVTagRTDP_INPUT_ID,            		/* 0x25 */
	TLVTagRTDP_INPUT_COUNT,         		/* 0x26 */
	TLVTagRTDP_INPUT_NAME,				/* 0x27 */
	TLVTagINSTANCE,					/* 0x28 */
	TLVTagQUANTITY,					/* 0x29 */
	TLVTagAPPLICATION,				/* 0x2a */
	TLVTagLOCATION,					/* 0x2b */
	TLVTagSENSOR,					/* 0x2c */
	TLVTagAMPLIFIER,				/* 0x2d */
	TLVTagDATE,					/* 0x2e */
	TLVTagMANUFACTURER,				/* 0x2f */
	TLVTagMODEL,					/* 0x30 */
	TLVTagSERIAL,					/* 0x31 */
	TLVTagNETCDF_NAME,				/* 0x32 */
	TLVTagPLOT_MIN,					/* 0x33 */
	TLVTagPLOT_MAX,					/* 0x34 */
	TLVTagRANGE,					/* 0x35 */
	TLVTagIMAGE_FILTER,				/* 0x36 */
	TLVTagCONFIG_LEVEL,				/* 0x37 */
	TLVTagERROR_SEVERITY,				/* 0x38 */
	TLVTagMESSAGE,					/* 0x39 */
	TLVTagERROR_COMPONENT,				/* 0x3a */
	TLVTagERROR_REMEDY,				/* 0x3b */
	TLVTagPLOT_COMPONENT,				/* 0x3c */
	TLVTagFLID,					/* 0x3d */
	TLVTagFLIGHT_NUMBER,				/* 0x3e */
	TLVTagFILE_OF_DAY,				/* 0x3f */
	TLVTagPROCESS_ID,				/* 0x40 */
	TLVTagSVN_LIB_REV,				/* 0x41 */
	TLVTagDSN_FILTER,				/* 0x42 */
	TLVTagDATA_WIDTH,				/* 0x43 */
	TLVTagDATA_FORMAT,				/* 0x44 */
	TLVTagVCO_FREQUENCY,				/* 0x45 */
	TLVTagTRANSFER_TYPE,				/* 0x46 */
	TLVTagSTATION_ID,				/* 0x47 */
	TLVTagMAXFL_WIND,				/* 0x48 */
	TLVTagUSER_NAME,				/* 0x49 */
	TLVTagPASSWORD,					/* 0x4A */
	TLVTagMISSION_ID,				/* 0x4B */
	TLVTagSTORM_ID,					/* 0x4C */
	TLVTagFLID_NUMBER,				/* 0X4D */
	TLVTagWMO_AREA,					/* 0X4E */

	TLVTagMAX	
};	

extern int g_nTLVDataType[TLVTagMAX];

typedef struct IPCMessage
{
	int32_t 	nMessageType;
	int32_t 	nTLVCount;
	char 		cTLVData;
};

typedef struct TLV
{
	int32_t 	lTag;
	int32_t		lLength;
	char   		cValue;
};

class CMessage
{
private:
	int32_t	m_nMessageType;
	int32_t	m_nTLVCount;
	int32_t m_nMsgLength;
	
	CLinkedList<CTLV *> *m_pTLVList;
	
	// Private methods
	ESYSReturn	InitClassMembers( void );
	ESYSReturn  SetNetworkByteOrder( char *pBuffer, int32_t buflen, int32_t lDataType );
public:
	// Class Constructors 

	CMessage( int32_t nMsgType );
	CMessage( char *szRawMsg, bool bNetworkByteOrder );

	virtual ~CMessage( void );
	
	int32_t		Unpack( char *szRawMsg, bool bNetworkByteOrder );
	int32_t		Pack( char *buffer, bool bNetworkByteOrder, int32_t lDataType = ESYSDataTypeMAX );
	int32_t 	PackToSend( char *buffer, bool bNetworkByteOrder, int32_t lDataType = ESYSDataTypeMAX );
	
	void		SetMessageType( int32_t MessageType ){ m_nMessageType = MessageType; }
	int32_t		GetMessageType( void ){ return m_nMessageType; }
	
	int32_t		GetTLVCount( void ){ return m_nTLVCount; }
	int32_t		GetMsgLength( void ){ return m_nMsgLength; };
	
	ESYSReturn	AddTLV( CTLV *pTLV );
	ESYSReturn	AddTLV( int32_t nTag, int32_t nValue );
	ESYSReturn	AddTLV( int32_t nTag, float fValue );
	ESYSReturn	AddTLV( int32_t nTag, double fValue );
	ESYSReturn	AddTLV( int32_t nTag, int32_t nLen, const char *pValue );
	
	CTLV *	GetTLVByTag( int32_t nTag );
};

#endif /*MESSAGE_H_*/
