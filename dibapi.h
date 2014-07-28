/*
 *  dibapi.h
 *
 *  Copyright (c) 1991 Microsoft Corporation. All rights reserved
 *
 *  Header file for Device-Independent Bitmap (DIB) API.  Provides
 *  function prototypes and constants for the following functions:
 *
 *  BitmapToDIB()        - Creates a DIB from a bitmap
 *  ChangeBitmapFormat() - Changes a bitmap to a specified DIB format
 *  ChangeDIBFormat()    - Changes a DIB's BPP and/or compression format
 *  CopyScreenToBitmap() - Copies entire screen to a standard Bitmap
 *  CopyScreenToDIB()    - Copies entire screen to a DIB
 *  CopyWindowToBitmap() - Copies a window to a standard Bitmap
 *  CopyWindowToDIB()    - Copies a window to a DIB
 *  CreateDIBPalette()   - Creates a palette from a DIB
 *  CreateDIB()          - Creates a new DIB
 *  DestroyDIB()         - Deletes DIB when finished using it
 *  DIBError()           - Displays message box with error message
 *  DIBHeight()          - Gets the DIB height
 *  DIBNumColors()       - Calculates number of colors in the DIB's color table
 *  DIBToBitmap()        - Creates a bitmap from a DIB
 *  DIBWidth()           - Gets the DIB width
 *  FindDIBBits()        - Sets pointer to the DIB bits
 *  GetSystemPalette()   - Gets the current palette
 *  LoadDIB()            - Loads a DIB from a file
 *  PaintBitmap()        - Displays standard bitmap in the specified DC
 *  PaintDIB()           - Displays DIB in the specified DC
 *  PalEntriesOnDevice() - Gets the number of palette entries
 *  PaletteSize()        - Calculates the buffer size required by a palette
 *  PrintDIB()           - Prints the specified DIB
 *  PrintScreen()        - Prints the entire screen
 *  PrintWindow()        - Prints all or part of a window
 *  SaveDIB()            - Saves the specified dib in a file
 *
 * See the file DIBAPI.TXT for more information about these functions.
 *
 */


/* Handle to a DIB */
#define HDIB HANDLE


/* Print Area selection */
#define PW_WINDOW        1
#define PW_CLIENT        2


/* Print Options selection */
#define PW_BESTFIT       1
#define PW_STRETCHTOPAGE 2
#define PW_SCALE         3

/* DIB Macros*/

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed 
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

/* Error constants */
enum {
      ERR_MIN = 0,                     // All error #s >= this value
      ERR_NOT_DIB = 0,                 // Tried to load a file, NOT a DIB!
      ERR_MEMORY,                      // Not enough memory!
      ERR_READ,                        // Error reading file!
      ERR_LOCK,                        // Error on a GlobalLock()!
      ERR_OPEN,                        // Error opening a file!
      ERR_CREATEPAL,                   // Error creating palette.
      ERR_GETDC,                       // Couldn't get a DC.
      ERR_CREATEDDB,                   // Error create a DDB.
      ERR_STRETCHBLT,                  // StretchBlt() returned failure.
      ERR_STRETCHDIBITS,               // StretchDIBits() returned failure.
      ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice() failed.
      ERR_STARTDOC,                    // Error calling StartDoc().
      ERR_NOGDIMODULE,                 // Couldn't find GDI module in memory.
      ERR_SETABORTPROC,                // Error calling SetAbortProc().
      ERR_STARTPAGE,                   // Error calling StartPage().
      ERR_NEWFRAME,                    // Error calling NEWFRAME escape.
      ERR_ENDPAGE,                     // Error calling EndPage().
      ERR_ENDDOC,                      // Error calling EndDoc().
      ERR_SETDIBITS,                   // Error calling SetDIBits().
      ERR_FILENOTFOUND,                // Error opening file in GetDib()
      ERR_INVALIDHANDLE,               // Invalid Handle
      ERR_DIBFUNCTION,                 // Error on call to DIB function
      ERR_MAX                          // All error #s < this value
     };



/* Function prototypes */
/*
HDIB      FAR  BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);

HDIB      FAR  ChangeBitmapFormat (HBITMAP  hBitmap,
                                   WORD     wBitCount,
                                   DWORD    dwCompression,
                                   HPALETTE hPal);
HDIB      FAR  ChangeDIBFormat (HDIB hDIB, WORD wBitCount,
                                DWORD dwCompression);
HBITMAP   FAR  CopyScreenToBitmap (LPRECT);
HDIB      FAR  CopyScreenToDIB (LPRECT);
HBITMAP   FAR  CopyWindowToBitmap (HWND, WORD);
HDIB      FAR  CopyWindowToDIB (HWND, WORD);
HPALETTE  FAR  CreateDIBPalette (HDIB hDIB);
HDIB      FAR  CreateDIB(DWORD, DWORD, WORD);
WORD      FAR  DestroyDIB (HDIB);
void      FAR  DIBError (int ErrNo);
DWORD     FAR  DIBHeight (LPSTR lpDIB);
WORD      FAR  DIBNumColors (LPSTR lpDIB);
HBITMAP   FAR  DIBToBitmap (HDIB hDIB, HPALETTE hPal);
DWORD     FAR  DIBWidth (LPSTR lpDIB);
LPSTR     FAR  FindDIBBits (LPSTR lpDIB);
HPALETTE  FAR  GetSystemPalette (void);
HDIB      FAR  LoadDIB (LPSTR);
BOOL      FAR  PaintBitmap (HDC, LPRECT, HBITMAP, LPRECT, HPALETTE);
BOOL      FAR  PaintDIB (HDC, LPRECT, HDIB, LPRECT, HPALETTE);
int       FAR  PalEntriesOnDevice (HDC hDC);
WORD      FAR  PaletteSize (LPSTR lpDIB);
WORD      FAR  PrintDIB (HDIB, WORD, WORD, WORD, LPSTR);
WORD      FAR  PrintScreen (LPRECT, WORD, WORD, WORD, LPSTR);
WORD      FAR  PrintWindow (HWND, WORD, WORD, WORD, WORD, LPSTR);
WORD      FAR  SaveDIB (HDIB, LPSTR);
*/


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DIBAPIDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DIBAPIDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef DIBAPIDLL_EXPORTS
#define DIBAPIDLL_API __declspec(dllexport)
#else
#define DIBAPIDLL_API __declspec(dllimport)
#endif
/*
// This class is exported from the DIBAPIDLL.dll
class DIBAPIDLL_API CDIBAPIDLL {
public:
	CDIBAPIDLL(void);
	// TODO: add your methods here.
};

extern DIBAPIDLL_API int nDIBAPIDLL;
*/

#ifdef __cplusplus
extern "C" {
#endif

DIBAPIDLL_API int fnDIBAPIDLL(void);


DIBAPIDLL_API HDIB      BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);

DIBAPIDLL_API HDIB      ChangeBitmapFormat (HBITMAP  hBitmap,
                                   WORD     wBitCount,
                                   DWORD    dwCompression,
                                   HPALETTE hPal);
DIBAPIDLL_API HDIB      ChangeDIBFormat (HDIB hDIB, WORD wBitCount,
                                DWORD dwCompression);
DIBAPIDLL_API HBITMAP   CopyScreenToBitmap (LPRECT);
DIBAPIDLL_API HDIB      CopyScreenToDIB (LPRECT);
DIBAPIDLL_API HBITMAP   CopyWindowToBitmap (HWND, WORD);
DIBAPIDLL_API HDIB      CopyWindowToDIB (HWND, WORD);
DIBAPIDLL_API HPALETTE  CreateDIBPalette (HDIB hDIB);
DIBAPIDLL_API HDIB      CreateDIB(DWORD, DWORD, WORD);
DIBAPIDLL_API WORD      DestroyDIB (HDIB);
DIBAPIDLL_API void      DIBError (int ErrNo);
DIBAPIDLL_API DWORD     DIBHeight (LPSTR lpDIB);
DIBAPIDLL_API WORD      DIBNumColors (LPSTR lpDIB);
DIBAPIDLL_API HBITMAP   DIBToBitmap (HDIB hDIB, HPALETTE hPal);
DIBAPIDLL_API DWORD     DIBWidth (LPSTR lpDIB);
DIBAPIDLL_API LPSTR     FindDIBBits (LPSTR lpDIB);
DIBAPIDLL_API HPALETTE  GetSystemPalette (void);
DIBAPIDLL_API HDIB      LoadDIB (LPSTR);
DIBAPIDLL_API BOOL      PaintBitmap (HDC, LPRECT, HBITMAP, LPRECT, HPALETTE);
DIBAPIDLL_API BOOL      PaintDIB (HDC, LPRECT, HDIB, LPRECT, HPALETTE);
DIBAPIDLL_API int       PalEntriesOnDevice (HDC hDC);
DIBAPIDLL_API WORD      PaletteSize (LPSTR lpDIB);
DIBAPIDLL_API WORD      PrintDIB (HDIB, WORD, WORD, WORD, LPSTR);
DIBAPIDLL_API WORD      PrintScreen (LPRECT, WORD, WORD, WORD, LPSTR);
//DIBAPIDLL_API WORD      PrintWindow (HWND, WORD, WORD, WORD, WORD, LPSTR);
DIBAPIDLL_API WORD      SaveDIB (HDIB, LPSTR);

#ifdef __cplusplus
}
#endif

