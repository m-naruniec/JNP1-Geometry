#include "geometry.h"
#include <cassert>

//checks if n is one of coll's indexes
static bool in_range(int n, const Rect_coll& coll);

//checks if rectangles are horizontally mergable
static bool mergable_hor(const Rectangle& rect1, const Rectangle& rect2);


// ** POSITION **

//normal constructor
Position::Position(int new_x, int new_y)
    : vx(new_x),
      vy(new_y)
{}

//coordinate getters
int Position::x() const {
    return vx;
}

int Position::y() const {
    return vy;
}

//equality operator
bool Position::operator==(const Position& other) const {
    return (this == &other || (vx == other.vx && vy == other.vy));
}

//reflection over x = y
Position Position::reflection() const {
    return Position(vy, vx);
}

//vector adding assignment
Position& Position::operator+=(const Vector& vec) {
    vx += vec.x();
    vy += vec.y();
    return *this;
}

//constant Position(0, 0)
const Position& Position::origin() {
    static const Position origin_position = Position(0, 0);
    return origin_position;
}

// ** VECTOR **

//normal constructor
Vector::Vector(int new_x, int new_y)
    : vx(new_x),
      vy(new_y)
{}

//coordinate getters
int Vector::x() const {
    return vx;
}

int Vector::y() const {
    return vy;
}

//equality operator
bool Vector::operator==(const Vector& other) const {
    return (this == &other || (vx == other.vx && vy == other.vy));
}

//reflection over x = y
Vector Vector::reflection() const {
    return Vector(vy, vx);
}

//adding assignment
Vector& Vector::operator+=(const Vector &vec) {
    vx += vec.vx;
    vy += vec.vy;
    return *this;
}


// ** RECTANGLE **

//normal constructor
Rectangle::Rectangle(int new_width, int new_height, const Position& new_pos)
    : vwidth(new_width),
      vheight(new_height),
      vpos(new_pos)
{
    assert(new_width >= 0 && new_height >= 0);
}

//default pos constructor
Rectangle::Rectangle(int new_width, int new_height)
    : vwidth(new_width),
      vheight(new_height),
      vpos(0, 0)
{
    assert(new_width >= 0 && new_height >= 0);
}

//size and position getters
int Rectangle::width() const {
    return vwidth;
}

int Rectangle::height() const {
    return vheight;
}

const Position& Rectangle::pos() const {
    return vpos;
}

//equality operator
bool Rectangle::operator==(const Rectangle& other) const {
    return (this == &other
            || (vwidth == other.vwidth && vheight == other.vheight && vpos == other.vpos));
}

//reflection by line y = x
Rectangle Rectangle::reflection() const {
    return Rectangle(vheight, vwidth, vpos.reflection());
}

//vector adding assignment
Rectangle& Rectangle::operator+=(const Vector& vec) {
    vpos += vec;
    return *this;
}

//area of rectangle
unsigned int Rectangle::area() const {
    int result = vwidth * vheight;
    assert(result >= 0);
    return unsigned(result);
}

//pair of rectangles cut by line y = place
Rect_pair Rectangle::split_horizontally(int place) {
    Rectangle rec1 = Rectangle(vwidth, place, vpos);
    Rectangle rec2 =
        Rectangle(vwidth, vheight - place, Position(vpos.x(), vpos.y() + place));

    return std::make_pair(std::move(rec1), std::move(rec2));
}

//pair of rectangles cut by line x = place
Rect_pair Rectangle::split_vertically(int place) {
    Rect_pair splitted = std::move(this->reflection().split_horizontally(place));
    return std::make_pair(std::move(splitted.first.reflection()),
                          std::move(splitted.second.reflection()));
}


// ** RECTANGLES **

//empty constructor
Rectangles::Rectangles()
{}

//normal (initializer_list) constructor
Rectangles::Rectangles(const std::initializer_list<Rectangle> &rect_list)
    :vcollection(rect_list)
{}

//i-th rectangle accessors
Rectangle& Rectangles::operator[](int i) {
    assert(in_range(i, vcollection));
    return vcollection[i];
}

const Rectangle& Rectangles::operator[](int i) const {
    assert(in_range(i, vcollection));
    return vcollection[i];
}

//number of rectangles held
unsigned long Rectangles::size() const {
    return vcollection.size();
}

//equality operator
bool Rectangles::operator==(const Rectangles& others) const {
    return this == &others || vcollection == others.vcollection;
}

//vector adding assignment (moves each rectangle's position)
Rectangles& Rectangles::operator+=(const Vector &vec) {
    for(Rect_coll::iterator iter = vcollection.begin(); iter != vcollection.end(); ++iter)
        *iter += vec;
    return *this;
}

//splitting idx-th rectangle and inserting resulting ones in its place
void Rectangles::split(int idx, int place, bool horizontally) {
    assert(in_range(idx, vcollection));
    Rect_pair splitted =
        (horizontally ? vcollection[idx].split_horizontally(place)
                      : vcollection[idx].split_vertically(place));
    vcollection[idx] = std::move(splitted.first);
    vcollection.insert(vcollection.begin() + idx + 1, std::move(splitted.second));
}

void Rectangles::split_horizontally(int idx, int place) {
    split(idx, place, true);
}

void Rectangles::split_vertically(int idx, int place) {
    split(idx, place, false);
}


// ** ADDITIONAL FUNCTIONS **

//adding operators
const Position operator+(const Position& pos, const Vector& vec) {
    return Position(pos) += vec;
}

const Position operator+(const Vector& vec, const Position& pos) {
    return pos + vec;
}

const Vector operator+(const Vector& left, const Vector& right) {
    return Vector(left) += right;
}

const Rectangle operator+(const Rectangle& rect, const Vector& vec) {
    return Rectangle(rect) += vec;
}

const Rectangle operator+(const Vector& vec, const Rectangle& rect) {
    return rect + vec;
}

const Rectangles operator+(Rectangles&& recs, const Vector& vec) {
    return Rectangles(std::move(recs)) += vec;
}

const Rectangles operator+(const Rectangles& recs, const Vector& vec) {
    return Rectangles(recs) += vec;
}

const Rectangles operator+(const Vector& vec, Rectangles&& recs) {
    return std::move(recs) + vec;
}

const Rectangles operator+(const Vector& vec, const Rectangles& recs) {
    return recs + vec;
}

//rectangle merging by the rules of reverse splitting
Rectangle merge_horizontally(const Rectangle& rect1, const Rectangle& rect2) {
    assert(mergable_hor(rect1, rect2));
    return Rectangle(rect1.width(), rect1.height() + rect2.height(), rect1.pos());
}

Rectangle merge_vertically(const Rectangle& rect1, const Rectangle& rect2) {
    return merge_horizontally(rect1.reflection(), rect2.reflection()).reflection();
}

//checks if n is one of coll's indexes
static bool in_range(int n, const Rect_coll& coll) {
    return n >= 0 && unsigned(n) < coll.size();
}

//checks if rectangles are horizontally mergable
static bool mergable_hor(const Rectangle& rect1, const Rectangle& rect2) {
    return rect1.width() == rect2.width()
           && rect1.pos() + Vector(0, rect1.height()) == rect2.pos();
}
