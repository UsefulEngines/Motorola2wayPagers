//*************************************************************
//  File name: MDA.H
//
//  Description:  
//      Declaration file for the CHpMda class.
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
 
 
#ifndef _HPMDA_
#define _HPMDA_
 

#ifndef DISABLE_TUNING    // conditional compilation of this file

#include "..\..\cpager\ciooper.h"


/////////////////////////////////////////////////////////////////////
//  CHpMda
//
// 
class CHpMda : public CObject
{
  DECLARE_DYNAMIC(CHpMda)

  // attributes
  private:
    int			m_nDvm;					// gpib device descriptor
    BOOL		m_bDeviceConfigured; 
    int 		m_nPad, m_nSad, m_nTmo, m_nEot, m_nEos;
    Addr4882_t 	m_Addr4882_t;           // for NI-488.2M GPIB calls
       
    // protocol IO control elements     
    CString		m_szDisplayOnFailure;
       
  public:
    // constructors 
    CHpMda();
    CHpMda(CHpMda &rCHpMda);
    
    // destructor
    ~CHpMda();  
    
    // IO control functions
  private:
    int		ExecuteIoOpList(void);
    
    // operations
  private:
	int 	RegisterMdaError(UINT nstrResourceId, BOOL bCheckGpib = TRUE);
	    
  public:
    void 	OnOpFailure(const char *pszFailure = NULL);   
    
    // GPIB operations  
    int		ConfigureDevice(void);
    BOOL	IsDeviceConfigured(void) { return(m_bDeviceConfigured); } 
    int		GetDeviceDescriptor(void) { return(m_nDvm); } 
    int		GetPrimaryAddress(void) { return(m_nPad); }
    int		GetSecondaryAddress(void) { return(m_nSad); }
    void 	GetMdaError(CString &msg);
    int		Enable(BOOL bEnable); 
    
    // Common Instrument Commands 
    int		Reset(void);
    
    // Subsystem Commands 
    int 	ConfigureModBalLow(double dFreqCenter);
    int 	AnalyzeModBalLow(double *pdMeanAmplitude); 
    int 	ConfigureModBalHigh(void); 
    int		AnalyzeModBalHigh(double *pdMeanAmplitude);
    int		ConfigureSymbolDev(double dFreqCenter);
    int		AnalyzeSymbolDev(double *pdNeg24Delta, double *pdPos24Delta,
                             double *pdNeg8Delta, double *pdPos8Delta);
    int 	DisplayHistogram(BOOL bDisplay=TRUE);
   
  private:   
	int 	AnalyzeModBal(double *pdMean, double dTimePos1, 
                          double dTimePosDelta, int nNumTimeDivisions);
	int 	MeasureSymbolFreqDelta(double dFc, double dSymOffset, 
                                   char *pszBuf, double *pdSymDelta);
    
    // helper functions
  public:
	static	BOOL 	BoolRange(BOOL bVal);
	static	BOOL 	IntRange(int val, int min, int max);
	static	BOOL 	LongRange(long val, long min, long max);
	static	BOOL 	DoubleRange(double val, double min, double max);
    static	BOOL 	DoubleMinDelta(double val1, double val2, double delta);
    static  double  DoubleGetDelta(double val1, double val2);
    static  int 	IntGetDelta(int val1, int val2);
    
  public:
    #ifdef _DEBUG 
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
    #endif
    
    // IoOperation callback interface - see file 'ciooper.h'
    class IoCallBackInterface : public ICallBack
    {
      virtual int	OnIoOperationCallBack(int nCallBackId); 
      virtual int	OnIoOperationComplete(UINT nStatus, UINT nIoOpId); 
    } m_IoCallBackInterface;

    friend class IoCallBackInterface;
};


#endif   // DISABLE_TUNING - conditional compilation of this file
  
  
#endif // _HPMDA_

