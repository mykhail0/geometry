#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include <cassert>
#include <cstdint>

using ScalarType = int_fast32_t;
using Coordinate = ScalarType;

class XYObject {
  protected:
    Coordinate x_, y_;

  public:
    XYObject(Coordinate x, Coordinate y);

    Coordinate x() const;
    Coordinate y() const;
};

class Position;

class Vector : public XYObject {
  public:
    Vector(Coordinate x, Coordinate y);
    explicit Vector(const Position &p);

    Vector reflection() const;

    bool operator==(const Vector &other) const;
    Vector &operator+=(const Vector &other);
    Vector operator+(const Vector &other) const;
};

class Position : public XYObject {
  public:
    Position(Coordinate x, Coordinate y);
    explicit Position(const Vector &v);

    Position reflection() const;

    bool operator==(const Position &other) const;
    Position &operator+=(const Vector &v);

    static const Position &origin();
};

class Rectangle {
    ScalarType width_, height_;
    Position left_bottom_corner;

  public:
    Rectangle(ScalarType width, ScalarType height);
    Rectangle(ScalarType width, ScalarType height, Position position);

    Rectangle reflection() const;

    ScalarType height() const;
    ScalarType width() const;
    Position pos() const;
    uint64_t area() const; // TODO type alias?

    bool operator==(const Rectangle &other) const;
    Rectangle &operator+=(const Vector &v);
};

Position operator+(const Position &, const Vector &);
Position operator+(const Vector &, const Position &);

Rectangle operator+(const Rectangle &, const Vector &);
Rectangle operator+(const Vector &, const Rectangle &);

#endif // GEOMETRY_GEOMETRY_H
