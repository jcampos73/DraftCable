/*H**************************************************************
 * Filename: cboxcall.h
 * Description:
 *        used by many CRYPTO-BOX implementations
 *        makes type definition platform independend
 *        calling conventions are initialized
 *
 * Usage:
 *        this header needs to be available during compilation of
 *        a sample program. It is included by other header files.
 * Notes:
 *        none
 *
 * Copyright (c) 1998, MARX International Inc. All rights reserved.
 *
 * Changes:
 *
 * DATE      WHO  DETAIL
 * 11AUG1998 MJ   First version
 * 29SEP1998 PG   Made it CPP friendly
 * 03JAN1999 MJ   added EXPORT + EXTERN
 * 11JAN1999 MJ   added BC detection via __BORLANDC__
 * 13JAN1999 MJ   introduced MARX_LIB_BUILD to control extern "C"
 * 26APR1999 MJ   introduces BIT_16...BIT_31
 * 02JUL1999 AK   add INT32, INT18, INT8, STRING40
 * 11AUG1999 AK   add defines for GNU compiler in DOS, Windows
 * 21MAR2000 NE   add INT, UINT defines
 * 12APR2001 SM   all STRING defines changed: unsigned char -> char
 *
 *H*/

#ifndef CBOXCALL_INCLUDED
#define CBOXCALL_INCLUDED
#define CBOXCALL_VERSION          10        /* Version of this header file */

/**************** type definitions and related ******************
  define windows.h types so that no additional (win-) header is
  needed.
*/
#ifndef _INC_WINDOWS

 #ifndef VOID
  #define VOID void
 #endif

 #ifndef BOOL
  #define BOOL int
 #endif

 #ifndef BYTE
  #define BYTE unsigned char
 #endif

 #ifndef WORD
  #define WORD unsigned short
 #endif

 #ifndef DWORD
  #define DWORD unsigned long
 #endif

 #ifndef UCHAR
  #define UCHAR unsigned char
 #endif

 #ifndef USHORT
  #define USHORT unsigned short
 #endif

 #ifndef ULONG
  #define ULONG unsigned long
 #endif

 #ifndef CHAR
  #define CHAR char
 #endif

 #ifndef SHORT
  #define SHORT short
 #endif

 #ifndef LONG
  #define LONG long
 #endif

 #ifndef INT
  #define INT int
 #endif

 #ifndef UINT
  #define UINT unsigned int
 #endif

#endif                                                     /* _INC_WINDOWS */

#ifndef STRING
 #define STRING  char 
#endif

#ifndef STRING10
 #define STRING10  char 
#endif

#ifndef STRING40
 #define STRING40 char
#endif

#ifndef STRING80
 #define STRING80  char 
#endif

/* Compiler portability
 * this part is not implemented yet
 __STDC__     ANSI C compiler
 __BORLANDC__ defined is Borland C compiler is compiling
*/ 

/* Operating System detection
   _WIN32    : defined for Windows 32-bit programs (real Win or Console)
   _WINDOWS  : windows 16 bit program is compiled
*/

           /********************  calling conventions **********************/
           /* the following section checks for the programming environment */

         /* C++ programs need to know that the header contains C functions */

#ifndef MARX_LIB_BUILD
  #ifdef __cplusplus
  extern "C"
  {
  #endif
#endif                                                      /* __cplusplus */

#ifdef  EXPORT
#define EXTERN
#else
#define EXTERN extern
#endif


/* calling conventions for Windows 32 bit program */
#ifdef WIN32                         
 #ifndef PASCAL
  #define PASCAL _stdcall
 #endif
 #ifndef FAR
  #define FAR
 #endif
 #ifndef WINAPI
  #define WINAPI FAR PASCAL
 #endif
 #define CBOXCALL WINAPI

#else
 /* calling conventions for Windows 16 bit program */
 #ifdef _WINDOWS                     
  #ifndef FAR
    #define FAR _far
  #endif
  #ifndef PASCAL
    #define PASCAL pascal
  #endif
  #ifndef WINAPI
   #define WINAPI FAR PASCAL
  #endif
  #define CBOXCALL WINAPI

 #else
  /* calling conventions for GNU compiler in DOS, Windows */
  #ifdef _GNU
   #ifndef FAR
    #define FAR
   #endif
	#define CBOXCALL

 #else
 /* Library functions will be C functions (default value for C compilers)
    otherwise: DOS program
    FAR functions ans parameters will always be _far
 */
  #ifndef  FAR
   #define FAR /*_far*/
  #endif
  #define CBOXCALL

 #endif                                                        /* _WINDOWS */
#endif                                                         /*  WIN32   */
#endif

           /******************* miscellaneous defines **********************/

                           /* Compiler independent soft define TRUE/FALSE: */
#ifndef  TRUE
#define TRUE    (1==1)
#endif

#ifndef  FALSE
#define FALSE   (1==0)
#endif

#undef  EXTERN
#undef  EXPORT

#ifndef MARX_LIB_BUILD
  #ifdef __cplusplus
  }
  #endif                                                      /* __cplusplus */
#endif

#endif                                                /* CBOXCALL_INCLUDED */



