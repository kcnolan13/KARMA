//////////////////////////////////////////////////////////////////////////////
//++
//
// SystemTextStrings.h
//
// Description:
//	  This file contains string values that correspond to constants
//    defined in SystemConstants.h.
//
// Copyright 2009 - Remote Sensing Solutions Inc.
//					Barnstable, MA
//
// Creation Author: Dan Robinson
// Creation Date:   09/17/2009
//
//--
///////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEMTEXTSTRINGS_H_
#define SYSTEMTEXTSTRINGS_H_

#include "SystemConstants.h"
#include <string>

// System return values
const std::string ESYSReturnText[ESYSReturnMAX] =
{
	"Success",						// ESYSReturnSUCCESS = 0, 
	"Failure",						// ESYSReturnFAILURE,
	"String Error",					// ESYSReturnSTRINGERROR,
	"Not Initialized",				// ESYSReturnNOT_INITIALIZED,
	"Invalid XML",					// ESYSReturnINVALID_XML,
	"NetCDF File Error",			// ESYSReturnNETCDF_FILE_ERROR,
	"NetCDF Write Error",			// ESYSReturnNETCDF_WRITE_ERROR,
	"Message Too Long",				// ESYSReturnMSG_TOO_LONG,
	"Socket Error",					// ESYSReturnSOCKET_ERROR,
	"Invalid Message Type",			// ESYSReturnINVALID_MSG_TYPE,
	"Invalid Data Type",			// ESYSReturnINVALID_DATA_TYPE,
	"Not Found",					// ESYSReturnNOT_FOUND,
	"Encountered Negative Time",	// ESYSReturnNEGATIVE_TIME,
	"Time Out of Sync",				// ESYSReturnTIME_OUT_OF_SYNC,
	"Invalid Device Type"			// ESYSReturnINVALID_DEVICE_TYPE,
	"No Parameters of This Type",   // ESYSReturnNO_PARAMS
	"File I/O Error",				// ESYSReturnFILE_IO_ERROR
	"File Open Error",				// ESYSReturnFILE_OPEN_ERROR
	"Parsing Error",				// ESYSReturnPARSE_ERROR
	"No Device"						// ESYSReturnNO_DEVICE
	
};

// Process Types
const std::string ESYSProcessText[ESYSProcessMAX+1] = 
{
	"MDS_CDH", 				// ESYSProcessMDS_CDH = 0,
	"CAM_CDH", 				// ESYSProcessCAM_CDH,
	"DDS",					// ESYSProcessDDS,
	"DISPLAY_GUI", 			// ESYSProcessDISPLAY_GUI,
	"RTDP",					// ESYSProcessRTDP,
	"CONFIG_GUI",			// ESYSProcessCONFIG_GUI,
	"WMM",					// ESYSProcessWMM,
	"LFL",					// ESYSProcessLFL
	"IWG",					// ESYSProcessIWG
	"SFMR",					// ESYSProcessSFMR
	"PROCESS SIMULATOR",    // ESYSProcessSIM
	"AVAPS RDS",			// ESYSProcessAVAPS_RDS
	"NOVATEL RDS",			// ESYSProcessNOVATEL_RDS
	"SFMR RDS",				// ESYSProcessSFMR_RDS
	"WMM SERVER",			// ESYSProcessWMM_SERVER
	"RDR",				// ESYSProcessRDR
	"REPLAY",			// ESYSProcessREPLAY
	"JRTDP",			// ESYSProcessJRTDP
	"OZONE RDS",			// ESYSProcessOZONE_RDS
	"JAVA CLIENT",			// ESYSProcessCLIENT
	"KARMA",				// ESYSProcessKARMA
	
	"UNIDENTIFIED"			// ESYSProcessMAX
};	

// Message types
const std::string IPCMsgTypeText[IPCMsgMAX] =
{
	"Undefined",				// IPCMsgTypeUNDEFINED = 0,					
	"Data Source Notification",	// IPCMsgTypeDATA_SOURCE_NOTIFICATION,		
	"Data Feed Subscribe",		// IPCMsgTypeDATA_FEED_SUBSCRIBE,			
	"Config Update",			// IPCMsgTypeGUI_CONFIG_UPDATE,				
	"AAMPS Data",				// IPCMsgTypeAAMPS_DATA,					
 	"Time Sync",				// IPCMsgTypeTIME_SYNC,						
 	"Error",					// IPCMsgTypeERROR,		
 	"User Notification",		// IPCMsgTypeUSER_NOTIFICATION
 	"Process Information",		// IPCMsgTypePROCESS_INFO
 	"DDS Ready",				// IPCMsgTypeDDS_READY
 	"MDS Ready",                // IPCMsgTypeMDS_READY
 	"File Transfer Request",	// IPCMsgTypeFILE_TRANFER_REQUEST
	"WMM Update"				// IPCMsgTypeWMM_GUI

};	

// Device Types
const std::string ESYSDeviceText[ESYSObjectDEVICE_MAX] =
{
	"None",						// ESYSObjectDEVICE_NONE = SYSObjectDEVICE,
	"DIO",						// ESYSObjectDEVICE_DIO,
	"AD",						// ESYSObjectDEVICE_AD,
	"TIMING",					// ESYSObjectDEVICE_TIMING,
	"MIL1553", 					// ESYSObjectDEVICE_1553,
	"ARINC429",					// ESYSObjectDEVICE_ARINC429,
	"SERIAL",					// ESYSObjectDEVICE_SERIAL,
	"DA",						// ESYSObjectDEVICE_DA,
	"SYNCHRO",					// ESYSObjectDEVICE_SYNCHRO,
	"ARINC708",					// ESYSObjectDEVICE_ARINC708,
	"CT9",						// ESYSObjectDEVICE_CT9,
	"AVAPS",					// ESYSObjectDEVICE_AVAPS,
	"NOVATEL",					// ESYSObjectDEVICE_NOVATEL,
	"SFMR",						// ESYSObjectDEVICE_SFMR,
	"GSHSS",					// ESYSObjectDEVICE_GSHSS
	"AAD",						// ESYSObjectDEVICE_AAD
	"SEA",						// ESYSObjectDEVICE_SEA
	"OZONE"						// ESYSObjectDEVICE_OZONE
};	

// Genereic Parameter Descriptors
const std::string ESYSParamDescText[ESYS_PARAM_DESC_END] =
{
	"CAM_ID",					// ESYS_PARAM_DESC_CAM_ID = 0,
	"Slot_Type",					// ESYS_PARAM_DESC_SLOT_TYPE,
	"Source",					// ESYS_PARAM_DESC_SOURCE,
	"Units",					// ESYS_PARAM_DESC_UNITS,			
	"ParamName",					// ESYS_PARAM_DESC_PARAM_NAME,
	"Description",					// ESYS_PARAM_DESC_DESCRIPTION,
	"Slot",						// ESYS_PARAM_DESC_SLOT,		 
    	"Channel",					// ESYS_PARAM_DESC_CHANNEL,
    	"Tag",						// ESYS_PARAM_DESC_TAG,			
    	"DeviceInstance",				// ESYS_PARAM_DESC_DEVINST,			
	"Min",						// ESYS_PARAM_DESC_MIN,
	"Max",						// ESYS_PARAM_DESC_MAX,
	"CAMNumber"					// ESYS_PARAM_DESC_CAM_NUM,
};
    	
// RTDP Parameter Descriptors
const std::string ESYSRTDPParamDescText[ESYS_RTDP_PARAM_DESC_END] =
{	
	"Quantity",					// ESYS_RTDP_PARAM_DESC_QUANTITY = 0,
	"Application",					// ESYS_RTDP_PARAM_DESC_APPLICATION,
	"Location",					// ESYS_RTDP_PARAM_DESC_LOCATION,
	"ParamName",					// ESYS_RTDP_PARAM_DESC_PARAM_NAME,
	"Description",					// ESYS_RTDP_PARAM_DESC_DESCRIPTION,
	"units",					// ESYS_RTDP_PARAM_DESC_UNITS,
	"Routine",					// ESYS_RTDP_PARAM_DESC_SUBROUTINE,
	"long_name",					//ESYS_RTDP_PARAM_DESC_LONG_NAME,
	"standard_name", 				//ESYS_RTDP_PARAM_DESC_STANDARD_NAME,
	"strptime_format",				//ESYS_RTDP_PARAM_DESC_TIME_FORMAT,
	"Min",						// ESYS_RTDP_PARAM_DESC_MIN,
	"Max",						// ESYS_RTDP_PARAM_DESC_MAX,
	"Tag",						// ESYS_RTDP_PARAM_DESC_TAG,
	"Instance",					// ESYS_RTDP_PARAM_DESC_INSTANCE,
	"Ref_Input",					// ESYS_RTDP_PARAM_DESC_REF_INPUT,
	"Ref_Index"					// ESYS_RTDP_PARAM_DESC_REF_INDEX,
};	
   
const std::string ESYSTransferTypeText[ESYSTransferTypeMAX] = 
{
	"SCP",						// ESYSTransferTypeSCP = 0,
	"FTP"						// ESYSTransferTypeFTP,
};
 		
#endif
