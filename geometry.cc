#include "geometry.h"

#define m_assert(expr, msg) assert(((void)(msg), (expr)))

namespace {
    bool horizontal_merge_possible(const Rectangle &rect1,
                                   const Rectangle &rect2) {
        return rect1.width() == rect2.width() &&
               rect1.pos() + Vector(0, rect1.height()) == rect2.pos();
    }

    bool vertical_merge_possible(const Rectangle &rect1,
                                 const Rectangle &rect2) {
        return rect1.height() == rect2.height() &&
               rect1.pos() + Vector(rect1.width(), 0) == rect2.pos();
    }
} // namespace

XYObject::~XYObject() {};

Vector::Vector(const Position &p) : XYObject{p.x(), p.y()} {}

Vector &Vector::operator+=(const Vector &other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
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

Rectangle::Rectangle(ScalarType width, ScalarType height,
                     const Position position)
    : width_(width), height_(height), left_bottom_corner(position) {
    m_assert(height_ > 0 && width_ > 0,
             "Both dimensions of a rectangle must be positive.");
}

bool Rectangle::operator==(const Rectangle &other) const {
    return width_ == other.width_ && height_ == other.height_ &&
           left_bottom_corner == other.left_bottom_corner;
}

Rectangle &Rectangle::operator+=(const Vector &v) {
    left_bottom_corner += v;
    return *this;
}

Position operator+(const Position &p, const Vector &v) {
    return Position(p.x() + v.x(), p.y() + v.y());
}

// TODO would move to .h
Position operator+(const Vector &v, const Position &p) {
    return p + v;
}

Rectangle operator+(const Rectangle &r, const Vector &v) {
    return Rectangle(r.width(), r.height(), r.pos() + v);
}

// TODO would move to .h
Rectangle operator+(const Vector &v, const Rectangle &r) {
    return r + v;
}

Rectangle &Rectangles::operator[](size_type n) {
    m_assert(n < rectangles_.size(),
             "Trying to access an element out of bounds.");
    return rectangles_[n];
}

const Rectangle &Rectangles::operator[](size_type n) const {
    m_assert(n < rectangles_.size(),
             "Trying to access an element out of bounds.");
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

Rectangles operator+(Rectangles rects, const Vector &v) {
    // or to allow copy elision(?) (rects += v)
    return std::move(rects += v);
}

Rectangles operator+(const Vector &v, Rectangles rects) {
    // or to allow copy elision(?) (rects + v)
    return std::move(rects) + v;
}

Rectangle merge_horizontally(const Rectangle &r1, const Rectangle &r2) {
    m_assert(horizontal_merge_possible(r1, r2),
             "Horizontal merge is impossible");
    return Rectangle(r1.width(), r1.height() + r2.height(), r1.pos());
}

Rectangle merge_vertically(const Rectangle &r1, const Rectangle &r2) {
    m_assert(vertical_merge_possible(r1, r2), "Vertical merge is impossible");
    return Rectangle(r1.width() + r2.width(), r1.height(), r1.pos());
}

Rectangle merge_all(const Rectangles &rects) {
    m_assert(rects.size() > 0,
             "Merge failed, empty collection cannot be merged");
    Rectangle ans = rects[0];
    for (Rectangles::size_type i = 1; i < rects.size(); ++i) {
        if (horizontal_merge_possible(ans, rects[i])) {
            //Rectangle dummy(merge_horizontally(ans, rects[i]));
            //ans = dummy;
            ans = merge_horizontally(ans, rects[i]);
        } else if (vertical_merge_possible(ans, rects[i])) {
            //Rectangle dummy(merge_vertically(ans, rects[i]));
            //ans = dummy;
            ans = merge_vertically(ans, rects[i]);
        }else {
            m_assert(false,
                     "Merge failed, certain rectangles cannot be merged");
        }
    }
    return ans;
}
