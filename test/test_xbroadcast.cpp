/***************************************************************************
* Copyright (c) 2016, Johan Mabille, Sylvain Corlay and Wolf Vollprecht    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"
#include "xtensor/xbroadcast.hpp"
#include "xtensor/xarray.hpp"

namespace xt
{
    TEST(xbroadcast, broadcast)
    {
        xarray<double> m1
          {{1, 2, 3},
           {4, 5, 6}};

        auto m1_broadcast = broadcast(m1, {1, 2, 3});
        ASSERT_EQ(1.0, m1_broadcast(0, 0, 0));
        ASSERT_EQ(4.0, m1_broadcast(0, 1, 0));
        ASSERT_EQ(5.0, m1_broadcast(0, 1, 1));
        ASSERT_EQ(m1_broadcast.layout(), m1.layout());

        auto shape = std::vector<std::size_t> {1, 2, 3};
        auto m1_broadcast2 = broadcast(m1, shape);
        ASSERT_EQ(1.0, m1_broadcast2(0, 0, 0));
        ASSERT_EQ(4.0, m1_broadcast2(0, 1, 0));
        ASSERT_EQ(5.0, m1_broadcast2(0, 1, 1));

        double f = *(m1_broadcast.xbegin());
        xarray<double> m1_assigned = m1_broadcast;
        ASSERT_EQ(5.0, m1_assigned(0, 1, 1));
    }

    TEST(xbroadcast, element)
    {
        xarray<double> m1
          {{1, 2, 3},
           {4, 5, 6}};

        auto m1_broadcast = broadcast(m1, {4, 2, 3});

        // access with the right number of arguments
        std::array<std::size_t, 3> index1 = {0, 1, 1};
        ASSERT_EQ(5.0, m1_broadcast.element(index1.begin(), index1.end()));
        // too many arguments = using the last ones only
        std::array<std::size_t, 4> index3 = {4, 0, 1, 1};
        ASSERT_EQ(5.0, m1_broadcast.element(index3.begin(), index3.end()));
    }

    TEST(xbroadcast, shape_forwarding)
    {
        std::array<std::size_t, 2> bc_shape;
        auto m1_broadcast = broadcast(123, bc_shape);
    }

    TEST(xbroadcast, xiterator)
    {
        xarray<int> m1 = { 1, 2, 3 };
        auto m1_broadcast = broadcast(m1, { 2, 3 });
        size_t nb_iter = 3;

        // broadcast_iterator
        {
            auto iter = m1_broadcast.xbegin();
            auto iter_end = m1_broadcast.xend();
            for (size_t i = 0; i < nb_iter; ++i) ++iter;
            EXPECT_EQ(1, *iter);
            for (size_t i = 0; i < nb_iter; ++i) ++iter;
            EXPECT_EQ(iter, iter_end);
        }

        // shaped_xiterator
        {
            std::vector<size_t> shape = { 2, 2, 3 };
            auto iter = m1_broadcast.xbegin(shape);
            auto iter_end = m1_broadcast.xend(shape);
            for (size_t i = 0; i < 2 * nb_iter; ++i) ++iter;
            EXPECT_EQ(1, *iter);
            for (size_t i = 0; i < 2 * nb_iter; ++i) ++iter;
            EXPECT_EQ(iter, iter_end);
        }
    }
    
    TEST(xbroadcast, reverse_xiterator)
    {
        xarray<int> m1 = { 1, 2, 3 };
        auto m1_broadcast = broadcast(m1, { 2, 3 });
        size_t nb_iter = 3;

        // reverse_broadcast_iterator
        {
            auto iter = m1_broadcast.xrbegin();
            auto iter_end = m1_broadcast.xrend();
            for (size_t i = 0; i < nb_iter; ++i) ++iter;
            EXPECT_EQ(3, *iter);
            for (size_t i = 0; i < nb_iter; ++i) ++iter;
            EXPECT_EQ(iter, iter_end);
        }

        // reverse_shaped_xiterator
        {
            std::vector<size_t> shape = { 2, 2, 3 };
            auto iter = m1_broadcast.xrbegin(shape);
            auto iter_end = m1_broadcast.xrend(shape);
            for (size_t i = 0; i < 2 * nb_iter; ++i) ++iter;
            EXPECT_EQ(3, *iter);
            for (size_t i = 0; i < 2 * nb_iter; ++i) ++iter;
            EXPECT_EQ(iter, iter_end);
        }
    }
}

