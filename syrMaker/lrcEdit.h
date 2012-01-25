#pragma once
#include "afxwin.h"

class ClrcEdit : public CEdit
{
public:
	ClrcEdit(void);
	~ClrcEdit(void);

public:
	int m_idx;

public:
	void setIndex(int idx);
	int getIndex();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChange();
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnSetfocus();
};
