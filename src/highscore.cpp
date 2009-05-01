#include "highscore.hpp"
#include "engine/paths.hpp"
#include "engine/screen.hpp"

#include <fstream>
#include <stdexcept>
#include <cassert>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <jngl.hpp>

Highscore::Highscore(GameType type) : type_(type)
{
	if(type == NORMAL)
	{
		filename_ = GetPaths().Config() + "normal.txt";
	}
	else if(type == FIFTYLINES)
	{
		filename_ = (GetPaths().Config() + "fiftylines.txt");
	}
	else
	{
		throw std::runtime_error("Unknown GameType");
	}
	std::ifstream fin(filename_.c_str());
	if(fin)
	{
		for(int i = 0; i < 5; ++i)
		{
			Data temp;
			std::getline(fin, temp.name);
			fin >> (temp.score) >> (temp.time);
			highscores_.push_back(temp);
			fin.ignore(3, '\n'); // Read till next \n
		}
	}
	else
	{
		for(int i = 0; i < 5; ++i)
		{
			Data temp;
			temp.name = "Jan-Nik";
			temp.score = 0;
			temp.time = 13 * 60 + 37;
			highscores_.push_back(temp);
		}
	}
}

void Highscore::Draw() const
{
	assert(highscores_.size() == 5);
	jngl::PushMatrix();
	GetScreen().SetFontSize(50);
	jngl::SetFontColor(0, 0, 0);
	std::list<Data>::const_iterator end = highscores_.end();
	int n = 1;
	for(std::list<Data>::const_iterator it = highscores_.begin(); it != end; ++it)
	{
		std::string score;
		if(type_ == NORMAL)
		{
			score = boost::lexical_cast<std::string>(it->score);
		}
		else
		{
			int minutes = int(it->time / 60);
			int seconds = int(it->time - minutes * 60);
			int tenthOfASecond = int((it->time - seconds - minutes * 60) * 10);
			std::stringstream sstream;
			sstream.fill('0');
			sstream << minutes << ":" << std::setw(2) << seconds << "." << tenthOfASecond;
			score = sstream.str();
		}
		GetScreen().Print(score, 700 - GetScreen().GetTextWidth(score), 0);
		GetScreen().Print(boost::lexical_cast<std::string>(n) + ". " + it->name, 0, 0);
		GetScreen().Translate(0, 80);
		++n;
	}
	jngl::PopMatrix();
}

bool CompareScore(const Data& lhs, const Data& rhs)
{
	return lhs.score > rhs.score;
}

bool CompareTime(const Data& lhs, const Data& rhs)
{
	return lhs.time < rhs.time;
}

void Highscore::Add(Data data)
{
	highscores_.push_back(data);
	if(type_ == NORMAL)
	{
		highscores_.sort(CompareScore);
	}
	else
	{
		highscores_.sort(CompareTime);
	}
	highscores_.pop_back();
}

bool Highscore::IsHighscore(Data data) const
{
	if(type_ == NORMAL)
	{
		return data.score > highscores_.back().score;
	}
	else
	{
		return data.time < highscores_.back().time;
	}
}

void Highscore::Save() const
{
	std::ofstream fout(filename_.c_str());
	std::list<Data>::const_iterator end = highscores_.end();
	for(std::list<Data>::const_iterator it = highscores_.begin(); it != end; ++it)
	{
		fout << it->name << std::endl << it->score << " " << it->time << std::endl;
	}
}
