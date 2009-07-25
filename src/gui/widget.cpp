#include "widget.hpp"

Widget::Widget() : sensitive_(true)
{
}

Widget::~Widget()
{
}

void Widget::SetSensitive(bool sensitive)
{
	sensitive_ = sensitive;
}
