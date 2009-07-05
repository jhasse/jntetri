#pragma once

#include "singleton.hpp"
#include "work.hpp"

#include <boost/shared_ptr.hpp>
#include <string>

class Procedure : public Singleton<Procedure>
{
public:
	Procedure();
	void MainLoop();
	void SetWork(Work*); // Take ownership
	void SetWork(boost::shared_ptr<Work>);
	boost::shared_ptr<Work> GetWork() const;
	void Quit();
	bool ShowWindow();
private:
	double oldTime_;
	bool needDraw_;
	double fps_;
	double fpsTemp_;
	int fpsCounter_;
	std::string fpsText_;
	const static double timePerStep_ = 0.01; // 100 FPS
	boost::shared_ptr<Work> currentWork_;
	bool changeWork_;
	boost::shared_ptr<Work> newWork_;
	bool running_;
	bool showFps_;
};

Procedure& GetProcedure();
