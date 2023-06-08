//*************************************************************
//  File name: W16GPIB.H
//
//  Description:  
//      Declaration file for the CWin16Gpib class.
//
//
//  Comments:
//      
//      
//
//  History:    Date       Author               Comment
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************
 
 
#ifndef _WIN16GPIB_
#define _WIN16GPIB_
 

#ifndef DISABLE_TUNING    // conditional compilation of this file


#include "gpib\windecl.h"



// CALLBACK INTERFACE - Another class may use this callback
// interface to 'plug-into' the CWin16Gpib class and receive
// notifications of gpib port transactions.  See the files
// '..\views\commview\commview.*'. 
struct IGpibViewSocket
{
  virtual void	GpibNote(const char *pszPort, const char *pszOp, BYTE *pData, int nNumBytes) = 0;
  virtual void  GpibNote(const char *pszPort, const char *pszOp, const char *pszString) = 0;
  virtual void  GpibError(const char *pszPort, const char *pszOp, DWORD dwStatusCode) = 0; 
};

// Use this macro to construct a pointer to the class which nests an
// IGpibViewSocket derived class.  This macro is for 16-bit Windows only.
#define METHOD_PROLOGUE(theClass, localClass) \
  theClass* pThis = ((theClass*)((char*)(this) - offsetof(theClass, localClass)));




/////////////////////////////////////////////////////////////////////
//  CWin16Gpib
//
// 
//
class CWin16Gpib : public CObject
{
  DECLARE_DYNAMIC(CWin16Gpib)
  
  // attributes 
  private:
  int				m_nCardId;
  BOOL				m_bCardConfigured;	
  IGpibViewSocket   *m_pIGpibViewPlug;    // gpib viewer callback mechanism
                                                                                    
  public:
    // constructors 
    CWin16Gpib();
    CWin16Gpib(CWin16Gpib &rCWin16Gpib);
    
    // destructor
    ~CWin16Gpib();    
  
    // operations - card functions 
    int		ConfigureCard(void);
    BOOL	IsCardConfigured(void) { return(m_bCardConfigured); } 
    int		GetCardId(void) { return(m_nCardId); }
    void 	GetGpibError(CString &msg);
    int		EnableCard(BOOL bEnable);
    long	GetLastCount(void); 
    
    // operations - device functions 
	int	    FindDevice(const char *pszIDN, Addr4882_t *pAddr4882_t);
    int		Open(int nPad, int nSad, int nTmo, int nEot, int nEos, int *nDvm);
    int		Clear(int nDvm);
    int		Write(int nDvm, char *pszString, long lCount);
    int		Trigger(int nDvm);
    int 	Wait(int nDvm, int nMask);
    int 	ReturnSerialPoll(int nDvm, char *pcResponse);
    int		Read(int nDvm, BYTE *pbyBuffer, long lCount);
    int		EnableDevice(int nDvm, BOOL bEnable);
    
    // operations - card/device functions
    
    // operations - diagnostic support functions
    IGpibViewSocket 	*AttachGpibViewPlug(IGpibViewSocket *pIGpibViewPlug);
    void	DetachGpibViewPlug(IGpibViewSocket *pIGpibViewPlug = NULL)
            {  m_pIGpibViewPlug = pIGpibViewPlug; }
     
  public:
    #ifdef _DEBUG 
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
    #endif
};


#endif   // DISABLE_TUNING - conditional compilation of this file
  
  
#endif // _WIN16GPIB_

      