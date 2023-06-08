//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
/////////////////////////////////////////////////////////////////////////////
// bsound.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBasicSound object

#ifndef _CBASICSOUND_
#define _CBASICSOUND_

class CBasicSound : public CObject
{
  DECLARE_DYNAMIC(CBasicSound);
	static BOOL bSoundDriverEnabled;   
  private:
    UINT		nResId;
    LPCSTR  	szSoundImage;
	HGLOBAL 	hData;
  public:
    CBasicSound();
    CBasicSound(CBasicSound &rCBasicSound);     
    ~CBasicSound();
    UINT	GetResourceId(void) { return(nResId); }
    BOOL 	Load(UINT nResourceId);         // load a specific .wav resource
    BOOL 	Play(UINT nFlags = 0);                              
    void	ReleaseSound(void);
    static BOOL Stop(void);
    static BOOL Play(LPCSTR szFileName);	// play a .wav file 
    static void NoSoundDriverMsg(void); 
    static BOOL IsSoundDriverEnabled(void) { return(CBasicSound::bSoundDriverEnabled); }
};



#endif // _CBASICSOUND_
