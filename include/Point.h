/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef Point_DEFINED
#define Point_DEFINED
namespace drawing {
template <typename T>
struct Point {
    T x_, y_;
    Point() {}
    Point(T x, T y) : x_(x), y_(y) {}
    bool operator==(const Point<T>& rhs) const {
        return x_ == rhs.x_ && y_ == rhs.y_;
    }
    bool operator!=(const Point<T>& rhs) const { return !(*this == rhs); }
};
}
#endif  // Point_DEFINED
