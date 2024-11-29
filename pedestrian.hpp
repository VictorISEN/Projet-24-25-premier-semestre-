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
const float pedestrianSize = 20;
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
	entry destination_;
	sf::RectangleShape shape_;
	sf::Vector2f direction_;
public:
	explicit pedestrian(float posX, float posY, const entry destination, const sf::Vector2f direction) 
		: destination_{ destination }, direction_{direction} {
		shape_.setFillColor(sf::Color::White);
		shape_.setSize(sf::Vector2f(pedestrianSize, pedestrianSize));
		shape_.setPosition(posX, posY);
		shape_.setOrigin(pedestrianSize / 2, pedestrianSize / 2);
	}
	void move(std::map<std::string, Crossing> &crossings);
	bool canCross(const Crossing crossing);
	float getPosition_x();
	float getPosition_y();
	sf::Vector2f getDirection();
	sf::Vector2f getPosition();
	void setPosition_x(const float nb);
	void setPosition_y(const float nb);
	sf::RectangleShape getShape();
	float distanceX(pedestrian& p);
	float distanceY(pedestrian& p);
};

void run_pedestrians(std::vector<pedestrian>& pedestrians, std::map<std::string, Crossing> &crossings, std::stop_token stop_token);

void generatePedestrians(std::vector<pedestrian>& pedestrians);

bool canMove(std::vector<pedestrian>& pedestrians, pedestrian& p);