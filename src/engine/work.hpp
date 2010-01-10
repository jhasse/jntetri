#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>

class Widget;

class Work
{
public:
	Work();
	virtual void Step() = 0;
	virtual void Draw() const = 0;
	virtual void QuitEvent();
	virtual ~Work();
	void StepWidgets();
	void DrawWidgets() const;
	void AddWidget(boost::shared_ptr<Widget>);
	void FocusNext();
	void FocusPrevious();
protected:
	void StepFocus();
	std::vector<boost::shared_ptr<Widget> > widgets_;
	boost::shared_ptr<Widget> focusedWidget_;
};
