#pragma once

class Work
{
public:
	virtual void Step() = 0;
	virtual void Draw() const = 0;
	virtual void QuitEvent();
	virtual ~Work();
};
