// eLearningDoc.cpp : implementation of the CELearningDoc class
//

#include "stdafx.h"
#include "eLearning.h"

#include "eLearningDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELearningDoc

IMPLEMENT_DYNCREATE(CELearningDoc, CDocument)

BEGIN_MESSAGE_MAP(CELearningDoc, CDocument)
	//{{AFX_MSG_MAP(CELearningDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELearningDoc construction/destruction

CELearningDoc::CELearningDoc()
{
}

CELearningDoc::~CELearningDoc()
{
}

BOOL CELearningDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CELearningDoc serialization

void CELearningDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CELearningDoc diagnostics

#ifdef _DEBUG
void CELearningDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CELearningDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CELearningDoc commands
