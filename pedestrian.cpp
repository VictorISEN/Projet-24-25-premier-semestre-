#pragma once
// condition_variable::wait(with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "lights.hpp"
#include "pedestrian.hpp"
#include <cstdlib>
#include <ctime>

using namespace std::chrono_literals;

bool pedestrian::canCross(Crossing crossing) {
	if (crossing.get_crossing_color() == Crossing_color::green)
		return(true);
	return(false);
};

void pedestrian::move(std::map<std::string, Crossing> &crossings) {
	if (direction_ == entry::top) {
		if ((position_y_ - pedestrianSpeed) < crossings["right"].getPosition_y() + 215 && (position_y_ - pedestrianSpeed) >= crossings["right"].getPosition_y() + 210) {
			if (canCross(crossings["right"]) == true)
				this->position_y_ -= pedestrianSpeed;
		}
		else
			this->position_y_ -= pedestrianSpeed;
	}
	if (direction_ == entry::bottom) {
		if ((position_y_ + pedestrianSpeed) > crossings["right"].getPosition_y() - 40 && (position_y_ + pedestrianSpeed) <= crossings["right"].getPosition_y() - 35) {
			if (canCross(crossings["right"]) == true)
				this->position_y_ += pedestrianSpeed;
		}
		else
			this->position_y_ += pedestrianSpeed;
	}
	if (direction_ == entry::left) {
		if ((position_x_ - pedestrianSpeed) < (crossings["top"].getPosition_x() + 215) && (position_x_ - pedestrianSpeed) >= (crossings["top"].getPosition_x() + 210)) {
			if (canCross(crossings["top"]) == true)
				this->position_x_ -= pedestrianSpeed;
		}
		else
			this->position_x_ -= pedestrianSpeed;
	}
	if (direction_ == entry::right) {
		if ((position_x_ + pedestrianSpeed) > crossings["top"].getPosition_x() - 40 && (position_x_ + pedestrianSpeed) <= crossings["top"].getPosition_x() - 35) {
			if (canCross(crossings["top"]) == true)
				this->position_x_ += pedestrianSpeed;
		}
		else 
			position_x_ += pedestrianSpeed;
	}
		
	return;
}

float pedestrian::getPosition_x() {
	return position_x_;
}

float pedestrian::getPosition_y() {
	return position_y_;
}

void pedestrian::setPosition_x(const float nb) {
	position_x_ = position_x_ + nb;
}

void pedestrian::setPosition_y(const float nb) {
	position_x_ = position_y_ + nb;
}

entry pedestrian::getDirection() {
	return direction_;
}

bool canMove(std::vector<pedestrian>& pedestrians, pedestrian& p, int j) {
	for (int i = 0; i < pedestrians.size(); i++)
	{
		if (j != i) {
			switch (p.getDirection())
			{
			case entry::top:
				if (pedestrians.at(i).getDirection() == entry::top && (p.getPosition_x() - pedestrianSpeed - 25) < pedestrians.at(i).getPosition_x())
					return false;
				break;
			case entry::bottom:
				if (pedestrians.at(i).getDirection() == entry::bottom && (p.getPosition_y() + pedestrianSpeed + 25) > pedestrians.at(i).getPosition_y())
					return false;
				break;
			case entry::left:
				if (pedestrians.at(i).getDirection() == entry::left && (p.getPosition_x() - pedestrianSpeed - 25) < pedestrians.at(i).getPosition_x())
					return false;
				break;
			case entry::right:
				if (pedestrians.at(i).getDirection() == entry::right && (p.getPosition_x() + pedestrianSpeed + 25) > pedestrians.at(i).getPosition_x())
					return false;
				break;
			default:
				return false;
			}
		}
	}
	return(true);
}

void generatePedestrians(std::vector<pedestrian>& pedestrians) {
	if (pedestrians.size() >= 20)
		return;
	int rdm = rand() % 160;
	bool freeSpace = true;
	if (rdm == 0) {
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_x() == 200 && pedestrians.at(i).getPosition_y() < 50)
				freeSpace = false;
				
		}
		if(freeSpace == true)
			pedestrians.push_back(pedestrian(entry::top, entry::bottom, 200, 0, entry::bottom));
	}

	if (rdm == 2) {
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_x() == 600 && pedestrians.at(i).getPosition_y() > 750)
				freeSpace = false;
		}
		if (freeSpace == true)
			pedestrians.push_back(pedestrian(entry::bottom, entry::top, 600, 775, entry::top));
	}

	if (rdm == 4) {
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_y() == 200 && pedestrians.at(i).getPosition_x() < 50)
				freeSpace = false;
		}
		if (freeSpace == true)
			pedestrians.push_back(pedestrian(entry::left, entry::right, 0, 200, entry::right));
	}

	if (rdm == 6) {
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_y() == 600 && pedestrians.at(i).getPosition_x() > 750)
				freeSpace = false;
		}
		if (freeSpace == true)
			pedestrians.push_back(pedestrian(entry::right, entry::left, 775, 600, entry::left));
	}

	return;
}

void run_pedestrians(std::vector<pedestrian>& pedestrians, std::map<std::string, Crossing>& crossings, std::stop_token stop_token)
{
	while (!stop_token.stop_requested())
	{
		generatePedestrians(pedestrians);
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (canMove(pedestrians, pedestrians.at(i), i) == true) {
				pedestrians.at(i).move(crossings);
				if (pedestrians.at(i).getPosition_x() > 800 || pedestrians.at(i).getPosition_y() > 800 || pedestrians.at(i).getPosition_x() < 0 || pedestrians.at(i).getPosition_y() < 0)
					pedestrians.erase(pedestrians.begin() + i);
			}
		}
		std::this_thread::sleep_for(speed_delay);
	}
}