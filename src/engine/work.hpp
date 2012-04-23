#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>
#include <jngl.hpp>

class Widget;

class Work : public jngl::Work
{
public:
	Work();
	virtual void step() = 0;
	virtual void draw() const = 0;
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
