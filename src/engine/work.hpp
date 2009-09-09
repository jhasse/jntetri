#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>

class Widget;

class Work
{
public:
	virtual void Step() = 0;
	virtual void Draw() const = 0;
	virtual void QuitEvent();
	virtual ~Work();
	void StepWidgets();
	void DrawWidgets() const;
	void AddWidget(boost::shared_ptr<Widget>);
protected:
	std::vector<boost::shared_ptr<Widget> > widgets_;
	boost::shared_ptr<Widget> focusedWidget_;
	void FocusNext();
};
