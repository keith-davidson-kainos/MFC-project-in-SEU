# Date_Conversion Test Suite (Draft)

**Date:** 25 March 2026  
**Module:** `scr/Date_Conversion.cpp`, `scr/Date_Conversion.h`  
**Test Types:** Unit, edge-case/robustness, integration/contract (module interface)  
**Status:** Design-ready (repo currently has no automated C++ test framework configured)

## Scope

Public members and key code paths targeted:
- `Date_Conversion::OnBnClickedButton1()` - date difference workflow
- `Date_Conversion::OnBnClickedButton2()` - current date display workflow
- `Date_Conversion::OnBnClickedButton4()` - currently empty handler
- Leap-year logic path (`IsLeapYear` semantics used by internal date math)
- Internal date arithmetic paths (`operator+`, `operator-`, comparison/equality loops) through observable behavior of button handlers

## Assumptions

- Current implementation parses input using `atof` then `int(...)` conversion.
- Date validation is not enforced before computing differences.
- No API endpoints/services exist; integration tests are dialog-interface contract tests.

---

## A) Unit Tests (Public Methods + Key Paths)

### A1. Leap-Year Semantics

| ID | Target | Input | Expected Result |
|---|---|---|---|
| UT-LY-001 | Leap-year rule (divisible by 4, not by 100) | `2024` | `true` |
| UT-LY-002 | Century non-leap year | `1900` | `false` |
| UT-LY-003 | Century leap year (divisible by 400) | `2000` | `true` |
| UT-LY-004 | Common year | `2023` | `false` |

### A2. Date Difference Core Paths (via `OnBnClickedButton1` behavior)

| ID | Path | Input Date1 | Input Date2 | Expected Difference |
|---|---|---|---|---:|
| UT-DD-001 | Equal dates path (`*this == d`) | 2024-03-10 | 2024-03-10 | 0 |
| UT-DD-002 | Forward traversal path (`*this > d`) | 2024-03-11 | 2024-03-10 | 1 |
| UT-DD-003 | Backward traversal path (`*this <= d`) | 2024-03-10 | 2024-03-11 | 1 |
| UT-DD-004 | Month boundary | 2024-03-01 | 2024-02-28 | 2 (leap year) |
| UT-DD-005 | Leap-day crossing | 2024-03-01 | 2024-02-29 | 1 |
| UT-DD-006 | Year boundary | 2025-01-01 | 2024-12-31 | 1 |
| UT-DD-007 | Long range | 2026-03-25 | 2020-03-25 | 2191 |

### A3. Current Date Display Path

| ID | Target | Action | Expected Result |
|---|---|---|---|
| UT-CD-001 | `OnBnClickedButton2` format path | Click “current date” button | Message shown with format `现在北京时间是YYYY年MM月DD日` and title `当前日期` |

### A4. Empty Handler Regression

| ID | Target | Action | Expected Result |
|---|---|---|---|
| UT-EH-001 | `OnBnClickedButton4` no-op | Click button 4 | No crash, no state corruption, no unexpected dialog close |

---

## B) Edge-Case Tests (Error Handling, Boundaries, Null/Empty Inputs)

> These tests should be tracked as **expected-fail or behavior-gap** if current implementation lacks validation.

### B1. Empty/Null-like Inputs

| ID | Input (`y1,m1,d1,y2,m2,d2`) | Expected (Robust Behavior) | Current Risk |
|---|---|---|---|
| EC-IN-001 | `"", "", "", "", "", ""` | Validation error message, no computation | `atof("") -> 0`, computes with invalid year/month/day |
| EC-IN-002 | `"2024", "", "10", "2024", "03", "09"` | Validation error message | Invalid month converted to 0 |
| EC-IN-003 | `"2024", "03", "", "2024", "03", "09"` | Validation error message | Invalid day converted to 0 |

### B2. Non-numeric / mixed input

| ID | Input | Expected (Robust Behavior) | Current Risk |
|---|---|---|---|
| EC-IN-004 | `"abcd", "03", "10", "2024", "03", "09"` | Validation error message | Non-numeric year becomes 0 |
| EC-IN-005 | `"2024x", "03", "10", "2024", "03", "09"` | Validation error message | Partial parse accepted by `atof` |
| EC-IN-006 | `"  ", "03", "10", "2024", "03", "09"` | Validation error message | Whitespace resolves to 0 |

### B3. Calendar boundary validity

| ID | Input Date1 | Input Date2 | Expected (Robust Behavior) | Current Risk |
|---|---|---|---|---|
| EC-DT-001 | 2023-02-29 | 2023-03-01 | Reject invalid date | No explicit validation path |
| EC-DT-002 | 2024-02-29 | 2024-03-01 | Accept as valid | Depends on internal path only |
| EC-DT-003 | 2024-13-01 | 2024-12-01 | Reject invalid month | Invalid month may flow into algorithm |
| EC-DT-004 | 2024-00-10 | 2024-01-10 | Reject invalid month | Same risk |
| EC-DT-005 | 2024-04-31 | 2024-05-01 | Reject invalid day | Day/month mismatch not prevalidated |
| EC-DT-006 | 1899-12-31 | 1900-01-01 | Reject out-of-range year (if policy is >=1900) | Internal helper has policy but is unused in handler |

### B4. Extreme values / overflow / performance

| ID | Input | Expected (Robust Behavior) | Current Risk |
|---|---|---|---|
| EC-EX-001 | 999999-12-31 vs 1900-01-01 | Fast rejection/limit check | Very long iterative loop |
| EC-EX-002 | Negative years/month/day | Reject input | Undefined semantics in current flow |
| EC-EX-003 | Very large numeric strings | Reject input with clear message | Conversion truncation / undefined behavior |

---

## C) Integration/Contract Tests (Module Interface)

Since no HTTP/service APIs exist, contract tests are defined against dialog/module interfaces.

### C1. UI Control Binding Contract

| ID | Interface Contract | Test Step | Expected |
|---|---|---|---|
| IT-UI-001 | Edit controls -> member fields (`DDX_Text`) | Enter values in `IDC_EDIT1/2/4/5/6/7`, click difference button | Values are consumed from correct fields and output message corresponds to entered dates |
| IT-UI-002 | Message consistency contract | Trigger date difference | Message title is `日期差值`; body contains computed integer difference |
| IT-UI-003 | Current-date contract | Click current-date button | Message title is `当前日期`; body follows fixed Chinese format |

### C2. Cross-module Invocation Contract (if called from parent dialog)

| ID | Interface Contract | Test Step | Expected |
|---|---|---|---|
| IT-MD-001 | Parent dialog can open Date Conversion dialog | Open from Molecule/main navigation | Dialog opens successfully and controls are interactive |
| IT-MD-002 | Re-open behavior | Open/close/open repeatedly | No crash or stale state corruption |

---

## Suggested Automation Mapping

- **Unit automation candidates:** leap-year semantics and date-difference calculations after extracting pure logic to helper functions.
- **GUI automation candidates:** `OnBnClickedButton1/2` message-box and control-binding checks via WinAppDriver/UIAutomation.
- **Current blocker:** no test framework configured in repository (`gtest/catch2/doctest` absent).

## Minimal Acceptance Set (First Pass)

1. `UT-DD-001` to `UT-DD-007`
2. `EC-IN-001`, `EC-IN-004`, `EC-DT-001`, `EC-DT-005`
3. `IT-UI-001`, `IT-UI-002`, `IT-MD-001`

Passing this set provides baseline confidence for key logic, boundary handling, and module interface behavior.
