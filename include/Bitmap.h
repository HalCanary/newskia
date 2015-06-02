/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef Bitmap_DEFINED
#define Bitmap_DEFINED

#include <memory>
#include "Memory.h"
#include "Dimensions.h"
#include "Point.h"

namespace drawing {

template <typename ColorType>
class Bitmap {
public:
    Bitmap() : dimensions_(0, 0), stride_(0), offset_(0) {}
    Bitmap(Dimensions<int> d);
    Bitmap<ColorType> subset(Point<int> min, Dimensions<int> d) const;
    Bitmap<ColorType>& operator=(const Bitmap<ColorType>&) = default;
    Bitmap(const Bitmap<ColorType>&) = default;
    Bitmap<ColorType>& operator=(Bitmap<ColorType>&&) = default;
    Bitmap(Bitmap<ColorType>&&) = default;

    const ColorType& operator()(int x, int y) const;
    ColorType& operator()(int x, int y);
    const ColorType* scanline(int y) const;
    ColorType* scanline(int y);
    const ColorType* pixels() const { return pixels_.get() + offset_; }
    ColorType* pixels() { return pixels_.get() + offset_; }
    int stride() { return stride_; }
    Dimensions<int> dimensions() { return dimensions_; }
    explicit operator bool() const;

private:
    Dimensions<int> dimensions_;
    int stride_;
    int offset_;
    Memory<ColorType> pixels_;

    Bitmap(Dimensions<int> d, int st, int of, Memory<ColorType>&& p);
};

// template<typename Color>
// class IndexedBitmap : public Bitmap<uint8_t> {
//     const Color& getColor(int x, int y) const {
//         return color_table_[(*this)(x, y)];
// private:
//     Memory<Color> color_table_;
// }

////////////////////////////////////////////////////////////////////////////////

static bool inrange(int x, int min, int max) { return x >= min && x < max; }

template <typename ColorType>
inline const ColorType& Bitmap<ColorType>::operator()(int x, int y) const {
    assert(inrange(x, 0, dimensions_.width_));
    return this->scanline(y)[x];
}

template <typename ColorType>
inline ColorType& Bitmap<ColorType>::operator()(int x, int y) {
    assert(inrange(x, 0, dimensions_.width_));
    return this->scanline(y)[x];
}

template <typename ColorType>
inline ColorType* Bitmap<ColorType>::scanline(int y) {
    assert(inrange(y, 0, dimensions_.height_));
    return this->pixels() + y * stride_;
}

template <typename ColorType>
inline const ColorType* Bitmap<ColorType>::scanline(int y) const {
    assert(inrange(y, 0, dimensions_.height_));
    return this->pixels() + y * stride_;
}

template <typename ColorType>
inline Bitmap<ColorType>::Bitmap(Dimensions<int> d)
    : dimensions_(d)
    , stride_(d.width_)
    , offset_(0)
    , pixels_(d.width_ * d.height_) {}

template <typename ColorType>
inline Bitmap<ColorType> Bitmap<ColorType>::subset(Point<int> p,
                                                   Dimensions<int> d) const {
    assert(p.x_ >= 0);
    assert(p.y_ >= 0);
    assert(p.x_ + d.width_ <= dimensions_.width_);
    assert(p.y_ + d.height_ <= dimensions_.height_);
    Memory<ColorType> copy(pixels_);
    return Bitmap(d, offset_ + p.y_ * stride_ + p.x_, stride_, std::move(copy));
}

template <typename ColorType>
inline Bitmap<ColorType>::Bitmap(Dimensions<int> d,
                                 int st,
                                 int of,
                                 Memory<ColorType>&& p)
    : dimensions_(d), stride_(st), offset_(of), pixels_(p) {}

template <typename ColorType>
inline Bitmap<ColorType>::operator bool() const {
    return dimensions_.width_ > 0 && dimensions_.height_ > 0;
}

}  // namespace drawing

#endif  // Bitmap_DEFINED
