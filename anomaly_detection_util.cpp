/*
 * anomaly_detection_util.cpp
 *
 * Author:308540343 , Maor Biton.
 */
#include "anomaly_detection_util.h"
#include <iostream>
#include "cmath"

float avg(float *x, int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    return (sum / (float) size);

}

float var(float *x, int size) {
    float sum = 0.0;
    float m = avg(x, size);
    for (int i = 0; i < size; i++) {
        sum += (std::pow(x[i] - m, 2));
    }
    return sum / (float) size;

}

float cov(float *x, float *y, int size) {
    float xm = avg(x, size);
    float ym = avg(y, size);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += ((x[i] - xm) * (y[i] - ym));
    }
    return sum / (float) size;
}

float pearson(float *x, float *y, int size) {
    return (cov(x, y, size) / ((std::sqrt(var(x, size))) * (std::sqrt(var(y, size)))));
}

Line linear_reg(Point **points, int size) {
    float x[size];
    float y[size];
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - a * (avg(x, size));
    Line *n = new Line(a, b);
    return *n;
}

float dev(Point p, Line l) {
    return std::abs(l.f(p.x) - p.y);
}

float dev(Point p, Point **points, int size) {
    Line line = linear_reg(points, size);
    return dev(p, line);
}
