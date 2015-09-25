#define WM_UNDO_ACTION (WM_APP + 5270)
#define WM_REDO_ACTION (WM_APP + 5271)

class CStackObj: public CObject
{
public:
	CStackObj(){};
	~CStackObj();
	CWnd*			m_pWnd;
	CString			m_strOp;
	CString			m_strBeforeOp;
	CString			m_strAfterOp;
	int				m_nOp1;
	int				m_nOp2;
	CObArray*		m_pobArrayBeforeOp;
	CObArray*		m_pobArrayAfterOp;
};

class CUndoRedoObj
{
public:
	CUndoRedoObj();
	~CUndoRedoObj();

protected:
	CObArray	m_obUndoArray;
	CObArray	m_obRedoArray;

public:
	BOOL IsUndoStackEmpty();
	BOOL IsRedoStackEmpty();
	void Undo();
	void Redo();
	void PushToUndoStack(CWnd* pWnd, CString strOp, CObArray* pobArrayBefore, CObArray* pObArrayAfter);
	void PushToUndoStack(CWnd* pWnd, CString strOp, CString strParam1, CString strParam2, int nOp1, int nOp2);
	void PushToUndoStack(CStackObj* pObj);
	void PushToRedoStack(CStackObj* pObj);
	void ClearStack();
	void ClearUndoStack();
	void ClearRedoStack();

protected:
	void CheckIfStackFull(CObArray& obArray);
	CStackObj* PopUndoStack();
	CStackObj* PopRedoStack();
};
