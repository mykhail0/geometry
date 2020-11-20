#include "geometry.h"

Vector::Vector(const Position &p) : XYObject{p.x(), p.y()} {
}

Position operator+(const Position &p, const Vector &v) {
    return Position(p) += v;
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