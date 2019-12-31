// cdib.h declaración de la clase CDib de Programación avanzada con Visual C++

#ifndef _INSIDE_VISUAL_CPP_CDIB
#define _INSIDE_VISUAL_CPP_CDIB

class CDib : public CObject
{
	enum Alloc {noAlloc, crtAlloc, heapAlloc};
	DECLARE_SERIAL(CDib)
public:
	LPVOID m_lpvColorTable;
	HBITMAP m_hBitmap;
	LPBYTE m_lpImage;  // dirección de comienzo de los bits del DIB
	LPBITMAPINFOHEADER m_lpBMIH; //  búfer que contiene la BITMAPINFOHEADER
private:
	HGLOBAL m_hGlobal; // para ventanas externas necesitamos liberar;
	                   //  podría ser asignada por esta clase o externamente
	Alloc m_nBmihAlloc;
	Alloc m_nImageAlloc;
	DWORD m_dwSizeImage; // de bits -- no BITMAPINFOHEADER o BITMAPFILEHEADER
	int m_nColorTableEntries;
	
	HANDLE m_hFile;
	HANDLE m_hMap;
	LPVOID m_lpvFile;
	HPALETTE m_hPalette;
public:
	CDib();
	CDib(CSize size, int nBitCount);	// construye BITMAPINFOHEADER
	~CDib();
	int GetSizeImage() {return m_dwSizeImage;}
	int GetSizeHeader()
		{return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;}
	CSize GetDimensions();
	BOOL AttachMapFile(const TCHAR* strPathname, BOOL bShare = FALSE);
	BOOL CopyToMapFile(const TCHAR* strPathname);
	BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = FALSE, HGLOBAL hGlobal = NULL);
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);  // hasta que implementamos CreateDibSection
	HBITMAP CreateSection(CDC* pDC = NULL);
	UINT UsePalette(CDC* pDC, BOOL bBackground = FALSE);
	BOOL MakePalette();
	BOOL SetSystemPalette(CDC* pDC);
	BOOL Compress(CDC* pDC, BOOL bCompress = TRUE); // FALSE significa descomprimir
	HBITMAP CreateBitmap(CDC* pDC);
	BOOL Read(CFile* pFile);
	BOOL ReadSection(CFile* pFile, CDC* pDC = NULL);
	BOOL Write(CFile* pFile);
	void Serialize(CArchive& ar);
	void Empty();
private:
	void DetachMapFile();
	void ComputePaletteSize(int nBitCount);
	void ComputeMetrics();
};
#endif // _INSIDE_VISUAL_CPP_CDIB
