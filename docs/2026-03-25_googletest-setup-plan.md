# GoogleTest Setup Plan For This Solution

**Date:** 25 March 2026  
**Target solution:** `scr/Calculate_atom.sln`  
**Goal:** Add GoogleTest with executable tests for `Date_Conversion`

## 1) Proposed Project Structure

Add a testable core library and a test runner project:

```text
tests/
  Calculate_atom.Tests/
    Calculate_atom.Tests.vcxproj      # new console test executable
    pch.h
    pch.cpp
    DateConversionCoreTests.cpp
scr/
  Calculate_atom.sln
  Calculate_atom.vcxproj              # existing MFC app
  DateConversionCore/
    DateConversionCore.vcxproj        # new static library
    DateConversionCore.h
    DateConversionCore.cpp
vcpkg.json                            # optional (manifest mode)
```

Why this structure:
- All test code is isolated in top-level `tests/` for clean separation from production code.
- `Date_Conversion.cpp` mixes UI and logic; tests should target pure logic without MFC dialog/message boxes.
- `DateConversionCore` isolates date math for deterministic, fast, executable unit tests.

## 2) Dependency Setup (GoogleTest)

Use **vcpkg manifest mode** (recommended for reproducibility).

Create `vcpkg.json` at repo root:

```json
{
  "name": "mfc-project-in-seu",
  "version-string": "0.1.0",
  "dependencies": [
    "gtest"
  ]
}
```

In Visual Studio 2019 (on Windows):
1. Open solution [scr/Calculate_atom.sln](../scr/Calculate_atom.sln)
2. Ensure vcpkg integration is enabled on that machine.
3. Restore/build once; vcpkg resolves GoogleTest automatically for each triplet.

## 3) `DateConversionCore` API (to enable executable tests)

Create [scr/DateConversionCore/DateConversionCore.h](../scr/DateConversionCore/DateConversionCore.h):

```cpp
#pragma once

namespace dateconv {

struct Date {
    int year;
    int month;
    int day;
};

bool IsLeapYear(int year);
bool IsValidDate(const Date& d);            // policy: year >= 1900
int DaysBetween(Date lhs, Date rhs);        // absolute day difference

} // namespace dateconv
```

Create [scr/DateConversionCore/DateConversionCore.cpp](../scr/DateConversionCore/DateConversionCore.cpp):

```cpp
#include "DateConversionCore.h"

namespace {

int MonthDays(int year, int month)
{
    static const int kDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int d = kDays[month - 1];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        ++d;
    }
    return d;
}

bool Greater(const dateconv::Date& a, const dateconv::Date& b)
{
    if (a.year != b.year) return a.year > b.year;
    if (a.month != b.month) return a.month > b.month;
    return a.day > b.day;
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

bool Equal(const dateconv::Date& a, const dateconv::Date& b)
{
    return a.year == b.year && a.month == b.month && a.day == b.day;
}

} // namespace

namespace dateconv {

bool IsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool IsValidDate(const Date& d)
{
    if (d.year < 1900) return false;
    if (d.month < 1 || d.month > 12) return false;
    if (d.day < 1 || d.day > MonthDays(d.year, d.month)) return false;
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
```

## 4) `vcxproj` Settings (Concrete)

### 4.1 `DateConversionCore.vcxproj` (static library)

Key settings:

```xml
<PropertyGroup Label="Configuration">
  <ConfigurationType>StaticLibrary</ConfigurationType>
  <PlatformToolset>v142</PlatformToolset>
  <CharacterSet>Unicode</CharacterSet>
  <UseOfMfc>false</UseOfMfc>
</PropertyGroup>

<ItemDefinitionGroup>
  <ClCompile>
    <LanguageStandard>stdcpp17</LanguageStandard>
    <WarningLevel>Level3</WarningLevel>
    <SDLCheck>true</SDLCheck>
  </ClCompile>
</ItemDefinitionGroup>
```

### 4.2 `Calculate_atom.Tests.vcxproj` (console test runner)

Key settings:

```xml
<PropertyGroup Label="Configuration">
  <ConfigurationType>Application</ConfigurationType>
  <PlatformToolset>v142</PlatformToolset>
  <CharacterSet>Unicode</CharacterSet>
  <UseOfMfc>false</UseOfMfc>
</PropertyGroup>

<ItemDefinitionGroup>
  <ClCompile>
    <LanguageStandard>stdcpp17</LanguageStandard>
    <WarningLevel>Level3</WarningLevel>
    <SDLCheck>true</SDLCheck>
  </ClCompile>
  <Link>
    <SubSystem>Console</SubSystem>
  </Link>
</ItemDefinitionGroup>

<ItemGroup>
  <ProjectReference Include="..\..\scr\DateConversionCore\DateConversionCore.vcxproj" />
</ItemGroup>
```

If vcpkg manifest mode is not auto-enabled, add:

```xml
<PropertyGroup>
  <VcpkgEnableManifest>true</VcpkgEnableManifest>
</PropertyGroup>
```

## 5) First 5 Executable GoogleTest Cases For `Date_Conversion`

Create [tests/Calculate_atom.Tests/DateConversionCoreTests.cpp](../tests/Calculate_atom.Tests/DateConversionCoreTests.cpp):

```cpp
#include <gtest/gtest.h>
#include "DateConversionCore.h"

using dateconv::Date;

TEST(DateConversionCore, IsLeapYear_2024_True)
{
    EXPECT_TRUE(dateconv::IsLeapYear(2024));
}

TEST(DateConversionCore, IsLeapYear_1900_False)
{
    EXPECT_FALSE(dateconv::IsLeapYear(1900));
}

TEST(DateConversionCore, IsValidDate_EmptyEquivalentInvalid)
{
    // Represents current UI empty-input path after atof/int conversion -> 0/0/0
    EXPECT_FALSE(dateconv::IsValidDate({0, 0, 0}));
}

TEST(DateConversionCore, DaysBetween_SameDate_Zero)
{
    EXPECT_EQ(dateconv::DaysBetween({2024, 3, 10}, {2024, 3, 10}), 0);
}

TEST(DateConversionCore, DaysBetween_LeapBoundary)
{
    EXPECT_EQ(dateconv::DaysBetween({2024, 3, 1}, {2024, 2, 29}), 1);
}
```

Optional test main in [tests/Calculate_atom.Tests/pch.cpp](../tests/Calculate_atom.Tests/pch.cpp) or dedicated `main.cpp`:

```cpp
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

## 6) Integration Point Back To Existing Dialog

After tests are green, wire [scr/Date_Conversion.cpp](../scr/Date_Conversion.cpp) to `DateConversionCore`:
1. Parse UI strings.
2. Validate via `dateconv::IsValidDate`.
3. Compute via `dateconv::DaysBetween`.
4. Keep MessageBox/formatting in dialog layer only.

This preserves current UX while making core logic testable.

## 7) Build/Run Procedure (Windows)

1. Add both new projects to [scr/Calculate_atom.sln](../scr/Calculate_atom.sln)
2. Build `DateConversionCore` then `Calculate_atom.Tests` (`Debug|x64` recommended)
3. Run `Calculate_atom.Tests.exe`
4. Expected initial result: **5 tests, all passing**

Note: keep the test project files physically under `tests/Calculate_atom.Tests/`, then add that project into the existing solution.

## 8) Definition Of Done

- GoogleTest dependency restored successfully.
- `DateConversionCore` compiled as static library.
- Test runner project builds and executes.
- First 5 tests pass in CI/local Windows environment.
- `Date_Conversion` dialog uses tested core functions for date logic.

## 9) Minimal macOS CMake Path (keeps VS solution unchanged)

This repository now includes a root [CMakeLists.txt](../CMakeLists.txt) that builds only:
- `DateConversionCore` static library
- `DateConversionCoreTests` test executable

Existing Visual Studio solution files remain unchanged for Windows development.

Run on macOS:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure
```

If GoogleTest is already installed, CMake will use `find_package(GTest)`.
If not installed, CMake falls back to `FetchContent` and downloads GoogleTest automatically.
