//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
/////////////////////////////////////////////////////////////////////////////
// ppsdoc.h : interface of the CPpsDoc class
//
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//  CModificationRecord
//
//  This CObject based class is used to maintain a list of codeplug
//  field modifications made by the PPS user.  This list of modifications
//  is subsequently used to ensure that only those modifications made are
//  written to the attached pager.
//
class CModificationRecord : public CObject
{
  DECLARE_DYNCREATE(CModificationRecord)
  public:
    int			nIndex;
    int			nNumBytes;  
    CByteArray  byRecordedDelta;  // for Utility-Recording support
    // constructors
    CModificationRecord()
    { 
      nIndex = 0;  
      nNumBytes = 0; 
    }
    CModificationRecord(CModificationRecord &rCModificationRecord);
    CModificationRecord(int Index, int NumBytes) 
    {  
      nIndex = Index;  
      nNumBytes = NumBytes;
    } 
    CModificationRecord(int Index, int NumBytes, CByteArray &rSource);
    // operator overloads
    const CModificationRecord &operator=(const CModificationRecord &rModRec); 
    int operator==(const CModificationRecord &rRecord)
    { return((nIndex == rRecord.nIndex) && (nNumBytes == rRecord.nNumBytes)); } 
    //
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
};  // CModificationRecord





//////////////////////////////////////////////////////////////////////////
//  CDocument
//
//  The only document class used in this application... corresponds to the
//  codeplug.
//
class CPpsDoc : public CDocument
{
  private: 
    enum { SREC_SIZE = 200 };					// max num lines in .s19 file
    BYTE			m_byCpVersion;              // current codeplug version
    BYTE			m_byDeviceId;	            // current device id
    CString			m_strDefaultS19FileName;    // default codeplug file name
    CByteArray  	m_byCodePlugArray;          // the document
    CByteArray		m_byDefaultCodePlugArray;   // defaults loaded from 'default.s19'
    CByteArray		m_byOriginalCodePlugArray;  // copy of original for undo's 
    CByteArray		m_byArchivedCodePlugArray;   // archived loaded from any .s19 file
    CObList			m_ModificationList;			// list of CModificationRecord(s)
    POSITION		m_IterativeWritePosition;   // points to next modification in list 
    BOOL			bResetEnabled;				// lab-use only flag to toggle reset command 
    BOOL			bRecordingEnabled;			// user modifications recorded for playback 
    CObList			m_RecordingList;			// Modification playback list 
    enum { AUTO_IDLE=0, AUTO_READ=2, 
           AUTO_PLAY=4, AUTO_WRITE=8 };         // Automatic Read, Play Recording, Write
    UINT			m_nReadPlayWriteState;      // one of IDLE, READ, PLAY, WRITE   
    BOOL			m_bAllowEsnFixes;			// allow user to fix bad ESN's    
    BOOL			m_bEnablePatchSecurity;	    // enable patching only to patch area
    
  protected: 
	CPpsDoc();
	DECLARE_DYNCREATE(CPpsDoc)

  // Attributes
  public:
    int 	Field(int nIndex, int nNumBytes, BYTE *pDest);
    int 	SetField(int nIndex, int nNumBytes, BYTE *pSource, BOOL bIssueViewUpdate=FALSE); 
    int 	SetField(CByteArray &rArray, int nIndex, int nNumBytes, BOOL bIssueViewUpdate=FALSE); 
    int     SetField(CByteArray &rByteArray, int nIndex, BOOL bIssueViewUpdate=FALSE);
    int 	SetFieldFromDefault(int nIndex, int nNumBytes, BOOL bIssueViewUpdate=FALSE);
    int 	SetFieldFromOriginal(int nIndex, int nNumBytes, BOOL bIssueViewUpdate=FALSE);
    int 	SetFieldFromArchived(int nIndex, int nNumBytes, BOOL bIssueViewUpdate=FALSE); 
    int     GetOriginalField(int nIndex, int nNumBytes, BYTE *pDest);
    int 	GetDocSize(void) { return(m_byCodePlugArray.GetSize()); }  
    BYTE 	GetCpVersion(void) { return(m_byCpVersion); }  
    BYTE 	GetDeviceId(void)  { return(m_byDeviceId); }
    UINT 	GetDocBaseAddress(void);
    BOOL 	IsDefaultDataAvailable(void); 
    BOOL	IsReflex50Enabled(void);
    BOOL	IsReflex25Enabled(void);
    
  // Operations
  public:
    void 	OnWmReadCodeplugDone(UINT, LONG);
	void 	OnWmWriteCodeplugDone(UINT, LONG);
	void 	WriteEntireCodeplug(void);
    int 	LoadArchivedConfiguration(void);
    void    OnWmIoOpListDone(WPARAM wParam, LPARAM lParam);
    	    
  // Overrides
    virtual void DeleteContents();
    virtual BOOL OnOpenDocument(const char *pszPathName);
    virtual BOOL OnSaveDocument(const char *pszPathName);
    virtual BOOL CanCloseFrame(CFrameWnd *pFrame);
  protected:
	virtual BOOL OnNewDocument();
    
  // Implementation 
  private:
    BOOL 	SaveToFile(const char *pszPathName);
    void 	ImportDefaultCodeplugData(void);
    int 	S0InvalidPatchHeader(const char *szBuffer, CString &rString);  
    int 	S1RecordImport(const char *rStr, CByteArray& rArray);
    int 	S1RecordDecode(const char *rStr, BYTE *byArray, UINT *pnAddress, int *pnNumBytesInRecord);
    BYTE 	HexAlpha2Binary(const char ch);
    void 	RememberModification(int Index, int NumBytes);
    void 	ForgetModifications(void);
    int 	SRecSecurityCheck(int nRecordAddress, int nNumBytesInRecord);
    BOOL 	ValidPatchRecord(int nAddress, int nLength, int nOffset, int nNumBytes);
    int 	LoadFileToByteArray(const char *pszPathName, CByteArray &rByteArray); 
  
  private:  // ESN validation password stuff
    int		PasswordToKey(const char *pSource, char *pDest);
    int		KeyToPassword(const char *pSource, char *pDest);
    void 	EncodePassword(char pwd[], char epwd[]);
    void 	DecodePassword(char epwd[], char pwd[]);
    void 	padRSpace(char s[], int length);
    int 	strloc(char s[], char c);
        
  public:  
    void AutoValidateEsn(void);
    static  DWORD GenerateEsn(DWORD dwPersAddr);
	virtual ~CPpsDoc();
	virtual void Serialize(CArchive& ar);   
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif

  // Generated message map functions
  protected:
  	//{{AFX_MSG(CPpsDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnPagerWrite();
	afx_msg void OnUpdatePagerWrite(CCmdUI* pCmdUI);
	afx_msg void OnPagerRead();
	afx_msg void OnUpdatePagerRead(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUtilityOverlay();
	afx_msg void OnUpdateUtilityOverlay(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveasDefault();
	afx_msg void OnUpdateFileSaveasDefault(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnToggleResets();
	afx_msg void OnFileClose();
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnUtilityRecordingOnOff();
	afx_msg void OnUpdateUtilityRecordingOnOff(CCmdUI* pCmdUI);
	afx_msg void OnUtilityRecordingPlay();
	afx_msg void OnUpdateUtilityRecordingPlay(CCmdUI* pCmdUI);
	afx_msg void OnUtilityRecordingErase();
	afx_msg void OnUpdateUtilityRecordingErase(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUtilityDeltaRepeat();
	afx_msg void OnUpdateUtilityDeltaRepeat(CCmdUI* pCmdUI);
	afx_msg void OnKeyOwner();
	afx_msg void OnKeyUser();
	afx_msg void OnUpdateKeyOwner(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKeyUser(CCmdUI* pCmdUI);
	afx_msg void OnKeyForget();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
