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

using namespace std::chrono_literals;

bool pedestrian::canCross(const Crossing& crossing) {
	if (crossing.get_crossing_color() == Crossing_color::green)
		return(true);
	return(false);
};

void pedestrian::move() {
	if (destination_ == entry::top)
		this->position_y_-= pedestrianSpeed;
	if (destination_ == entry::bottom)
		this->position_y_ += pedestrianSpeed;
	if (destination_ == entry::right)
		this->position_x_ -= pedestrianSpeed;
	if (destination_ == entry::left)
		this->position_x_ += pedestrianSpeed;
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

void run_pedestrians(std::vector<pedestrian>& pedestrians, std::stop_token stop_token)
{
	while (!stop_token.stop_requested())
	{
		for (int i = 0; i < pedestrians.size(); i++)
		{
			pedestrians.at(i).move();
		}
		std::this_thread::sleep_for(speed_delay);
	}
}