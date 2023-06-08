//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
////////////////////////////////////////////////////////////////////////////
// bsound.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include "bsound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CBasicSound

IMPLEMENT_DYNAMIC(CBasicSound, CObject);

BOOL CBasicSound::bSoundDriverEnabled = TRUE;   

CBasicSound::CBasicSound()
{ 
   nResId = 0;
   szSoundImage = NULL;
   hData = NULL;
}


CBasicSound::CBasicSound(CBasicSound &rCBasicSound)
{ 
   nResId = rCBasicSound.nResId;
   szSoundImage = rCBasicSound.szSoundImage;
   hData = rCBasicSound.hData;
}
     


CBasicSound::~CBasicSound()
{
  if (hData)
  {
    Stop();
    ::UnlockResource(hData);
	::FreeResource(hData);
  }
}



/////////////////////////////////////////////////////////////////////////////
// CBasicSound commands


BOOL CBasicSound::Load(UINT nIDS)
{
    TRACE("\nCBasicSound::Load : nIDS=%d\n", nIDS); 
    HRSRC	hRes;
    
    if ((nIDS != nResId) && (hData))   // new resource requested?
    {
      ::UnlockResource(hData);
      ::FreeResource(hData);
      hData = NULL;
      szSoundImage = NULL;
    }
    
    if ((hData) && (szSoundImage))   // resource already loaded?
      return(TRUE); 

	if ((hRes = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDS), "sound")) != NULL &&
	    (hData = ::LoadResource(AfxGetResourceHandle(), hRes)) != NULL)
	{
	  szSoundImage = (LPCSTR)::LockResource(hData);
      nResId = nIDS;    
	  return(TRUE);   // found the resource
	} 
	return(FALSE);  // resource not found
}


BOOL CBasicSound::Play(UINT nFlags)
{
    TRACE("\nCBasicSound::Play : ResId=%d\n", nResId);
    
	BOOL bOk = FALSE; 
	
	if ((hData) && (szSoundImage) && (CBasicSound::bSoundDriverEnabled))
	{
		bOk = ::sndPlaySound(szSoundImage, (SND_MEMORY|SND_ASYNC|SND_NODEFAULT|nFlags));
	}
	else
	  return(FALSE);   // Nothing to play
	  
	// error upon attempt to play sound
	if (!bOk)
	{
	  CBasicSound::NoSoundDriverMsg();
	  ReleaseSound();
	}
    return(bOk); 
} 


void CBasicSound::NoSoundDriverMsg(void)
{
  // ensure this message is displayed only once during the execution of
  // the application.
  if (CBasicSound::bSoundDriverEnabled == TRUE)
  {
	AfxMessageBox("Cannot play sound.\n"
	              "Ensure that a system sound driver is enabled.");
	CBasicSound::bSoundDriverEnabled = FALSE;       // once please
  }
}  



BOOL CBasicSound::Stop(void)
{ 
   return(::sndPlaySound(NULL, 0));
}


void CBasicSound::ReleaseSound(void)
{   
   // NOTE: be sure CBasicSound::Stop() is called first
   
   if (hData)  
   {
     ::UnlockResource(hData);
     ::FreeResource(hData);
     hData = NULL;
     szSoundImage = NULL;
     nResId = 0;
   }
   return;
}  


BOOL CBasicSound::Play(LPCSTR szFileName)
{
  return(::sndPlaySound(szFileName, SND_SYNC|SND_NODEFAULT|SND_NOSTOP));
}   					     
