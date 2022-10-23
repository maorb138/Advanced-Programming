/*
 * minCircle.cpp
 *
 * Author:308540343 , Maor Biton.
 */

#include "minCircle.h"

Circle helperMinCircle(Point **&p, vector<Point> point1, int size);

Circle minTrivial(vector<Point> points);


float distance2(Point &p1, Point &p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

bool isContain(Circle &c, Point &p) {
    float a = p.x - c.center.x;
    float b = p.y - c.center.y;
    float r = c.radius;
    if ((a * a + b * b) <= r * r)
        return true;
    else
        return false;
}

bool equal(Point &p1, Point &p2) {
    return ((p1.x == p2.x) && (p1.y == p2.y) ||
            (p1.y == p2.x) && (p1.x == p2.y));
}

Circle createCircle(Point &p1, Point &p2, Point &p3) {
    float bx = p2.x - p1.x;
    float by = p2.y - p1.y;
    float cx = p3.x - p1.x;
    float cy = p3.y - p1.y;
    float b = pow(bx, 2) + pow(by, 2);
    float c = pow(cx, 2) + pow(cy, 2);
    float d = ((bx * cy) - (by * cx));
    float c1 = ((cy * b - by * c) / (2 * d));
    float c2 = ((bx * c - cx * b) / (2 * d));
    Point p = Point(c1, c2);
    p.x = p.x + p1.x;
    p.y = p.y + p1.y;
    return {p, distance2(p, p1)};
}

Circle makeCircle(Point &p1, Point &p2) {
    float mid1 = (p1.x + p2.x) / 2;
    float mid2 = (p1.y + p2.y) / 2;
    Point point = Point(mid1, mid2);
    float radius = distance2(p1, point);
    return Circle(point, radius);
}

Circle minTrivial(vector<Point> points) {
    int size = points.size();
    switch (size) {
        case 1:
            return {points.at(0), 0};
        case 2:
            return {midPoint(points.at(0), points.at(1)), distance2(points.at(0), points.at(1)) / 2};
        case 3:
            return createCircle(points.at(0), points.at(1), points.at(2));
        default:
            return {{0, 0}, 0};
    }
}

Point midPoint(Point &p1, Point &p2) {
    return {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
}

Circle helperMinCircle(Point **&pPoint, vector<Point> point1, int size) {
    if (size == 0 || point1.size() == 3) {
        return {minTrivial(point1)};
    }
    Point p = *pPoint[size - 1];
    Circle circle = helperMinCircle(pPoint, point1, (size - 1));
    if (isContain(circle, p)) {
        return circle;
    }
    point1.push_back(p);
    return helperMinCircle(pPoint, point1, size - 1);
}


Circle findMinCircle(Point **&points, int size) {
    vector<Point> v;
    return helperMinCircle(points, v, size);
}
