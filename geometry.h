#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include <utility>
#include <vector>

class Position;
class Vector;
class Rectangle;
class Rectangles;

typedef std::pair<Rectangle, Rectangle> Rect_pair;
typedef std::vector<Rectangle> Rect_coll;

class Position {
  public:
    //default copy/move constructors/assignments fit our needs

    //normal constructor
    Position(int new_x, int new_y);

    //coordinate getters
    int x() const;
    int y() const;

    //equality operator
    bool operator==(const Position& other) const;

    //reflection by line y = x
    Position reflection() const;

    //vector adding assignment
    Position& operator+=(const Vector& vec);

    //constant Position(0, 0)
    static const Position& origin();

  private:
    //coordinates
    int vx;
    int vy;
};

class Vector {
  public:
    //default copy/move constructors/assignments fit our needs

    //normal constructor
    Vector(int new_x, int new_y);

    //coordinate getters
    int x() const;
    int y() const;

    //equality operator
    bool operator==(const Vector& other) const;

    //reflection by line y = x
    Vector reflection() const;

    //adding assignment
    Vector& operator+=(const Vector& vec);

  private:
    //coordinates
    int vx;
    int vy;
};

class Rectangle {
  public:
    //default copy/move constructors/assignments fit our needs

    //normal constructor
    Rectangle(int new_width, int new_height, const Position& new_pos);

    //default pos constructor
    Rectangle(int new_width, int new_height);

    //size and position getters
    int width() const;
    int height() const;
    const Position& pos() const;

    //equality operator
    bool operator==(const Rectangle& other) const;

    //reflection by line y = x
    Rectangle reflection() const;

    //vector adding assignment (moving position by vector)
    Rectangle& operator+=(const Vector& vec);

    //area of rectangle
    unsigned int area() const;

    //pair of rectangles cut by line y = place
    Rect_pair split_horizontally(int place);

    //pair of rectangles cut by line x = place
    Rect_pair split_vertically(int place);

  private:
    //size
    int vwidth;
    int vheight;

    //left-bottom corner
    Position vpos;
};

class Rectangles {
  public:
    //default copy/move constructors/assignments fit our needs
    // empty constructor
    Rectangles();
    //normal (initializer_list) constructor
    Rectangles(const std::initializer_list<Rectangle>& rect_list);

    //i-th rectangle accessors
    Rectangle& operator[](int i);
    const Rectangle& operator[](int i) const;

    //number of rectangles held
    unsigned long size() const;

    //equality operator
    bool operator==(const Rectangles& others) const;

    //vector adding operator (moves each rectangle's position by vector)
    Rectangles& operator+=(const Vector& vec);

    //splitting idx-th rectangle and inserting resulting ones in its place
    void split_horizontally(int idx, int place);
    void split_vertically(int idx, int place);

  private:
    //std::vector of rectangles held
    Rect_coll vcollection;

    //function assisting in specific splitting
    void split(int idx, int place, bool horizontally);
};

//adding operators
const Position operator+(const Position& pos, const Vector& vec);
const Position operator+(const Vector& vec, const Position& pos);

const Vector operator+(const Vector& left, const Vector& right);

const Rectangle operator+(const Rectangle& rect, const Vector& vec);
const Rectangle operator+(const Vector& vec, const Rectangle& rect);

const Rectangles operator+(Rectangles&& recs, const Vector& rect);
const Rectangles operator+(const Rectangles& recs, const Vector& vec);
const Rectangles operator+(const Vector& vec, Rectangles&& recs);
const Rectangles operator+(const Vector& vec, const Rectangles& recs);

//rectangle merging by the rules of reverse splitting
Rectangle merge_horizontally(const Rectangle& rect1, const Rectangle& rect2);
Rectangle merge_vertically(const Rectangle& rect1, const Rectangle& rect2);

#endif //GEOMETRY_GEOMETRY_H
