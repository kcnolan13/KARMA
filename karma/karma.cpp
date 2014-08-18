/*
#=================================================================
# APPLICATION NAME: karma.cpp
#
# SUBROUTINES: readConfigfiles(), InitTCP(), InitUDPbcast(), InitUDPmcast(), ProcessInfo(), DataSources(), Subscribe(), broadcast(), checkDependencies(), legacyFL(), HandleInternalEvent(), pacer(void * argument)
#
# DESCRIPTION: Sends data feed to Kyle's Mission Coordinator database
#
# ENVIRONMENT PARAMS:
#
# INPUT PARAMETERS: 
#
# OUTPUT PARAMETERS:
#
# AUTHORS: Leonard Miller, Kyle Nolan
#
# DATE: 2011.10.27
#
# CONTRIBUTING AUTHORS: Sonia Otero
#
# REVISIONS: 
# 
# 2011.10.27 - JWH: Replicated from rdr
#  
#================================================================
*/

#include "global.h"
#include <vector>
#include <functional>
#include <signal.h>
#include <pqxx/pqxx>
#include <fstream>

using namespace std;
using namespace pqxx;

#define	 	SA1		struct sockaddr
#define 	MY_PROCESS_ID		ESYSProcessKARMA

pqxx::connection * dbConnect(string db, string user, string password, string host_address, string port);
int dbCreateFlight(pqxx::connection *C);
void dbNameFlight(pqxx::connection *C);
int dbNameFlightMostRecent(pqxx::connection *C);
int dbWriteData(pqxx::connection *C, int32_t theTime, string uniqueID, float value);

void readConfigfiles(char *argv[]);
void InitTCP();
void ProcessInfo();
void DataSources();
void Subscribe(int32_t);
void checkDependencies(int, int32_t, float);
string legacyFL(float args[], string names[], int, string flid, int32_t, string precision[]);
ESYSReturn 	HandleInternalEvent( int32_t nSeverity, char *pMessage, char *pRemedy=NULL );
void catch_ctrl_c( int sig_num );
void *make_perl_thread(void *);
void kill_perl(void);

bool			bDDSConnected = false;
bool 			gbShutdown = false;
bool			bStartthread = true;
bool			allParams = true;
bool			restart = false;
int 			tcpsd, udpbcastsd, udpmcastsd1, lenTCP, lenUDPbcast, msglen, rc, max_sd, nready = 0;
int			rcvbuf = AAMPS_SOCK_RCV_BUF_SIZE;
int			KARMAcnt = 0;             			/* lines read from parameter file */
int32_t			iEpoch;
float 			Latedata[256];
char			buf[MAX_PACKET_LEN], *p;
char 			*KARMAFileName = new char[MAX_AAMPS_PATH_LEN];
char    		lineBuffer[100];
char    		*temp;
char 			*cFlightID, *cFlight;
char 			szErrorBuffer[MAX_ERROR_LEN];
string 			whitespaces = " \t\r\n\v\f";
string 			sFlightID, sFlight;
string    		KARMAnames[1024], KARMAprecision[1024], KARMAunits[1024];
string 			comma = ",";
string 			flightName = "unknown";
struct sockaddr_in	serverTCP, serverUDPbcast, serverUDPmcast1;
struct in_addr		localUDP, localUDP1;
struct hostent 		*hpTCP, *hpUDP, *hpUDP3, *lhUDP, *lhUDP2;
struct ip_mreq		group;
fd_set 			allset, rset;
socklen_t 		fromlen, fromlen3;
FILE     		*fp;     						/* file pointer, for reading param file*/

struct SrcStruct*  	srcStruct = NULL;
struct KarmaStruct*  	karmaStruct = NULL;
struct ValueStruct* 	valueStruct = NULL;

SrcStructHashT     	srcHashTable;
KarmaStructHashT 		karmaHashTable;
ValueStructHashT     	valueHashTable;
pthread_t t1; 					/* declare pacer thread */
pthread_t perl_thread;
pthread_attr_t 			attr;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
      perror("usage: ./karma <KARMA conf file path>");
      exit(-1);
    }
    
    system("top -n 1 | grep karma > temp.out");
    string result;
    ifstream infile("temp.out");

    if (getline(infile,result))
    {
        fprintf(stderr, "I am already running!\n");
        exit(1);
    }

    // Register to receive ctrl+c signal
    signal( SIGINT, catch_ctrl_c );
    signal(SIGABRT, catch_ctrl_c);//If program aborts go to assigned function "catch_ctrl_c".
    signal(SIGTERM, catch_ctrl_c);//If program terminates go to assigned function "catch_ctrl_c"
    signal(SIGTERM, catch_ctrl_c);//If program terminates go to assigned function "catch_ctrl_c"

  try
  {
    for(int i = 0; i < 256; i++)
  	Latedata[i] = NaN;

    strcpy( KARMAFileName, argv[1]);

   // Register to receive ctrl+c signal
    signal( SIGINT, catch_ctrl_c );

    struct timeval select_time;

    readConfigfiles(argv);
    //InitUDPbcast();
    //InitUDPmcast();
    

    InitTCP();
    printf("connected to DDS on AMPS_DDS_NON_NBO_PORT\n");
    //ProcessInfo();

    //connect to flight_archive database
    pqxx::connection *dbConnection = dbConnect("flight_archive", "operator1", "", "127.0.0.1", "5432");
    if (dbConnection==NULL) {
    	printf("UNABLE TO LOG FLIGHT DATA. SHUTTING DOWN.\n");
    	exit;
    }

    printf("database connection established\n");


    //open config file for writing
    //FILE * fd_conf = fopen("karma_n49.conf","w");

    while ( !gbShutdown )
    {
      // reset the list of descriptors each time as it may be modified during normal processing.  Same for the timeout.
      rset = allset;		
      select_time.tv_sec = 0;
      select_time.tv_usec = 500000;

      nready = select(max_sd+1, &rset, NULL, NULL, &select_time);

      // See if there's anything to do, or if we just timed out
     if ( nready > 0 )
     {
      // check for server messages
      if (FD_ISSET(tcpsd, &rset))
      {
         int len = 0;
         if ( (len = readn(tcpsd, &msglen, sizeof(msglen))) <= 0)
         {
          	// connection closed by client 
          	close(tcpsd);
          	FD_CLR(tcpsd, &allset);
          	bDDSConnected = false;
          	gbShutdown = true;
          	HandleInternalEvent( LOG_CRIT, "Lost DDS Connection. Shutting down.", "Check DDS Logs." );
         }
         else
         {
           memset(buf, 0, MAX_PACKET_LEN);
  	       if ( ( len = readn(tcpsd, buf, msglen)) <=0)
    	     {
          		// connection closed by client 
          		memset( szErrorBuffer, 0, MAX_ERROR_LEN );
          		sprintf( szErrorBuffer, "Socket closed by peer." );
          		HandleInternalEvent( LOG_CRIT, szErrorBuffer );
          		close(tcpsd);
          		FD_CLR(tcpsd, &allset);
    	     }
  	       else
    	     {
    	       CMessage *pMsg = new CMessage(buf, false);
    	     // Check for valid message type
             switch (pMsg->GetMessageType())
             {
                case IPCMsgTypeDDS_READY:
                {
					DataSources();
					cFlightID = (pMsg->GetTLVByTag(TLVTagFLID))->GetValue();
					sFlightID = cFlightID;
					cFlight = (pMsg->GetTLVByTag(TLVTagFLIGHT_NUMBER))->GetValue();
					sFlight = cFlight;
					sFlightID = sFlightID.append(sFlight);
					iEpoch = *((int32_t *)(pMsg->GetTLVByTag(TLVTagSTART_TIME))->GetValue());

					//over the open database connection create the table for this flight
					//cout << "attempting to name flight (sFlightID = " << sFlightID << ")" << endl;
					dbNameFlight(dbConnection);
					//cout << "attempting to create flight archive entry" << endl;
					if (restart==false)
						dbCreateFlight(dbConnection);
					//else 
					//	dbNameFlightMostRecent(dbConnection);

					//start up the perl script
				    pthread_attr_init(&attr);
				    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

				    pthread_create(&perl_thread, &attr, make_perl_thread, (void *)NULL );

				    atexit (kill_perl);

					break;
            	}

                case IPCMsgTypeDATA_SOURCE_NOTIFICATION:
                {
					int32_t nUniqueID = *((int32_t *)(pMsg->GetTLVByTag(TLVTagUNIQUE_ID))->GetValue());
					int32_t nCAM = nUniqueID >> 16;
					if (nCAM == RTDP_CAM_NUMBER) //32
					{
					char *cUniqueName = (pMsg->GetTLVByTag(TLVTagUNIQUE_NAME))->GetValue();

					//write to the config file
					char *cUnits = (pMsg->GetTLVByTag(TLVTagUNITS))->GetValue();
					stringstream ss;
					ss << (string)cUniqueName << ", " << (string)cUnits << endl;
					int found_inval1, found_inval2;
					string name = (string)cUniqueName;
					found_inval1 = name.find(".d");
  	   				found_inval2 = name.find(".c");
  	   				if ((found_inval1 == std::string::npos)&&(found_inval2 == std::string::npos)&&(name != "Time"))
						{}//fputs(ss.str().c_str(),fd_conf);

					karmaStruct = karmaHashTable.GetMember(cUniqueName);

					if (karmaStruct)
					{
					  if (TRACE_CALLS) printf("\n");
					  if (TRACE_CALLS) printf("CAM:%d DataTag:%d Unique Name:%s\n", (nUniqueID >> 16), (nUniqueID & 0x0000FFFF), cUniqueName);
					  Subscribe(nUniqueID);

					  // add entry to source hash table
					  srcStruct = new SrcStruct;

					  if(srcStruct)
					  {
					    srcStruct->uniqueID = nUniqueID;
					    srcStruct->uniqueName = cUniqueName;

					    ostringstream sin;
					    sin << nUniqueID;
					    string uniqueID = sin.str();

					    srcHashTable.AddKey(uniqueID, srcStruct);
					    if (TRACE_CALLS) printf("added %i as key for %s\n", nUniqueID, cUniqueName);
					  } // if srcStruct
					} // if(karmaStruct)
					} // end if nCAM == 32
					break;
            	}

                case IPCMsgTypeAAMPS_DATA:
                {
					// Get UniqueID, Time, Data
					int32_t nUniqueID = *((int32_t *)(pMsg->GetTLVByTag(TLVTagUNIQUE_ID))->GetValue());
					int32_t nUnlimited = *((int32_t *)(pMsg->GetTLVByTag(TLVTagUNLIMITED_INDEX))->GetValue());
					float fValue = NaN;
					char *cValue = (pMsg->GetTLVByTag(TLVTagDATA_VALUES))->GetValue();
					if( cValue != NULL)
					fValue = *(float *)cValue;

					/*if (TRACE_CALLS)*/ printf("Received Live Data- CAM:%d DataTag:%d UniqueID:%0X Time:%d\n", (nUniqueID >> 16), (nUniqueID & 0x0000FFFF), nUniqueID, nUnlimited);
					//if (TRACE_CALLS) printf("fValue:%f DataTag:%d UniqueID:%0X\n\n", fValue, (nUniqueID & 0x0000FFFF), nUniqueID);

					//do we really need this?
					//checkDependencies(nUniqueID, nUnlimited, fValue);

					//write this AAMPS data to the flight database over the open postgres connection
					stringstream ss;
					ss << nUniqueID;
					dbWriteData(dbConnection, nUnlimited, ss.str(), fValue);
					break;
            	}

                default:
                {   
	                break;
            	}
             }

    	       delete pMsg;
    	     } // else
         } // else
       } // if (FD_ISSET)
      } //if ( nready > 0 )
    } //	while ( !gbShutdown )
    

    // clean up the tables and free allocated memory
    srcHashTable.RemoveAllKey(true);
    karmaHashTable.RemoveAllKey(true);
    //disconnect from the PostgreSQL database
    if (dbConnection != NULL) dbConnection->disconnect();

  }//end try
  catch (exception& e)
  {
  	memset( szErrorBuffer, 0, MAX_ERROR_LEN );
  	sprintf( szErrorBuffer, "main() Standard exception: %s\n", e.what() );
  	HandleInternalEvent( LOG_CRIT, szErrorBuffer );
  }
    exit(0);
} // main()


//A subroutine to set up a connection to a postgresql database
pqxx::connection * dbConnect(string db, string user, string password, string host_address, string port)
{
		pqxx::connection *C = NULL;
		stringstream ss;
		//organize arguments into connection statement
		ss << "dbname=" << db << " user=" << user << " password=" << password << " hostaddr=" << host_address << " port=" << port;
	try
	{
		//allocate memory for a connection and attempt to connect
		C = new pqxx::connection(ss.str());
		if (C->is_open()) {
			cout << "Connected to database: " << C->dbname() << endl;
			return C;
		} else {
			 cout << "Failed to connect to database: " << db << endl;
			 return NULL;
		}
	}
	catch (const std::exception &e)
	{
		cerr << e.what() << std::endl;
		return NULL;
	}
	return NULL;
}

//A function to dynamically generate a table for this flight in the flight archive database, dependent upon what parameters you have subscribed for
int dbCreateFlight(pqxx::connection *C)
{
	stringstream statement;
	string name;
	try {
		cout << "Creating Flight table in database: " << C->dbname() << endl;
		//compose the SQL statement
		statement << "CREATE TABLE " << flightName << "(" << "TIME REAL,";
		for (int i=0; i<KARMAcnt; i++)
		{
			name = KARMAnames[i];
			//replace any .'s or -'s in the parameter name, which won't work as SQL column names
			for (int j=0; j<name.length(); j++)
			{
				if (name[j] == '.')
					name[j] = '_';
				if (name[j] == '-')
					name[j] = '_';
			}
			statement << name << " REAL";
			if (i<KARMAcnt-1)
				statement << ",";
			else
				statement << ");";
		}

		cout << "executing SQL Statement: " << statement.str() << endl;

		//create a transactional object to do work over this connection
		work W(*C);

		//use it to execute the SQL statement
		W.exec(statement.str());
		//commit changes to the database
		W.commit();
		cout << "Table created successfully: " << flightName << endl;
		return 1;
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return 0;
	}
	return 0;
}

int dbNameFlightMostRecent(pqxx::connection *C)
{
	pqxx::work W(*C);
	stringstream statement;
	string retval;

	statement << "select relname from pg_class where relname LIKE '%fl_%' order by ctid desc limit 1;";
	pqxx::result R = W.exec(statement.str());
	retval = (string)(R[0][0].c_str());
	cout << "\n------\nMOST RECENT TABLE: " << retval << "\n------\n\n";
	flightName = retval;
	return 0;
}

void dbNameFlight(pqxx::connection *C)
{
	int append_number = 0;
	bool append = false, unique = false;
	string capsDemoted, retval;

	//create an object to do work
	pqxx::work W(*C);

	flightName = sFlightID;
	if (flightName.length() < 2)
		flightName = "unknown_flight";

	//search the database for this flight name and append numbers until there is no table entry for the flight
	for (int i=0; i<255; i++)
	{
		stringstream ssname, statement, validator;

		ssname << "fl_" << flightName;

		if (append)
			ssname << "_" << append_number;
		//cout << "composing sql statement" << endl;
		capsDemoted = ssname.str();
		transform(capsDemoted.begin(), capsDemoted.end(), capsDemoted.begin(), ::tolower);

		statement << "select count(*) from pg_class where relname='" << capsDemoted << "'";

		//cout << "SQL statement is: " << statement.str() << endl;
		//execute the SQL statement and obtain result
		pqxx::result R = W.exec(statement.str());
		retval = (string)(R[0][0].c_str());

		//cout << retval << endl;

		//if ((retval == "0")||(restart==true))
		{
			if (retval != "0")
			{
				restart = true;
				cout << "\nUSING POSTGRESQL LOGGING MODE: APPEND\n";
			} else {
				cout << "\nUSING POSTGRESQL LOGGING MODE: NEW\n";
			}
			//is unique
			unique = true;
			flightName = capsDemoted;
			cout << "flight named: " << flightName << endl;
			return;
		}
		//start appending numbers if it was not unique
		append_number++;
		append = true;
	}
	//no database changes need to be committed
}

int dbWriteData(pqxx::connection *C, int32_t theTime, string uniqueID, float value)
{
	string uniqueName, retval;
	struct SrcStruct* retStruct = NULL;
	stringstream statement, statement2, statement3;

	//create transactional objects to do work
	pqxx::work W(*C);

	//ensure there is a record to update for this time index
	statement2 << "select count(*) from " << flightName << " where time=" << theTime << ";";
	pqxx::result R = W.exec(statement2.str());
	retval = (string)(R[0][0].c_str());
	if (retval == "0")
	{
		//make a record for this time index
		statement3 << "INSERT INTO " << flightName << " (time) VALUES (" << theTime << ");";
		W.exec(statement3.str());
		//cout << "record generated for time index: " << theTime << endl;
	}

	//get this parameter ID to match the right database column
	retStruct = srcHashTable.GetMember(uniqueID);
	if (retStruct==NULL) return -1;
	uniqueName = retStruct->uniqueName;

	//replace periods with underscores to match SQL requirements
	for (int j=0; j<uniqueName.length(); j++)
	{
		if (uniqueName[j] == '.')
			uniqueName[j] = '_';
		if (uniqueName[j] == '-')
			uniqueName[j] = '_';
	}
	//cout << "uniqueName is: " << uniqueName << endl;

	if (isnan(value))
		value=NULL;

	//construct the SQL insertion statement
	statement << "UPDATE " << flightName << " set " << uniqueName << "=" << value << " where time=" << theTime << ";";
	cout << "\tudating SQL record: " << statement.str() << endl << endl;

	//perform the insertion
	W.exec(statement.str());
	//cout << "SQL Execution: " << statement.str();

	//commit all work performed on the database
	W.commit();
	return 0;
}


void readConfigfiles(char *argv[])
{
  try
  {
  	int found1, found2, found3;
  	string line;

  	for(int i = 0; i < 512; i++)
  	{
  		KARMAprecision[i] = "%.2f";
  	}

  	  fp = fopen (KARMAFileName,"r");

  	  if (fp == NULL)
  	  {
        memset( szErrorBuffer, 0, MAX_ERROR_LEN );
  	    sprintf( szErrorBuffer, "error on open of %s!!\n", KARMAFileName );
  	    HandleInternalEvent( LOG_CRIT, szErrorBuffer );
  	    exit(1);
  	  }
  	  //printf("\nargv[2] = %s\n",argv[2]);
  	  /*if (!(strcmp(argv[2],"new")))
  	  {
  	  	printf("\nPostgreSQL flight_archive logging mode: New Table\n\n");
  	  	restart = false;
  	  } else if (!(strcmp(argv[2],"append")))
  	  {
  	  	printf("\nPostgreSQL flight_archive logging mode: Append to Most Recent Table\n\n");
  	  	restart = true;
  	  } else
  	  {
  	  	printf("\nUNKNOWN POSTGRESQL LOGGING MODE: using New Table by default\n\n");
  	  }*/

  	  while((temp = fgets(lineBuffer,sizeof(lineBuffer),fp)))
  	  {
  		/*if((string)temp == "!restart")
  	     {
  	     	restart = true;
  	     	printf("method of operation: restart\n");
  	     	cout << "restart is " << restart << endl;
  	     	break;
  	     } else if ((string)temp == "!new")
  	     {
  	     	restart = false;
  	     	printf("method of operation: new\n");
  	     	break;
  	     } else */if (temp[0]=='!')
  	     {
  	     	printf("! detected. ending readConfigfiles\n");
  	     	break;
  	     }

  	   	line = temp;
  	   	found1 = line.find_last_of(whitespaces);
  	   	found2 = line.find_first_of(",");

    	KARMAunits[KARMAcnt] = line.substr(found2+2,found1-found2-2);
    	KARMAnames[KARMAcnt] = line.substr(0,found2);

    	//cout << "KARMAnames[" << KARMAcnt << "] = " << KARMAnames[KARMAcnt] << endl;

  	    found3 = line.find_first_of("%");
  	    if(found3 > 1)
  	    	KARMAprecision[KARMAcnt] = line.substr(found3, found1-found3);

  		if (TRACE_CALLS) printf("name:%s\tunits: %s\tprecision:%s\n", KARMAnames[KARMAcnt].c_str(), KARMAunits[KARMAcnt].c_str(), KARMAprecision[KARMAcnt].c_str());

  	    karmaStruct = karmaHashTable.GetMember(KARMAnames[KARMAcnt]);
  	    if(karmaStruct)
  	    {
  	    	if (TRACE_CALLS) printf("duplicate found\n");
  	    	return;
  	    }
  	    else
  	    {
  	     	karmaStruct = new KarmaStruct;
  	      	karmaStruct->order = KARMAcnt;
  	       	karmaHashTable.AddKey(KARMAnames[KARMAcnt], karmaStruct);
  	       	if (TRACE_CALLS) printf("KARMA Unique Name %s added as key.\n", KARMAnames[KARMAcnt].c_str());
  	        KARMAcnt++;
  	    }
  	  } //end while loop

  	  fclose(fp);    /* Close .prm file */
  }//end try
  catch (exception& e)
  {
  	memset( szErrorBuffer, 0, MAX_ERROR_LEN );
  	sprintf( szErrorBuffer, "readConfigfiles() Standard exception: %s\n", e.what() );
  	HandleInternalEvent( LOG_CRIT, szErrorBuffer );
  }

} // readConfigfiles()

void InitTCP()
{
  tcpsd = socket(AF_INET, SOCK_STREAM, 0);
  if (tcpsd < 0)
  {
	memset( szErrorBuffer, 0, MAX_ERROR_LEN );
	sprintf( szErrorBuffer, "socket allocation failed: InitTCP() rc = %i", tcpsd );
	HandleInternalEvent( LOG_CRIT, szErrorBuffer );
	exit(1);
  }

  memset(&serverTCP, 0, sizeof(serverTCP));
  serverTCP.sin_family = AF_INET;

  serverTCP.sin_port = htons(AAMPS_DDS_NON_NBO_PORT);

  hpTCP = gethostbyname("mds");
  if (hpTCP == 0)
  {
	memset( szErrorBuffer, 0, MAX_ERROR_LEN );
	sprintf( szErrorBuffer, "couldn't resolve MDS IP: InitTCP()" );
	HandleInternalEvent( LOG_CRIT, szErrorBuffer );	
	exit(1);
  }
  bcopy((char *)hpTCP->h_addr, (char *)&serverTCP.sin_addr, hpTCP->h_length);

  rc = connect(tcpsd, (SA1 *) &serverTCP, sizeof(serverTCP));
  if (rc < 0)
  {
	memset( szErrorBuffer, 0, MAX_ERROR_LEN );
	printf("rc = %d\n",rc);
	sprintf( szErrorBuffer, "connect failed: InitTCP()" );
	HandleInternalEvent( LOG_CRIT, szErrorBuffer );	
    exit(1);
  }

  bDDSConnected = true;
  printf("connection established\n");
  FD_ZERO(&allset);
  FD_SET(tcpsd, &allset);
  max_sd = tcpsd;
  rc = SendProcessInfo( MY_PROCESS_ID, tcpsd );
  printf("process info sent\n");
  if ( rc != ESYSReturnSUCCESS )
  {
	rc = HandleInternalEvent( LOG_CRIT, "Error connecting to DDS.", 
								"Check common library sync." );
	gbShutdown = true;
  } 

} // InitTCP()

void ProcessInfo()
{
  CMessage *pMsg = new CMessage(IPCMsgTypePROCESS_INFO);
  pMsg->AddTLV(TLVTagPROCESS_ID, ESYSProcessKARMA);
  pMsg->AddTLV(TLVTagSVN_LIB_REV, strlen(GetLibraryVersion()), GetLibraryVersion());
  lenTCP = pMsg->PackToSend(buf, false);
  writen(tcpsd, buf, lenTCP);
  delete pMsg;
} // ProcessInfo()

void DataSources()
{
  CMessage *pMsg = new CMessage(IPCMsgTypeDATA_FEED_SUBSCRIBE);
  pMsg->AddTLV(TLVTagLIVE_DATA_REQUEST, SUBSCRIBE);
  pMsg->AddTLV(TLVTagDSN_FILTER, DSN_FILTER_PROCESSED_DATA);
  pMsg->AddTLV(TLVTagUNIQUE_ID, MSG_SUBSCRIPTION_PREFIX+IPCMsgTypeDATA_SOURCE_NOTIFICATION);
  lenTCP = pMsg->PackToSend(buf, false);
  writen(tcpsd, buf, lenTCP);
  delete pMsg;
} // DataSources()

void Subscribe(int32_t nUniqueID)
{
if (TRACE_CALLS) printf("Subscribing for %d", nUniqueID);
if (TRACE_CALLS) printf("\n");
  CMessage *pMsg = new CMessage(IPCMsgTypeDATA_FEED_SUBSCRIBE);
  pMsg->AddTLV(TLVTagUNIQUE_ID, nUniqueID);
  pMsg->AddTLV(TLVTagLIVE_DATA_REQUEST, 1);
  lenTCP = pMsg->PackToSend(buf, false);
  writen(tcpsd, buf, lenTCP);
  delete pMsg;
} // Subscribe()

void checkDependencies(int id, int32_t time, float fValue)
{
try
{
  // get the members
  ostringstream sin;
  sin << id;
  string val = sin.str();
  srcStruct = srcHashTable.GetMember(val);
  string cUniqueName = srcStruct->uniqueName;

  karmaStruct = karmaHashTable.GetMember(cUniqueName);
  if(karmaStruct)
  {
     Latedata[karmaStruct->order] = fValue;
     ostringstream sin2;
     sin2 << time;
     string Time = sin2.str();
     valueStruct = valueHashTable.GetMember(Time);

	if(!valueStruct)
	{
	   // add entry to source hash table
	   valueStruct = new ValueStruct;
	   valueHashTable.AddKey(Time, valueStruct);
	   for(int i = 0; i < KARMAcnt; i++)
	  	valueStruct->value[i] = NaN;		
	}
  	valueStruct->value[karmaStruct->order] = fValue;
    }//end if karmaStruct
  }//end try
  catch (exception& e)
  {
	memset( szErrorBuffer, 0, MAX_ERROR_LEN );
	sprintf( szErrorBuffer, "checkDependencies() Standard exception: %s\n", e.what() );
	HandleInternalEvent( LOG_CRIT, szErrorBuffer );
  }
} // checkDependencies()

//////////////////////////////////////////////////////////////////////
//++
//
// Function: HandleInternalEvent
//
// Description:
//	  Common processing for internal errors
//
// Inputs:
//    nSeverity - Error Level
//    pErrorMsg - Error message text
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
//////////////////////////////////////////////////////////////////////

ESYSReturn HandleInternalEvent( int32_t nSeverity, char *pErrorMsg, char *pRemedy )
{
	if (TRACE_CALLS) printf("%s::HandleInternalEvent\n", 
							ESYSProcessText[MY_PROCESS_ID].c_str());
	
	char buf[MAX_PACKET_LEN];
	int	 nBytes = 0;
	
	ESYSReturn eRtn = ESYSReturnSUCCESS;
	
	if ( bDDSConnected )
	{
		// Build an error message and send it to subscribed processes
		CMessage *pMsg = new CMessage( IPCMsgTypeERROR );
		pMsg->AddTLV( TLVTagERROR_SEVERITY, nSeverity );
		pMsg->AddTLV( TLVTagMESSAGE, strlen(pErrorMsg), pErrorMsg );
		pMsg->AddTLV( TLVTagERROR_COMPONENT, 
					  ESYSProcessText[MY_PROCESS_ID].length(),
					  (char *)ESYSProcessText[MY_PROCESS_ID].c_str() );
		if ( pRemedy != NULL )
		{
			pMsg->AddTLV( TLVTagERROR_REMEDY, strlen(pRemedy), pRemedy );
		}
		
		memset( buf, 0, MAX_PACKET_LEN );
		int32_t nXmitSize = pMsg->PackToSend( buf, false );
	
		nBytes = writen( tcpsd, buf, nXmitSize );

		delete pMsg;
	}

	// check for errors 
	// if we can't send to DDS for writing to syslog, do it on our own
	if ( nBytes < 0 || !bDDSConnected )
	{
		WriteToSysLog( nSeverity, pErrorMsg, 
					   (char *)ESYSProcessText[MY_PROCESS_ID].c_str(), pRemedy );
		eRtn = ESYSReturnSOCKET_ERROR;
	}
	else
	{
		if (TRACE_CALLS) printf( "%s\n", pErrorMsg );
	}

	return( eRtn );
} //HandleInternalEvent()

//////////////////////////////////////////////////////////////////////
//++
//
// Function: catch_ctrl_c
//
// Description:
// 	  Catches SIGINT.  
//    This allows us to shut down in an orderly fashion, close files, etc.
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

void catch_ctrl_c( int sig_num )
{
	// Got ctrl+c signal, shut down
	HandleInternalEvent( LOG_NOTICE, "Received signal. Shutting down.");
	gbShutdown = true;
	return;
}

void *make_perl_thread(void *)
{
    printf("executing system call: perl ../../server/regen_kml.pl");
    system("perl -w ../../server/regen_kml.pl &> ../../perl_log.log");
}

void kill_perl(void)
{
	printf("killing regen_kml.pl\n");
    system("killall perl");
    pthread_exit(NULL);
}