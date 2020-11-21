#include "geometry.h"

#define m_assert(expr, msg) assert(((void)(msg), (expr)))

Vector::Vector(const Position &p) : XYObject{p.x(), p.y()} {
}

XYObject::XYObject(Coordinate x, Coordinate y) : x_(x), y_(y) {
}

Coordinate XYObject::x() const {
    return x_;
}

Coordinate XYObject::y() const {
    return y_;
}

Vector::Vector(Coordinate x, Coordinate y) : XYObject{x, y} {
}

bool Vector::operator==(const Vector &other) const {
    return x_ == other.x_ && y_ == other.y_;
}

Vector &Vector::operator+=(const Vector &other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}

Vector Vector::operator+(const Vector &other) const {
    return Vector(x_ + other.x_, y_ + other.y_);
}

Vector Vector::reflection() const {
    return Vector(y_, x_);
}

Position::Position(Coordinate x, Coordinate y) : XYObject{x, y} {
}

Position::Position(const Vector &v) : XYObject{v.x(), v.y()} {
}

bool Position::operator==(const Position &other) const {
    return x_ == other.x_ && y_ == other.y_;
}

Position Position::reflection() const {
    return Position(y_, x_);
}

const Position &Position::origin() {
    static Position o(0, 0);
    return o;
}

Position &Position::operator+=(const Vector &v) {
    x_ += v.x();
    y_ += v.y();
    return *this;
}

Rectangle::Rectangle(ScalarType width, ScalarType height)
    : width_(width), height_(height), left_bottom_corner(Position{0, 0}) {
    m_assert(height_ > 0 && width_ > 0, "Both dimensions of a rectangle must be positive.");
}

Rectangle::Rectangle(ScalarType width, ScalarType height, const Position position)
    : width_(width), height_(height), left_bottom_corner(position) {
    m_assert(height_ > 0 && width_ > 0, "Both dimensions of a rectangle must be positive.");
}

bool Rectangle::operator==(const Rectangle &other) const {
    return width_ == other.width_ && height_ == other.height_ &&
           left_bottom_corner == other.left_bottom_corner;
}

Rectangle Rectangle::reflection() const {
    return Rectangle(height_, width_, left_bottom_corner.reflection());
}

ScalarType Rectangle::height() const {
    return height_;
}

ScalarType Rectangle::width() const {
    return width_;
}

uint64_t Rectangle::area() const {
    return width_ * height_;
}

Position Rectangle::pos() const {
    return left_bottom_corner;
}

Rectangle &Rectangle::operator+=(const Vector &v) {
    left_bottom_corner += v;
    return *this;
}

Position operator+(const Position &p, const Vector &v) {
    return Position(p.x() + v.x(), p.y() + v.y());
}

Position operator+(const Vector &v, const Position &p) {
    return p + v;
}

Rectangle operator+(const Rectangle &r, const Vector &v) {
    return Rectangle(r.width(), r.height(), r.pos() + v);
}

Rectangle operator+(const Vector &v, const Rectangle &r) {
    return r + v;
}
