//////////////////////////////////////////////////////////////////////////////
//++
//
// TLV.h
//
// Description:
//	  Contains implementation of CTLV class, members and methods.  This class
//	  provides the base class for all interprocess TLVs.
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


#include "TLV.h"

//////////////////////////////////////////////////////////////////////
//++
//
// Method: CTLV
//
// Description:
//	  Class constructor.  Sets private member default values, allocates memory.
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

CTLV::CTLV( void )
{
	if (TRACE_CALLS) printf("CTLV::CTLV\n");
	
	InitClassMembers( );
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: CTLV
//
// Description:
//	  Class constructor.  Sets private member default values, allocates memory.
//
// Inputs:
//    lTag - TLV Tag
//    lLength - TLV Length
//    pValue - pointer to buffer containing TLV value
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

CTLV::CTLV( int32_t lTag, int32_t lLength, const char *pValue )
{
	if (TRACE_CALLS) printf("CTLV::CTLV\n");
	
	InitClassMembers( );
	
	m_lTag = lTag;
	m_lLength = lLength;
	memcpy( m_pValue, pValue, m_lLength );
}

//////////////////////////////////////////////////////////////////////
//++
//
// Method: ~CTLV
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

CTLV::~CTLV()
{
	if (TRACE_CALLS) printf("CTLV::~CTLV\n");
	
	// Release allocated memory
	if ( m_pValue != NULL )
	{
		free( m_pValue );
		m_pValue = NULL;
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

ESYSReturn CTLV::InitClassMembers( )
{
	if (TRACE_CALLS) printf("CTLV::InitClassMembers\n");
	
	m_lTag = 0;
	m_lLength = 0;
	
	m_pValue = NULL;
	m_pValue = (char *) malloc( MAX_PACKET_LEN );
	memset( m_pValue, 0, MAX_PACKET_LEN );	

	return ( ESYSReturnSUCCESS );
}

