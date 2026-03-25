#pragma once

namespace dateconv {

struct Date {
    int year;
    int month;
    int day;
};

bool IsLeapYear(int year);
bool IsValidDate(const Date& d);
int DaysBetween(Date lhs, Date rhs);

} // namespace dateconv
