//////////////////////////////////////////////////////////////////////////////
//++
//
// TLV.h
//
// Description:
//	  Contains definition of CTLV class, members and methods.  This class
//	  provides the basis for IPCTLV TLVs.
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

#ifndef TLV_H_
#define TLV_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SystemConstants.h"

class CTLV
{
private:
	int32_t		m_lTag;
	int32_t		m_lLength;
	char *		m_pValue;
	
	// Private methods
	ESYSReturn		InitClassMembers( void );
public:
	// Class Constructors 

	CTLV( void );
	CTLV( int32_t lTag, int32_t lLength, const char *pValue );

	virtual ~CTLV( void );
	
	void	SetTag( int32_t Tag ){ m_lTag = Tag; }
	int32_t	GetTag( void ){ return m_lTag; }
	
	void	SetLength( int32_t Length ){ m_lLength = Length; }
	int32_t	GetLength( void ){ return m_lLength; }
	
	void	SetValue( char *Value ){ m_pValue = Value; }
	char *	GetValue( void ){ return m_pValue; }
};

#endif /*TLV_H_*/
