const float safe_distance_car = 180.0f; // Distance minimale entre deux voitures
const float stop_distance_car = 150.0f; // Distance pour commencer à s'arrêter au feu rouge
const float max_speed_car = 20;
const float width_car = 30;
const float height_car = 60;
const auto delay_speed = 0.02s;

class Car {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    entry destination_;
    float speed_; // Vitesse actuelle
    float acceleration_; // Accélération
    bool in_crossing_;

public:
    Car(float x, float y, sf::Vector2f direction, entry destination)
        : shape_(sf::Vector2f(width_car, height_car)), direction_(direction), speed_(max_speed_car), destination_(destination), acceleration_(max_speed_car / 10.0f), in_crossing_(false) { // Réduire l'accélération
        shape_.setPosition(x, y);
        shape_.setFillColor(sf::Color::Yellow);
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
        if (speed_ < max_speed_car) {
            speed_ += acceleration_; // Accélération progressive
            if (speed_ > max_speed_car) speed_ = max_speed_car;
        }
    }

    sf::RectangleShape& getShape() { return shape_; }

    float getX() const { return shape_.getPosition().x; }
    float getY() const { return shape_.getPosition().y; }

    sf::Vector2f getDirection() const { return direction_; }

    bool isInCrossing() const { return in_crossing_; }
    void setInCrossing(bool value) { in_crossing_ = value; }

    float getDistanceTo(const Car& other) const {
        return std::sqrt(std::pow(other.getX() - getX(), 2) + std::pow(other.getY() - getY(), 2));
    }
};

void run_cars(std::vector<Car>& cars, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::stop_token stop_token);

void generate_cars(std::vector<Car>& cars);