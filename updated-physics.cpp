#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>

struct Vector2 {
    double x, y;
    Vector2(double x = 0, double y = 0) : x(x), y(y) {}
    ~Vector2() {}

    double dot(Vector2 other) const {
        return x * other.x + y * other.y;
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(double scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    double length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        double len = length();
        double invLength = 1.0f / len;
        if (len == 0) return Vector2();
        return Vector2(x * invLength, y * invLength);
    }
};

std::string remove_decimal(double f) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(7) << f;
    std::string s = ss.str();

    // Remove the decimal symbol and all digits before it if present
    size_t decimal_pos = s.find(".");
    if (decimal_pos != std::string::npos) 
        s.erase(0, decimal_pos + 1); // delete everything before and including the decimal point

    int j = 0;
    for (int i = 7; i >= 0; i -= 2) {
        s.erase(i + j, 1);
        j++;
    }

    return s;
}

class Particle {
public:
    Particle(double x, double y, double vx, double vy, double ax, double ay, double radius)
    : pos(x, y), vel(vx, vy), acc(ax, ay), radius(radius) {}
    ~Particle() {}

    void update(double dt) {
        vel += acc * dt;
        pos += vel * dt;
    }

    double radius;
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
};

class Circle {
public:
    Circle(double x, double y, double radius) : pos(x, y), radius(radius) {}
    ~Circle() {}

    bool checkCollision(const Particle& particle) {
        double dist = (particle.pos - pos).dot(particle.pos - pos);
        return dist > (radius - particle.radius) * (radius - particle.radius);
    }

    void handleCollision(Particle& particle) {
        Vector2 collisionPoint = closestPointOnCircle(particle.pos);
        Vector2 normal = (particle.pos - collisionPoint).normalized();
        Vector2 reflection = particle.vel - (normal * 2.0f * particle.vel.dot(normal));
        particle.vel = reflection;
    }

private:
    Vector2 pos;
    double radius;

    Vector2 closestPointOnCircle(const Vector2& point) {
        Vector2 direction = point - pos;
        direction = direction.normalized();
        return pos + direction * radius;
    }
};

int main() {

    Particle particle = Particle(0.121237, -0.897, 1.234984, -2.04567, 0, 0, 0);
    Circle perfectCircle(10.12359, -4.17853, 6.98765);
    std::string vals = "";
    std::ofstream outfile("nums.txt");

    for (int i = 0; i < 100000; i++) {
        vals = "";
        particle.update(2);
        if (perfectCircle.checkCollision(particle)) {
            perfectCircle.handleCollision(particle);
        }
        vals = remove_decimal(particle.pos.x);
        outfile << vals << std::endl;
    }

    return 0;
} 