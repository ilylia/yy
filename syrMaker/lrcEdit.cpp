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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

}

void ClrcEdit::OnEnKillfocus()
{
	((CsyrMakerDlg*)GetParent())->updateYYerLrc(this);
}

void ClrcEdit::OnEnSetfocus()
{
	((CsyrMakerDlg*)GetParent())->pauseSyr();
}
