#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include <cassert>
#include <cstdint>

using ScalarType = int_fast32_t;
using Coordinate = ScalarType;

#define m_assert(expr, msg) assert(((void)(msg), (expr)))

class XYObject {
  protected:
    Coordinate x_, y_;

  public:
    XYObject(Coordinate x, Coordinate y) : x_(x), y_(y) {
    }

    Coordinate x() const {
        return x_;
    }

    Coordinate y() const {
        return y_;
    }
};

class Position;
/*
- Vector implementuje operator "+=(vec)", gdzie vec jest obiektem klasy Vector.
  Operator powinien zwrócić referencję do wołanego obiektu.
 */
class Vector : public XYObject {
  public:
    Vector(Coordinate x, Coordinate y) : XYObject{x, y} {
    }
    explicit Vector(const Position &p);

    bool operator==(const Vector &other) const {
        return x_ == other.x_ && y_ == other.y_;
    }

    Vector &operator+=(const Vector &other) {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    Vector &operator+(const Vector &other) const {
        return Vector(other) += *this;
    }

    Vector reflection() const {
        return Vector(y_, x_);
    }
};

class Position : public XYObject {
  public:
    Position(Coordinate x, Coordinate y) : XYObject{x, y} {
    }

    explicit Position(const Vector &v) : XYObject{v.x(), v.y()} {
    }

    bool operator==(const Position &other) const {
        return x_ == other.x_ && y_ == other.y_;
    }

    Position reflection() const {
        return Position(y_, x_);
    }

    static const Position origin() {
        return Position(0, 0);
    }

    Position &operator+=(const Vector &v) {
        x_ += v.x();
        y_ += v.y();
        return *this;
    }
};

Position operator+(const Position &, const Vector &);
Position operator+(const Vector &, const Position &);

class Rectangle {
    ScalarType width_, height_;
    Position left_bottom_corner;

  public:
    Rectangle(ScalarType width, ScalarType height)
        : width_(width), height_(height), left_bottom_corner(Position{0, 0}) {
        m_assert(height_ > 0 && width_ > 0, "Both dimensions of a rectangle must be positive.");
    }

    Rectangle(ScalarType width, ScalarType height, const Position position)
        : width_(width), height_(height), left_bottom_corner(position) {
        m_assert(height_ > 0 && width_ > 0, "Both dimensions of a rectangle must be positive.");
    }

    bool operator==(const Rectangle &other) const {
        return width_ == other.width_ && height_ == other.height_ &&
               left_bottom_corner == other.left_bottom_corner;
    }

    Rectangle reflection() const {
        return Rectangle(height_, width_, left_bottom_corner.reflection());
    }

    ScalarType height() const {
        return height_;
    }

    ScalarType width() const {
        return width_;
    }

    uint64_t area() const { // TODO type alias
        return width_ * height_;
    }

    Position pos() const {
        return left_bottom_corner;
    }

    Rectangle &operator+=(const Vector &v) {
        left_bottom_corner += v;
        return *this;
    }
};

Rectangle operator+(const Rectangle &, const Vector &);
Rectangle operator+(const Vector &, const Rectangle &);

#endif // GEOMETRY_GEOMETRY_H
