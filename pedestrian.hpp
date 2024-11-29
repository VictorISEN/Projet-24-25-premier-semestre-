#pragma once
// condition_variable::wait(with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "lights.hpp"
#include <vector>

using namespace std::chrono_literals;

const float pedestrianSpeed = 1;
const auto speed_delay = 0.01s;

enum class entry
{
	top = 0,
	right = 1,
	bottom = 2,
	left = 3
};

class pedestrian
{
private :
	entry origin_;
	entry destination_;
	float position_x_;
	float position_y_;
	entry direction_;
public:
	explicit pedestrian(entry origin, entry destination, float potision_x, float position_y, entry direction) : origin_{ origin }, destination_{ destination }, position_x_{ potision_x }, position_y_{ position_y }, direction_{direction} {}
	void move(std::map<std::string, Crossing> &crossings);
	bool canCross(const Crossing crossing);
	float getPosition_x();
	float getPosition_y();
	entry getDirection();
	void setPosition_x(const float nb);
	void setPosition_y(const float nb);
};

void run_pedestrians(std::vector<pedestrian>& pedestrians, std::map<std::string, Crossing> &crossings, std::stop_token stop_token);

void generatePedestrians(std::vector<pedestrian>& pedestrians);

bool canMove(std::vector<pedestrian>& pedestrians, pedestrian& p, int j);