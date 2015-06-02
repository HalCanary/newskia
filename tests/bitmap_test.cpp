/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <cstdint>
#include "Bitmap.h"
#include "gtest/gtest.h"

typedef uint32_t u32;

TEST(BitmapTest, Basic) {
    using namespace drawing;
    Bitmap<u32> empty;
    EXPECT_FALSE(empty);
    Bitmap<u32> bm(Dimensions<int>(10, 10));
    EXPECT_TRUE(bm);
    Bitmap<u32> subset = bm.subset(Point<int>(2, 2), Dimensions<int>(5, 5));
    EXPECT_TRUE(subset);
    EXPECT_EQ(subset.dimensions(), Dimensions<int>(5, 5));

    Bitmap<u32> empty1(empty);
    EXPECT_FALSE(empty1);
    Bitmap<u32> empty2;
    empty2 = empty;
    EXPECT_FALSE(empty2);

    Bitmap<u32> bm1(bm);
    EXPECT_TRUE(bm1);
    Bitmap<u32> bm2;
    bm2 = bm;
    EXPECT_TRUE(bm2);

    Bitmap<u32> subset1(subset);
    EXPECT_TRUE(subset1);
    Bitmap<u32> subset2;
    subset2 = subset;
    EXPECT_TRUE(subset2);

    EXPECT_EQ(bm(2,3), 0u);
    bm(2,3) = 17u;
    EXPECT_EQ(bm(2,3), 17u);
    EXPECT_EQ(bm1(2,3), 17u);
    EXPECT_EQ(bm2(2,3), 17u);
    EXPECT_EQ(subset(0,1), 17u);
    EXPECT_EQ(subset1(0,1), 17u);
    EXPECT_EQ(subset2(0,1), 17u);
}
