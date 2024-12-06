const float safe_distance_bus = 350.0f; // Distance minimale entre deux voitures
const float stop_distance_bus = 190.0f; // Distance pour commencer à s'arrêter au feu rouge
const float max_speed_bus = 20;
const float acceleration_bus = 1.3;
const float width_bus = 40;
const float height_bus = 130;

class Bus {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    entry destination_;
    entry origin_;
    float speed_; // Vitesse actuelle
    float acceleration_; // Accélération
    bool in_crossing_;

public:
    Bus(float x, float y, sf::Vector2f direction, entry destination)
        : shape_(sf::Vector2f(width_bus, height_bus)), direction_(direction), speed_(max_speed_bus), destination_(destination), acceleration_(acceleration_bus), in_crossing_(false) { // Réduire l'accélération
        shape_.setPosition(x, y);
        shape_.setFillColor(sf::Color::Magenta);
        shape_.setOrigin(width_bus / 2, height_bus / 2);
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
        if (speed_ < max_speed_bus) {
            speed_ += acceleration_; // Accélération progressive
            if (speed_ > max_speed_bus) speed_ = max_speed_bus;
        }
    }

    sf::RectangleShape& getShape() { return shape_; }

    float getX() const { return shape_.getPosition().x; }
    float getY() const { return shape_.getPosition().y; }

    sf::Vector2f getDirection() const { return direction_; }

    bool isInCrossing() const { return in_crossing_; }
    void setInCrossing(bool value) { in_crossing_ = value; }

    float getDistanceTo(const Bus& other) const {
        return std::sqrt(std::pow(other.getX() - getX(), 2) + std::pow(other.getY() - getY(), 2));
    }

    entry getDestination() {
        return destination_;
    }

    void turn();
};

void run_buses(std::vector<Bus>& buses, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::stop_token stop_token);

void generate_buses(std::vector<Bus>& buses);