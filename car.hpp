class Car {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    float speed_; // Vitesse actuelle
    float max_speed_; // Vitesse maximale
    float acceleration_; // Accélération
    bool in_crossing_;

public:
    Car(float x, float y, float width, float height, sf::Vector2f direction, float max_speed)
        : shape_(sf::Vector2f(width, height)), direction_(direction), speed_(0), max_speed_(max_speed), acceleration_(max_speed / 240.0f), in_crossing_(false) { // Réduire l'accélération
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
        if (speed_ < max_speed_) {
            speed_ += acceleration_; // Accélération progressive
            if (speed_ > max_speed_) speed_ = max_speed_;
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