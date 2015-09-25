#include "stdafx.h"
#include "UndoRedoObj.h"
#include "GrdApi.h"

#define constStackSize	10

CStackObj::~CStackObj()
{
	::ReleaseObArray(m_pobArrayBeforeOp);
	delete m_pobArrayBeforeOp;
	::ReleaseObArray(m_pobArrayAfterOp);
	delete m_pobArrayAfterOp;
}

CUndoRedoObj::CUndoRedoObj()
{

}

CUndoRedoObj::~CUndoRedoObj()
{
	ClearStack();
}

void CUndoRedoObj::ClearStack()
{
	ClearUndoStack();
	ClearRedoStack();
}

void CUndoRedoObj::ClearUndoStack()
{
	int nSize = m_obUndoArray.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CStackObj* pObj = (CStackObj*)m_obUndoArray[0];
		m_obUndoArray.RemoveAt(0);
		delete pObj;
	}
}
	
void CUndoRedoObj::ClearRedoStack()
{
	int nSize = m_obRedoArray.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CStackObj* pObj = (CStackObj*)m_obRedoArray[0];
		m_obRedoArray.RemoveAt(0);
		delete pObj;
	}
}

BOOL CUndoRedoObj::IsUndoStackEmpty()
{
	return (m_obUndoArray.GetSize() == 0);
}

BOOL CUndoRedoObj::IsRedoStackEmpty()
{
	return (m_obRedoArray.GetSize() == 0);
}

void CUndoRedoObj::Undo()
{
	if (!IsUndoStackEmpty())
	{
		CStackObj* pObj = PopUndoStack();
		PushToRedoStack(pObj);
		if (pObj->m_pWnd)
			pObj->m_pWnd->SendMessage(WM_UNDO_ACTION, (WPARAM)pObj);
	}
}

void CUndoRedoObj::Redo()
{
	if (!IsRedoStackEmpty())
	{
		CStackObj* pObj = PopRedoStack();
		PushToUndoStack(pObj);
		if (pObj->m_pWnd)
			pObj->m_pWnd->SendMessage(WM_REDO_ACTION, (WPARAM)pObj);
	}
}

void CUndoRedoObj::PushToUndoStack(CWnd* pWnd, CString strOp, 
								   CObArray* pobArrayBefore, CObArray* pobArrayAfter)
{
	CStackObj* pObj = new CStackObj;
	pObj->m_pWnd = pWnd;
	pObj->m_strOp = strOp;
	pObj->m_pobArrayBeforeOp = pobArrayBefore;
	pObj->m_pobArrayAfterOp = pobArrayAfter;
	m_obUndoArray.InsertAt(0, pObj);
	CheckIfStackFull(m_obUndoArray);
	ClearRedoStack();
}

void CUndoRedoObj::PushToUndoStack(CWnd* pWnd, CString strOp, 
								   CString strParam1, CString strParam2, int nOp1, int nOp2)
{
	CStackObj* pObj = new CStackObj;
	pObj->m_pWnd = pWnd;
	pObj->m_strOp = strOp;
	pObj->m_strBeforeOp = strParam1;
	pObj->m_strAfterOp = strParam2;
	pObj->m_nOp1 = nOp1;
	pObj->m_nOp2 = nOp2;
	pObj->m_pobArrayBeforeOp = NULL;
	pObj->m_pobArrayAfterOp = NULL;
	m_obUndoArray.InsertAt(0, pObj);
	CheckIfStackFull(m_obUndoArray);
	ClearRedoStack();
}

CStackObj* CUndoRedoObj::PopUndoStack()
{
	if (IsUndoStackEmpty())
		return NULL;

	CStackObj* pObj = (CStackObj*)m_obUndoArray[0];
	m_obUndoArray.RemoveAt(0);	
	return pObj;
}

CStackObj* CUndoRedoObj::PopRedoStack()
{
	if (IsRedoStackEmpty())
		return NULL;

	CStackObj* pObj = (CStackObj*)m_obRedoArray[0];
	m_obRedoArray.RemoveAt(0);	
	return pObj;
}

void CUndoRedoObj::CheckIfStackFull(CObArray& obArray)
{
	if (obArray.GetSize() >= constStackSize)
	{
		int nIndex = obArray.GetSize() - 1;
		CStackObj* pObj = (CStackObj*)obArray[nIndex];
		obArray.RemoveAt(nIndex);
		delete pObj;
	}
}

void CUndoRedoObj::PushToUndoStack(CStackObj* pObj)
{
	m_obUndoArray.InsertAt(0, pObj);
	CheckIfStackFull(m_obUndoArray);
}

void CUndoRedoObj::PushToRedoStack(CStackObj* pObj)
{
	m_obRedoArray.InsertAt(0, pObj);
	CheckIfStackFull(m_obRedoArray);
}
