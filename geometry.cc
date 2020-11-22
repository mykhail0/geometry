#include "geometry.h"

#define m_assert(expr, msg) assert(((void)(msg), (expr)))

namespace {
    bool horizontal_merge_possible(const Rectangle &rect1, const Rectangle &rect2) {
        return rect1.width() == rect2.width() &&
               rect1.pos() + Vector(0, rect1.height()) == rect2.pos();
    }

    bool vertical_merge_possible(const Rectangle &rect1, const Rectangle &rect2) {
        return rect1.height() == rect2.height() &&
               rect1.pos() + Vector(rect1.width(), 0) == rect2.pos();
    }
} // namespace

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

// RECTANGLES
Rectangles::Rectangles() : rectangles_() {
}

Rectangles::Rectangles(std::initializer_list<Rectangle> il) : rectangles_(il) {
}

Rectangle &Rectangles::operator[](size_type n) {
    m_assert(n < rectangles_.size(), "Trying to access an element out of bounds.");
    return rectangles_[n];
}

size_type Rectangles::size() const {
    return rectangles_.size();
}

const Rectangle &Rectangles::operator[](size_type n) const {
    m_assert(n < rectangles_.size(), "Trying to access an element out of bounds.");
    return rectangles_[n];
}

bool Rectangles::operator==(const Rectangles &rects) const {
    if (rects.size() != this->size())
        return false;
    for (size_type i = 0; i < this->size(); ++i) {
        if (!((*this)[i] == rects[i]))
            return false;
    }
    return true;
}

Rectangles &Rectangles::operator+=(const Vector &v) {
    for (Rectangle &r : rectangles_)
        r += v;
    return *this;
}

Rectangles operator+(const Rectangles &rects, const Vector &v) {
    Rectangles ans = rects;
    for (size_type i = 0; i < ans.size(); ++i)
        ans[i] += v;
    return ans;
}

Rectangles operator+(const Vector &v, const Rectangles &rects) {
    return rects + v;
}

Rectangles operator+(Rectangles &&rects, const Vector &v) {
    return std::move(rects += v);
}

Rectangles operator+(const Vector &v, Rectangles &&rects) {
    return std::forward<Rectangles>(rects) + v;
}

Rectangle merge_horizontally(const Rectangle &r1, const Rectangle &r2) {
    m_assert(horizontal_merge_possible(r1, r2), "Horizontal merge is impossible");
    return Rectangle(r1.width(), r1.height() + r2.height(), r1.pos());
}

Rectangle merge_vertically(const Rectangle &r1, const Rectangle &r2) {
    m_assert(vertical_merge_possible(r1, r2), "Vertical merge is impossible");
    return Rectangle(r1.width() + r2.width(), r1.height(), r1.pos());
}

Rectangle merge_all(const Rectangles &rects) {
    m_assert(rects.size() > 0, "Merge failed, empty collection cannot be merged");
    Rectangle ans = rects[0];
    for (size_type i = 1; i < rects.size(); ++i) {
        if (horizontal_merge_possible(ans, rects[i]))
            ans = merge_horizontally(ans, rects[i]);
        else if (vertical_merge_possible(ans, rects[i]))
            ans = merge_vertically(ans, rects[i]);
        else
            m_assert(false, "Merge failed, certain rectangles cannot be merged");
    }
    return ans;
}
