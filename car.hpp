class Car {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    float speed_;
    bool in_crossing_; // Indique si la voiture est dans le croisement

public:
    Car(float x, float y, float width, float height, sf::Vector2f direction, float speed)
        : shape_(sf::Vector2f(width, height)), direction_(direction), speed_(speed), in_crossing_(false) {
        shape_.setPosition(x, y);
        shape_.setFillColor(sf::Color::Yellow);
    }

    void move() {
        shape_.move(direction_ * speed_);
    }

    void stop() {
        speed_ = 0; // Arrête la voiture
    }

    void resume(float speed) {
        speed_ = speed; // Reprend le mouvement
    }

    sf::RectangleShape& getShape() { return shape_; }

    float getX() const { return shape_.getPosition().x; }
    float getY() const { return shape_.getPosition().y; }

    bool isInCrossing() const { return in_crossing_; }
    void setInCrossing(bool value) { in_crossing_ = value; }
};