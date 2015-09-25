// eLearningDoc.h : interface of the CELearningDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEARNINGDOC_H__7337AAAA_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_)
#define AFX_ELEARNINGDOC_H__7337AAAA_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CELearningDoc : public CDocument
{
protected: // create from serialization only
	CELearningDoc();
	DECLARE_DYNCREATE(CELearningDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELearningDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CELearningDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CELearningDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEARNINGDOC_H__7337AAAA_EEE0_11D4_BE14_0020AF3085DB__INCLUDED_)
