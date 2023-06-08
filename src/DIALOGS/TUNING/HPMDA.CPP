//*************************************************************
//  File name: HPMDA.CPP
//
//  Description:  
//      Emplementation file for the CHpMda class.
//      
//      
//
//  History:    Date       Author               Comment
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************

#include "StdAfx.h" 
#include "math.h"   
#include "..\..\pps.h"
#include "..\..\cpager\ciooper.h"
#include "hpmda.h" 
#include "tune.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#ifndef DISABLE_TUNING    // conditional compilation of this file


extern CPpsApp 		    theApp; 
extern CIoController	theIoController;
extern CWin16Gpib	    theGpibPort;
 
IMPLEMENT_DYNAMIC(CHpMda, CObject)


#define BUFFER_SIZE		500		// 500 chars max write string


//  IO OPERATION IDENTIFIERS
// 

//  IOOP CALLBACK IDENTIFIERS
//


//  PROTOCOL IO OPERATION COMMAND ARRAYS and DATA BUFFERS
//








#ifdef _DEBUG
void CHpMda::AssertValid() const
{
    CObject::AssertValid();
}

void CHpMda::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc); 
}
#endif




// device response to IDN? query
static const char *pszHpMdaIDN = "HEWLETT-PACKARD,53310A"; // subset of actual

// *** Registry Info 
// Loaded from CHpMda::ConfigureDevice
// Saved from CHpMda::~CHpMda
static const char szMdaHeading[] = "MdaSettings";
static const char szGpibAddress[] = "GpibAddress";


// default constructor
CHpMda::CHpMda() 
{
  m_nDvm = 0;					// gpib device descriptor
  m_bDeviceConfigured = FALSE; 
  m_Addr4882_t = 0;
  m_nPad = 0;
  m_nSad = 0;
  m_nTmo = T3s;   // it may take some time to find and configure the MDA
  m_nEot = 1;
  m_nEos = 0;
}


// copy constructor
CHpMda::CHpMda(CHpMda &rCHpMda)
{
  m_nDvm = rCHpMda.m_nDvm;					// gpib device descriptor
  m_bDeviceConfigured = rCHpMda.m_bDeviceConfigured; 
  m_Addr4882_t = rCHpMda.m_Addr4882_t;
  m_nPad = rCHpMda.m_nPad;
  m_nSad = rCHpMda.m_nSad;
  m_nTmo = rCHpMda.m_nTmo;   
  m_nEot = rCHpMda.m_nEot;
  m_nEos = rCHpMda.m_nEos;
}
  



// destructor
CHpMda::~CHpMda() 
{ 
  // save the device address
  if (m_bDeviceConfigured)
    AfxGetApp()->WriteProfileInt(szMdaHeading, szGpibAddress, (UINT)m_Addr4882_t);
}       



/////////////////////////////////////////////////////////////////////////
//  ConfigureDevice()
//
int	CHpMda::ConfigureDevice(void)  
{ 
  TRACE("\nCHpMda::ConfigureDevice\n");
  
  theApp.BeginWaitCursor();
  
  // configure the gpib card if necessary
  if (theGpibPort.IsCardConfigured() == FALSE)
  {
    if (theGpibPort.ConfigureCard())
      return(RegisterMdaError(IDS_CHPMDA_CONFIG));
  }
  
  // initialize device address from saved .ini entry, if available
  m_Addr4882_t = (Addr4882_t)(AfxGetApp()->GetProfileInt(szMdaHeading, szGpibAddress, 0));
  
  // find the MDA on the GPIB bus. return on error.
  if (theGpibPort.FindDevice(pszHpMdaIDN, &m_Addr4882_t))
    return(RegisterMdaError(IDS_CHPMDA_FINDMDA));
  
  // open, configure, and enable the mda device 
  m_nPad = GetPAD(m_Addr4882_t);
  m_nSad = GetSAD(m_Addr4882_t); 
  if (theGpibPort.Open(m_nPad, m_nSad, m_nTmo, m_nEot, m_nEos, &m_nDvm))
    return(RegisterMdaError(IDS_CHPMDA_OPENMDA));
  
  // clear the device
  if (theGpibPort.Clear(m_nDvm))
    return(RegisterMdaError(IDS_CHPMDA_CLRMDA));
    
  m_bDeviceConfigured = TRUE;
  theApp.EndWaitCursor();
  return(0);  // no error detected
}
    

//////////////////////////////////////////////////////////////////////////
//  RegisterMdaError()
//
//  this function provides a simple 'on-error' return mechanizm which
//  simultaneously records any GPIB error along with the MDA error.
//
int CHpMda::RegisterMdaError(UINT nstrResourceId, BOOL bCheckGpib)
{
  CString	strTemp; 
  
  if (bCheckGpib) 
    theGpibPort.GetGpibError(m_szDisplayOnFailure); 
  // if the following assert fails, check the resource file for 
  // existence of the indicated string table resource.
  VERIFY(strTemp.LoadString(nstrResourceId));
  m_szDisplayOnFailure += strTemp;
  return(1);
}


/////////////////////////////////////////////////////////////////////////
//  GetMdaError()
//  
void CHpMda::GetMdaError(CString &msg)
{  
   msg = m_szDisplayOnFailure;
   return;
}
    


/////////////////////////////////////////////////////////////////////////
//  Reset()
//    
int	CHpMda::Reset(void)
{
  if (theGpibPort.Write(m_nDvm, "*RST;:CLE", 9L))
    return(RegisterMdaError(IDS_CHPMDA_RESETMDA));
  return(0);
}



/////////////////////////////////////////////////////////////////////////
//  Enable()
//    
int	CHpMda::Enable(BOOL bEnable)
{
   if (theGpibPort.EnableDevice(m_nDvm, bEnable))
   {
     theGpibPort.GetGpibError(m_szDisplayOnFailure);
     return(1);
   }
   return(0);
} 






////////////////////////////////////////////////////////////////////////
//  ConfigModBalLow()
// 
//  Call this function to prepare the MDA for Modulation Balance Low
//  signal analysis.  After the call, trigger the pager.  Be sure to
//  call ConfigureDevice and Reset prior to calling this function.
//
static char szConfigModBalLow[] = ":FUNC \"FREQ3\";"
								  ":CALC:HIST OFF;"
                                  ":SWE:TIME:DEL:LINK STAR;"
                                  ":TRIG:FREQ:MODE ENV;"
                                  ":MARK:TIME ON;"
                                  ":TRIG:AUTO OFF;"
                                  ":MARK:FREQ OFF;"
                                  ":CALC:ANAL:DEL1 MARK;"
                                  ":FREQ:RANG3 %fMHZ;"
                                  ":FREQ:RANG3:LOW %fMHZ;"
                                  ":SWE:TIME:SPAN 6.600000e-02;"
                                  ":SWE:TIME:DEL 0.007500;"
                                  ":FREQ:ARM:CFIN3 6.000000e-05;"
                                  ":INIT";
//                                   
int CHpMda::ConfigureModBalLow(double dFreqCenter)
{  
   TRACE("\nCHpMda::ConfigureModBalLow : dFc=%05lf\n", dFreqCenter);

   ASSERT(m_bDeviceConfigured == TRUE);

   double dUpperRange = dFreqCenter + 0.004;  // center + 4kHz
   double dLowerRange = dFreqCenter - 0.004;  // center - 4kHz
   char szWrtBuf[BUFFER_SIZE];
   
   // format the output string
   sprintf(szWrtBuf, szConfigModBalLow, dUpperRange, dLowerRange);
   TRACE("\tCHpMda::ConfigureModBalLow : szWrtBuf=%s\n", szWrtBuf);
   
   // output the MDA command string 
   long lStrLen = lstrlen(szWrtBuf);
   if (theGpibPort.Write(m_nDvm, szWrtBuf, lStrLen))
    return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
   return(0);
}



/////////////////////////////////////////////////////////////////////////
//  AnalyzeModBalLow()
// 
//  Call this function after the pager has been triggered in order to
//  analyze the MDA capture.  Be sure that the ConfigureModBalLow function
//  was called prior to the pager trigger.  
//
int CHpMda::AnalyzeModBalLow(double *pdMean)
{
  return(AnalyzeModBal(pdMean, 0.007500, 0.011, 6));
} 


/////////////////////////////////////////////////////////////////////////
//  AnalyzeModBalHigh()
// 
//  Call this function after the pager has been triggered in order to
//  analyze the MDA capture.  Be sure that the ConfigureModBalHigh function
//  was called prior to the pager trigger.  
//
int CHpMda::AnalyzeModBalHigh(double *pdMean)
{
  return(AnalyzeModBal(pdMean, 0.007500, 0.000833, 6));
} 



/////////////////////////////////////////////////////////////////////////
//  AnalyzeModBal()
//   
//  Called locally from AnalyzeModBalLow() and AnalyzeModBalHigh().
//
static char szAnalyzeModBalS0[] = ":CALC:ANAL:PTP ON;";
//                                  ":CALC:ANAL:DEL1 MARK;"
//                                  ":MARK:TIME ON";
                                  
static char szAnalyzeModBalS1[] = ":MARK:TIME:POS1 %f;"
                                  ":MARK:TIME:POS2 %f;"
                                  ":CALC:ANAL:PTP:RES?";
//
int CHpMda::AnalyzeModBal(double *pdMean, double dTimePos1, 
                          double dTimePosDelta, int nNumTimeDivisions)
{ 
  TRACE("\nCHpMda::AnalyzeModBal : dTimePos1=%f : dTimePosDelta=%f : #Div=%d\n",
        dTimePos1, dTimePosDelta, nNumTimeDivisions);

  ASSERT(m_bDeviceConfigured == TRUE);

  char 		szBuf[BUFFER_SIZE];
  double	*dFreqReadings = NULL;
  long 		lStrLen;
  int		lcv; 
  double	dDeviation = 0.0; 
  
  // prepare the MDA for peak to peak frequency measurements
  lStrLen = lstrlen(szAnalyzeModBalS0);
  TRACE("CHpMda::AnalyzeModBal : WRITE : %s\n", szAnalyzeModBalS0);
  if (theGpibPort.Write(m_nDvm, szAnalyzeModBalS0, lStrLen))
    return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
  
  // prepare to read time division peak-to-peak signal amplitudes.  
  dFreqReadings = new double[nNumTimeDivisions];
  ASSERT(dFreqReadings);
  
  // obtain the peak to peak frequency readings for each time division.
  // the time interval was established in the ConfigureModBalLow() call.
  for (lcv = 0; lcv < nNumTimeDivisions; ++lcv)
  {
    sprintf(szBuf, szAnalyzeModBalS1, (dTimePos1 + (lcv * dTimePosDelta)),
                                      (dTimePos1 + ((lcv+1) * dTimePosDelta)));
    TRACE("CHpMda::AnalyzeModBal : WRITE : szBuf=%s\n", szBuf);
    // output the MDA command string 
    lStrLen = lstrlen(szBuf);
    if (theGpibPort.Write(m_nDvm, szBuf, lStrLen))
    {
      delete [] dFreqReadings;
      return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
    }
    // read the MDA response
    if (theGpibPort.Read(m_nDvm, (BYTE *)szBuf, 25))
    {
      delete [] dFreqReadings;
      return(RegisterMdaError(IDS_CHPMDA_READMEAS)); 
    }
    TRACE("CHpMda::AnalyzeModBal : READ : szBuf=%s\n", szBuf);
    // convert the returned string to numeric data   
    dFreqReadings[lcv] = atof(szBuf);
  }  
  
  // calculate the sample average  
  *pdMean = 0.0;
  for (lcv = 0; lcv < nNumTimeDivisions; ++lcv)
  {
    *pdMean += dFreqReadings[lcv];
  }
  *pdMean = *pdMean / nNumTimeDivisions;
  // calculate standard deviation 
  for (lcv = 0; lcv < nNumTimeDivisions; ++lcv)
  {
    dDeviation += pow((dFreqReadings[lcv] - *pdMean), 2.0);
  }                 
  dDeviation = dDeviation / nNumTimeDivisions;
  dDeviation = sqrt(dDeviation);
  
  // test for good sampling.  The observed mean + deviation should be within
  // 1 sigma standard deviation of target mean (4800 Hz). 
  if ((*pdMean + dDeviation) > 6438.432)
  {
    sprintf(szBuf, "\nMDA : Modulation Balance : Mean=%.06fHz : Deviation=%.06fHz\n", *pdMean, dDeviation);
    m_szDisplayOnFailure += szBuf;
    delete [] dFreqReadings;
    return(RegisterMdaError(IDS_CHPMDA_BADMEAS, FALSE));
  }
  // clean up heap usage...
  delete [] dFreqReadings;
  return(0);
}



static char szConfigModBalHigh[] = ":SWE:TIME:SPAN 5.000000e-03;"
                                   ":FREQ:ARM:CFIN3 2.500000e-05;"
                                   ":INIT";
////////////////////////////////////////////////////////////////////////
//  ConfigModBalHigh()
// 
//  Call this function to prepare the MDA for Modulation Balance High
//  signal analysis.  After the call, trigger the pager.  The MDA should
//  have already been configured for ModBalLow, so not all options need
//  be re-configured.  
//
int CHpMda::ConfigureModBalHigh(void)
{  
   TRACE("\nCHpMda::ConfigureModBalHigh\n");

   ASSERT(m_bDeviceConfigured == TRUE);

   // output the MDA command string 
   long lStrLen = lstrlen(szConfigModBalHigh);
   if (theGpibPort.Write(m_nDvm, szConfigModBalHigh, lStrLen))
    return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));   
   return(0);
}




////////////////////////////////////////////////////////////////////////
//  ConfigSymbolDev()
// 
//  Call this function to prepare the MDA for Four-Level Symbol Deviation
//  signal analysis.  After the call, trigger the pager.  Be sure to
//  call ConfigureDevice and Reset prior to calling this function.
//
static char szConfigSymbolDev[] = ":FUNC \"FREQ3\";"
								  ":CALC:HIST ON;"
                                  ":SWE:TIME:DEL:LINK STAR;"
                                  ":CALC:ANAL:DEL2 MARK;"
                                  ":TRIG:AUTO OFF;"
                                  ":TRIG:FREQ:MODE ENV;"
                                  ":MARK:FREQ ON;"
                                  ":CALC:HIST:ACC OFF;"
                                  ":SWE:TIME:SPAN 2.000000e-02;"
                                  ":SWE:TIME:DEL 0.004500;"
                                  ":FREQ:ARM:CFIN3 1.000000e-04;"
                                  ":FREQ:RANG3 %fMHZ;"
                                  ":FREQ:RANG3:LOW %fMHZ;"
                                  ":CALC:ANAL:MEAN ON;"
                                  ":CALC:ANAL:PROB ON;"
                                  ":INIT";
//                                  
static double dExpectedCenterFreq;
//
int CHpMda::ConfigureSymbolDev(double dFreqCenter)
{  
   TRACE("\nCHpMda::ConfigureSymbolDev : dFc=%05lf\n", dFreqCenter);

   ASSERT(m_bDeviceConfigured == TRUE);

   double dUpperRange = dFreqCenter + 0.0035;  // center + 3.5kHz
   double dLowerRange = dFreqCenter - 0.0035;  // center - 3.5kHz
   char szWrtBuf[BUFFER_SIZE];
   
   // format the output string
   sprintf(szWrtBuf, szConfigSymbolDev, dUpperRange, dLowerRange);
   TRACE("\tCHpMda::ConfigureSymbolDev : szWrtBuf=%s\n", szWrtBuf);
   
   // output the MDA command string 
   long lStrLen = lstrlen(szWrtBuf);
   if (theGpibPort.Write(m_nDvm, szWrtBuf, lStrLen))
     return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
   dExpectedCenterFreq = dFreqCenter;   
   return(0);
}



/////////////////////////////////////////////////////////////////////////
//  AnalyzeSymbolDev()
//   
//
static char szAnalyzeSymbolDevS0[] = ":CALC:HIST OFF;"
                                     ":MARK:TIME ON;"
                                     ":CALC:ANAL:DEL1 MARK;"
                                     ":MARK:TIME:POS1 0.004500;"
                                     ":MARK:TIME:POS2 0.005400;"
                                     ":MARK:FREQ:POS1 %fMHZ;"
                                     ":MARK:FREQ:POS2 %fMHZ;"
                                     ":CALC:ANAL:MEAN:RES?";
//
static char szAnalyzeSymbolDevS1[] = ":MARK:TIME OFF;"
                                     ":CALC:HIST ON;"
                                     ":CALC:ANAL:DEL1 MARK";  // DEL2 ????
//                                     
static char szAnalyzeSymbolDevS2[] = ":MARK:FREQ:POS1 %fMHZ;"
									 ":MARK:FREQ:POS2 %fMHZ;"
									 ":CALC:ANAL:PROB:RES?";
//									 
static char szAnalyzeSymbolDevS3[] = ":CALC:ANAL:MEAN:RES?";
//
static char szAnalyzeSymbolDevS4[] = ":MARK:FREQ:POS1 %fMHZ;"
									 ":MARK:FREQ:POS2 %fMHZ;"
									 ":CALC:ANAL:MEAN:RES?";									 
//
int CHpMda::AnalyzeSymbolDev(double *pdNeg24Delta, double *pdPos24Delta,
                             double *pdNeg8Delta, double *pdPos8Delta)
{ 
  TRACE("\nCHpMda::AnalyzeSymbolDev\n");
  
  ASSERT(m_bDeviceConfigured == TRUE);

  char 		szBuf[BUFFER_SIZE];
  long 		lStrLen;
  double	dUpperRange, dLowerRange, dFcResult, dDelta;
  
  // First, test that the observed center frequency is near the
  // expected center frequency (as configured in ConfigureSymbolDev()). 
  dUpperRange = dExpectedCenterFreq + 0.0035;  // center + 3.5kHz
  dLowerRange = dExpectedCenterFreq - 0.0035;  // center - 3.5kHz  
  // construct the GPIB commands
  sprintf(szBuf, szAnalyzeSymbolDevS0, dUpperRange, dLowerRange);
  TRACE("\tCHpMda::AnalyzeSymbolDev : szBuf=%s\n", szBuf);
  lStrLen = lstrlen(szBuf); 
  // output the GPIB commands
  if (theGpibPort.Write(m_nDvm, szBuf, lStrLen))
    return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
  // read the MDA response
  if (theGpibPort.Read(m_nDvm, (BYTE *)szBuf, 25))
    return(RegisterMdaError(IDS_CHPMDA_READMEAS)); 
  TRACE("CHpMda::AnalyzeSymbolDev : READ : szBuf=%s\n", szBuf);
  // convert the returned string to numeric data   
  dFcResult = atof(szBuf);
  // test result, should be within 250 Hz
  dDelta = DoubleGetDelta(dFcResult, (dExpectedCenterFreq * 1.0e+06));  
  if (dDelta > 250.0) 
  {
    sprintf(szBuf, "\nMDA : Symbol Deviation : Expected Fc=%.06fMHz : Measured Fc=%.06fHz : Delta=%.06fHz\n",
            dExpectedCenterFreq, dFcResult, dDelta);
    m_szDisplayOnFailure += szBuf; 
    m_szDisplayOnFailure += "Reference Oscillator tuning may be required.\n";
    return(RegisterMdaError(IDS_CHPMDA_BADMEAS, FALSE));
  }
  
  // next, reconfigure the MDA for symbol levels measurement
  lStrLen = lstrlen(szAnalyzeSymbolDevS1);
  if (theGpibPort.Write(m_nDvm, szAnalyzeSymbolDevS1, lStrLen))
    return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));   
  
  // measure the four symbol level offsets from the observed center freq.
  if (MeasureSymbolFreqDelta(dFcResult, -0.0024, szBuf, pdNeg24Delta))  
     return(1);
  if (MeasureSymbolFreqDelta(dFcResult, +0.0024, szBuf, pdPos24Delta))  
     return(1);
  if (MeasureSymbolFreqDelta(dFcResult, -0.0008, szBuf, pdNeg8Delta))  
     return(1);
  if (MeasureSymbolFreqDelta(dFcResult, +0.0008, szBuf, pdPos8Delta))  
     return(1);
     
  return(0);
}


//////////////////////////////////////////////////////////////////////////
//  MeasureSymbolFreqDelta()
//
//  Helper function for AnalyzeSymbolDev.  
//
int CHpMda::MeasureSymbolFreqDelta(double dFcHz, double dSymOffset, 
                                   char *pszBuf, double *pdSymDelta)
{ 
  double dFcMhz = dFcHz / 1.0e+06; 
  double dSymCenter = dFcMhz + dSymOffset;
  double dUpperRange = dSymCenter + 0.0005;
  double dLowerRange = dSymCenter - 0.0005;
  double dProbability, dOuterFreqMean, dInnerFreqMean; 
  double dProbDelta, dOuterDelta, dInnerDelta;
  long lStrLen;
  
  // format the output string
  sprintf(pszBuf, szAnalyzeSymbolDevS2, dUpperRange, dLowerRange);
  TRACE("\tCHpMda::MeasureSymbolFreqDelta : szBuf=%s\n", pszBuf);
   
  // output the MDA command string 
  lStrLen = lstrlen(pszBuf);
  if (theGpibPort.Write(m_nDvm, pszBuf, lStrLen))
     return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
  
  // is the symbol freq probability close to expectation
  if (theGpibPort.Read(m_nDvm, (BYTE *)pszBuf, 25))
    return(RegisterMdaError(IDS_CHPMDA_READMEAS)); 
  TRACE("CHpMda::MeasureSymbolFreqDelta : READ : szBuf=%s\n", pszBuf);
  // convert the returned string to numeric data   
  dProbability = atof(pszBuf);
  // check for expected probability
  dProbDelta = DoubleGetDelta(dProbability, 20.0);
  if (dProbDelta > 5.0)
  {
    sprintf(pszBuf, "\nMDA : Symbol Deviation : FsymExpected=%.06fMHz : ProbMeas=%.06f : Delta=%.06f\n",
            dSymCenter, dProbability, dProbDelta); 
    m_szDisplayOnFailure += pszBuf;
    return(RegisterMdaError(IDS_CHPMDA_BADMEAS, FALSE));
  }
    
  // is the symbol freq outer mean close to expectation
  lStrLen = lstrlen(szAnalyzeSymbolDevS3);  
  TRACE("CHpMda::MeasureSymbolFreqDelta : WRITE : szBuf=%s\n", szAnalyzeSymbolDevS3);
  if (theGpibPort.Write(m_nDvm, szAnalyzeSymbolDevS3, lStrLen))
     return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
  // read the measurement
  if (theGpibPort.Read(m_nDvm, (BYTE *)pszBuf, 25))
    return(RegisterMdaError(IDS_CHPMDA_READMEAS)); 
  TRACE("CHpMda::MeasureSymbolFreqDelta : READ : szBuf=%s\n", pszBuf);
  // convert the returned string to numeric data   
  dOuterFreqMean = atof(pszBuf); 
  dOuterDelta = DoubleGetDelta(dOuterFreqMean, (dSymCenter * 1.0e+06));      
  if (dOuterDelta > 300.0)
  {
    sprintf(pszBuf, "\nMDA : Symbol Deviation : FsymExpected=%.06fMHz : FsymMean=%.06fHz : Delta=%.06fHz\n",
            dSymCenter, dOuterFreqMean, dOuterDelta); 
    m_szDisplayOnFailure += pszBuf;
    return(RegisterMdaError(IDS_CHPMDA_BADMEAS, FALSE));
  }
  
  // narrow the range and use the inner mean if better 
  dUpperRange = dSymCenter + 0.0002;
  dLowerRange = dSymCenter - 0.0002;
  // format the output string
  sprintf(pszBuf, szAnalyzeSymbolDevS4, dUpperRange, dLowerRange);
  TRACE("\tCHpMda::MeasureSymbolFreqDelta : szBuf=%s\n", pszBuf);   
  // output the MDA command string 
  lStrLen = lstrlen(pszBuf);
  if (theGpibPort.Write(m_nDvm, pszBuf, lStrLen))
     return(RegisterMdaError(IDS_CHPMDA_CONFMEAS));
  // read the measurement
  if (theGpibPort.Read(m_nDvm, (BYTE *)pszBuf, 25))
    return(RegisterMdaError(IDS_CHPMDA_READMEAS)); 
  TRACE("CHpMda::MeasureSymbolFreqDelta : READ : szBuf=%s\n", pszBuf);
  // convert the returned string to numeric data   
  dInnerFreqMean = atof(pszBuf); 
  dInnerDelta = DoubleGetDelta(dInnerFreqMean, (dSymCenter * 1.0e+06));      
  if (dInnerDelta > 300.0)
  {
    sprintf(pszBuf, "\nMDA : Symbol Deviation : FsymExpected=%.06fMHz : FsymMean=%.06fHz : Delta=%.06fHz\n",
            dSymCenter, dInnerFreqMean, dInnerDelta); 
    m_szDisplayOnFailure += pszBuf;
    return(RegisterMdaError(IDS_CHPMDA_BADMEAS, FALSE));
  }
  
  // choose the lesser of Outer or Inner Mean Delta
  if (dOuterDelta < dInnerDelta)
  {
    dInnerDelta = dOuterDelta;
    dInnerFreqMean = dOuterFreqMean;
  }
    
  // set the difference between observed and expected symbol frequency 
  // if Fmean is greater than Fsym, then need to decrease DAC setting.
  // if Fmean is less than Fsym, then need to increase DAC setting.
  if (dInnerFreqMean > (dSymCenter * 1.0e+06))
    *pdSymDelta = -(dInnerDelta);
  else
    *pdSymDelta = dInnerDelta;
  
  return(0);
}



///////////////////////////////////////////////////////////////////////// 
//  DisplayHistogram()
//
//  Switch between time display and histogram display.
//
static char szHistogramOff[] = ":CALC:HIST OFF;"
                               ":MARK:TIME ON;";
                              //
static char szHistogramOn[] = ":MARK:TIME OFF;"
                              ":CALC:HIST ON;"; 
                              //
int CHpMda::DisplayHistogram(BOOL bDisplay)
{
  TRACE("\nCHpMda::DisplayHistogram\n");  
  ASSERT(m_bDeviceConfigured == TRUE); 
  
  char *pszOutput = NULL;
  if (bDisplay == TRUE)   // turn histogram display on
    pszOutput = szHistogramOn;
  else
    pszOutput = szHistogramOff;
  
  long lStrLen = lstrlen(pszOutput); 
  // output the GPIB commands
  if (theGpibPort.Write(m_nDvm, pszOutput, lStrLen))
    return(RegisterMdaError(IDS_CHPMDA_CONFDISPLAY));  
  return(0);
}





/////////////////////////////////////////////////////////////////////////
//  ExecuteIoOpList()
//
//  The sequence of IoOperations is initiated.
//
int CHpMda::ExecuteIoOpList(void)
{ 
  TRACE("\nCHpMda::ExecuteIoOpList\n");
  
  if (IsDeviceConfigured() == FALSE)
  {
    if (ConfigureDevice())  
      return(1);                
  }
  
  return(theIoController.ExecuteIoOpList());
}


/////////////////////////////////////////////////////////////////////
//  OnOpCallBack()
//
//  This function recieves a callback from the currently executing
//  IoOperation when an IO_CALLBACK command is encountered.  The
//  nCallBackId parameter indicates the reason for the callback
//  which may be an intermediate verification of data received in
//  a previous IoCommand.  The function returns 0 on success or
//  a 1 on error.  
int CHpMda::IoCallBackInterface::OnIoOperationCallBack(int nCallBackId)
{
  TRACE("\nCHpMda::OnOpCallBack : nCallBackId=%d", nCallBackId);
  METHOD_PROLOGUE(CHpMda, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')
  
  /*
  switch(nCallBackId)
  {  
    case IS_HANDSHAKE_ACKED:
      if (pThis->HandleIsHandshakeAcked())
        return(1);
      break;
        
        
    default:
      ASSERT(0);
      break;
  }
  */
  return(0);
}


////////////////////////////////////////////////////////////////////
//  IoCallBackInterface::OnIoOperationComplete()
//
//  This function receives status notification from
//  the current CIoController::OnIoOpCompletion() function upon
//  completion of the current IoOperation.  The function returns 0 
//  on success and a 1 on error.  If 1 is returned, then the current
//  list of IoOperations is deleted by the CIoController and a
//  WM_IO_OPLIST_DONE failed message is generated. 
//  
int CHpMda::IoCallBackInterface::OnIoOperationComplete(UINT nStatus, UINT nIoOpId) 
{
  TRACE("\nCPegasus::IoCallBackInterface::OnIoOperationComplete : nStatus=%04X : "
          "nIoOpId=%04X\n", nStatus, nIoOpId);

  METHOD_PROLOGUE(CHpMda, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')
  
  // handle op completion based on IoOperation nIOOPid indicator
  /*
  switch(nIoOpId)
  {
    
    default:	
      // move on to next IoOperation in sequence
      break;
  }
  */
  
  return(nStatus); 
}



////////////////////////////////////////////////////////////////////
//  OnOpFailure()
//
//  Provides a central function for notifying the user of a
//  application/Mda interface failure.
//
void CHpMda::OnOpFailure(const char *pszFailString)
{ 
  TRACE("\nCHpMda::OnOpFailure : %s\n", pszFailString);

  if (pszFailString != NULL)  
    m_szDisplayOnFailure += pszFailString; 

  DWORD dwHelpId = HID_BASE_RESOURCE + IDP_MDA_GPIB_ERROR;    
  AfxMessageBox(m_szDisplayOnFailure, 
                MB_ICONINFORMATION | MB_APPLMODAL | MB_OK,
                (UINT)dwHelpId); 
  m_szDisplayOnFailure.Empty(); 
  
  // reset device configuration state. 
  m_bDeviceConfigured = FALSE;
  return;
}



//===========================================================================
// Function: Boolean Value Out Of Range - ViBoolean                          */
//===========================================================================
BOOL CHpMda::BoolRange(BOOL bVal)
{
    return ((bVal != FALSE && bVal != TRUE) ? TRUE : FALSE);
}

//===========================================================================
// Function: Signed Integer Value Out Of Range 							     */
//===========================================================================
BOOL CHpMda::IntRange(int val, int min, int max)
{
    return ((val < min || val > max) ? TRUE : FALSE);
}

//===========================================================================
// Function: Long Signed Integer Value Out Of Range 		                 */
//===========================================================================
BOOL CHpMda::LongRange(long val, long min, long max)
{
    return ((val < min || val > max) ? TRUE : FALSE);
}

//===========================================================================
// Function: Real (Double) Value Out Of Range 			                     */
//===========================================================================
BOOL CHpMda::DoubleRange(double val, double min, double max)
{
    return ((val < min || val > max) ? TRUE : FALSE);
} 


//===========================================================================
// Function: Real (Double) Values within minimum difference                 */
//===========================================================================
BOOL CHpMda::DoubleMinDelta(double val1, double val2, double delta)
{ 
    double dDiff = fabs(val1 - val2);
    return((dDiff <= delta) ? TRUE : FALSE);
} 
    

//===========================================================================
// Function: Real (Double) Values within minimum difference                 */
//===========================================================================
double CHpMda::DoubleGetDelta(double val1, double val2)
{ 
    return(fabs(val1 - val2));
} 


//===========================================================================
// Function: Real (Double) Values within minimum difference                 */
//===========================================================================
int CHpMda::IntGetDelta(int val1, int val2)
{ 
    return(abs(val1 - val2));
} 



#endif   // DISABLE_TUNING - conditional compilation of this file


 