#include "widget.hpp"

Widget::Widget() : sensitive_(true), focus_(false)
{
}

Widget::~Widget()
{
}

bool Widget::GetSensitive() const
{
	return sensitive_;
}

void Widget::SetSensitive(bool sensitive)
{
	sensitive_ = sensitive;
}

void Widget::SetFocus(bool focus)
{
	bool old = focus_;
	focus_ = focus;
	if(focus_ != old)
	{
		OnFocusChanged();
	}
}

void Widget::OnFocusChanged()
{
}

void Widget::OnAdd(Work&)
{
}
