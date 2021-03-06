//////////////////////////////////////////////////////////////////////////////
//++
//
// SystemConstants.h
//
// Description:
//	  This file contains constant values used throughout the AAMPS system.
//
// Copyright 2008 - Remote Sensing Solutions Inc.
//					Barnstable, MA
//
// Creation Author: Dan Robinson
// Creation Date:   06/17/2008
//
//--
///////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEMCONSTANTS_H_
#define SYSTEMCONSTANTS_H_

#include <stdint.h>

#define		VERBOSE					0   // Enable/Disable diagnostic printing
#define		REALLY_VERBOSE				0   // Enables printing of byte streams
#define		TRACE_CALLS				0   // Enables tracing of calls through object methods
#define		PRINT_FIFO_BYTES			0   // Enables printing of received FIFO data as hex bytes
#define		DEBUG_TIMESTAMPS			0   // Enables debugging of timestamps on CAM
#define		SHOW_TAGS_RECEIVED			0   // Enables printing of tag and index for each payload
#define		SUPPRESS_NETCDF				0   // Disables writing raw data to NetCDF (for testing over long periods)
#define		DISPLAY_RTDP_VALUES			1   // Enables display of RTDP values being written to NetCDF
#define		DEBUG_DDS_SOCKETS			1   // Enables display of DDS socket connect/disconnect info

#define		INT32_MAX				0x7fffffff
#define		INT16_MAX				0x7fff

#define		MAX_SOCK_RCV_SIZE			1048576
#define		MAX_SOCK_SND_SIZE			1048576

// Message Subscription Constants
#define		MSG_SUBSCRIPTION_PREFIX			0X63000000  // Equivalent of 99 in high order byte
#define		UNSUBSCRIBE				0
#define		SUBSCRIBE				1

// String Length Constants
#define		MAX_AAMPS_PATH_LEN			255
#define		MAX_AAMPS_SVN_COMMENT_LEN	MAX_AAMPS_PATH_LEN + 16
#define		MAX_AAMPS_CAMS				32
// "CAM Numbers" assigned to the RTDP and remote data sources
enum
{
	RTDP_CAM_NUMBER	= MAX_AAMPS_CAMS, 
	AVAPS_RDS_CAM_NUMBER,
	NOVATEL_RDS_CAM_NUMBER,
	SFMR_RDS_CAM_NUMBER,
	OZONE_RDS_CAM_NUMBER,
	PROCESS_SIM_CAM_NUMBER,
	MAX_CONFIGURABLE_CAM_NUMBER
};

// Special Data Source Notification Subscription Filters
#define		DSN_FILTER_ALL_DATA			1000
#define		DSN_FILTER_RAW_DATA			1001
#define		DSN_FILTER_PROCESSED_DATA		1002
#define		DSN_FILTER_REMOTE_DATA			1003

// Display Levels
#define		DISPLAY_NONE				0
#define		DISPLAY_ALL				1

#define		MAX_CAM_PARAMS				756
#define		MAX_CAM_DEVICES				32
#define		MAX_CAM_ID_LEN				32

#define		MAX_DESCRIPTION_LEN			200 //50
#define		MAX_PACKET_LEN				8192
#define		MAX_IP_ADDR_LEN				16
#define		MAX_ERROR_LEN				255

// NetCDF File Constants
#define		MAX_NETCDF_PARAMS			2000
#define		MAX_NETCDF_VARS				1000
#define		MAX_NETCDF_NAME_LEN			50
#define		MAX_NETCDF_DIM				10000

#define		MAX_RTDP_REFERENCES			32

#define		MAX_M1553_PARAMS			32

// Socket Constants
#define		LISTENQ					1024		/* 2nd argument to listen() */
#define		MAXSOCKADDR  				128		/* max socket address structure size */

#define		AAMPS_MULTICAST_PORT			9154
#define		AAMPS_DDS_NBO_PORT			9132
#define		AAMPS_DDS_NON_NBO_PORT			9133
#define		TDFP_MULTICAST_PORT			9134
#define		DISPLAY_PORT				9135
#define     	AAMPS_MDS_STATUS_PORT       		9136
#define		AAMPS_DDS_CDH_CMD_PORT			9137
#define		CONFIG_PORT				9138
#define		WMM_SERVER_PORT				9139
#define 	AAMPS_MULTICAST_ADDR  			"225.0.0.37"
#define		AAMPS_SOCK_RCV_BUF_SIZE			16384*4096  	// 16k is OS default

/* SBS DA16 ioctls */
#define RTL_SBS_DA16_RESET        			0
#define RTL_SBS_DA16_GET_STAT     			1
#define RTL_SBS_DA16_WRITE_VALUES 			2


// Parameter Descriptors for CAM Data
// NOTE: Any changes to this enumeration must also be accompanied by
// corresponding changes to the list of string constants in the 
// variable gszParamDescriptor contained in SystemTextStrings.h
enum ESYS_PARAM_DESC
{
	ESYS_PARAM_DESC_CAM_ID = 0,
	ESYS_PARAM_DESC_SLOT_TYPE,
	ESYS_PARAM_DESC_SOURCE,
	ESYS_PARAM_DESC_UNITS,			
	ESYS_PARAM_DESC_PARAM_NAME,
	ESYS_PARAM_DESC_DESCRIPTION,
	ESYS_PARAM_DESC_SLOT,		 
    	ESYS_PARAM_DESC_CHANNEL,
    	ESYS_PARAM_DESC_TAG,			
    	ESYS_PARAM_DESC_DEVINST,			
	ESYS_PARAM_DESC_MIN,
	ESYS_PARAM_DESC_MAX,
	ESYS_PARAM_DESC_CAM_NUM,
    	ESYS_PARAM_DESC_END		
};
// Param descriptors for A429
enum
{
	ESYS_PARAM_DESC_LABEL = ESYS_PARAM_DESC_END,
	ESYS_PARAM_DESC_RES,
	ESYS_PARAM_DESC_CONVERSION
};
// Param descriptors for AD
enum
{
	ESYS_PARAM_DESC_C1 = ESYS_PARAM_DESC_END,
	ESYS_PARAM_DESC_C2,
	ESYS_PARAM_DESC_C3,
	ESYS_PARAM_DESC_C4,
	ESYS_PARAM_DESC_RANGE,
	ESYS_PARAM_DESC_IMAGE_FILTER,
	ESYS_PARAM_DESC_DATA_WIDTH,
	ESYS_PARAM_DESC_DATA_FORMAT,
	ESYS_PARAM_DESC_SENSOR,
	ESYS_PARAM_DESC_AMPLIFIER,
	ESYS_PARAM_DESC_DATE,
	ESYS_PARAM_DESC_MANUFACTURER,
	ESYS_PARAM_DESC_MODEL,
	ESYS_PARAM_DESC_SERIAL,
	ESYS_PARAM_DESC_AIRCRAFT,
	ESYS_PARAM_DESC_PORT,
	ESYS_PARAM_DESC_IP
};
// Param descriptors for 1553
enum
{
	ESYS_PARAM_DESC_SA = ESYS_PARAM_DESC_END,
	ESYS_PARAM_DESC_RT,
	ESYS_PARAM_DESC_WC,
	ESYS_PARAM_DESC_WORD,
	ESYS_PARAM_DESC_REFRESH
};
// Param descriptors for CT9		L.Miller;20101001
enum
{
	ESYS_PARAM_DESC_MSG_SIZE = ESYS_PARAM_DESC_END,
	ESYS_PARAM_DESC_CHAR_SIZE,
	ESYS_PARAM_DESC_PARITY,
	ESYS_PARAM_DESC_EXTRA_STOP_BIT,
	ESYS_PARAM_DESC_BAUD_RATE,
	ESYS_PARAM_DESC_MODE,			//for GS HSS
	ESYS_PARAM_DESC_DCE			//for GS HSS
};

// Parameter Descriptors for RTDP Data
// NOTE: Any changes to this enumeration must also be accompanied by
// corresponding changes to the list of string constants in the 
// variable gszParamDescriptor contained in SystemTextStrings.h
enum ESYS_RTDP_PARAM_DESC
{
	// strings
	ESYS_RTDP_PARAM_DESC_QUANTITY = 0,
	ESYS_RTDP_PARAM_DESC_APPLICATION,
	ESYS_RTDP_PARAM_DESC_LOCATION,
	ESYS_RTDP_PARAM_DESC_PARAM_NAME,
	ESYS_RTDP_PARAM_DESC_DESCRIPTION,
	ESYS_RTDP_PARAM_DESC_UNITS,
	ESYS_RTDP_PARAM_DESC_SUBROUTINE,
	ESYS_RTDP_PARAM_DESC_LONG_NAME,
	ESYS_RTDP_PARAM_DESC_STANDARD_NAME,
	ESYS_RTDP_PARAM_DESC_TIME_FORMAT,
	// floats
	ESYS_RTDP_PARAM_DESC_MIN,
	ESYS_RTDP_PARAM_DESC_MAX,
	// ints
	ESYS_RTDP_PARAM_DESC_TAG,
	ESYS_RTDP_PARAM_DESC_INSTANCE,
	
	// RTDP Reference vars
	ESYS_RTDP_PARAM_DESC_REF_INPUT,
	ESYS_RTDP_PARAM_DESC_REF_INDEX,
	
    ESYS_RTDP_PARAM_DESC_END		
};

// Value-Definitions of the different String constants
// used in case statements throughout the XML processing code
enum StringValue 
{ 
    evNotDefined,
    evStringLabel,
    evStringDataType,
    evStringUnits,
    evStringMin,
    evStringMax,
    evStringRes,
    evStringSource,
    evStringRate,
    evStringDataRate,
    evStringDescription,
    evStringArraySize,
    evStringTag,
    evStringSA,
    evStringRT,
    evStringWordCount,
    evStringWord,
    evStringRefresh,
    evStringParamName,
    evStringC1,
    evStringC2,
    evStringC3,
    evStringC4,
    evStringQuantity,
    evStringApplication,
    evStringLocation,
    evStringInstance,
    evStringSubroutine,
    evStringInputName,
    evStringInputTag,
    evStringSourceCAMNumber,
    evStringConversion,
    evStringSensor,
    evStringAmplifier,
    evStringDate,
    evStringManufacturer,
    evStringModel,
    evStringSerial,
    evStringAircraft,
    evStringPlotMin,
    evStringPlotMax,
    evStringIndex,
    evStringArchive,
    evStringDisplay,
    evStringMsgSize,
    evStringCharSize,
    evStringMsgRate,
    evStringBaud,
    evStringParity,
    evStringStopBit,
    evStringDce,
    evStringPort,
    evStringIP,
    evStringLongName,
    evStringStandardName,
    evStringStrptimeFormat,
    evStringCAMNumber,
    evEnd 
};

// System return values
// Be sure to update SystemTextStrings.h with any additions made here
enum ESYSReturn
{
	ESYSReturnSUCCESS = 0, 
	ESYSReturnFAILURE,
	ESYSReturnSTRINGERROR,
	ESYSReturnNOT_INITIALIZED,
	ESYSReturnINVALID_XML,
	ESYSReturnNETCDF_FILE_ERROR,
	ESYSReturnNETCDF_WRITE_ERROR,
	ESYSReturnMSG_TOO_LONG,
	ESYSReturnSOCKET_ERROR,
	ESYSReturnINVALID_MSG_TYPE,
	ESYSReturnINVALID_DATA_TYPE,
	ESYSReturnNOT_FOUND,
	ESYSReturnNEGATIVE_TIME,
	ESYSReturnTIME_OUT_OF_SYNC,
	ESYSReturnINVALID_DEVICE_TYPE,
	ESYSReturnNO_PARAMS,
	ESYSReturnFILE_IO_ERROR,
	ESYSReturnFILE_OPEN_ERROR,
	ESYSReturnPARSE_ERROR,
	ESYSReturnNO_DEVICE,
	
	ESYSReturnMAX
};

// System Data Types
enum ESYSDataType
{
	ESYSDataTypeBYTE,
	ESYSDataTypeINT16,
	ESYSDataTypeINT32,
	ESYSDataTypeFLOAT,
	ESYSDataTypeDOUBLE,
	ESYSDataTypeSTRING,
	ESYSDataTypeOTHER,
	
	ESYSDataTypeMAX
};

// System Object Class Hierarchy
#define	SYSObjectAAMPS 				0
#define SYSObjectRDS				500
#define	SYSObjectCOMPONENT			1000
#define	SYSObjectDEVICE 			2000
#define SYSObjectOCTET				3000
#define	SYSObjectGROUP 				4000
#define	SYSObjectCHANNEL 			5000
#define	SYSObjectPARAMETER			6000
#define SYSObjectPARAM_DESCRIPTOR		7000
#define SYSObjectRTDP_QUANTITY			8000
#define SYSObjectRTDP_INPUT			9000
#define SYSObjectTRIVIAL_FEED			10000

// AAMPS System Objects
enum ESYSObjectCOMPONENT
{
	ESYSObjectCOMPONENT_CAM = SYSObjectCOMPONENT,
	ESYSObjectCOMPONENT_MDS,
	ESYSObjectCOMPONENT_RTDP,
	ESYSObjectCOMPONENT_RDS
};

// Device Objects
// NOTE: Any changes to this enumeration must also be accompanied by
// corresponding changes to the list of string constants in the 
// variable gszDeviceName contained in CXMLToNetCDF::CreateNCDFile()
enum ESYSObjectDEVICE
{
	ESYSObjectDEVICE_NONE = SYSObjectDEVICE,
	ESYSObjectDEVICE_DIO,
	ESYSObjectDEVICE_AD,
	ESYSObjectDEVICE_TIMING,
	ESYSObjectDEVICE_1553,
	ESYSObjectDEVICE_ARINC429,
	ESYSObjectDEVICE_SERIAL,
	ESYSObjectDEVICE_DA,
	ESYSObjectDEVICE_SYNCHRO,
	ESYSObjectDEVICE_ARINC708,
	ESYSObjectDEVICE_CT9,
	ESYSObjectDEVICE_AVAPS,
	ESYSObjectDEVICE_NOVATEL,
	ESYSObjectDEVICE_SFMR,
	ESYSObjectDEVICE_GSHSS,				//for GS HSS
	ESYSObjectDEVICE_AAD,				//for AAD
	ESYSObjectDEVICE_SEA,				//for SEA, 2016
	ESYSObjectDEVICE_OZONE,
	ESYSObjectDEVICE_MAX
};

enum ESYSObjectOCTET
{
	ESYSObjectOCTET_AD = SYSObjectOCTET
};

enum ESYSObjectGROUP
{
	ESYSObjectGROUP_AD = SYSObjectGROUP
};

enum ESYSObjectCHANNEL
{
	ESYSObjectCHANNEL_NONE = SYSObjectCHANNEL,
	ESYSObjectCHANNEL_DIO,
	ESYSObjectCHANNEL_AD,
	ESYSObjectCHANNEL_TIMING,
	ESYSObjectCHANNEL_1553,
	ESYSObjectCHANNEL_ARINC429,
	ESYSObjectCHANNEL_SERIAL,
	ESYSObjectCHANNEL_DA,
	ESYSObjectCHANNEL_SYNCHRO,
	ESYSObjectCHANNEL_ARINC708,
	ESYSObjectCHANNEL_GSHSS,			//for GS HSS
	ESYSObjectCHANNEL_AAD,				//for AAD
	ESYSObjectCHANNEL_SEA,				//for SEA
	ESYSObjectCHANNEL_MAX
};

enum ESYSObjectPARAMETER
{
	ESYSObjectPARAMETER_NONE = SYSObjectPARAMETER,
	ESYSObjectPARAMETER_DIO,
	ESYSObjectPARAMETER_AD,
	ESYSObjectPARAMETER_TIMING,
	ESYSObjectPARAMETER_1553,
	ESYSObjectPARAMETER_ARINC429,
	ESYSObjectPARAMETER_SERIAL,
	ESYSObjectPARAMETER_DA,
	ESYSObjectPARAMETER_SYNCHRO,
	ESYSObjectPARAMETER_ARINC708,
	ESYSObjectPARAMETER_GSHSSSERIAL,		//for GS HSS
	ESYSObjectPARAMETER_AAD,			//for AAD
	ESYSObjectPARAMETER_SEA,			//for SEA
	ESYSObjectPARAMETER_MAX
};

enum ESYSObjectPARAM_DESCRIPTOR
{
	ESYSObjectPARAM_DESCRIPTOR_NONE = SYSObjectPARAM_DESCRIPTOR,
	ESYSObjectPARAM_DESCRIPTOR_DIO,
	ESYSObjectPARAM_DESCRIPTOR_AD,
	ESYSObjectPARAM_DESCRIPTOR_TIMING,
	ESYSObjectPARAM_DESCRIPTOR_1553,
	ESYSObjectPARAM_DESCRIPTOR_ARINC429,
	ESYSObjectPARAM_DESCRIPTOR_SERIAL,
	ESYSObjectPARAM_DESCRIPTOR_DA,
	ESYSObjectPARAM_DESCRIPTOR_SYNCHRO,
	ESYSObjectPARAM_DESCRIPTOR_ARINC708,
	ESYSObjectPARAM_DESCRIPTOR_MAX
};

enum ESYSObjectRTDP_QUANTITY
{
	ESYSObjectRTDP_QUANTITY = SYSObjectRTDP_QUANTITY,
	ESYSObjectRTDP_REFERENCE,
	ESYSObjectRDS_QUANTITY,
	ESYSObjectRTDP_QUANTITY_MAX
};

// Process Definitions
// Be sure to update SystemTextStrings.h with any additions made here
enum ESYSProcess
{
	ESYSProcessMDS_CDH = 0,
	ESYSProcessCAM_CDH,
	ESYSProcessDDS,
	ESYSProcessDISPLAY_GUI,
	ESYSProcessRTDP,
	ESYSProcessCONFIG_GUI,
	ESYSProcessWMM,
	ESYSProcessLFL,
	ESYSProcessIWG,
	ESYSProcessSFMR,
	ESYSProcessSIM,
	ESYSProcessAVAPS_RDS,
	ESYSProcessNOVATEL_RDS,
	ESYSProcessSFMR_RDS,
	ESYSProcessWMM_SERVER,
	ESYSProcessRDR,		/* 0x0F */
	ESYSProcessREPLAY,	/* 0x10 */
	ESYSProcessJRTDP,	/* 0x11 */
	ESYSProcessOZONE_RDS,	/* 0x12 */
	ESYSProcessCLIENT,	/* 0x13 */
	ESYSProcessKARMA,	/* 0x14 */
	
	ESYSProcessMAX
};

// XML Processing Level
// MDS_CDH = Parse master XML, calculate, track and insert CAM Numbers, Device  
//     Instances and Data Tags. Create and write out CAM specific XML files.  
//     Used by MDS CDH.
// MDS_OTHER = Parse master XML, track CAM Numbers, Device Instances and Data Tags.
//     Do not generate CAM specific XML files.  Used by non-CDH MDS processes.
// CAM = Parse CAM specific XML file.  CAM Number, Device Instance, and Data Tags
//     are present in the file.  Used by CAMs.
enum ESYS_XMLProcessingLevel
{
	ESYS_XMLProcessingLevelMDS_CDH = 0, 	
	ESYS_XMLProcessingLevelMDS_OTHER,
	ESYS_XMLProcessingLevelCAM,
	
	ESYS_XMLProcessingLevelMAX
};

// IPC Message Type Definitions
// Be sure to update SystemTextStrings.h with any additions made here
enum IPCMsgType
{
	IPCMsgTypeUNDEFINED = 0,					
	IPCMsgTypeDATA_SOURCE_NOTIFICATION,		
	IPCMsgTypeDATA_FEED_SUBSCRIBE,			
	IPCMsgTypeGUI_CONFIG_UPDATE,				
	IPCMsgTypeAAMPS_DATA,					
 	IPCMsgTypeTIME_SYNC,						
 	IPCMsgTypeERROR,		
 	IPCMsgTypeUSER_NOTIFICATION,	
 	IPCMsgTypePROCESS_INFO,	
 	IPCMsgTypeDDS_READY,	
 	IPCMsgTypeMDS_READY,	
 	IPCMsgTypeFILE_TRANSFER_REQUEST,
	IPCMsgTypeWMM_GUI,	
	
	IPCMsgMAX		
};						

// Configuration Level Constants
// To be used with Config Level TLV
enum ESYSConfigLevel
{
	ESYSConfigLevelCAM = 0,
	ESYSConfigLevelRTDP_ONLY,
	ESYSConfigLevelHDOB,
	ESYSConfigLevelINBOUND,
	
	ESYSConfigLevelMAX
};

// Error Severity Constants
enum ESYSError
{
	ESYSErrorINFORMATIVE = 0,
	ESYSErrorMINOR,
	ESYSErrorMAJOR,
	ESYSErrorCRITICAL,
	
	ESYSErrorMAX
};

// Plot component Constants
enum ESYSPlot
{
	ESYSPlotNORMAL = 0,
	ESYSPlotLATITUDE,
	ESYSPlotLONGITUDE,
	
	ESYSPlotMAX
};

// Transfer Type Constants
enum ESYSTransferType
{
	ESYSTransferTypeSCP = 0,
	ESYSTransferTypeFTP,

	ESYSTransferTypeMAX
};
	
// The following is for reference.  Defines CAM payloads as of 7/3/08
// derived from cam_common.h

#define MAX_AD_RATE			1000  			/* CAM's maximum practical rate of reading the A/D FIFO */
#define NUM_DA_CHANNELS 		8   			/* Number of D/A channels per board (not system) */
#define DEFAULT_BINS_PER_FRAME  	512 			/* from Condor P708 driver */
#define DEFAULT_CONTROL_WORD_CT 	4
#define MAX_M1553_RATE			50			// may be a practical limit, but discussions continue
#define MAX_A429_RATE			50  			// maximum sample rate of any a429 sample (Hz)
#define CAM_MAX_DEVICES	 		16
#define MAX_SERIAL_MSG_RATE 		100			// arbitrary, to say the least
#define MAX_SERIAL_MSG_SIZE 		32			// ditto
#define DEFAULT_WORDS_PER_FRAME 	100			// words are shorts
#define A708_FRAME_RATE			160			// nominally 128 Hz
#define FRAMES_PER_DECTET	   	A708_FRAME_RATE/10	// frames per one-tenth sec.

struct rxFrame {					// structure for p708 payload
	uint32_t	timetag;			// 64 bits for a 48-bit time-tag
	uint16_t   	bins[DEFAULT_WORDS_PER_FRAME];	// 512 bins + 4 control words
};

/* Base device names - append _# for multiple boards */
#define DEV_DIO_NAME 		"/dev/amg_464"
#define DEV_AD_NAME 		"/dev/gs_ad"
#define DEV_TIMING_NAME 	"/dev/bwine_timing"
#define DEV_M1553_NAME 		"/dev/sbs_1553"
#define DEV_A429_NAME 		"/dev/sbs_429"
#define DEV_SERIAL_NAME 	"/dev/sbs_dscc4"
#define DEV_DA_NAME 		"/dev/sbs_da16"
#define DEV_SYNCHRO_NAME	"/dev/na_synchro"
#define DEV_P708_NAME 		"/dev/cdr_708"
#define DEV_RTL_AVN 		"/dev/sbspci"
#define DEV_CT9_PORT_NAME	"/dev/ttyS"
#define DEV_GS_HSS_PORT_NAME    "/dev/sio4"

enum DEVICE_TYPES {
	DEV_DIO = 1,
	DEV_AD,
	DEV_TIMING,
	DEV_M1553,
	DEV_A429,
	DEV_SERIAL,
	DEV_DA,
	DEV_SYNCHRO,
	DEV_P708,
	DEV_CT9_PORT,
	DEV_AVAPS,
	DEV_NOVATEL,
	DEV_SFMR,
	DEV_GS_HSS_CHANNEL,
	DEV_AAD_CHANNEL,
	DEV_SEA_SENTENCE,
	DEV_NODEV /* optional - for requests that aren't device-specific */
};
//---------------------	Structure for DAC device -----------------------------------
struct aad_sample {
	uint16_t ch1;
	uint16_t ch2;
	uint16_t ch3;
	uint16_t ch4;
	uint16_t ch5;
	uint16_t ch6;
	uint16_t ch7;
	uint16_t ch8;
	uint8_t status;
	uint8_t checksum;
	uint8_t sample_sync; /* last two are 1 2 byte frame sync on last sample of each frame */
	uint8_t status2;
};
struct aad_frame {
	struct aad_sample samples[100];
};

#define NUM_AAD_CHANNELS	16
struct dev_aad_channel {
	/* ip/port is stored from xml in network order - no need to translate at runtime */
	uint32_t ip_addr; 
	uint16_t port;  
	int			  tag;   	// unique tag for use by MDS in identifying data.
};

#define NUM_SEA_SENTENCES 	12			// number of sentences per second
struct dev_sea_sentence {
	/* ip/port is stored from xml in network order - no need to translate at runtime */
	uint32_t ip_addr; 
	uint16_t port;  
	int			  tag;   	// unique tag for use by MDS in identifying data.
};

#define NUM_DA_CHANNELS 8   		/* Number of D/A channels per board (not system) */
struct da_port {
	int value; 						/* datum in microvolts */
	int set; 						/* TRUE to display datum on this port */
};

struct dac_hwstat {
	int	numChan;		/* count of channels as shipped from SBS */
	int	biPol_1_4;		/* polarity of channels 1-4, per jumpers */
	int	biPol_5_8;		/* polarity of channels 5-8, per jumpers */
};

struct cam_payload_timestamp {
	uint32_t low32bits;
	uint32_t high32bits;
};

#define CAM_TIMESOURCE_GPS 			1
#define CAM_TIMESOURCE_IRIG			2	
#define CAM_TIMESOURCE_HAVEQUICK	3

struct cam_payload_mgmt {
	uint8_t 	devtype;
	uint8_t 	devnum;
	uint32_t 	seqnum;
	uint32_t 	tag;
};

struct cam_payload {
	struct cam_payload_timestamp ts;
	struct cam_payload_mgmt mgmt;
	uint32_t rate;

	union {
		struct {
			uint32_t low32bits;
			uint32_t high32bits;
		} dio;
		struct {
			int32_t  channel;
			uint32_t values[MAX_AD_RATE]; /* still in bit form */
		} ad;					// this union drives the size of the struct @ max rate of 1kHz
		struct {
			int channel;
			int label;
			uint32_t values[MAX_A429_RATE];
		} a429;
		struct {
			uint8_t  chan;						// each M1553_PMC2 has 2 independent 1553 buses (looks like 2 devices)
			uint8_t  rt;						// identify the RT sending the message
			uint8_t  sa;						// identify the msg number (also called sub-address)
			uint8_t  sa_size;					// specify the msg size (in unsigned shorts)
			uint16_t msg[MAX_M1553_RATE][32];	// max number of 32-word msgs (SAs) in a payload
		} m1553;
		struct {
			struct da_port ports[NUM_DA_CHANNELS];
		} da;
		struct {										// bytes: 2 + 204*FRAMES_PER_DECTET
			uint8_t channel;			
			uint8_t dectet;						// 10 payloads comprise approx 1 sec of Collins-radar frames
			struct rxFrame frame[FRAMES_PER_DECTET];	// 204 bytes per frame - includes 4 bytes of clock
		} p708;
		struct {
			uint32_t msg_size;
   			char string[MAX_SERIAL_MSG_RATE][MAX_SERIAL_MSG_SIZE];	// [100][32]
		} ct9_port;
                struct {
                        uint32_t msg_size;
                        char string[MAX_SERIAL_MSG_RATE][MAX_SERIAL_MSG_SIZE];  // [100][32]
                } gs_hss;
		struct {
			uint32_t ip_addr;
			uint16_t port;
   			struct aad_frame frame;
		} aad;
                struct {
			uint32_t ip_addr;
			uint16_t port;
			char *string[NUM_SEA_SENTENCES];                              //[12]
                } sea;
	};
};

struct cam_signal {
	enum DEVICE_TYPES devtype; 			/* what type of device to control */
	int devnum; 						/* which one - 0, 1, 2, etc. Usually 0. */
	int ioctl; 							/* ioctl() that is going to be called on the driver */
	union { 							/* all types of requests that could come along behind */

		struct cam_payload payload;

		/* SBS 16-bit, 8-channel DAC hardware status */
		struct dac_hwstat dac_stat;
	};
};

/* structure to come back to cli from rt side */
struct cam_signal_resp {
	struct cam_signal signal;
	struct cam_payload_timestamp ts;
	int result;
};

/* Device for CLI to push requests up to real-time side */
#define CAM_SIGNAL_DEVICE 		"/tmp/cam_cli_to_rt"
#define CAM_SIGNAL_SIZE			1<<20

/* Device for real-time side to push responses to CLI */
#define CAM_SIGNAL_RESP_DEVICE 		"/tmp/cam_rt_to_cli"
#define CAM_SIGNAL_RESP_SIZE		1<<20

#endif /*SYSTEMCONSTANTS_H_*/
