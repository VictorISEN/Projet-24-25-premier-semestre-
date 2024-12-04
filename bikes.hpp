const float safe_distance_bike = 80.0f; // Distance minimale entre deux voitures
const float stop_distance_bike = 40.0f; // Distance pour commencer à s'arrêter au feu rouge
const float max_speed_bike = 10;
const float acceleration_bikes = 2.0f;
const float width_bike = 15;
const float height_bike = 30;

class Bike {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    entry destination_;
    float speed_; // Vitesse actuelle
    float acceleration_; // Accélération
    bool in_crossing_;

public:
    Bike(float x, float y, sf::Vector2f direction, entry destination)
        : shape_(sf::Vector2f(width_bike, height_bike)), direction_(direction), speed_(max_speed_bike), destination_(destination), acceleration_(acceleration_bikes), in_crossing_(false) { // Réduire l'accélération
        shape_.setPosition(x, y);
        shape_.setFillColor(sf::Color::Blue);
    }

    void move() {
        shape_.move(direction_ * speed_);
    }

    void stop() {
        if (speed_ > 0) {
            speed_ -= acceleration_; // Décélération progressive
            if (speed_ < 0) speed_ = 0;
        }
    }

    void accelerate() {
        if (speed_ < max_speed_bike) {
            speed_ += acceleration_; // Accélération progressive
            if (speed_ > max_speed_bike) speed_ = max_speed_bike;
        }
    }

    sf::RectangleShape& getShape() { return shape_; }

    float getX() const { return shape_.getPosition().x; }
    float getY() const { return shape_.getPosition().y; }

    sf::Vector2f getDirection() const { return direction_; }

    bool isInCrossing() const { return in_crossing_; }
    void setInCrossing(bool value) { in_crossing_ = value; }

    float getDistanceTo(const Bike& other) const {
        return std::sqrt(std::pow(other.getX() - getX(), 2) + std::pow(other.getY() - getY(), 2));
    }
};

void run_bikes(std::vector<Bike>& bikes, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::stop_token stop_token);

void generate_bikes(std::vector<Bike>& bikes);