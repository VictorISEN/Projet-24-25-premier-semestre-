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
const float pedestrianSize = 10;
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
	sf::CircleShape shape_;
	sf::Vector2f direction_;
	sf::Texture texture_;
public:
	explicit pedestrian(float posX, float posY, const entry destination, const sf::Vector2f direction) 
		: destination_{ destination }, direction_{direction} {
		static sf::Texture carTexture;
		// Chargement de la texture
		if (carTexture.getSize().x == 0) {
			if (!carTexture.loadFromFile("pieton.png")) {
				std::cerr << "Erreur : Impossible de charger l'image car_texture.png" << std::endl;
			}
		}
		shape_.setTexture(&carTexture);
		shape_.setRadius(pedestrianSize);
		shape_.setPosition(posX, posY);
		shape_.setOrigin(pedestrianSize, pedestrianSize);
	}
	void move(std::map<std::string, Crossing> &crossings);
	bool canCross(const Crossing crossing);
	float getPosition_x();
	float getPosition_y();
	sf::Vector2f getDirection();
	sf::Vector2f getPosition();
	void setDirection(const sf::Vector2f direction);
	void setPosition_x(const float nb);
	void setPosition_y(const float nb);
	sf::CircleShape getShape();
	float distanceX(pedestrian& p);
	float distanceY(pedestrian& p);
	entry getDestination();
};

void run_pedestrians(std::vector<pedestrian>& pedestrians, std::map<std::string, Crossing> &crossings, std::stop_token stop_token);

void generatePedestrians(std::vector<pedestrian>& pedestrians);

bool canMove(std::vector<pedestrian>& pedestrians, pedestrian& p);

void freeCrossing(std::vector<pedestrian>& pedestrians, std::map<std::string, Traffic_light>& traffic_lights, std::map<std::string, Crossing>& crossings, bool& CTop, bool& CBottom, bool& CLeft, bool& CRight);