#include "pch.h"

#include <gtest/gtest.h>

#include "DateConversionCore.h"

using dateconv::Date;

TEST(DateConversionCore, IsLeapYear2024True)
{
    EXPECT_TRUE(dateconv::IsLeapYear(2024));
}

TEST(DateConversionCore, IsLeapYear1900False)
{
    EXPECT_FALSE(dateconv::IsLeapYear(1900));
}

TEST(DateConversionCore, IsValidDateEmptyEquivalentInvalid)
{
    EXPECT_FALSE(dateconv::IsValidDate({0, 0, 0}));
}

TEST(DateConversionCore, DaysBetweenSameDateZero)
{
    EXPECT_EQ(dateconv::DaysBetween({2024, 3, 10}, {2024, 3, 10}), 0);
}

TEST(DateConversionCore, DaysBetweenLeapBoundary)
{
    EXPECT_EQ(dateconv::DaysBetween({2024, 3, 1}, {2024, 2, 29}), 1);
}

TEST(DateConversionCore, IsLeapYear2000True)
{
    EXPECT_TRUE(dateconv::IsLeapYear(2000));
}

TEST(DateConversionCore, IsValidDateRejectsInvalidMonth)
{
    EXPECT_FALSE(dateconv::IsValidDate({2024, 13, 1}));
}

TEST(DateConversionCore, IsValidDateRejectsInvalidDay)
{
    EXPECT_FALSE(dateconv::IsValidDate({2023, 2, 29}));
}

TEST(DateConversionCore, IsValidDateAcceptsLeapDay)
{
    EXPECT_TRUE(dateconv::IsValidDate({2024, 2, 29}));
}

TEST(DateConversionCore, DaysBetweenForwardSameMonth)
{
    EXPECT_EQ(dateconv::DaysBetween({2024, 3, 10}, {2024, 3, 12}), 2);
}

TEST(DateConversionCore, DaysBetweenForwardAcrossYearBoundary)
{
    EXPECT_EQ(dateconv::DaysBetween({2024, 12, 31}, {2025, 1, 1}), 1);
}

TEST(DateConversionCore, DaysBetweenBackwardAcrossYearBoundary)
{
    EXPECT_EQ(dateconv::DaysBetween({2025, 1, 1}, {2024, 12, 31}), 1);
}
