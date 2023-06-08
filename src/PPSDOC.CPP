//*************************************************************
//  File name: PPSDOC.CPP
//
//  Description:  
//    Implementation file for class CPpsDoc.
//      
//      
//
//  History:    Date       Author               Comment
//              3/08/96    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************

#include "stdafx.h"
#include "pps.h"
#include "ppsdoc.h" 
#include "mainfrm.h"
#include "cpager\cpager.h"
#include "cpager\codeplug.h"
#include "dialogs\dwnldpwd\dwnldpwd.h"

extern CPpsApp			theApp;		  // see files pps.h and pps.cpp


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModificationRecord

IMPLEMENT_DYNCREATE(CModificationRecord, CObject)

// constructor
CModificationRecord::CModificationRecord(int Index, int NumBytes, CByteArray &rSource) 
{ 
  // Call this constructor to record not only the Delta index and 
  // numbytes but also the changed bytes as well.  This mechanism 
  // is used herein to support the modification record-playback
  // utility menu option.
  ASSERT((Index+NumBytes) <= rSource.GetSize());
  if ((Index+NumBytes) <= rSource.GetSize()) 
  {   
    nIndex = Index;  
    nNumBytes = NumBytes;
    for (int lcv = 0; lcv < NumBytes; ++lcv)
    {  
      byRecordedDelta.Add(rSource[Index + lcv]);
    }
  }
  else
  {
    nIndex = 0;
    nNumBytes = 0;
  }           
} 

// copy constructor
CModificationRecord::CModificationRecord(CModificationRecord &rCModificationRecord)
{
  nIndex = rCModificationRecord.nIndex;
  nNumBytes = rCModificationRecord.nNumBytes;
  for (int lcv = 0; lcv < nNumBytes; ++lcv)
  {
    byRecordedDelta.Add(rCModificationRecord.byRecordedDelta[lcv]);
  }
}     


// operator overloads
const CModificationRecord &CModificationRecord::operator=(const CModificationRecord &rModRec)
{
  nIndex = rModRec.nIndex;
  nNumBytes = rModRec.nNumBytes;
  for (int lcv = 0; lcv < nNumBytes; ++lcv)
  {
    byRecordedDelta.Add(rModRec.byRecordedDelta[lcv]);
  } 
  return(*this);
}     


#ifdef _DEBUG
void CModificationRecord::AssertValid() const
{
	CObject::AssertValid();
}


void CModificationRecord::Dump(CDumpContext& dc) const
{
    TRACE("\nCModificationRecord::Dump\n");
	CObject::Dump(dc); 
	dc << "\n\t" << nIndex << "\t" << nNumBytes;
}
#endif //_DEBUG




/////////////////////////////////////////////////////////////////////////////
// CPpsDoc

IMPLEMENT_DYNCREATE(CPpsDoc, CDocument)

BEGIN_MESSAGE_MAP(CPpsDoc, CDocument)
	//{{AFX_MSG_MAP(CPpsDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_PAGER_WRITE, OnPagerWrite)
	ON_UPDATE_COMMAND_UI(ID_PAGER_WRITE, OnUpdatePagerWrite)
	ON_COMMAND(ID_PAGER_READ, OnPagerRead)
	ON_UPDATE_COMMAND_UI(ID_PAGER_READ, OnUpdatePagerRead)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_UTILITY_OVERLAY, OnUtilityOverlay)
	ON_UPDATE_COMMAND_UI(ID_UTILITY_OVERLAY, OnUpdateUtilityOverlay)
	ON_COMMAND(ID_FILE_SAVEAS_DEFAULT, OnFileSaveasDefault)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS_DEFAULT, OnUpdateFileSaveasDefault)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_TOGGLE_RESETS, OnToggleResets)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_COMMAND(ID_UTILITY_DELTA_ONOFF, OnUtilityRecordingOnOff)
	ON_UPDATE_COMMAND_UI(ID_UTILITY_DELTA_ONOFF, OnUpdateUtilityRecordingOnOff)
	ON_COMMAND(ID_UTILITY_DELTA_PLAY, OnUtilityRecordingPlay)
	ON_UPDATE_COMMAND_UI(ID_UTILITY_DELTA_PLAY, OnUpdateUtilityRecordingPlay)
	ON_COMMAND(ID_UTILITY_DELTA_ERASE, OnUtilityRecordingErase)
	ON_UPDATE_COMMAND_UI(ID_UTILITY_DELTA_ERASE, OnUpdateUtilityRecordingErase)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_UTILITY_DELTA_REPEAT, OnUtilityDeltaRepeat)
	ON_UPDATE_COMMAND_UI(ID_UTILITY_DELTA_REPEAT, OnUpdateUtilityDeltaRepeat)
	ON_COMMAND(ID_KEY_OWNER, OnKeyOwner)
	ON_COMMAND(ID_KEY_USER, OnKeyUser)
	ON_UPDATE_COMMAND_UI(ID_KEY_OWNER, OnUpdateKeyOwner)
	ON_UPDATE_COMMAND_UI(ID_KEY_USER, OnUpdateKeyUser)
	ON_COMMAND(ID_KEY_FORGET, OnKeyForget)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CPpsDoc diagnostics

#ifdef _DEBUG
void CPpsDoc::AssertValid() const
{
	CDocument::AssertValid();
}


void CPpsDoc::Dump(CDumpContext& dc) const
{
    TRACE("\nCPpsDoc::Dump\n");
	CDocument::Dump(dc);  

    // NOTE: enable the depth indicator in the constructor above for full debug.	
	dc << "\nm_byCodePlugArray.Size = " <<  m_byCodePlugArray.GetSize() 
	   << "\nm_byCodePlugArray.UpperBound = " << m_byCodePlugArray.GetUpperBound();	   
	dc << "\n" << m_byCodePlugArray << "\n";
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CPpsDoc construction/destruction

CPpsDoc::CPpsDoc()
{
    m_byCodePlugArray.SetSize(0, theApp.GetThePager()->GetCodePlugSize());  // initial size, grow by
    m_byDefaultCodePlugArray.SetSize(0, theApp.GetThePager()->GetCodePlugSize());
    m_byOriginalCodePlugArray.SetSize(0, theApp.GetThePager()->GetCodePlugSize());
    m_byArchivedCodePlugArray.SetSize(0, theApp.GetThePager()->GetCodePlugSize());
    m_byCpVersion = 0x00;
    bResetEnabled = TRUE;
    bRecordingEnabled = FALSE;
    m_nReadPlayWriteState = AUTO_IDLE;
    m_bAllowEsnFixes = FALSE;
    m_bEnablePatchSecurity = TRUE;
        
    // this will take time to display when debugging, so
    // uncomment as necessary.
    #ifdef _DEBUG
    // afxDump.SetDepth(1);     // uncomment to ensure dump of all array elements
    #endif
}



CPpsDoc::~CPpsDoc()
{ 
  OnUtilityRecordingErase();   // free any recorded modifications
  DeleteContents();		       // free memory
}


UINT CPpsDoc::GetDocBaseAddress(void) 
{ 
  return(theApp.GetThePager()->GetCodePlugBaseAddress()); 
} 



/////////////////////////////////////////////////////////////////
//  OnNewDocument()
//
//  Parameters:
//		None.
// 
//  Comments:
//      TRUE:  Document was successfully loaded.
//      FALSE: Document was unsuccessfully loaded!
//
//
BOOL CPpsDoc::OnNewDocument()
{   
  TRACE("\nCPpsDoc::OnNewDocument\n"); 
  static int nFirstTimeFlag = 1;
  
  if (nFirstTimeFlag == 0)
    return(CDocument::OnNewDocument());

    // The first call to this function is when the CPpsDoc is instantiated
    // from within the CPpsApp::InitInstance function.  It is necessary that
    // no action be taken during this call since the main program elements
    // (e.g. MainFrame window) have not yet been instantiated.  The calls
    // to thePager object herein depend upon the MainFrame window being
    // instantiated and assigned an HWND.  The serial port cannot be configured
    // until a window is instantiated which serves to receive the WM_COMMNOTIFY
    // messages.

  if (nFirstTimeFlag)
    nFirstTimeFlag = 0; 
    
  return(TRUE);
}


/////////////////////////////////////////////////////////////////////////
//  OnWmIoOperationDone()
//
//  Called from CMainFrame::OnWmIoOpListDone() in response to any
//  device IO operation list completion.
//
void CPpsDoc::OnWmIoOpListDone(WPARAM wParam, LPARAM lParam)
{ 
  if (wParam)   // Failure... 
  {
    m_nReadPlayWriteState = AUTO_IDLE;  // abort any automatic read, play, write
  }
    
  if (m_nReadPlayWriteState == AUTO_READ)  // initiate next step
  {
    m_nReadPlayWriteState = AUTO_PLAY;
    OnUtilityRecordingPlay(); 
  }
  
  if (m_nReadPlayWriteState == AUTO_PLAY)  // initiate next step
  {
    m_nReadPlayWriteState = AUTO_WRITE;
    OnPagerWrite();  // will be set to AUTO_IDLE after all writes done.
  }
  
  return;   
}



///////////////////////////////////////////////////////////////////////////
//  OnPagerRead()
//
//  This function initiates a read of the codeplug from the 
//  pager. 
void CPpsDoc::OnPagerRead()
{
    TRACE("\nCPpsDoc::OnPagerRead\n");
    if (theApp.GetThePager()->InitiateRead())
      AfxMessageBox ("Device-Read Operation Aborted.\n",
                     MB_OK | MB_ICONHAND); 
    return;
}

void CPpsDoc::OnUpdatePagerRead(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
   pCmdUI->SetCheck(theApp.GetThePager()->IsReadRequested());
}



////////////////////////////////////////////////////////////////////////////
//  OnWmReadCodeplugDone()
//
//  Message handler for the WM_READ_CP_DONE message.
//    wParam = 0 if successful complete; 1 if failed complete
//    lParam = not used 
//
static char *szVersionPrompt = "This application does not support the\n"
    				 		   "indicated device and/or codeplug version.\n"
    				 		   "Please contact customer service in order to\n"
    				 		   "obtain the latest software release.\n\n"
    				 		   "Indicated device/version = %s-%02X / %02X\n";
static char *szR50 = "R50";
static char *szR25 = "R25";
//
void CPpsDoc::OnWmReadCodeplugDone(UINT wParam, LONG lParam)
{
  TRACE("\nCPpsDoc::OnWmUserReadCodeplugDone : wParam=%04X "
          "lParam=%lX\n", wParam, lParam);
  
  // NOTE:  An intermittent bug exists with the Centaur family of
  // pagers in which a reset after read may cause the pager to not
  // subsequently communicate with the PPS.  Until the bug is 
  // resolved by the Host Developers, then avoid resetting the
  // pager after reads.
  //
  //if (bResetEnabled)  // lab-use only flag, normally always TRUE             
  //  theApp.GetThePager()->DoSoftwareReset(1);
  
  if (wParam)    // return on failure... 
  {
    m_nReadPlayWriteState = AUTO_IDLE;  // abort any automatic read, play, write
    return;      // see pegasus.cpp and pager.cpp 
  }
   
  CPager *pThePager = theApp.GetThePager();
  ASSERT(pThePager);
  
  // delete current document contents get new from pager. 
  DeleteContents();
  pThePager->GetPagerCodePlug(m_byCodePlugArray);
  
  // check for previously unprogrammed pager
  if (pThePager->IsCodePlugUnprogrammed())
  {
    AfxMessageBox("The device is currently unprogrammed.\n\n"
                  "First, open and modify an existing configuration\n"
                  "file and then initiate a 'Device-Write' operation.\n", 
                   MB_OK | MB_ICONINFORMATION);                       
    DeleteContents();
    ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarShort();
    return;
  }
    
  // NOTE:  0x0E and 0x10 must remain constant with each codeplug version.
  BYTE byPreviousCpVer = m_byCpVersion;
  BYTE byPreviousDevId = m_byDeviceId;
  Field(0x0E, 1, &m_byCpVersion);
  Field(0x10, 1, &m_byDeviceId);
  BOOL bReflex50Enabled = IsReflex50Enabled();      
  
  // Forget any recorded modifications if new device/cp_version
  if ((byPreviousCpVer != m_byCpVersion) || (byPreviousDevId != m_byDeviceId))
    OnUtilityRecordingErase(); 
   
  // Select the view set associated with this device.
  theApp.SelectViewSet(m_byDeviceId, m_byCpVersion, bReflex50Enabled);
   
  // check for any possible application/pager version mismatches
  if ((theApp.IsPagerSupported(m_byDeviceId, m_byCpVersion, bReflex50Enabled)) == FALSE) 
  {
    char  cBuffer[400]; 
    if (bReflex50Enabled)
      sprintf(cBuffer, szVersionPrompt, szR50, m_byDeviceId, m_byCpVersion);
    else
      sprintf(cBuffer, szVersionPrompt, szR25, m_byDeviceId, m_byCpVersion); 
    AfxMessageBox (cBuffer, MB_OK | MB_ICONHAND);
    if (((CMainFrame *)(theApp.m_pMainWnd))->IsAccessRestricted())
    {
      DeleteContents();
      ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarShort(); 
      return;
    }
    else  // allow lab users to use hexedit and commview
      ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarLabUseOnly(); 
  }
  else  // no error, allow full user view
    ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarLong();

  // copy original for undo's
  for (int lcv = 0; lcv < m_byCodePlugArray.GetSize(); ++lcv)
    m_byOriginalCodePlugArray.Add(m_byCodePlugArray[lcv]);
  
  // retrieve default codeplug data if available 
  ImportDefaultCodeplugData();
  
  // change the document path and title.  
  #ifdef _DEBUG
  SetPathName("codeplug.s19");
  #else
  SetPathName("");
  #endif
  SetTitle("Codeplug");
   
  // validate the PA/ESN pair for Reflex50 devices.
  if (bReflex50Enabled == TRUE) 
    AutoValidateEsn();
       
  // notify the views of document update
  UpdateAllViews(NULL, 0L, NULL);  
  ::MessageBeep(MB_OK);      // give user a completion indicator  
  return;
}


////////////////////////////////////////////////////////////////////////
//  AutoValidateEsn()
//
//  Some devices may have shipped with bad PA/ESN pairs.  This function
//  allows the PPS user to fix the values if they have authorization.
//  Otherwise, the user is just warned of the condition.
//
void CPpsDoc::AutoValidateEsn(void)
{
    DWORD dwPersAddress, dwOrigESN, dwCalcESN;
    CP_Personal_addrs.dwSetFromDoc(&dwPersAddress);
    CP_Electronic_Serial_Number.dwSetFromDoc(&dwOrigESN);
    dwCalcESN = GenerateEsn(dwPersAddress);
    if (dwOrigESN != dwCalcESN)  
    {
      DWORD dwHelpID = HID_BASE_RESOURCE;   
      if (m_bAllowEsnFixes == TRUE)
      {
        dwHelpID += ID_ESNVALFIX;   
        int iReturn = AfxMessageBox(ID_ESNVALFIX, MB_YESNO | MB_ICONINFORMATION, (UINT)dwHelpID);
        if (iReturn == IDYES)
        { 
          // temporarily disable recording
          BOOL bTemp = bRecordingEnabled;  
          bRecordingEnabled = FALSE;
          // also, ensure the personal base frame and home control channel
          // are valid
          BYTE byPersonalBaseFrame = (BYTE)(dwPersAddress % 128);
          BYTE byHomeLogicalNum = (BYTE)(dwPersAddress % 12 + 1);
          CP_PagerFrame.bySetToDoc(byPersonalBaseFrame);
          CP_HomeLogicalNum.bySetToDoc(byHomeLogicalNum);          
          CP_Electronic_Serial_Number.dwSetToDoc(dwCalcESN); 
          bRecordingEnabled = bTemp;  // reset recording flag
        }
      }
      else
      {
       dwHelpID += ID_ESNVALWARN;   
       AfxMessageBox(ID_ESNVALWARN, MB_OK | MB_ICONINFORMATION, (UINT)dwHelpID);
      }
    }
    return;
}


static char *szIniSection = "Validation";
static char *szIniEntry = "Owner";
static char *szIniDefault = "None";

/////////////////////////////////////////////////////////////////
// OnKeyOwner()
//
// The user will enter an encrypted string which corresponds to
// the user password.  The PPS stores the key into the .ini file.
//
// User must press CTRL+ALT+F5 to register authorization key.
//
void CPpsDoc::OnKeyOwner()
{
  CDwnLdPwd dlg;
  int ret;
  
  do 
  {
    ret = dlg.DoModal("Enter the ESN modify authorization key.");
    if (ret == IDCANCEL)
      break;
    if (dlg.m_Edit1 != dlg.m_Edit2)
    {
      AfxMessageBox("The two password entries must compare exactly.", 
      			    MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);      
      continue;
    } 
    if (dlg.m_Edit1.GetLength() > 0)
      AfxGetApp()->WriteProfileString(szIniSection, szIniEntry, (LPCTSTR)dlg.m_Edit1);
    break;
  }
  while(1); 
  return;	
}


/////////////////////////////////////////////////////////////////
// OnKeyUser()
//
// The user will enter an unencrypted string which corresponds to
// the user password.  The PPS encrypts the string and compares
// it with a previously entered encrypted string which has been
// stored in the .ini file.  ESN fixes are based upon the two 
// strings matching. 
//
// User must press CTRL+F5 to login user password.
//
void CPpsDoc::OnKeyUser()
{
  CDwnLdPwd dlg;
  int ret;
  
  do 
  {
    ret = dlg.DoModal("Enter your ESN modify authorization password.");
    if (ret == IDCANCEL)
      break;
    if (dlg.m_Edit1 != dlg.m_Edit2)
    {
      AfxMessageBox("The two password entries must compare exactly.", 
      			    MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);      
      continue;
    } 
    if (dlg.m_Edit1.GetLength() > 0)     
    {
      // compare with stored .ini entry
      CString strText;
      strText = AfxGetApp()->GetProfileString(szIniSection, szIniEntry);
      if (!strText.IsEmpty())  
      {
        // compare encoded values... 
        char cBuffer[100];
        PasswordToKey((LPCTSTR)dlg.m_Edit1, cBuffer);
        if (strText.Compare(cBuffer) == 0)  
          m_bAllowEsnFixes = TRUE;
        else
          m_bAllowEsnFixes = FALSE;
      }
    }
    break;
  }
  while(1); 
  return;	
}


void CPpsDoc::OnUpdateKeyOwner(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
  return;	
}

void CPpsDoc::OnUpdateKeyUser(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
  return;	
}


//////////////////////////////////////////////////////////////////
//  OnKeyForget()
//
//  Press Ctrl+Alt+Shift+F5 to delete the stored encrypted password
//  in the .ini file.
//
void CPpsDoc::OnKeyForget()
{ 
  // delete the encoded password from the .ini file.
  AfxGetApp()->WriteProfileString(szIniSection, szIniEntry, NULL);
  return;	
}



DWORD CPpsDoc::GenerateEsn(DWORD dwPersAddr)
{
  DWORD	dwEsn, dwTemp;
  
  dwEsn = dwPersAddr;
  for (int lcv = 0; lcv < 25; ++lcv)
  {
    dwTemp = (((dwEsn & 0x20000000) >> 29) ^
               ((dwEsn & 0x20) >> 5) ^
               ((dwEsn & 0x08) >> 3) ^
               (dwEsn & 0x01));
    dwEsn = ((dwEsn << 1) | dwTemp) & 0x3FFFFFFF;
  }
  return(dwEsn);
}


BOOL CPpsDoc::IsReflex50Enabled(void)
{
  BOOL bR50Enable = FALSE;
  CP_reflex50_enable.bSetFromDoc(&bR50Enable); 
  return(bR50Enable);
}



BOOL CPpsDoc::IsReflex25Enabled(void)
{ 
  BOOL bR50Enable = FALSE;
  CP_reflex50_enable.bSetFromDoc(&bR50Enable); 
  return(!bR50Enable);
}




/////////////////////////////////////////////////////////////////////////////
// ImportDefaultCodeplugData()
//
// This function is called upon pager reads and/or file opens.  If the 
// indicated pager codeplug version has a corresponding codeplug S-record
// file, then the file is uploaded into the byDefaultCodeplugArray.
//
void CPpsDoc::ImportDefaultCodeplugData(void)
{
  const char *pFileName = NULL;
  BOOL		bR50Enable;

  // query the pager for the default codeplug file name.  
  CP_reflex50_enable.bSetFromDoc(&bR50Enable);
  pFileName = theApp.GetDefaultS19FileName(m_byDeviceId, m_byCpVersion, bR50Enable);

  if (pFileName == NULL)   // default .s19 filename not available
    return;
    
  m_strDefaultS19FileName.Empty();
  m_byDefaultCodePlugArray.RemoveAll();
  
  if (LoadFileToByteArray(pFileName, m_byDefaultCodePlugArray)) 
  { 
    m_byDefaultCodePlugArray.RemoveAll();
    return;
  }
  
  // check for size mismatch
  if (m_byDefaultCodePlugArray.GetSize() != (theApp.GetThePager()->GetCodePlugSize()))
  {
    m_byDefaultCodePlugArray.RemoveAll();
    return;
  }
  
  // check for pager and codeplug version mismatch
  BYTE byPagerId = m_byDefaultCodePlugArray[CP_OTA_Protected_H.Offset()];
  BYTE byCodeplugVersion = m_byDefaultCodePlugArray[CP_Version.Offset()];  
  if ((byPagerId != m_byDeviceId) || (byCodeplugVersion != m_byCpVersion))
  {
    m_byDefaultCodePlugArray.RemoveAll();
    return;
  }                        
  
  // construct the defaults file path name 
  m_strDefaultS19FileName = theApp.GetAppPathName();
  m_strDefaultS19FileName += "\\";
  m_strDefaultS19FileName += pFileName;
  //AfxMessageBox(m_strDefaultS19FileName); 
  return;   
}




/////////////////////////////////////////////////////////////////////////////
// CPpsDoc serialization 
//
//  see comments below 'OnOpenDocument'
//
void CPpsDoc::Serialize(CArchive& ar)
{
    TRACE("Entering CPpsDoc::Serialize\n");
    
	if (ar.IsStoring())
	{   
	   // TODO:  add storing code here
	}
	else
	{
	   // TODO:  add loading code here
	}  
}




/////////////////////////////////////////////////////////////////////////////
//  DeleteContents()
//
void CPpsDoc::DeleteContents()     
{
  TRACE("CPpsDoc::DeleteContents\n");
  #ifdef _DEBUG
    Dump(afxDump);
  #endif
   
  ForgetModifications();
  bRecordingEnabled = FALSE; 
   
  if (m_byCodePlugArray.GetSize())  
    m_byCodePlugArray.RemoveAll();
  if (m_byDefaultCodePlugArray.GetSize())  
    m_byDefaultCodePlugArray.RemoveAll();
  if (m_byOriginalCodePlugArray.GetSize())  
    m_byOriginalCodePlugArray.RemoveAll(); 
  if (m_byArchivedCodePlugArray.GetSize())  
    m_byArchivedCodePlugArray.RemoveAll(); 
  return;
} 




/////////////////////////////////////////////////////////////////
//  OnOpenDocument() : Loads a text S-Record file into the document
//
//  Parameters: 
//      pszPathName - points to the path of the document to be
//                    opened.
//  Comments:  
//      Files of up to SREC_SIZE lines can be read.  Override of 
//      CDocument::OnOpenDocument().  The serialization 
//      mechanism is not used because it supports only read/write
//      of binary (non-ASCII) data.
//
//  Return: (BOOL)
//      TRUE:  Document was successfully loaded.
//      FALSE: Document was unsuccessfully loaded!
//
//  History:    Date       Author     Comment
//              3/08/96    PGP        Created
//
BOOL CPpsDoc::OnOpenDocument(const char *pszPathName)
{
   TRACE("\nCPpsDoc::OnOpenDocument - path = %s\n", pszPathName);
   const int c_cBufferSize = 200; 
   const int c_cMaxString  = SREC_SIZE;   // max number of lines in SRec file  
   char szBuffer[c_cBufferSize]; 
   CStdioFile f;
   CFileException fe;

   if(!f.Open(pszPathName, CFile::modeRead | CFile::typeText))
   {
      ReportSaveLoadException(pszPathName, &fe,
         FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
      return FALSE;
   } 
   
   DeleteContents();
    
   TRY
   {
      BeginWaitCursor(); 
      int cString = 0;           
      while(f.ReadString(szBuffer, c_cBufferSize)) 
      {
         
         if (cString++ == c_cMaxString)
         {
            // document is too long.
            // display warning message and don't load the remainder   
            AfxMessageBox ("Error - Document is too large.", MB_OK | MB_ICONEXCLAMATION);                       
            break;
         }
        
         int l;
         // if the line ends with a newline character, replace with
         // null terminator.      
         if (szBuffer[l=lstrlen(szBuffer)-1] == '\n')
            szBuffer[l--] = '\0';            
         // trim off trailing blanks
         while (szBuffer[l] == ' ')
            szBuffer[l--] = '\0';
         
         // translate the possible S1 Record to the CByteArray
         if (S1RecordImport(szBuffer, m_byCodePlugArray))
         {
           ReportSaveLoadException(pszPathName, &fe,
                                   FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
           break;
         }      
      }      
      f.Close();      
   } // TRY
   CATCH_ALL(e)
   {
      f.Abort();
      DeleteContents();      
      EndWaitCursor(); 
      
      TRY
         ReportSaveLoadException(pszPathName, e,
            FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
      END_TRY
      return FALSE;
   }
   END_CATCH_ALL 
   
   EndWaitCursor();

   if (m_byCodePlugArray.GetSize() != (theApp.GetThePager()->GetCodePlugSize()))
   {
     DeleteContents();
     ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarShort();
     AfxMessageBox ("Error - Document does not match required codeplug size.", MB_OK | MB_ICONEXCLAMATION);
     return FALSE;
   }                       

   // NOTE:  0x0E and 0x10 must remain constant with each codeplug version.
   BYTE byPreviousCpVer = m_byCpVersion;
   BYTE byPreviousDevId = m_byDeviceId;
   Field(0x0E, 1, &m_byCpVersion);
   Field(0x10, 1, &m_byDeviceId);  
   BOOL bReflex50Enabled = IsReflex50Enabled();      
   
   // Forget any recorded modifications if new device/cp_version
   if ((byPreviousCpVer != m_byCpVersion) || (byPreviousDevId != m_byDeviceId))
     OnUtilityRecordingErase(); 

   // Select the view set associated with this device.
   theApp.SelectViewSet(m_byDeviceId, m_byCpVersion, bReflex50Enabled);
   
   // check for any possible application/pager version mismatches
   if ((theApp.IsPagerSupported(m_byDeviceId, m_byCpVersion, bReflex50Enabled)) == FALSE) 
   {
     char  cBuffer[400];
     if (bReflex50Enabled)
       sprintf(cBuffer, szVersionPrompt, szR50, m_byDeviceId, m_byCpVersion);
     else
       sprintf(cBuffer, szVersionPrompt, szR25, m_byDeviceId, m_byCpVersion); 
     AfxMessageBox(cBuffer, MB_OK | MB_ICONHAND);
     if (((CMainFrame *)(theApp.m_pMainWnd))->IsAccessRestricted())
     {
       DeleteContents();
       ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarShort();
       return(FALSE);
     }
     else   // allow lab users access to hexedit and commview
       ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarLabUseOnly(); 
   } 
   else
     ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarLong();
   
   // copy original for undo's
   for (int lcv = 0; lcv < m_byCodePlugArray.GetSize(); ++lcv)
     m_byOriginalCodePlugArray.Add(m_byCodePlugArray[lcv]);
     
   // retrieve default codeplug data if available 
   ImportDefaultCodeplugData();
  
   SetPathName(pszPathName);

   UpdateAllViews(NULL, 0L, NULL);
   ::MessageBeep(MB_OK);      // give user a completion indicator   
   return TRUE;
}




/////////////////////////////////////////////////////////////////
//  OnSaveDocument() : Saves the document into an S-Record File.
//
//  Parameters: 
//      pszPathName - points to the path of the document to be
//                    opened.
//  Comments:  
//      Override of CDocument::OnSaveDocument().  The serialization 
//      mechanism is not used because it supports only read/write
//      of binary (non-ASCII) data.
//
//  Return: (BOOL)
//      TRUE:  Document was successfully saved.
//      FALSE: Document was unsuccessfully saved!
//
//  History:    Date       Author     Comment
//              3/12/96    PGP        Created
//
BOOL CPpsDoc::OnSaveDocument(const char *pszPathName)
{
   TRACE("\nCPpsDoc::OnSaveDocument - path = %s\n", pszPathName);

   if (SaveToFile(pszPathName) == FALSE)
     return(FALSE);
   
   // reset current doc flags and info  
   SetPathName(pszPathName);
   SetModifiedFlag(FALSE);    
   return(TRUE);
}



////////////////////////////////////////////////////////////////////
//  SaveToFile
//
//  Saves the current document to the named file.  Called by
//  OnSaveDocument and OnSaveAsDefault above.  Does not reset
//  the active document path and title information.
//  
//  Returns FALSE on error, TRUE otherwise.
//
BOOL CPpsDoc::SaveToFile(const char *pszPathName)
{
   TRACE("\nCPpsDoc::SaveToFile - path = %s\n", pszPathName);
  
   if (m_byCodePlugArray.GetSize() <= 0)
     return(FALSE);
     
   // This version of MFC has inconsistencies in the definition of
   // the CString class which makes it difficult to use the CString
   // class exclusively.  Consequently, to avoid mixing the CString
   // operations with normal 'C' string operations, just use the
   // 'C' string library functions for S-Record encoding.
   char szData[80] = "";     
   char szBuffer[60] = "";
   char szConvert[3] = "";
   unsigned int nRecLength, nAddress, nCheckSum;
   unsigned int nNumbytes, index; 
   CStdioFile f;
   CFileException fe;

   // Save and validate current data if needed.
   ((CPpsApp*)AfxGetApp())->SaveActiveViewsData();

   if(!f.Open(pszPathName, CFile::modeCreate | CFile::typeText))
   {
      ReportSaveLoadException(pszPathName, &fe,
         TRUE, AFX_IDP_FAILED_TO_OPEN_DOC);
      return(FALSE);
   } 

   TRY
   {
      BeginWaitCursor();      
      nNumbytes = m_byCodePlugArray.GetSize();
      nCheckSum = 0;
      nRecLength = 3;    // always count the address & checksum char pairs
      nAddress = GetDocBaseAddress();
      
      // Encode S-Records
      // write S1 records until end of m_byCodePlugArray
      for(index = 0; index < nNumbytes; index++)
      { 
         sprintf(szConvert, "%02hX", m_byCodePlugArray[index]);
         strncat(szData, szConvert, 2);
         nCheckSum += m_byCodePlugArray[index];
         nRecLength++;       
         if (!((index + 1) % 16) || ((index + 1) == nNumbytes))
         {
           nCheckSum += (BYTE)nRecLength;
           nCheckSum += (BYTE)((nAddress >> 8) & 0x00FF);
           nCheckSum += (BYTE)(nAddress & 0x00FF); 
           nCheckSum = ~(nCheckSum);
           sprintf(szBuffer, "S1%02hX%04hX%s%02hX\n",
                   nRecLength, nAddress, szData, (BYTE)nCheckSum);
           f.Write(szBuffer, strlen(szBuffer));
           nCheckSum = 0;
           nRecLength = 3;
           nAddress += 16; 
           szData[0] = '\0'; 
          } 
      }     
      f.Close();      
   } // TRY
   CATCH_ALL(e)
   {
      f.Abort();
      EndWaitCursor(); 
      
      TRY
         ReportSaveLoadException(pszPathName, e,
            TRUE, AFX_IDP_FAILED_TO_OPEN_DOC);
      END_TRY
      return(FALSE);
   }
   END_CATCH_ALL 
   
   EndWaitCursor();
   return(TRUE);
}


void CPpsDoc::OnUpdateFileOpen(CCmdUI* pCmdUI)
{                                                      
    if (theApp.GetThePager()->IsOperationInProgress())
	  pCmdUI->Enable(FALSE);
	else
	  pCmdUI->Enable(TRUE);	
}

   
void CPpsDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
    if (theApp.GetThePager()->IsReadRequested())
	  pCmdUI->Enable(FALSE);
	else
	  pCmdUI->Enable(m_byCodePlugArray.GetSize() > 0);
}



void CPpsDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
    if (theApp.GetThePager()->IsReadRequested())
	  pCmdUI->Enable(FALSE);
	else
	  pCmdUI->Enable(m_byCodePlugArray.GetSize() > 0);	
}



//////////////////////////////////////////////////////////////////////
//	Field()
//
//	Read a codeplug field
//
int CPpsDoc::Field(int nIndex, int nNumBytes, BYTE *pDest)
{
  TRACE("\nCPpsDoc::Field : nIndex=%d : nNumBytes=%d\n", nIndex, nNumBytes);
  
  ASSERT((nIndex >= 0) && ((nIndex+nNumBytes) <= m_byCodePlugArray.GetSize()));
  int 	lcv;
  
  if ((nIndex < 0) || ((nIndex+nNumBytes) > m_byCodePlugArray.GetSize()))
  {
    #ifdef _DEBUG 
      char	szBuffer[200];
      sprintf(szBuffer, "CPpsDoc::Field : Array Bounds Access Violation. Index=%d : NumBytes=%d",
              nIndex, nNumBytes);
      AfxMessageBox(szBuffer, MB_ICONINFORMATION | MB_APPLMODAL | MB_OK); 
    #endif     
    return(1);
  }
  
  for(lcv = 0; lcv < nNumBytes; ++lcv)
  {
    pDest[lcv] = m_byCodePlugArray[nIndex + lcv]; 
  }
  
  return(0);
}


//////////////////////////////////////////////////////////////////////
//  GetOriginalField()
//
//  Same as Field() above, but retrieves from original document copy. 
//
int CPpsDoc::GetOriginalField(int nIndex, int nNumBytes, BYTE *pDest)
{
  TRACE("\nCPpsDoc::GetOriginalField : nIndex=%d : nNumBytes=%d\n", nIndex, nNumBytes);
  
  ASSERT((nIndex >= 0) && ((nIndex+nNumBytes) <= m_byOriginalCodePlugArray.GetSize()));
  int 	lcv;
  
  if ((nIndex < 0) || ((nIndex+nNumBytes) > m_byOriginalCodePlugArray.GetSize()))
  {
    #ifdef _DEBUG 
      char	szBuffer[200];
      sprintf(szBuffer, "CPpsDoc::GetOriginalField : Array Bounds Access Violation. Index=%d : NumBytes=%d",
              nIndex, nNumBytes);
      AfxMessageBox(szBuffer, MB_ICONINFORMATION | MB_APPLMODAL | MB_OK); 
    #endif     
    return(1);
  }
  
  for(lcv = 0; lcv < nNumBytes; ++lcv)
  {
    pDest[lcv] = m_byOriginalCodePlugArray[nIndex + lcv]; 
  }
  
  return(0);
}


//////////////////////////////////////////////////////////////////////
//	SetField()
//
//	Modify a code plug field.  If it is unnecessary to notify all the
//  application views of the update, either accept the default
//  bIssueViewUpdate value, or call this function with a FALSE value.
//  This saves on excessive 'OnUpdate()' calls.   
//  
int CPpsDoc::SetField(int nIndex, int nNumBytes, BYTE *pSource, BOOL bIssueViewUpdate)
{
  TRACE("CPpsDoc::SetField : nIndex=%d : nNumBytes=%d\n", nIndex, nNumBytes);  
  
  ASSERT((nIndex >= 0) && (nIndex <= m_byCodePlugArray.GetSize()));
  ASSERT((nIndex + nNumBytes) <= m_byCodePlugArray.GetSize());
        
  if ((nIndex < 0) || 
      (nIndex > m_byCodePlugArray.GetSize()) ||
      ((nIndex + nNumBytes) > m_byCodePlugArray.GetSize()))
    return(1); 
        
  for(int lcv = 0; lcv < nNumBytes; ++lcv)
  {
    m_byCodePlugArray[nIndex + lcv] = pSource[lcv];
  }

  RememberModification(nIndex, nNumBytes);
      
  SetModifiedFlag();    
  // notify the views of document update
  if (bIssueViewUpdate == TRUE)
    UpdateAllViews(NULL, 0L, NULL);
  return(0);
} 



//////////////////////////////////////////////////////////////////////
//	SetField()
//
//  Set data from another CByteArray to the m_byCodePlugArray.  The
//  array is assumed to be the same size as the m_byCodePlugArray.
//
int CPpsDoc::SetField(CByteArray &rByteArray, int nIndex, int nNumBytes, BOOL bIssueViewUpdate)
{
  TRACE("\nCPpsDoc::SetField : nIndex=%d  nNumBytes=%d\n", nIndex, nNumBytes);
  
  ASSERT((nIndex >= 0) && (nIndex <= m_byCodePlugArray.GetSize()));
  ASSERT(rByteArray.GetSize() == m_byCodePlugArray.GetSize());
  ASSERT((nIndex + nNumBytes) <= m_byCodePlugArray.GetSize());
  
  if ((nIndex < 0) || 
      (nIndex > m_byCodePlugArray.GetSize()) ||
      ((nIndex + nNumBytes) > m_byCodePlugArray.GetSize()) ||
      (rByteArray.GetSize() != m_byCodePlugArray.GetSize()))
    return(1); 
  
  // move data from the rByteArray to the m_byCodePlugArray.
  for (int lcv = 0; lcv < nNumBytes; ++lcv)
    m_byCodePlugArray[nIndex + lcv] = rByteArray[nIndex + lcv];
  
  RememberModification(nIndex, nNumBytes);
      
  SetModifiedFlag();    
  // notify the views of document update
  if (bIssueViewUpdate == TRUE)
    UpdateAllViews(NULL, 0L, NULL);
  return(0);
}



//////////////////////////////////////////////////////////////////////
//	SetField()
//
//  Set data from another CByteArray to the m_byCodePlugArray. The
//  array may not be the same size as the m_byCodePlugArray.  All of 
//  the contents of the CByteArray are placed at the given index in
//  the m_byCodePlugArray.
//
int CPpsDoc::SetField(CByteArray &rByteArray, int nIndex, BOOL bIssueViewUpdate)
{
  TRACE("\nCPpsDoc::SetField : nIndex=%d  \n", nIndex);
  
  ASSERT((nIndex >= 0) && (nIndex <= m_byCodePlugArray.GetSize()));
  ASSERT((rByteArray.GetSize()) <= m_byCodePlugArray.GetSize());
   
  if ((nIndex < 0) ||
      (nIndex > m_byCodePlugArray.GetSize()) ||
      ((nIndex + rByteArray.GetSize()) > m_byCodePlugArray.GetSize()))
    return(1); 
   
  // move data from the rByteArray to the m_byCodePlugArray.
  int NumBytes = rByteArray.GetSize();
  for (int lcv = 0; lcv < NumBytes; ++lcv)
    m_byCodePlugArray[nIndex + lcv] = rByteArray[lcv];
  
  RememberModification(nIndex, rByteArray.GetSize());
      
  SetModifiedFlag();    
  // notify the views of document update
  if (bIssueViewUpdate == TRUE)
    UpdateAllViews(NULL, 0L, NULL);
  return(0);
}



//////////////////////////////////////////////////////////////////////
//	SetFieldFromDefault()
//
//	Modify a code plug field by moving data from the 
//	m_byDefaultCodePlugArray to the m_byCodePlugArray.  If the
//  m_byDefaultCodePlugArray has not been loaded from file 'default.s19'
//  then notify the user and return error. 
//          
static const char *szInfo = "Default configuration data is not\n"
						    "available for this device.";                                        	
int CPpsDoc::SetFieldFromDefault(int nIndex, int nNumBytes, BOOL bIssueViewUpdate)
{
  TRACE("\nCPpsDoc::SetFieldFromDefault : nIndex=%d : nNumBytes=%d\n", nIndex, nNumBytes);
  
  if (IsDefaultDataAvailable() == FALSE)
  {
      AfxMessageBox(szInfo, MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);      
      return(1);
  }   
  return(SetField(m_byDefaultCodePlugArray, nIndex, nNumBytes, bIssueViewUpdate));
}

BOOL CPpsDoc::IsDefaultDataAvailable(void)
{
  return((m_byDefaultCodePlugArray.GetSize() > 0) && (!m_strDefaultS19FileName.IsEmpty())); 
}


////////////////////////////////////////////////////////////////////////
//  SetFieldFromOriginal()
//
//  Use this function to 'undo' edits to a field.  The original field
//  value, as obtained upon 'pager-read' or 'file-open' is moved into
//	the m_byCodePlugArray field.
//
int CPpsDoc::SetFieldFromOriginal(int nIndex, int nNumBytes, BOOL bIssueViewUpdate)
{
  TRACE("\nCPpsDoc::SetFieldFromOriginal : nIndex=%d : nNumBytes=%d\n", nIndex, nNumBytes);

  return(SetField(m_byOriginalCodePlugArray, nIndex, nNumBytes, bIssueViewUpdate));
}


////////////////////////////////////////////////////////////////////////
//  SetFieldFromArchived()
//
//  Set content from an archived .s19 file.
//
int CPpsDoc::SetFieldFromArchived(int nIndex, int nNumBytes, BOOL bIssueViewUpdate)
{
  TRACE("\nCPpsDoc::SetFieldFromArchived : nIndex=%d : nNumBytes=%d\n", nIndex, nNumBytes);
  
  if (m_byArchivedCodePlugArray.GetSize() > 0)
    return(SetField(m_byArchivedCodePlugArray, nIndex, nNumBytes, bIssueViewUpdate)); 
  return(0);  // ignore request if no archive loaded. 
}




//////////////////////////////////////////////////////////////////////
//	CanCloseFrame()
//
//	Save and validate data for current view before closing.
//
BOOL CPpsDoc::CanCloseFrame(CFrameWnd *pFrame)
{
  TRACE("\nCPpsDoc::CanCloseFrame\n");
  
  // Save and validate current data if needed.
  // Will also mark document as modified if changes it.
  if (!((CPpsApp*)AfxGetApp())->SaveActiveViewsData())
    return FALSE;  // validation failed

  // call base class to save modified data etc.
  return(CDocument::CanCloseFrame(pFrame));
}
 



/////////////////////////////////////////////////////////////////
//  S1RecordImport() : Imports a single srecord into a CByteArray
//
//  Parameters: 
//      rStr - reference to the S-record string
//      rByteArray - reference to the byte array              
//  Comments:
//  	Assumptions:
//			1.  any S0 record can be ignored and simply saved
//			2.  any S9 record can be ignored and simply saved
//		    3.  only S1 records will be used
//
//
int CPpsDoc::S1RecordImport(const char *rStr, CByteArray& rByteArray)
{  
   //TRACE("CPpsDoc::S1RecordImport : Srec = %s\n", rStr);
   int				iRecordLength;
   int     			iLcv;
   int				nAddress; 
   BYTE			    byArray[SREC_SIZE];
   
   if (*rStr == '\0') 	   // ignore empty strings
     return(0);
   
   if (rStr[1] != '1')     // ignore all but S1 records
     return(0); 
   
   if (S1RecordDecode(rStr, byArray, (UINT*)&nAddress, &iRecordLength))
     return(1); 
   
   if (iRecordLength == 0) // no data to import 
     return(0);
     
   nAddress = (nAddress - (theApp.GetThePager()->GetCodePlugBaseAddress())); 
    
   // verify the S-Record address is in range
   if ((nAddress < 0) || (nAddress > (theApp.GetThePager()->GetCodePlugSize())))
     return(1);
     
   for (iLcv = 0; iLcv < iRecordLength; ++iLcv)
   {
     rByteArray.SetAtGrow((int)nAddress+iLcv, byArray[iLcv]);
   }   
   return(0);
}



////////////////////////////////////////////////////////////////////////
//  S1RecordDecode()
//
int CPpsDoc::S1RecordDecode(const char *rStr, 
						    BYTE *pbyArray, 
						    UINT *pnAddress,
						    int  *pnNumBytesInRecord)
{
   int				iRecordLength;
   int     			nTempA, nTempB; 
   BYTE			    byTemp;
   UINT				nSum;
   BYTE				byCheckSum;
   
   if ((*rStr == '\0')  || 	   // ignore empty strings
       (rStr[1] == '0') ||     // or any S0 records
       (rStr[1] == '9'))       // or any S9 records
   {
     *pnNumBytesInRecord = 0;  // return 0 bytes to import.
     return(0);
   }
   
   if (rStr[1] != '1')         // invalid if not S1 records
     return(1);
    
   // number of character pairs in S-Record
   iRecordLength = (int)((HexAlpha2Binary(rStr[2]) << 4) |
                         (HexAlpha2Binary(rStr[3]))); 
   nSum = (UINT)iRecordLength;
   iRecordLength *= 2;   // account for pairs of characters in S-Record 

   // verify record length - add 4 to account for S1 and length fields  
   nTempA = strlen(rStr);

   if ((iRecordLength + 4) != nTempA) 
   {
     TRACE("\nS1RecordDecode : iRecordLength+4=%d : strlen=%d\n", 
           iRecordLength+4, nTempA);
     return(1);
   }

   // retreive checksum from end of S-Record
   byCheckSum = (BYTE)(HexAlpha2Binary(rStr[nTempA-2]) << 4);
   byCheckSum |= (HexAlpha2Binary(rStr[nTempA-1]));
   
   // convert the address field to usable data
   byTemp = (BYTE)(HexAlpha2Binary(rStr[4]) << 4);
   byTemp |= HexAlpha2Binary(rStr[5]);
   nSum += (UINT)byTemp;
   *pnAddress = (((UINT)byTemp) << 8); 
   byTemp = (BYTE)(HexAlpha2Binary(rStr[6]) << 4);
   byTemp |= HexAlpha2Binary(rStr[7]);
   nSum += (UINT)byTemp;
   *pnAddress |= ((UINT)byTemp);
     
   // ignore address & checksum characters from further consideration                      
   iRecordLength -= 6;   
   
   // Merge each character pair into one binary byte 
   // and add the byte to the byte array.
   // The record data begins at char 8 of the S-Record string.
   for (nTempA=8, nTempB=0; nTempA < (8 + iRecordLength); nTempA+=2, nTempB++)
   {
     byTemp = (BYTE)(HexAlpha2Binary(rStr[nTempA]) << 4);
     byTemp |= (HexAlpha2Binary(rStr[nTempA+1]));
     pbyArray[nTempB] = byTemp;
   } 
   *pnNumBytesInRecord = nTempB;
   
   // verify the checksum 
   for (nTempB = 0; nTempB < (*pnNumBytesInRecord); nTempB++)
     nSum += (UINT)(pbyArray[nTempB]);
   nSum = (~(nSum));
   if (((BYTE)nSum) != byCheckSum)
   {
     TRACE("\nS1RecordDecode : nSum=%04X : byCheckSum=%02X\n", nSum, byCheckSum);
     return(1);                                                 
   } 

   TRACE("\nS1RecordDecode : %s : %04X : %02X\n", rStr, *pnAddress, *pnNumBytesInRecord);

   return(0);
}
 

BYTE CPpsDoc::HexAlpha2Binary(const char ch)
{  
   if ((ch >= '0') && (ch <= '9'))   
     return((BYTE)(ch - 0x30));
   else if ((ch >= 'A') && (ch <= 'F'))
     return((BYTE)(ch - 0x37));
   else if ((ch >= 'a') && (ch <= 'f'))
     return((BYTE)(ch - 0x57)); 
   return (0x00);
}  
    


////////////////////////////////////////////////////////////////////////////////
//  OnFileSaveAsDefault
//
//  saves the active configuration content to the pager/codeplug specific 
//  default configuration file.
//
void CPpsDoc::OnFileSaveasDefault()
{ 
  if (m_strDefaultS19FileName.IsEmpty())
    return;
  if (SaveToFile((LPCTSTR)m_strDefaultS19FileName) == FALSE)
    AfxMessageBox("File-Save operation failed.", MB_ICONINFORMATION | MB_APPLMODAL | MB_OK); 
  else 
  { 
    // reset the default codeplug array to the current codeplug array.
    m_byDefaultCodePlugArray.RemoveAll();
    for (int lcv = 0; lcv < m_byCodePlugArray.GetSize(); ++lcv) 
    { 
      m_byDefaultCodePlugArray.Add(m_byCodePlugArray[lcv]);
    }
    AfxMessageBox("File-Save operation completed.", MB_ICONINFORMATION | MB_APPLMODAL | MB_OK); 
  }
  return;       
}

void CPpsDoc::OnUpdateFileSaveasDefault(CCmdUI* pCmdUI)
{
    if (theApp.GetThePager()->IsReadRequested())
	  pCmdUI->Enable(FALSE);
	else  
	  pCmdUI->Enable((m_byCodePlugArray.GetSize() > 0) &&
	                 (!m_strDefaultS19FileName.IsEmpty()));
}



//////////////////////////////////////////////////////////////////////////
//  OnUpdateFilePrint()
//
void CPpsDoc::OnUpdateFilePrint(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_byCodePlugArray.GetSize() > 0);
}



/////////////////////////////////////////////////////////////////////////
//  RememberModification()
//
//  This function is called upon a codeplug field modification in order
//  to track which fields are modified.  The resultant modification list
//  is used to ensure that only user modifications are written to the
//  device.  Additionally, if the user has enabled modification recording,
//  then the modifications are stored to a recording list for subsequent
//  playback at user command.
//
void CPpsDoc::RememberModification(int Index, int NumBytes)
{ 
  TRACE("\nCPpsDoc::RememberModification : Index=%04X : NumBytes=%04X\n", Index, NumBytes);
  CModificationRecord	*pRecord = NULL;
  int nPatchAreaIndex = Cp_Codeplug_Patch.Offset();
  int nPatchAreaLength = Cp_Codeplug_Patch.Length();
   
  // save modification to recording playback list
  if (bRecordingEnabled)
  {
    pRecord = new CModificationRecord(Index, NumBytes, m_byCodePlugArray);
    m_RecordingList.AddTail(pRecord);
  }

  // look for any duplicate entry in modification list
  POSITION	pos = m_ModificationList.GetHeadPosition();
  while (pos != NULL)
  {
    pRecord = (CModificationRecord *)m_ModificationList.GetNext(pos); 
    ASSERT(pRecord->IsKindOf(RUNTIME_CLASS(CModificationRecord))); 
    // if patch area modification, check for duplicate entry... 
    if ((Index >= nPatchAreaIndex) && (NumBytes <= nPatchAreaLength))
    {
      if ((pRecord->nIndex >= nPatchAreaIndex) && (pRecord->nNumBytes <= nPatchAreaLength))
        return;  // patch area modification already noted.
    }
    if ((pRecord->nIndex == Index) && (pRecord->nNumBytes == NumBytes))
      return;  // modification already noted.
  }

  // Add new modification record to end of list. 
  // If the new modification is anywhere within the patch area, record
  // the change as the entire patch area.  This ensures that the patch
  // area is downloaded contiguously.  If the patch download is not 
  // done all-at-once, the pager may crash recursively upon reset.
  if ((Index >= nPatchAreaIndex) && (NumBytes <= nPatchAreaLength))
    pRecord = new CModificationRecord(nPatchAreaIndex, nPatchAreaLength);
  else 
    pRecord = new CModificationRecord(Index, NumBytes);
  m_ModificationList.AddTail(pRecord);  
  return;
}


void CPpsDoc::ForgetModifications(void)
{ 
  TRACE("\nCPpsDoc::ForgetModifications : IsEmpty=%d\n", m_ModificationList.IsEmpty());
  CModificationRecord *pRecord = NULL;
  
  while (!(m_ModificationList.IsEmpty()))
  {
    pRecord = (CModificationRecord *)m_ModificationList.RemoveHead();
    delete pRecord;
  }
  SetModifiedFlag(FALSE);    
  return;
}


//////////////////////////////////////////////////////////////////////
//  OnPagerWrite
//
//  This function initiates a write of the codeplug to the pager.
// 
void CPpsDoc::OnPagerWrite()
{
  TRACE("\nCPpsDoc::OnPagerWrite\n"); 
  CModificationRecord *pRecord = NULL;
  
  if (m_byCodePlugArray.GetSize() == 0) 
  {
    m_nReadPlayWriteState = AUTO_IDLE;   // stop any auto Read-Play-Write
    return;
  }
  
  // Save and validate current data if needed.
  if (!((CPpsApp*)AfxGetApp())->SaveActiveViewsData()) 
  {
    m_nReadPlayWriteState = AUTO_IDLE;   // stop any auto Read-Play-Write
    return;
  }

  if (m_ModificationList.IsEmpty())
  {
    m_nReadPlayWriteState = AUTO_IDLE;  // stop any auto Read-Play-Write
    AfxMessageBox("The configuration document is up-to-date.", 
                   MB_ICONINFORMATION | MB_APPLMODAL | MB_OK); 
    return;
  }
  
  // quit recording modifications, if enabled.
  bRecordingEnabled = FALSE;  

  // ensure the pager object has the latest document contents 
  if (theApp.GetThePager()->SetPagerCodePlug(m_byCodePlugArray)) 
  {
    m_nReadPlayWriteState = AUTO_IDLE;   // stop any auto Read-Play-Write
    AfxMessageBox("Please wait until the current operation is complete.", 
                   MB_ICONINFORMATION | MB_APPLMODAL | MB_OK); 
    return;
  }
  
  // initiate download of modified configuration data
  m_IterativeWritePosition =  m_ModificationList.GetHeadPosition();
  pRecord = (CModificationRecord *)m_ModificationList.GetNext(m_IterativeWritePosition);
  ASSERT(pRecord->IsKindOf(RUNTIME_CLASS(CModificationRecord)));
  
  if (theApp.GetThePager()->WriteCodePlug(pRecord->nIndex, pRecord->nNumBytes))
  {
     m_nReadPlayWriteState = AUTO_IDLE;  // abort any automatic Read-Play-Write
     AfxMessageBox("Device-Write Operation Aborted.\n",
                    MB_OK | MB_ICONHAND); 
  }
  return;	
}


void CPpsDoc::OnUpdatePagerWrite(CCmdUI* pCmdUI)
{ 
   pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
   pCmdUI->SetCheck(theApp.GetThePager()->IsWriteRequested());
}




/////////////////////////////////////////////////////////////////////////////
//  WriteEntireCodeplug
//
//  This function should not be accessible to the average user.  It is 
//  initiated from the CHexEdit only.
//
void CPpsDoc::WriteEntireCodeplug(void)
{
  TRACE("\nCPpsDoc::WriteEntireCodeplug\n");

  if (m_byCodePlugArray.GetSize() == 0)
    return;
  
  // prevent duplicate requests
  if (theApp.GetThePager()->IsWriteRequested()) 
    return;

  // Save and validate current data if needed.
  if (!((CPpsApp*)AfxGetApp())->SaveActiveViewsData())
    return;
  
  // do not maintain the modification list since now writing entire codeplug
  ForgetModifications();
   
  if (theApp.GetThePager()->WriteCodePlug(m_byCodePlugArray))
    AfxMessageBox ("Device-Write Operation Aborted.\n",
                    MB_OK | MB_ICONHAND);    
  return;	
}
 


////////////////////////////////////////////////////////////////////////////
//  OnWmWriteCodeplugDone
//
//  Message handler for the WM_WRITE_CP_DONE message.
//    wParam = 0 if successful complete; 1 if failed complete
//    lParam = unused 
void CPpsDoc::OnWmWriteCodeplugDone(UINT wParam, LONG lParam)
{
  TRACE("\nCPpsDoc::OnWmWriteCodeplugDone\n");
  
  // write failed or end of WriteEntireCodeplug() operation...
  if ((wParam) || (m_ModificationList.IsEmpty()))  
  {
    m_nReadPlayWriteState = AUTO_IDLE;  // stop any automatic Read-Play-Write. 
    return; 
  }
  
  // All modifications downloaded?  Reset the pager and forget modifications.
  // If the 'codeplug unprogrammed' status exists, then assume that the
  // write operation downloaded the entire codeplug and that further writes
  // are not required.
  if ((m_IterativeWritePosition == NULL) || 
      (theApp.GetThePager()->IsCodePlugUnprogrammed()))   
  {                                       
    m_nReadPlayWriteState = AUTO_IDLE;  // end any automatic Read-Play-Write. 
    ForgetModifications(); 
    if (bResetEnabled)  // lab-use only flag, normally always TRUE             
      theApp.GetThePager()->DoHardwareReset(1);
    return;
  }
  
  // iterate to the next modification record and initiate download 
  CModificationRecord *pRecord = (CModificationRecord *)m_ModificationList.GetNext(m_IterativeWritePosition);
  ASSERT(pRecord->IsKindOf(RUNTIME_CLASS(CModificationRecord)));
   
  theApp.GetThePager()->WriteCodePlug(pRecord->nIndex, pRecord->nNumBytes);
  return;
}



/////////////////////////////////////////////////////////////////////
//  OnFileClose()
//
void CPpsDoc::OnFileClose()
{
  TRACE("\nCPpsDoc::OnFileClose\n");

  // Save and validate current data if needed.
  if (!((CPpsApp*)AfxGetApp())->SaveActiveViewsData())
    return;

  if (IsModified())
  { 
    if (!SaveModified())   // stop close on exception
      return;
  }
  
  DeleteContents(); 
  
  // change the document path and title.  
  #ifdef _DEBUG
  SetPathName("codeplug.s19");
  #else
  SetPathName("");
  #endif
  SetTitle("Codeplug"); 
  ((CMainFrame *)(theApp.m_pMainWnd))->ViewToolBarShort();
  UpdateAllViews(NULL, 0L, NULL);  
  return;
}


void CPpsDoc::OnUpdateFileClose(CCmdUI* pCmdUI)
{
   pCmdUI->Enable((!theApp.GetThePager()->IsOperationInProgress()) &&
                  (m_byCodePlugArray.GetSize() > 0));
}




///////////////////////////////////////////////////////////////////
// OnToggleResets
//
// This function is called in response to the CTRL+ALT+F12 key 
// sequence.  The effect is to toggle a flag used to dictate 
// whether or not the RESET command is sent after all device
// writes are performed (as is normal).  This functionality
// intended for lab-use only.
void CPpsDoc::OnToggleResets()
{ 
  // respond only if this is a lab-use build
  if (((CMainFrame *)(theApp.m_pMainWnd))->IsAccessRestricted())
    return;

  bResetEnabled = !(bResetEnabled);
  // Use the read/write bitmaps as a visual indicator...   
  ((CMainFrame *)(theApp.m_pMainWnd))->ToggleReadWriteBmps(bResetEnabled);
  ((CMainFrame *)(theApp.m_pMainWnd))->RefreshToolBar();
  // Use the status panes as a visual state indicator
  //((CMainFrame *)(theApp.m_pMainWnd))->ToggleStatusIndicators(); 
  return;
}



/////////////////////////////////////////////////////////////////////
//  OnUtilityOverlay()
//
//  Read an S-Record file and overwrite the codeplug document with
//  the contents of that file.  The file may be only a patch area
//  revision.
//
void CPpsDoc::OnUtilityOverlay()
{ 
  CFileDialog 		dlg(TRUE, "s19", "*.s19");
  const int 		c_cBufferSize = 200; 
  const int 		c_cMaxString  = SREC_SIZE;   // max number of lines in SRec file  
  char 				szBuffer[c_cBufferSize]; 
  BYTE				byArray[c_cBufferSize];
  int				nNumBytesInRecord;	
  CStdioFile 		f;
  int				nRecordAddress;
  int				nBase = theApp.GetThePager()->GetCodePlugBaseAddress();
  CString			strFail;
  
  // ensure the current view modifications are collected first.
  if (!(theApp.SaveActiveViewsData()))
    return;

  if (dlg.DoModal() != IDOK)
    return;

  // open the file 
  if(!f.Open(dlg.GetPathName(), CFile::modeRead))
  {
    AfxMessageBox("File Open Failure.  Operation Aborted.", MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);      
    return;
  } 

  int cString = 0;           
  while(f.ReadString(szBuffer, c_cBufferSize)) 
  {
    if (cString++ == c_cMaxString)
    { 
      strFail += "File exceeds maximum of 200 S-Records.  ";
      break;
    }
    int l;
    // if the line ends with a newline character, replace with
    // null terminator.      
    if (szBuffer[l=lstrlen(szBuffer)-1] == '\n')
      szBuffer[l--] = '\0';            
    // trim off trailing blanks
    while (szBuffer[l] == ' ')
      szBuffer[l--] = '\0'; 
    // if this is an S0 record, then validate the data therein.   
    if (S0InvalidPatchHeader(szBuffer, strFail))
    {
      strFail += "The patch is not compatible with the current configuration.";
      break;
    }          
    // translate the possible S1 Record to the CByteArray
    if (S1RecordDecode(szBuffer, byArray, (UINT*)&nRecordAddress, &nNumBytesInRecord))
    {
      strFail += "Invalid S-Record Length or Checksum Field.  ";
      break;
    } 
    if (nNumBytesInRecord)  // any data to import?
    { 
      nRecordAddress = nRecordAddress - nBase;
      if (((CMainFrame*)(theApp.m_pMainWnd))->IsAccessRestricted())   // if not lab-use only PPS
      {
        if (SRecSecurityCheck(nRecordAddress, nNumBytesInRecord)) 
        {
          strFail += "An S-Record has failed the anti-cloning security check.";
          break;
        }
      }
      if (SetField(nRecordAddress, nNumBytesInRecord, byArray))
      { 
        strFail += "Invalid S-Record Address or Code/Data Field Length.";
        break;
      }
    }
    nNumBytesInRecord = 0;              
  } 
  
  f.Close();      
  if (strFail.IsEmpty())  // success
    AfxMessageBox("Patch overlay operation completed.", MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);
  else                    // failure
  {
    m_byCodePlugArray.InsertAt(0, &m_byOriginalCodePlugArray);
    strFail += "\nOperation aborted.  All codeplug modifications undone.\n\n";   
    strFail += szBuffer;   
    AfxMessageBox(strFail, MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);
  }
  UpdateAllViews(NULL, 0L, NULL);  
  return;
}



//////////////////////////////////////////////////////////////////////////////////
//  S0InvalidPatchHeader()
//
//  If the S-record is an S0 record within a patch file, then the record contains
//  the following data:
//
//		S0LLDDVVHHSSCC		where:   S0 = S0 Record header
//									 LL = Record Length (# char pairs DD - CC)
//									 DD = Device ID
//									 VV = codeplug version
//									 HH = Host Patch Level
//									 SS = DSP Patch Level
//									 CC = Check-Sum (one's complement LL - SS)
//
int CPpsDoc::S0InvalidPatchHeader(const char *szBuffer, CString &rString)  
{
  BYTE	byLL, byDD, byVV, byHH, bySS, byCC;
  UINT	nSum;
  int 	nTemp;
  //BYTE  byTemp;
  
  if (*szBuffer == '\0') 	   // ignore empty strings
    return(0);
  
  if (szBuffer[1] != '0')         // ignore if not S0 record
    return(0);
    
  // number of character pairs in S-Record, not counting S0 and LL.
  byLL = (BYTE)((HexAlpha2Binary(szBuffer[2]) << 4) |
                (HexAlpha2Binary(szBuffer[3]))); 

  // verify record length
  nTemp = lstrlen(szBuffer);
  if ((((int)byLL) != ((nTemp - 4) / 2)) || (byLL < 0x05))
  { 
    rString += "\nInvalid record length value.\n";
    return(1);
  }
    
  // retrieve device id from S-Record
  byDD = (BYTE)(HexAlpha2Binary(szBuffer[4]) << 4);
  byDD |= (HexAlpha2Binary(szBuffer[5]));
  if (byDD != m_byDeviceId)
  {
    rString += "\nInvalid device ID.\n";
    return(1);
  }
  
  // retrieve codeplug version from S-Record
  byVV = (BYTE)(HexAlpha2Binary(szBuffer[6]) << 4);
  byVV |= (HexAlpha2Binary(szBuffer[7]));
  if (byVV != m_byCpVersion)
  {
    rString += "\nInvalid codeplug version.\n";
    return(1);
  }
  
  // retrieve host patch level from S-Record
  byHH = (BYTE)(HexAlpha2Binary(szBuffer[8]) << 4);
  byHH |= (HexAlpha2Binary(szBuffer[9])); 
  /*
  CP_HostPatchLevel.bySetFromDoc(&byTemp);
  if (byHH < byTemp)
  {
    rString += "\nInvalid host patch level.\n";
    return(1);
  }
  */
     
  // retrieve DSP patch level from S-Record
  bySS = (BYTE)(HexAlpha2Binary(szBuffer[10]) << 4);
  bySS |= (HexAlpha2Binary(szBuffer[11]));
  /* 
  CP_SPPatchLevel.bySetFromDoc(&byTemp);
  if (bySS < byTemp)
  {
    rString += "\nInvalid DSP patch level.\n";
    return(1);
  }
  */  
  
  // if the patch levels are 0xFF, 0xFF, then disable security checking  
  if ((byHH == 0xFF) && (bySS == 0xFF))
    m_bEnablePatchSecurity = FALSE;
  else
    m_bEnablePatchSecurity = TRUE;
    
  // verify the checksum
  nSum = byLL;
  nSum += byDD;
  nSum += byVV;
  nSum += byHH;
  nSum += bySS;
  nSum = (~(nSum));

  // retrieve checksum from end of S-Record
  byCC = (BYTE)(HexAlpha2Binary(szBuffer[nTemp-2]) << 4);
  byCC |= (HexAlpha2Binary(szBuffer[nTemp-1]));  
  if (((BYTE)nSum) != byCC)
  { 
    rString += "\nInvalid record checksum.\n"; 
    return(1);                                                 
  }
  return(0);
}




///////////////////////////////////////////////////////////////////////////
// SRecSecurityCheck()
//
// use in conjunction with the UtilityOverlay function to prevent
// over-writing of specific document codeplug fields.
//
int CPpsDoc::SRecSecurityCheck(int nAddress, int nLength)
{ 
  // if the patch level indicators in the S0 header record are set
  // to 0xFF, 0xFF, then security checking will be disabled for the
  // patch file.
  if (m_bEnablePatchSecurity == FALSE)
    return(0);
  
  // If not a LabUseOnly PPS or Service Center PPS, limit patch to Patch 
  // Block area and select fields associated with the patch block.
  
  #ifdef DISABLE_TUNING
  if (((CMainFrame *)(theApp.m_pMainWnd))->IsAccessRestricted()) 
  {
     if ((ValidPatchRecord(nAddress, nLength, CP_StartSpNormalPatch.Offset(), (CP_StartSpNormalPatch.Length()+1)) == TRUE) ||
         (ValidPatchRecord(nAddress, nLength, CP_SizeSpNormalPatch.Offset(), CP_SizeSpNormalPatch.Length()) == TRUE) ||
         (ValidPatchRecord(nAddress, nLength, CP_HostPatchLevel.Offset(), (CP_HostPatchLevel.Length()+1)) == TRUE) ||
         (ValidPatchRecord(nAddress, nLength, CP_SPPatchLevel.Offset(), CP_SPPatchLevel.Length()) == TRUE) ||
         (ValidPatchRecord(nAddress, nLength, Cp_Codeplug_Patch.Offset(), Cp_Codeplug_Patch.Length()) == TRUE))  
       return(0);
     else
       return(1);  // attempt to write into non-patch area 
  }
  #endif
  return(0);
}


BOOL CPpsDoc::ValidPatchRecord(int nAddress, int nLength, int nOffset, int nNumBytes)
{
  if ((nAddress >= nOffset) && 
      ((nAddress + nLength) <= (nOffset + nNumBytes)))
    return(TRUE);
  else
    return(FALSE);
}



void CPpsDoc::OnUpdateUtilityOverlay(CCmdUI* pCmdUI)
{ 
  if ((theApp.GetThePager()->IsOperationInProgress()) ||
      (m_byCodePlugArray.GetSize() <= 0))
    pCmdUI->Enable(FALSE);
  else
    pCmdUI->Enable(TRUE);
}





/////////////////////////////////////////////////////////////////////
//  LoadArchivedConfiguration()
//
//  Read an archived .s19 configuration file in anticipation of 
//  setting select document fields with the corresponding archived
//  content.
//
int CPpsDoc::LoadArchivedConfiguration(void)
{ 
  CFileDialog 		dlg(TRUE, "s19", "*.s19");
  
  if (dlg.DoModal() != IDOK)
    return(1);

  m_byArchivedCodePlugArray.RemoveAll();
  
  if (LoadFileToByteArray((LPCTSTR)dlg.GetPathName(), m_byArchivedCodePlugArray))
  { 
    AfxMessageBox("Error : Unable to load the indicated file.  Operation Aborted", MB_OK | MB_ICONEXCLAMATION);
    return(1);
  }
  
  // check for size mismatch
  if (m_byArchivedCodePlugArray.GetSize() != (theApp.GetThePager()->GetCodePlugSize()))
  {
    m_byArchivedCodePlugArray.RemoveAll();
    AfxMessageBox("Error : Document does not match required codeplug size.  Operation Aborted.", MB_OK | MB_ICONEXCLAMATION);
    return(1);
  }
  
  // check for pager and codeplug version mismatch
  BYTE byPagerId = m_byArchivedCodePlugArray[CP_OTA_Protected_H.Offset()];
  BYTE byCodeplugVersion = m_byArchivedCodePlugArray[CP_Version.Offset()];
  
  if ((byPagerId != m_byDeviceId) || (byCodeplugVersion != m_byCpVersion))
  {
    m_byArchivedCodePlugArray.RemoveAll();
    AfxMessageBox("Error : Configuration version mis-match.  Operation aborted.", MB_OK | MB_ICONEXCLAMATION);
    return(1);
  }                        
  return(0);
}
 


////////////////////////////////////////////////////////////////////////////////////
//  LoadFileToByteArray
//
//  Loads an .s19 file to any of the CByteArray members.  Returns 1 on error.
//
int CPpsDoc::LoadFileToByteArray(const char *pszPathName, CByteArray &rByteArray)
{
  const int 		c_cBufferSize = 200; 
  const int 		c_cMaxString  = SREC_SIZE;   // max number of lines in SRec file  
  char 				szBuffer[c_cBufferSize]; 
  CStdioFile 		f;

  // open the file 
  if(!f.Open(pszPathName, CFile::modeRead))
  {
    return(1);
  } 

  int cString = 0;           
  while(f.ReadString(szBuffer, c_cBufferSize)) 
  {
    if (cString++ == c_cMaxString) // document is too long.
    {
      f.Close();                       
      return(1);
    }
    int l;
    // if the line ends with a newline character, replace with
    // null terminator.      
    if (szBuffer[l=lstrlen(szBuffer)-1] == '\n')
      szBuffer[l--] = '\0';            
    // trim off trailing blanks
    while (szBuffer[l] == ' ')
      szBuffer[l--] = '\0';         
    // translate the possible S1 Record to the CByteArray
    if (S1RecordImport(szBuffer, rByteArray))
    {
       f.Close();
       return(1);
    }      
  } 
  f.Close();
  return(0);
}



///////////////////////////////////////////////////////////////
//  OnUtilityRecordingOnOff
//
//  Toggle recording of modifications on/off.
//
void CPpsDoc::OnUtilityRecordingOnOff()
{
  TRACE("\nCPpsDoc::OnUtilityRecordingOnOff\n");

  if (bRecordingEnabled)  // currently on, going off
  {
    if (!(theApp.SaveActiveViewsData()))
      return;  // abort toggle if bad entry on current view
  }

  bRecordingEnabled = !(bRecordingEnabled); 
  ::MessageBeep(MB_OK);      // give user a completion indicator   
}

void CPpsDoc::OnUpdateUtilityRecordingOnOff(CCmdUI* pCmdUI)
{  
   pCmdUI->Enable((m_byCodePlugArray.GetSize() > 0));
   pCmdUI->SetCheck(bRecordingEnabled);
   return;	
}




///////////////////////////////////////////////////////////////
//  OnUtilityRecordingPlay
//
//  Initiate codeplug overlay of recorded modifications.
//
void CPpsDoc::OnUtilityRecordingPlay()
{
  TRACE("\nCPpsDoc::OnUtilityRecordingPlay\n");
  CModificationRecord	*pRecord = NULL;
  POSITION				pos = m_RecordingList.GetHeadPosition();
  
  BOOL bNotNow = ((m_RecordingList.IsEmpty() == TRUE) || 
                  (theApp.GetThePager()->IsOperationInProgress() == TRUE));  
  if (bNotNow) 
  {
    m_nReadPlayWriteState = AUTO_IDLE;  // abort any automatic Read-Play-Write
    return;
  }
  
  // ensure the current view modifications are collected first.
  if (!(theApp.SaveActiveViewsData())) 
  {
    m_nReadPlayWriteState = AUTO_IDLE;  // abort any automatic Read-Play-Write
    return; 
  }

  // temporarily turn recording off (if on) so that calls to SetField will
  // not result in duplicate modifications being recorded.
  BOOL bTemp = bRecordingEnabled;
  bRecordingEnabled = FALSE;  
  while (pos != NULL)
  {
    pRecord = (CModificationRecord *)m_RecordingList.GetNext(pos); 
    ASSERT(pRecord->IsKindOf(RUNTIME_CLASS(CModificationRecord))); 
    SetField(pRecord->byRecordedDelta, pRecord->nIndex);
  } 
  bRecordingEnabled = bTemp;  // reset flag
  
  // notify the views of document update
  UpdateAllViews(NULL, 0L, NULL);
  ::MessageBeep(MB_OK);      // give user a completion indicator   
  return;
}

void CPpsDoc::OnUpdateUtilityRecordingPlay(CCmdUI* pCmdUI)
{  
   BOOL bEnable = ((m_RecordingList.IsEmpty() == FALSE) && 
                   (theApp.GetThePager()->IsOperationInProgress() == FALSE));
   pCmdUI->Enable(bEnable);
   return;	
}


/////////////////////////////////////////////////////////////////////////////
//  OnUtilityRecordingErase
//
//  Allow the user to clear all recorded modifications.
// 
void CPpsDoc::OnUtilityRecordingErase()
{
  TRACE("\nCPpsDoc::OnUtilityRecordingErase\n");
  CModificationRecord *pRecord = NULL;
  
  // clear the recording list
  while (!(m_RecordingList.IsEmpty()))
  {
    pRecord = (CModificationRecord *)m_RecordingList.RemoveHead();
    ASSERT(pRecord->IsKindOf(RUNTIME_CLASS(CModificationRecord))); 
    pRecord->byRecordedDelta.RemoveAll();
    delete pRecord;
  }
  bRecordingEnabled = FALSE;  
  return;	
}

void CPpsDoc::OnUpdateUtilityRecordingErase(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_RecordingList.IsEmpty() == FALSE);
}



///////////////////////////////////////////////////////////////////////////
// OnUtilityDeltaRepeat()
//
// Allow the user to automatically read from the attached device, apply
// any previously recorded configuration changes, and then write to the
// attached device.
//
void CPpsDoc::OnUtilityDeltaRepeat()
{  
  if (m_RecordingList.IsEmpty())
  {
    AfxMessageBox("No modifications have been recorded.\nOperation canceled.");
    return;
  }
  
  if (m_nReadPlayWriteState != AUTO_IDLE)  // operation in progress?
    return;
    
  // initiate a Device-Read
  m_nReadPlayWriteState = AUTO_READ;
  OnPagerRead();
  return;	
}

void CPpsDoc::OnUpdateUtilityDeltaRepeat(CCmdUI* pCmdUI)
{
  BOOL bEnable = ((m_RecordingList.IsEmpty() == FALSE) && 
                  (theApp.GetThePager()->IsOperationInProgress() == FALSE));
  pCmdUI->Enable(bEnable);
}


/////////////////////////////////////////////////////////////////////////
//  ESN/PA FIX VALIDATION PASSWORD STUFF - copied from Tango work...
//  
static char security[] = "]j\\i0h[@ZgEfYIXeDdWHVcCbUGTaB`SFR_A^Q";

int	CPpsDoc::PasswordToKey(const char *pSource, char *pDest)
{
  char byTmpBUFF[11];  
  
  if (lstrlen(pSource) > 10)
	return(1);
  //
  memcpy(byTmpBUFF, pSource, 11);
  _fstrupr((char *)byTmpBUFF);
  padRSpace((char *)byTmpBUFF, 10);
  EncodePassword(byTmpBUFF, pDest);
  return(0);
}

int	CPpsDoc::KeyToPassword(const char *pSource, char *pDest)
{
  char byTmpBUFF[11];  
  
  if (lstrlen(pSource) > 10)
	return(1);
  //
  lstrcpy(byTmpBUFF, pSource);
  padRSpace((char *)byTmpBUFF, 10);
  DecodePassword(byTmpBUFF, pDest);
  return(0);
}

void CPpsDoc::EncodePassword(char pwd[], char epwd[])
{ 
  int h,i,j,k;

  h = lstrlen(pwd);
  k = lstrlen(security);
  for (i=0; i < h; i++) 
  {
    j = (strloc(security, (char)(pwd[i] + 16))) + i + 1;
    if (j > k-1) j -= k;
    epwd[i] = (char)(security[j] - 16);
  }
  epwd[i] = '\0';
  return;                              
}

void CPpsDoc::DecodePassword(char epwd[], char pwd[])
{
  int h,i,j,k;
  
  h = lstrlen(epwd);
  k = lstrlen(security);
  for (i=0; i < h; i++)
  {
    j = strloc(security, (char)(epwd[i]+16));
    if (j > i) j -= i;
    else j += k - i;
    pwd[i] = (char)(security[j-1] - 16);
  }
  pwd[i] = '\0';
  
  return;
}

void CPpsDoc::padRSpace(char s[], int length)
{
  int i,k;

  k = lstrlen(s);
  if (k < length)
  {
    for (i=k; i < length; i++)
	 s[i] = ' ';
    s[i] = '\0';
  }

  return;
}

int CPpsDoc::strloc(char s[], char c)
{ 
  int i,k;

  k = lstrlen(s);
  for (i=0;  i < k; i++)
  {
    if (s[i] == c) return i;
  }
  return(-1);
}

/////////////////////////////////////////////////////////////////
