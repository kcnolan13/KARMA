/********************************************************************************************
 *PROGRAM:	global.h
 *
 *DESCRIPTION:	contains header files, user defined variables, and structs for all programs
 *
 *
 *INPUT:
 *
 *OUTPUT:
 *
 *REFERENCES:
 *
 *REFERENCED BY:
 *******************************************************************************************/

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <cstring>
#include <ctype.h>
#include <cctype>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <math.h>
#include <netcdf.h>		/* HEADER MAY BE DEPENDENT ON COMPUTER FILE SYSTEM!! */
#include <netdb.h>         	/* host to IP resolution       */
#include <netinet/in.h>    	/* Internet address structures */
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mtio.h>
#include <sys/shm.h>
#include <sys/socket.h>    	/* socket interface functions  */
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include "hash_table.h"
#include "stl.h"
#include "Message.h"
#include "Helpers.h"
#include "SystemConstants.h"
#include "SystemTextStrings.h"

#define STL_USING_STRING
using namespace std;

#define  HOSTNAMELEN   	40   	/* maximal host name length */
#define  BUFLEN        	4200   	/* maximum response size  */
#define  PORT          	5052
#define  CH_VAR_LEN 	50        	/* max characters in ch.var[], the short names */
#define  CH_UNITS_LEN 	20      	/* max characters in ch.units[] */
#define  CH_NAME_LEN 	100      	/* max characters in ch.name[], the long names */
#define  CHSIZE 		80
#define  MAX_RATE		1000
#define  AAMPS_BCAST_ADDR  "10.10.16.255"
//#define  AAMPS_BCAST_ADDR  "10.10.10.255"
#define  PORTNO2 	   10546

const float         NaN = atof("NaN");

/*******************************************************************************************
 Source Hash Table structure
 *******************************************************************************************/
struct SrcStruct
  {
    int32_t 	uniqueID;
    string 		uniqueName;
    string 		units;
    string 		ParamName;
  };

/*******************************************************************************************
 RDR Hash Table structure
 *******************************************************************************************/
struct KarmaStruct
 {
	int 	order;
	
 };

/*******************************************************************************************
 Value Hash Table structure
 *******************************************************************************************/
struct ValueStruct
 {
	float	value[200];
 };

typedef CHashTable<struct SrcStruct> SrcStructHashT;

typedef CHashTable<struct KarmaStruct> KarmaStructHashT;

typedef CHashTable<struct ValueStruct> ValueStructHashT;

#endif /* GLOBAL_H_ */
