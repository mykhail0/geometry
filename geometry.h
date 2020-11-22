#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <vector>

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

using size_type = std::vector<Rectangle>::size_type;

class Rectangles {
    std::vector<Rectangle> rectangles_;

  public:
    Rectangles();
    Rectangles(std::initializer_list<Rectangle>);

    Rectangle &operator[](size_type n);
    const Rectangle &operator[](size_type n) const;

    size_type size() const;

    bool operator==(const Rectangles &) const;
    Rectangles &operator+=(const Vector &);
};

Position operator+(const Position &, const Vector &);
Position operator+(const Vector &, const Position &);

Rectangle operator+(const Rectangle &, const Vector &);
Rectangle operator+(const Vector &, const Rectangle &);

Rectangles operator+(const Rectangles &, const Vector &);
Rectangles operator+(const Vector &, const Rectangles &);
Rectangles operator+(Rectangles &&, const Vector &);
Rectangles operator+(const Vector &, Rectangles &&);

// TODO sps unnecessary to do rvalue reference stuff
Rectangle merge_horizontally(const Rectangle &, const Rectangle &);
Rectangle merge_vertically(const Rectangle &, const Rectangle &);
Rectangle merge_all(const Rectangles &);

#endif // GEOMETRY_GEOMETRY_H
