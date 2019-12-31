/* ---------------------------------------------------------------------------------------
File:  mpi.h
Date:  July 19, 2002
Desc:  Header file for the MARX Programming Interface for MARX hardware
Tools: MSVC++ 6.0 SP3

HISTORY:

VER     DATE      WHO      DETAIL
-----  ---------- ------   ---------------------------------------------------------------
0.00   31MAR1999  001AK    initial version 
0.01   19APR1999  001AK    modified functions and error codes
1.00   26APR1999  001MJ    introduced cboxcall.h and serial number
1.01   29APR1999  001MJ,AK split SetChannel into SetLocalChannel/RemoteChannel
1.02   28JUN1999  001AK    instead WORD, LONG - INT32; ERACE_ALL_PW -> ERASE_ALL_PW
1.02   29JUN1999  001MJ    every define starts with MPI_
1.03   01JUL1999  001AK,MJ modification error codes and instead long, WORD - INT32.
1.04   01JUL1999  001AK,MJ instead SHORT -> INT16, CHAR -> STRING40 (if no length)
1.05   08JUL1999  001AK    modified defines for passwords
1.06   22JUL1999  001MK    added error code Bit 18 for missing library
1.06   26JUL1999  001AK    removed define for password DeveloperID
1.07   03AUG1999  001AK    add function MPI_WhatIsAvailable()
1.08   29NOV1999  001AK    added support for PC/SC compatible smart card readers
1.09   02JAN2000  001AK    MARX hardware and port types are passed as string now
1.10   14FEB2000  001AK    renamed cbu.h to mpi.h
                           first parameter in MPI_Encrypt/Decrypt() is STRING now
1.11   28FEB2000  001AK    changed MARX hardware type "560" to "CBN"
                           added "PASSWORD_ID5" ... "PASSWORD_ID8"
1.12   30MAY2000  001AK    Added network part to the MPI
1.13   05JUN2000  001AK    Added parameter for port to the MPI_WhatIsAvailable()
1.14   18AUG2000  RE,SM    Corrected for integration with network part:
                              1. In comments: added "*" constant for any server names; 
                              2. Added #define MPI_MAX_SERVER_NAME 96;
                              3. Added new error codes: 
                                 MPI_ERR_NOT_LOGGED_IN, MPI_ERR_TOO_MANY_USERS, MPI_ERR_SRV_INT_ERROR 
                                 MPI_ERR_NEED_PASSWORD, MPI_ERR_NOT_CONNECTED,  MPI_ERR_CONNECTED    
                                 MPI_ERR_DISCONNECTED,  MPI_ERR_NO_SERVER,      MPI_ERR_NO_TRANSPORT  
                                 MPI_ERR_OUT_OF_MEMORY, MPI_ERR_BAD_REQUEST,    MPI_ERR_NET_TIME_OUT.
1.15   22AUG2000 SM        1. Added new error code: MPI_ERR_INVALID_LC.
                           2. Added new function: MPI_GetLicenseInfo().
1.16   23AUG2000 SM        1. Added new error code: MPI_ERR_OUT_OF_RANGE.
1.17   25AUG2000 SM        1. Added new error code: MPI_ERR_UNLIMITED_USERS.
                           2. Changed: "VERSA" -> "CBV". 
1.18   28AUG2000 SM        1. Added in comments: network parameters:
                                 "TIMEOUT", "CLIENT_UDP_PORT", "SERVER_UDP_PORT".
1.19   29AUG2000 SM        1. Changed: "AUTOFIND" -> "AUTONET".
                           2. Added: "AUTONET EXCLUDE NETBIOS" string constant. 
                           3. Added: "SERVER" string constant in network parameters.
                           4. Added functions: MPI_SetNetworkParameter(), MPI_GetNetworkParameter().
1.20   05SEP2000 SM        1. Added in comments the "PROTOCOL" network parameter.
1.21   26SEP2000 SM        1. Added error codes:
                                 MPI_ERR_CLOSE_HANDLE,  MPI_ERR_GET_HANDLE, MPI_ERR_IOCTL_RESULT
                                 MPI_ERR_EXCEPTION_DRV, MPI_ERR_SYSTEM_UNKNOWN.
1.22   06OCT2000 SM        1. Added error codes:
                                 MPI_ERR_INVALID_LC_ID, MPI_ERR_INVALID_LC_RAM, MPI_ERR_INVALID_LC_DCR.
1.23   23OCT2000 SM        1. Added error code: MPI_ERR_BAD_RESPONSE.
1.24   30OCT2000 SM        1. Added CBU support. 
1.25   18JAN2001 SM        1. MPI_GetSerialNr() function was enabled.  
1.26   27FEB2001 SM        1. "CBU" (USB Entrix Key) renamed into "UEK".
1.27   03MAR2001 SM        1. Added CBU (CB USB) support. 
1.28   04MAR2001 SM        1. Added new function MPI_SubmitPasswordEx().
1.29   25MAR2001 SM        1. Added new functions:
                                 MPI_SubmitEncryptionKey(), MPI_EraseEncryptionKey()
                                 MPI_EncryptEx() / MPI_DecryptEx(), MPI_GetRandomSequence().
                           2. Added in comments: crypt parameters:
                                 new algorithm types: "RIJNDAEL_ALGORITHM", "RSA_ALGORITHM"; 
                                 encryption key types:
                                 "IDEA_KEY", "MARX_KEY",   "IDEA_EXTERN_KEY", "MARX_EXTERN_KEY"
                                 "RIJNDAEL_FIXED_KEY",     "RIJNDAEL_PRIVATE_KEY"
                                 "RIJNDAEL_SESSION_KEY",   "RIJNDAEL_EXTERN_KEY"
                                 "RSA_FIXED_PRIVATE_KEY",  "RSA_FIXED_PUBLIC_KEY"
                                 "RSA_CBU_PRIVATE_KEY",    "RSA_CBU_PUBLIC_KEY"
                                 "RSA_EXTERN_PRIVATE_KEY", "RSA_EXTERN_PUBLIC_KEY".
1.30   16APR2001 SM        1. Added error code: MPI_ERR_NEED_ENC_KEY.
1.31   18APR2001 SM        1. Added error codes: MPI_ERR_INVALID_ENC_KEY, MPI_ERR_BUFFER_TOO_SHORT.
                           2. Added new constants: 
                                 MPI_MAX_PSW_SIZE, MPI_MAX_ERR_MSG_SIZE
                                 MPI_MAX_IDEA_KEY_SIZE, MPI_MAX_MARX_KEY_SIZE      
                                 MPI_MAX_RIJNDAEL_KEY_SIZE, MPI_RIJNDAEL_IV_OFFSET     
                                 MPI_MAX_RSA_KEY_SIZE, MPI_CBU_RSA_KEY_SIZE, MPI_MIN_RSA_KEY_SIZE.
                           3. Added new function: MPI_GenerateKeyPairRSA(). 
                           4. Added new constants: 
                                 MPI_RSA_KEY_MODULUS_2048, MPI_RSA_KEY_MODULUS_1024, MPI_RSA_KEY_MODULUS_512.
                           5. Added macros to calculate size of RSA Key buffer in bytes 
                              from modulus length in bits: MPI_RSA_KEY_BUF_LEN(x).    
1.32   27APR2001 SM        1. Added macros to calculate size of output buffer in bytes 
                              for RSA encryption (for 8-multiple keys only): MPI_RSA_OUT_BUF_LEN(x,y).    
1.33   04MAY2001 SM        1. Added new function: MPI_GetHash(). 
                           2. Added new constant: "MD4_ALGORITHM", MPI_MD4_HASH_BUF_LEN.
1.34   02AUG2001 SM        1. Added new constant: MPI_BASIC_MARX_IDEA_KEYS_SIZE.  
1.35   20AUG2001 SM        1. MPI Data Objects interface implementation. 
1.36   20SEP2001 SM        1. Added constant MPI_UNLIMITED_VALUE.
1.37   24SEP2001 SM        1. MPI_ProcessDataObject() function interface changed
                              to work with DO's in PC memory.
1.38   10OCT2001 SM        1. Added new function: MPI_GetBoxInfo(). 
                           2. Added constants: MPI_CBU_VERSA, MPI_CBU_XS, MPI_CBU_XL,
                                               MPI_CBU_OEMXS, MPI_CBU_OEMXL.
1.39   19JUL2002 SM        1. Added new function: MPI_GetManufacturerId().
                           2. Added new network parameter: "KEEP_ALIVE_MSG_INT".
                           3. Added new encryption key type: "IDEA2_KEY".
                           4. Added new constants: "RIJNDAEL_PRIVATE_IV", "RIJNDAEL_SESSION_IV",
                                                   "RIJNDAEL_EXTERN_IV".
                           5. Added new constant: MPI_MAX_RIJNDAEL_IV_SIZE.
------------------------------------------------------------------------------------------
NOTE:
This header file in conjunction with cboxcall.h allows the cross-platform
integration of MARX hardware using C/C++ compilers. Please report any problems
with this header.
*/
#ifndef MPI_INCLUDED
#define MPI_INCLUDED
 
#include "cboxcall.h"      /* calling conventions and Marx defines */

#define MPI_VERSION 0x0036 /* version of this header file */


/*
==========================================================================================
                                  MPI String Constants 
==========================================================================================


 -----------------------------------------------------------------------
  The following strings are reserved to control the MARX hardware type:
 -----------------------------------------------------------------------

    "AUTO"                    any CRYPTO-BOX  
    "CBV"                     CRYPTO-BOX Versa  
    "CBN"                     CRYPTO-BOX 560  
    "CB7"                     CRYPTO-BOX CB7  
    "SX7"                     SX7 smart card    (CB7 compatible)  
    "SX7VT"                   Virtual Token     (CB7 compatible)   
    "CB10"                    CRYPTO-BOX CB10   (CB7 compatible) 
    "CBS"                     CRYPTO-BOX Serial  
    "UEK"                     USB Entrix Key 
    "CBU"                     CRYPTO-BOX USB
    
    "VERSA+COMPATIBLE"        CRYPTO-BOX Versa  including compatible types
    "560+COMPATIBLE"          CRYPTO-BOX 560    including compatible types
    "CB7+COMPATIBLE"          CRYPTO-BOX CB7    including compatible types
    "SX7+COMPATIBLE"          SX7 smart card    including compatible types
    "SX7VT+COMPATIBLE"        Virtual Token     including compatible types
    "CB10+COMPATIBLE"         CRYPTO-BOX CB10   including compatible types
    "CBS+COMPATIBLE"          CRYPTO-BOX Serial including compatible types

  
 --------------------------------------------------------------------------
  The following strings are reserved to control the port that is accessed:
 --------------------------------------------------------------------------

  Note:
  MARX hardware can be attached to LPT, COM, PCMCIA and USB ports. This part 
  of the interface leaves room for all existing and future ports that are/will
  be supported by any of the MARX hardware solutions.

    Local channels:
    ---------------
    "AUTO"     automatic search order: PCSC, LPT, COM, USB, PCMCIA, ADB
    "LPT"      automatic search on all LPT ports 
    "LPT1"     parallel port #1 addr: 0x03bc  
    "LPT2"     parallel port #2 addr: 0x0378  
    "LPT3"     parallel port #3 addr: 0x0278  
    "COM"      automatic search on all COM ports 
    "COM1"     serial port #1  
    "COM2"     serial port #2  
    "COM3"     serial port #3  
    "USB"      automatic search on all USB ports 
    "PCMCIA"   automatic search on all PCMCIA ports  
    "ADB"      automatic search on Macintosh ADB port
    "PCSC"     for all PC/SC compatible Smart Card readers
    
    Remote channels:
    ----------------
    "AUTONET"                 use whatever is available  
    "AUTONET EXCLUDE NETBIOS" use whatever is available exclude NetBIOS
    "SPX_IPX"                 SPX/IPX protocol  
    "NETBIOS"                 NetBIOS protocol  
    "TCP_IP"                  TCP/IP protocol  
    
    Server names for remote CRYPTO-BOX channels: "*" - for any names 
    --------------------------------------------


 ----------------------------------------------------------------------------
  The following strings are reserved to identify passwords and access codes:
 ----------------------------------------------------------------------------

    ID's passwords:
    ---------------
    "PASSWORD_ID1", "PASSWORD_ID2", "PASSWORD_ID3", "PASSWORD_ID4"
    "PASSWORD_ID5", "PASSWORD_ID6", "PASSWORD_ID7", "PASSWORD_ID8"
    
    Memory passwords: "PASSWORD_MEM1", "PASSWORD_MEM2"
    -----------------
    
    Access modes:     "READ", "WRITE"
    -------------
    
    Erase all passwords: "ERASE_ALL"
    --------------------


 -----------------------------------------------------------------------
  The following strings are reserved to identify crypt/hash parameters:
 -----------------------------------------------------------------------
   
    Encrypt/Decrypt algorithms:
    ---------------------------
    "IDEA_ALGORITHM",     "MARX_ALGORITHM"
    "RIJNDAEL_ALGORITHM", "RSA_ALGORITHM" 

    Encryption key types:
    ---------------------
    "IDEA_KEY", "MARX_KEY", "IDEA_EXTERN_KEY", "MARX_EXTERN_KEY"
    "IDEA2_KEY"

    "RIJNDAEL_FIXED_KEY",   "RIJNDAEL_PRIVATE_KEY" 
    "RIJNDAEL_SESSION_KEY", "RIJNDAEL_EXTERN_KEY"
    "RIJNDAEL_PRIVATE_IV",  "RIJNDAEL_SESSION_IV"
    "RIJNDAEL_EXTERN_IV"

    "RSA_FIXED_PRIVATE_KEY",  "RSA_FIXED_PUBLIC_KEY"
    "RSA_CBU_PRIVATE_KEY",    "RSA_CBU_PUBLIC_KEY"
    "RSA_EXTERN_PRIVATE_KEY", "RSA_EXTERN_PUBLIC_KEY"

    Erase all keys: "ERASE_ALL"
    ---------------------------

    Hash algorithms:
    ----------------
    "MD4_ALGORITHM"

 --------------------------------------------------------------------
  The following strings are reserved to identify network parameters:
 --------------------------------------------------------------------

    Server: "SERVER"
    ----------------
    Description: Specifies server name(s) or address(es).
    Example:               
    MPI_SetNetworkParameter("SERVER","AURORA");
    Possible server identification ways:
    "*"                                - all servers;                              
    "10.10.10.10"                      - specified by IP address;                   
    "AURORA"                           - specified by name;                         
    "10.10.10.10 267.11.13.197 AURORA" - list of servers separated by spaces.

    Network protocol: "PROTOCOL"
    ----------------------------
    Example:
    char NetProtocol[32];               
    MPI_GetNetworkParameter("PROTOCOL",NetProtocol,dwIdentifier);

    Timeout: "TIMEOUT"
    ------------------
    Description: Network timeout in milliseconds. 
    Maximum time that client is waiting for answer from server. 
    Default value: 2 seconds.
    Example: MPI_SetNetworkParameter("TIMEOUT","5000"); // set to 5 seconds

    Client UDP port for receiving: "CLIENT_UDP_PORT"
    ------------------------------------------------
    Default value: 8765.
    Example: MPI_SetNetworkParameter("CLIENT_UDP_PORT","8000"); // set to 8000 port

    Server UDP port for listening: "SERVER_UDP_PORT"
    ------------------------------------------------
    Default value: 8766.
    Note: this parameter is set for client to specify which server port 
          will be request. 
    Example: MPI_SetNetworkParameter("SERVER_UDP_PORT","8001"); // set to 8001 port

    Keep-Alive messages interval: "KEEP_ALIVE_MSG_INT"
    --------------------------------------------------
    Description: Keep-Alive messages interval in seconds. 
    Interval between messages which inform the server that client is alive.
    Default value: 1 minute.
    Example: MPI_SetNetworkParameter("KEEP_ALIVE_MSG_INT","30"); // set to 30 seconds

==========================================================================================
*/

#define SECADAY 86400

/* MARX hardware type */ 
#define MPI_CBU_VERSA 1
#define MPI_CBU_XS    2
#define MPI_CBU_XL    3
#define MPI_CBU_OEMXS 4
#define MPI_CBU_OEMXL 5
#define MPI_CBN_BOX 100
#define MPI_CBV_BOX 101
#define MPI_CB7_BOX 102
#define MPI_SX7_BOX 103
#define MPI_CBS_BOX 104
#define MPI_UEK_BOX 105
 
#define MPI_MEMORY_NR1              0x1      /* number of memory to Read/Write */
#define MPI_MEMORY_NR2              0x2      /* number of memory to Read/Write */

#define MPI_GUIERRORSTYLE           0x1      /* show error code using message box */
#define MPI_CONSOLERRORSTYLE        0x2      /* show error code using console */

#define MPI_MAX_ERR_MSG_SIZE         80      /* maximum size of error message */

#define MPI_MAX_SERVER_NAME          96      /* maximum size of a server name for */
                                             /* remote CRYPTO-BOX channel */

#define MPI_MAX_PSW_SIZE             16      /* CBU maximum password size (in bytes) */
                                             /* ("PASSWORD_ID1" and "PASSWORD_MASTER" only) */

#define MPI_RSA_KEY_MODULUS_2048   2048      /* 2048 bits for RSA modulus */
#define MPI_RSA_KEY_MODULUS_1024   1024      /* 1024 bits for RSA modulus */
#define MPI_RSA_KEY_MODULUS_512     512      /*  512 bits for RSA modulus */

#define MPI_RSA_KEY_BUF_LEN(x) (2+(((x)/8)*2)) /* macros to calculate size of RSA Key buffer in bytes */ 
                                             /* from modulus length in bits */ 

#define MPI_RSA_OUT_BUF_LEN(x,y) ((((x)/(((y)/8)-1))+1)*((y)/8)) 
                                             /* macros to calculate size of RSA encrypted output buffer */
                                             /* in bytes. For 8-multiple keys only (512, 1024, 2048...). */
                                             /* x - length of input buffer in bytes */
                                             /* y - modulus length in bits */   

#define MPI_BASIC_MARX_IDEA_KEYS_SIZE 2      /* size (in bytes) of "IDEA_KEY" / "MARX_KEY" encryption keys */

#define MPI_MAX_IDEA_KEY_SIZE        16      /* maximum size (in bytes) of encryption key for IDEA */
#define MPI_MAX_MARX_KEY_SIZE         4      /* maximum size (in bytes) of encryption key for MARX */

#define MPI_MAX_RIJNDAEL_KEY_SIZE    32      /* maximum size (in bytes) of encryption key for RIJNDAEL */
                                             /* (key + IV = 16 + 16) */ 
#define MPI_RIJNDAEL_IV_OFFSET       16
#define MPI_MAX_RIJNDAEL_IV_SIZE     16
#define MPI_MAX_RSA_KEY_SIZE        514      /* maximum size (in bytes) of encryption key for RSA */
                                             /* (extern key only) */ 
                                             /* (length in bits of modulus + modulus + exponent = 2 + 256 + 256) */
#define MPI_CBU_RSA_KEY_SIZE        130      /* size (in bytes) of encryption key for RSA */
                                             /* (CBU hardware key) */ 
                                             /* (length in bits of modulus + modulus + exponent = 2 + 64 + 64) */
#define MPI_MIN_RSA_KEY_SIZE        130      /* minimum size (in bytes) of encryption key for RSA */
                                             /* (length in bits of modulus + modulus + exponent = 2 + 64 + 64) */

#define MPI_MD4_HASH_BUF_LEN         32      /* length in bytes of MD4-algorithm output hash-buffer */

#define MPI_UNLIMITED_VALUE          0xFFFFFFFF

/* MPI Data Object types */
#define MPI_DO_EXPIRATION_DATE 1
#define MPI_DO_NUMBER_OF_DAYS  3
#define MPI_DO_TIME_ALLOWED    4
#define MPI_DO_USAGE_COUNTER   5
#define MPI_DO_MEMORY          9 

/* MPI Data Object operations */
#define MPI_DO_OP_SET          1
#define MPI_DO_OP_GET          2
#define MPI_DO_OP_INC          3
#define MPI_DO_OP_DEC          4
#define MPI_DO_OP_VERIFY       5
#define MPI_DO_OP_CLEAR        6
#define MPI_DO_OP_UNLIMITED    7  

/* Error codes: */
#define MPI_SUCCESS                         0
#define MPI_ERR_FUNCTION_NOT_SUPPORTED 0x1001  /* function not implemented or not supported */
#define MPI_ERR_UNKNOWN_OS             0x1002  /* unknown operation system */
#define MPI_ERR_LIB_VERSION            0x1003  /* wrong version of using library */
#define MPI_ERR_DRIVER_MISSING         0x1004  /* wrong version of using driver */
#define MPI_ERR_DRIVER_VERSION         0x1005  /* wrong version of using driver */
#define MPI_ERR_LIBRARY_MISSING        0x1006  /* library is missing */
#define MPI_ERR_PORT_FAILED            0x1010  /* port not found */
#define MPI_ERR_NO_BOX                 0x1011  /* CRYPTO-BOX not found */
#define MPI_ERR_NO_BOX_TYPE            0x1012  /* CRYPTO-BOX type not found */
#define MPI_ERR_BOX_FAILED             0x1013  /* CRYPTO-BOX failed */
#define MPI_ERR_BOX_TIME_OUT           0x1014  /* communication level error */ 
#define MPI_ERR_FIRMWARE_VERSION       0x1015  /* wrong version of firmware */
#define MPI_ERR_BOX_BUSY               0x1016  
#define MPI_ERR_PARAMETER              0x1021  /* wrong parameter */
#define MPI_ERR_PASSWORD               0x1022  /* incorrect password */
#define MPI_ERR_AUTENTICATION          0x1023  /* incorrect authentication */
#define MPI_ERR_MUTEX_BUSY             0x1060  

#define MPI_ERR_SPX_IPX_FAILED         0x1031  /* no connection (incorrect name etc.) */
#define MPI_ERR_NETBIOS_FAILED         0x1032  /* no connection (incorrect name etc.) */
#define MPI_ERR_TCP_IP_FAILED          0x1033  /* no connection (incorrect name etc.) */
#define MPI_ERR_UNKNOWN                0x1034  /* unknown error */

#define MPI_ERR_NOT_LOGGED_IN          0x1024  /* not logged in correctly */
#define MPI_ERR_TOO_MANY_USERS         0x1025  /* too many users connected */
#define MPI_ERR_SRV_INT_ERROR          0x1026  /* server internal table is full */
#define MPI_ERR_NEED_PASSWORD          0x1027  /* password must be submitted before */
#define MPI_ERR_BAD_REQUEST            0x1028  /* internall error (invalid request from client) */

#define MPI_ERR_NOT_CONNECTED          0x1035  /* connection error */
#define MPI_ERR_CONNECTED              0x1036  /* already connected */
#define MPI_ERR_DISCONNECTED           0x1037  /* already disconnected */
#define MPI_ERR_NO_SERVER              0x1038  /* server not found */
#define MPI_ERR_NO_TRANSPORT           0x1039  /* no compatible transport found */
#define MPI_ERR_OUT_OF_MEMORY          0x1040  /* out of memory */
#define MPI_ERR_NET_TIME_OUT           0x1041  /* network communication timeout */ 
#define MPI_ERR_BAD_RESPONSE           0x1047  /* an invalid response received from server */

#define MPI_ERR_INVALID_LC             0x1042  /* licenses counter is damaged */
#define MPI_ERR_OUT_OF_RANGE           0x1043  /* out of range error during operations with CRYPTO-BOX memory */
#define MPI_ERR_INVALID_LC_ID          0x1044  /* licenses counter read error: unable to read ID1 */
#define MPI_ERR_INVALID_LC_RAM         0x1045  /* licenses counter read error: unable to read RAM */
#define MPI_ERR_INVALID_LC_DCR         0x1046  /* licenses counter read error: unable to decrypt */

#define MPI_ERR_NEED_ENC_KEY           0x1050  /* encryption key must be submitted before */
#define MPI_ERR_INVALID_ENC_KEY        0x1051  /* RSA Key structure is invalid */
#define MPI_ERR_BUFFER_TOO_SHORT       0x1052  /* the submitted destination buffer is too short */

#define MPI_ERR_UNLIMITED_USERS        0x1999  /* licenses counter was programmed for the unlimited connections quantity */

#define MPI_ERR_CLOSE_HANDLE	       0x2000  /* low-level CRYPTO-BOX driver error */
#define MPI_ERR_GET_HANDLE	           0x2001  /* low-level CRYPTO-BOX driver error */  
#define MPI_ERR_IOCTL_RESULT	       0x2002  /* low-level CRYPTO-BOX driver error */  
#define MPI_ERR_EXCEPTION_DRV          0x2003  /* low-level CRYPTO-BOX driver error */  
#define MPI_ERR_SYSTEM_UNKNOWN         0x2004  /* low-level CRYPTO-BOX driver error */

#define MPI_ERR_NO_DO                  0x1101  /* data object not found */ 
#define MPI_ERR_NO_LICENCES            0x1102  /* licence not available */
#define MPI_ERR_NO_USAGE_COUNTER       0x1103  /* end of usage counter */
#define MPI_ERR_MODIFIED_DATE          0x1104  /* was detected modification of date of PC */
#define MPI_ERR_EXPIRED                0x1105  /* value was expired */
#define MPI_ERR_UNLIMITED              0x1106  /* data object has unlimited value */



#ifdef __cplusplus
extern "C"
{
#endif /* (__cplusplus) */

#ifdef  EXPORT
#define EXTERN
#else
#define EXTERN extern
#endif


/*
==========================================================================================
                                     MPI Functions 
==========================================================================================

 Note:
 The MPI-API gives you a great flexibility. To start with any MARX Hardware 
 the following function calls are necessary regardless of the operating system, 
 MARX hardware type you are using, local or remote access.


    Example: (for local channel)
    
    ... begin of program
    MPI_SetChannel()     to select a local communication channel (e.g. LPT + CB560 or VERSA)
    MPI_SubmitPassword() to submit a CRYPTO-BOX access password
    MPI_SearchFirst()    to find a local CRYPTO-BOX, in 99% of the cases 
                         this is the only BOX
    MPI_Open()           to open the channel, if channel opens the submitted password is ok
    MPI_Close()          to close channel and destroy passwords 
    ... end of program
    
    or (for remote channel)
    
    ... begin of program
    MPI_SetNetworkParameter() to set a server for looking  
    MPI_SetChannel()          to select a remote communication channel
    MPI_SubmitPassword()      to submit a CRYPTO-BOX access password
    MPI_SearchFirst()         to find a remote CRYPTO-BOX
    MPI_GetNetworkParameter() to get a found server name
    MPI_Open()                to open the channel, if channel opens the submitted password is ok
    MPI_Close()               to close channel 
    ... end of program


 Remark:
 The following functions are listed in the order they are used.
*/


/* 
----------------------------------------------
 Functions to set/retrieve network parameters  
----------------------------------------------
*/

/* Set value of the specified network parameter */ 
/* (server name, timeout, UDP port ...) */
EXTERN DWORD CBOXCALL MPI_SetNetworkParameter(
       STRING *ParamName,           /* specifies parameter to set */
       STRING *ParamValue);	        /* a new value for specified parameter */

/* Get value of the specified network parameter */
/* (server name, timeout, UDP port ...) */
EXTERN DWORD CBOXCALL MPI_GetNetworkParameter(
       STRING *ParamName,           /* specifies parameter to retrieve */
       STRING *paramValue,          /* returns a current value for specified parameter */
       DWORD cIdentifier);          /* for some kind of parameter (such as "SERVER") */
                                    /* a valid handle from the MPI_Search functions is needed */


/* 
-------------------------------
 Functions to manage passwords  
-------------------------------
 Note:
 Submitted passwords are submitted to a password pool that is kept in memory for 
 future access. 
 Typical functions that require verified passwords are
 ReadMem, WriteMem, ReadID. 
*/

/* Submit a password to the access keychain */
EXTERN DWORD CBOXCALL MPI_SubmitPassword(  
       STRING *PasswType,                  /* type of password (programmable or not) */
	   STRING *Access,                     /* password access type (read/write) */
       DWORD Passw);                       /* password value  */  

/* Submit a password as BYTE string */
EXTERN DWORD CBOXCALL MPI_SubmitPasswordEx(
       STRING *PasswType,                  /* type of password (programmable or not) */
	   STRING *Access,                     /* password access type (read/write) */
       BYTE   *lpPassw,                	   /* pointer to password value as BYTE string */
       DWORD  PasswLen);	               /* length of password BYTE string */
 
/* Remove a password from the password pool */
EXTERN DWORD CBOXCALL MPI_ErasePassword(   
       STRING *PasswType );                /* type of password to erase */ 
       

/* 
---------------------------------------------
 Functions to set up a communication channel 
---------------------------------------------
 Note:
 A communication channel is the combination of 
 library - device driver - port - MARX hardware (SMARX Card or CRYPTO-BOX).
 There are hundreds of possible combinations. You have the choice between a 
 well defined excluisive channel, a selection of channels or all available 
 channels. The more precise channels are defined, the faster the search 
 functions can access a particular system. On the other hand, the more 
 channels included the more flexible the protection is.

 IMPORTANT:
 *** The search order is defined by the order channels are submitted! ***
*/

/* Where to look for what kind of CRYPTO-BOX */
EXTERN DWORD CBOXCALL MPI_SetChannel( 
       STRING *CBoxType,              /* select the CRYPTO-BOX solutions to be included */
       STRING *Port,                  /* select the included ports e.g. LPT, COM, AUTONET, TCP_IP */
       void *reserved);               /* reserved for future use */ 


/* 
------------------------------------------------
 Functions to establish a communication channel
------------------------------------------------
 Note:
 Make sure to define a channel (which type of CRYPTO-BOX, what type of port) 
 and submit at least the ID1 password before calling MPI_Search -> MPI_Open 
 functions. Call MPI_SetChannel() to do this.
*/

/* Looks for the first CRYPTO-BOX with no certain ID code */
EXTERN DWORD CBOXCALL MPI_WhatIsAvailable( 
	   STRING10 FAR *cBoxType,         /* returns the type of CRYPTO-BOX that was found */
	   STRING10 FAR *port,             /* returns the type of port where found BOX */ 
	   DWORD *numberOfBoxTypes);       /* returns number of BOX types */

/* Looks for the first CRYPTO-BOX with a certain ID code */
EXTERN DWORD CBOXCALL MPI_SearchFirst( 
       STRING10 FAR *cboxType,         /* returns the type of CRYPTO-BOX that was found */
       STRING10 FAR *port,             /* returns the port where it was found (LPT, COM..) */  
       DWORD *cIdentifier);            /* returns a handle for future reference */

/* Looks for the next CRYPTO-BOX with a certain ID code */
EXTERN DWORD CBOXCALL MPI_SearchNext ( 
       STRING10 FAR *cboxType,         /* returns the type of CRYPTO-BOX that was found */
       STRING10 FAR *port,             /* returns the port where it was found (LPT, COM..) */  
       DWORD *cIdentifier);            /* returns a handle for future reference */

/* Open a CRYPTO-BOX for "the real stuff" */
EXTERN DWORD CBOXCALL MPI_Open(        
       DWORD cIdentifier);             /* valid handle from the MPI_Search functions */

/* Releases licenses, destroys passwords and frees memory */      
EXTERN DWORD CBOXCALL MPI_Close(       
       DWORD cIdentifier);             /* valid handle from the MPI_Search functions */


/* 
------------------------------------ 
 Functions to identify a CRYPTO-BOX
------------------------------------
*/

/* Every developer has a unique ID that can be */
/* accessed through this function */
EXTERN DWORD CBOXCALL MPI_GetDeveloperId(
       DWORD FAR *DevelopId);            

/* Some CRYPTO-BOXes have a unique number programmed */
/* by MARX during production */
EXTERN DWORD CBOXCALL MPI_GetSerialNr( 
       DWORD FAR *SerNr);              

/* Some CRYPTO-BOXes have an additional unique number programmed */
/* by MARX during production */
EXTERN DWORD CBOXCALL MPI_GetManufacturerId( 
       DWORD FAR *ManufacturerId);              

/* Find out what type of CRYPTO-BOX is attached */
EXTERN DWORD CBOXCALL MPI_GetBoxType(    
       STRING10 FAR *BoxType);    /* CRYPTO-BOX type */       

/* Read an ID code to identifiy the CRYPTO-BOX */
EXTERN DWORD CBOXCALL MPI_ReadID( 
       DWORD IdNr,                /* number of ID, (requires the maching password to be verified first) */
       DWORD FAR *id);            /* returns the ID code if the submitted password is correct */
								 

/* 
------------------------------------------------
 Functions to access the memory of a CRYPTO-BOX 
------------------------------------------------
*/

EXTERN DWORD CBOXCALL MPI_ReadMem(
       DWORD MemNr,                 /* memory number */
       DWORD MemoryAddress,         /* start address */
       DWORD BufferLength,          /* length of buffer */
       CHAR  FAR *szBuffer);        /* buffer for output data */

EXTERN DWORD CBOXCALL MPI_WriteMem(
       DWORD MemNr,                 /* memory number */
       DWORD MemoryAddress,         /* start address */
       DWORD BufferLength,          /* length of buffer */
       CHAR  FAR *szBuffer);        /* buffer for input data */


/* 
---------------------------------------
 Functions to encrypt and decrypt data
---------------------------------------
*/

/* Submit an encryption key */
EXTERN DWORD CBOXCALL MPI_SubmitEncryptionKey( 
       STRING *Key,                 /* type of encryption key */
       DWORD  len,	                /* length of key */
       BYTE	  *lpKey,		        /* pointer to key value as BYTE string */
       DWORD  offset);	        	/* for RSA_CBU keys only */ 

/* Generate a RSA Public/Private Keys pair */ 
EXTERN DWORD CBOXCALL MPI_GenerateKeyPairRSA(
       DWORD RSA_KeyBits,           /* number of bits for RSA modulus (RSA Key length) */
                                    /* can be MPI_RSA_KEY_MODULUS_2048, MPI_RSA_KEY_MODULUS_1024 */
                                    /* or MPI_RSA_KEY_MODULUS_512 */
       BYTE  *PrivateKeyBuffer,     /* buffer to store generated Private Key */
                                    /* must be not less than (2 + ((RSA_KeyBits / 8) * 2)) bytes length */
                                    /* can use MPI_RSA_KEY_BUF_LEN(x) macros to calculate necessary length */
       BYTE  *PublicKeyBuffer,      /* buffer to store generated Private Key */
                                    /* length the same as for the Private Key buffer */
       DWORD *KeyBufferLen);        /* length in bytes of allocated buffer for Private or Public Key */
                                    /* after execution contains actual Key size in bytes */
                                    /* calculated by formula: (2 + ((RSA_KeyBits / 8) * 2)) */  

/* Encrypt data using IDEA or MARX algorythms */
EXTERN DWORD CBOXCALL MPI_Encrypt(  
       STRING *Algorithm,           /* type of algorythm */
       WORD seed,                   /* secret seed */
       WORD len,                    /* lenth of data */
       CHAR FAR *szmem);            /* data */

/* Decrypt data using IDEA or MARX algorythms */
EXTERN DWORD CBOXCALL MPI_Decrypt(  
       STRING *Algorithm,           /* type of algorythm */
       WORD seed,                   /* secret seed */
       WORD len,                    /* lenth of data */
       CHAR FAR *szmem);            /* data */

EXTERN DWORD CBOXCALL MPI_EncryptEx(			
       STRING *Algorithm,	        /* type of algorithm */
       STRING *Key,	            	/* type of encryption key */
       DWORD  len_src,	            /* length of buffer to be encrypted */
       VOID	  *lpsrc,           	/* pointer to source buffer - byte string */
       DWORD  *plen_dst,	        /* length of destination buffer */
       VOID	  *lpdst);	            /* pointer to destination buffer */

EXTERN DWORD CBOXCALL MPI_DecryptEx(			
       STRING *Algorithm,	        /* type of algorithm */
       STRING *Key,		            /* type of encryption key */
       DWORD  len_src,	            /* length of buffer to be decrypted */
       VOID	  *lpsrc,           	/* pointer to source buffer - byte string */
       DWORD  *plen_dst,           	/* length of destination buffer */
       VOID	  *lpdst);	            /* pointer to destination buffer */

/* Erase an encryption key */
EXTERN DWORD CBOXCALL MPI_EraseEncryptionKey(	
       STRING *Key);		        /* type of encryption key */


/* 
--------------------------------------
 Function to generate random sequence
--------------------------------------
*/

EXTERN DWORD CBOXCALL MPI_GetRandomSequence(		
       DWORD len,		            /* length of buffer for random sequence */
       VOID	 *lpbuffer);	        /* pointer to buffer for random sequence */

/* 
----------------------
 Function to get hash
----------------------
*/

EXTERN DWORD CBOXCALL MPI_GetHash(
       STRING *Algorithm,	        /* type of algorithm */                           
       WORD   len_src,              /* length of source buffer */
       VOID   *lpsrc,               /* pointer to source buffer */
       WORD   *plen_hash,         	/* length of hash-buffer */
       BYTE	  *lphash);	            /* pointer to hash-buffer */

/* 
----------------------------------------------
 Network (remote channels) specific functions
----------------------------------------------
*/

/* Get information about licenses: */
/* max quantity and currently used number of licenses */
EXTERN DWORD CBOXCALL MPI_GetLicenseInfo( 
       DWORD cIdentifier,         /* valid handle from the MPI_Search functions */   
       DWORD FAR *LicMaximum,     /* max number of licenses */
       DWORD FAR *LicUsed);       /* currently used number of licenses */


/* 
-----------------------------------------------------------------------
 Functions that tell us more about the CRYPTO-BOX, the API and drivers
-----------------------------------------------------------------------
 Note:
 Most of the functions in this sections require an open CRYPTO-BOX channel.
 See MPI_Search and MPI_Open for further details.
*/

/* Checks the version of the library that accesses the CRYPTO BOX */
EXTERN DWORD CBOXCALL MPI_GetLibVersion( 
       DWORD cIdentifier,                /* valid handle from the MPI_Search functions */
       DWORD FAR *libver);               /* returns the version of the library */
 
/* Finds out the driver version of the active channel */
EXTERN DWORD CBOXCALL MPI_GetDrvVersion( 
       DWORD cIdentifier,                /* valid handle from the MPI_Search functions */
       DWORD FAR *driverVersion);        /* returns the driver version */

EXTERN DWORD CBOXCALL MPI_GetBoxInfo(
       DWORD cIdentifier,                /* valid handle from the MPI_Search functions */
       WORD  *box,                       /* MARX hardware type */
       DWORD *memSize);                  /* CRYPTO-BOX memory size */


/* 
--------------------------------------------
 Functions to administer the CRYPTO-BOX API
--------------------------------------------
*/

EXTERN DWORD CBOXCALL MPI_GetLastError( 
       DWORD *LastErr,                    /* return last error */  
       STRING80 *MssgErr);                /* return error message */


/* 
-----------------------------------------
 Functions to work with MPI Data Objects
-----------------------------------------
*/

EXTERN DWORD CBOXCALL MPI_ProcessDataObject( 
       DWORD  ObjectType,      /* MPI Data Object type */ 
       DWORD  MemNr,           /* Object location: memory number */
       DWORD  MemoryAddress,   /* Object location: memory address */
       DWORD  ObjectOperation, /* Object operation */
       VOID  *lpData,          /* pointer to the operation data */
                               /* (optional- specific to Object/operation) */
       DWORD *pDataLen,        /* pointer to the data len variable */
                               /* (used as input parameter to define allocated buffer size) */
                               /* (in some cases is used as output- returned by function- */
                               /* to show required size) */
                               /* (optional- specific to Object/operation) */
       VOID  *pDOBuffer );     /* pointer to the memory buffer with Data Objects */

#undef EXTERN
#undef EXPORT

#ifdef __cplusplus
}
#endif /* (__cplusplus) */

#endif /* MPI_INCLUDED */

/* EOF */