#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <vector>

class XYObject {
  public:
    using ScalarType = int_fast32_t;
    using Coordinate = ScalarType;

    XYObject(Coordinate x, Coordinate y) : x_(x), y_(y) {}
    virtual ~XYObject() = 0;

    Coordinate x() const {return x_;}
    Coordinate y() const {return y_;}

  protected:
    Coordinate x_, y_;
};

class Position;

class Vector : public XYObject {
  public:
    Vector(Coordinate x, Coordinate y) : XYObject{x, y} {};
    explicit Vector(const Position &p);

    Vector() = delete;
    Vector(const Vector &) = default;
    Vector &operator=(const Vector &) = default;
    ~Vector() = default;

    Vector reflection() const {return Vector(y_, x_);}

    // TODO would move to .cc
    bool operator==(const Vector &other) const {
        return x_ == other.x_ && y_ == other.y_;
    };

    Vector &operator+=(const Vector &other);

    // TODO would move to .cc
    Vector operator+(const Vector &other) const {
        return Vector(x_ + other.x_, y_ + other.y_);
    }
};

class Position : public XYObject {
  public:
    Position(Coordinate x, Coordinate y) : XYObject{x, y} {};
    explicit Position(const Vector &v) : XYObject{v.x(), v.y()} {};

    Position() = delete;
    Position(const Position &) = default;
    Position &operator=(const Position &) = default;
    ~Position() = default;

    Position reflection() const {return Position(y_, x_);}

    // TODO would move to .cc
    bool operator==(const Position &other) const {
        return x_ == other.x_ && y_ == other.y_;
    }

    Position &operator+=(const Vector &v);

    static const Position &origin();
};

class Rectangle {
    using ScalarType = XYObject::ScalarType;

    ScalarType width_, height_;
    Position left_bottom_corner;

  public:
    Rectangle(ScalarType width, ScalarType height, Position position = {0, 0});

    Rectangle() = delete;
    Rectangle(const Rectangle &) = default;
    Rectangle &operator=(const Rectangle &) = default;
    ~Rectangle() = default;

    // TODO would move to .cc
    Rectangle reflection() const {
        return Rectangle(height_, width_, left_bottom_corner.reflection());
    }

    ScalarType height() const {return height_;}

    ScalarType width() const {return width_;}

    Position pos() const {return left_bottom_corner;}

    ScalarType area() const {return width_ * height_;}

    bool operator==(const Rectangle &other) const;
    Rectangle &operator+=(const Vector &v);
};

class Rectangles {
    std::vector<Rectangle> rectangles_;

  public:
    using size_type = std::vector<Rectangle>::size_type;

    Rectangles() = default;
    Rectangles(const Rectangles &) = default;
    Rectangles &operator=(const Rectangles &) = default;
    Rectangles(Rectangles &&) noexcept = default;
    Rectangles &operator=(Rectangles &&) noexcept = default;
    ~Rectangles() = default;

    Rectangles(std::initializer_list<Rectangle> il) : rectangles_(il) {}

    Rectangle &operator[](size_type n);
    const Rectangle &operator[](size_type n) const;

    size_type size() const {return rectangles_.size();}

    bool operator==(const Rectangles &) const;
    Rectangles &operator+=(const Vector &);
};

Position operator+(const Position &, const Vector &);
Position operator+(const Vector &, const Position &);

Rectangle operator+(const Rectangle &, const Vector &);
Rectangle operator+(const Vector &, const Rectangle &);

Rectangles operator+(Rectangles, const Vector &);
Rectangles operator+(const Vector &, Rectangles);

Rectangle merge_horizontally(const Rectangle &, const Rectangle &);
Rectangle merge_vertically(const Rectangle &, const Rectangle &);
Rectangle merge_all(const Rectangles &);

#endif // GEOMETRY_GEOMETRY_H
