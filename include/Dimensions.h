/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef Dimensions_DEFINED
#define Dimensions_DEFINED
namespace drawing {
template <typename T>
struct Dimensions {
    T width_, height_;
    Dimensions() {}
    Dimensions(T w, T h) : width_(w), height_(h) {}
    bool operator==(const Dimensions<T>& rhs) const {
        return width_ == rhs.width_ && height_ == rhs.height_;
    }
    bool operator!=(const Dimensions<T>& rhs) const { return !(*this == rhs); }
};
}
#endif  // Dimensions_DEFINED
