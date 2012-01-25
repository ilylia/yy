#include "StdAfx.h"
#include "lrcEdit.h"
#include "syrMakerDlg.h"

ClrcEdit::ClrcEdit(void)
	: m_idx(0)
{
}

ClrcEdit::~ClrcEdit(void)
{
}
BEGIN_MESSAGE_MAP(ClrcEdit, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, &ClrcEdit::OnEnChange)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &ClrcEdit::OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &ClrcEdit::OnEnSetfocus)
END_MESSAGE_MAP()

void ClrcEdit::setIndex(int idx)
{
	m_idx = idx;
}

int ClrcEdit::getIndex()
{
	return m_idx;
}

void ClrcEdit::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

}

void ClrcEdit::OnEnKillfocus()
{
	((CsyrMakerDlg*)GetParent())->updateYYerLrc(this);
}

void ClrcEdit::OnEnSetfocus()
{
	((CsyrMakerDlg*)GetParent())->pauseSyr();
}
