#include "DateConversionCore.h"

namespace {

int MonthDays(int year, int month)
{
    static const int kDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int d = kDays[month - 1];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        ++d;
    }
    return d;
}

bool Greater(const dateconv::Date& a, const dateconv::Date& b)
{
    if (a.year != b.year) {
        return a.year > b.year;
    }
    if (a.month != b.month) {
        return a.month > b.month;
    }
    return a.day > b.day;
}

bool Equal(const dateconv::Date& a, const dateconv::Date& b)
{
    return a.year == b.year && a.month == b.month && a.day == b.day;
}

void AddOneDay(dateconv::Date& d)
{
    ++d.day;
    int md = MonthDays(d.year, d.month);
    if (d.day > md) {
        d.day = 1;
        ++d.month;
        if (d.month > 12) {
            d.month = 1;
            ++d.year;
        }
    }
}

void SubOneDay(dateconv::Date& d)
{
    --d.day;
    if (d.day < 1) {
        --d.month;
        if (d.month < 1) {
            d.month = 12;
            --d.year;
        }
        d.day = MonthDays(d.year, d.month);
    }
}

} // namespace

namespace dateconv {

bool IsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool IsValidDate(const Date& d)
{
    if (d.year < 1900) {
        return false;
    }
    if (d.month < 1 || d.month > 12) {
        return false;
    }
    if (d.day < 1 || d.day > MonthDays(d.year, d.month)) {
        return false;
    }
    return true;
}

int DaysBetween(Date lhs, Date rhs)
{
    int count = 0;

    if (Greater(lhs, rhs)) {
        while (!Equal(lhs, rhs)) {
            SubOneDay(lhs);
            ++count;
        }
    } else {
        while (!Equal(lhs, rhs)) {
            AddOneDay(lhs);
            ++count;
        }
    }

    return count;
}

} // namespace dateconv
