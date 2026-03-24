# Remediation Backlog (P1/P2/P3)

> Generated: 24 March 2026  
> Basis: hotspot ranking from the previous analysis  
> Scope: security, correctness, reliability, and maintainability issues in `scr/`

## Sizing Guide

- `XS`: < 0.5 day
- `S`: 0.5-1 day
- `M`: 1-2 days
- `L`: 2-4 days

---

## P1 (Immediate)

### P1-01: Remove hardcoded plaintext credentials
- Priority: `P1`
- Hotspot: [scr/CLoginDlg.cpp](../scr/CLoginDlg.cpp#L43)
- Problem: Usernames/passwords are embedded directly in source.
- Risk: Critical authentication failure; credentials are exposed in repository history.
- Fix:
  - Replace hardcoded checks with config-backed hashed credentials (e.g., PBKDF2/bcrypt).
  - Add a migration-safe fallback path for development-only credentials behind build flag.
  - Remove sensitive literal strings from source and rotate/revoke affected credentials.
- Estimated size: `M`
- Acceptance criteria:
  - No plaintext credentials remain in C++ source.
  - Login validation uses hashed comparison.
  - Build and manual login test still pass in Debug/Release.

### P1-02: Fix Unicode build break in integration dialog
- Priority: `P1`
- Hotspot: [scr/integration.cpp](../scr/integration.cpp#L58)
- Problem: Uses `_T(result)` where `result` is a runtime `CString`.
- Risk: x64/Unicode configuration compile failure.
- Fix:
  - Replace `MessageBox(_T(result), ...)` with `MessageBox(result, ...)` at all occurrences.
- Estimated size: `XS`
- Acceptance criteria:
  - `integration.cpp` compiles in Unicode configurations.
  - Dialog behavior unchanged.

### P1-03: Correct matrix multiplication output
- Priority: `P1`
- Hotspot: [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp#L61)
- Problem: `c12` and `c21` formulas are swapped.
- Risk: Incorrect mathematical result for multiplication.
- Fix:
  - Correct formulas for `c12` and `c21`.
  - Add at least 3 manual verification cases (identity, diagonal, asymmetric matrix).
- Estimated size: `XS`
- Acceptance criteria:
  - Multiplication output matches expected values for all verification cases.

---

## P2 (Next)

### P2-01: Harden numeric parsing across dialogs
- Priority: `P2`
- Hotspots:
  - [scr/Complex_cal.cpp](../scr/Complex_cal.cpp#L128)
  - [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp#L61)
  - [scr/linear.cpp](../scr/linear.cpp#L78)
  - [scr/Base_conversion.cpp](../scr/Base_conversion.cpp#L54)
- Problem: Extensive `atof` usage with silent failure to `0.0`.
- Risk: Hidden bad input, wrong output, invalid math operations.
- Fix:
  - Introduce helper parse function using `_tcstod` with end-pointer validation.
  - Show user-facing validation message when parsing fails.
- Estimated size: `L`
- Acceptance criteria:
  - Invalid numeric input is rejected with clear message.
  - No hotspot path relies on `atof` directly.

### P2-02: Prevent divide-by-zero in complex division
- Priority: `P2`
- Hotspot: [scr/Complex_cal.cpp](../scr/Complex_cal.cpp#L128)
- Problem: Division by `R2^2 + I2^2` without zero guard.
- Risk: Infinity/NaN display and undefined user outcomes.
- Fix:
  - Check denominator against epsilon before division.
  - Return user error message when divisor is zero.
- Estimated size: `XS`
- Acceptance criteria:
  - Dividing by `0+0i` never performs arithmetic division.
  - User receives deterministic error message.

### P2-03: Fix unary-minus preprocessing safety and logic
- Priority: `P2`
- Hotspot: [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L263)
- Problem: `a[i + 1]` can read out of bounds; brace layout is fragile.
- Risk: Undefined behavior/crash on edge expressions.
- Fix:
  - Add bounds checks before `i + 1` access.
  - Make intended control flow explicit with braces.
- Estimated size: `S`
- Acceptance criteria:
  - Expressions ending with `(` or malformed input do not crash.
  - Unary-minus transform behavior is deterministic.

### P2-04: Fix prime factorization loop for perfect squares
- Priority: `P2`
- Hotspot: [scr/Prime_factor.cpp](../scr/Prime_factor.cpp#L38)
- Problem: Loop condition `< sqrt(k)` misses factor when `i == sqrt(k)`.
- Risk: Wrong output for values like 9, 25, 49.
- Fix:
  - Change bound to include square root case.
  - Validate with regression set: `4, 9, 12, 25, 49, 97`.
- Estimated size: `XS`
- Acceptance criteria:
  - Perfect squares decompose correctly.

### P2-05: Normalize timezone math and prevent negative-hour display
- Priority: `P2`
- Hotspots:
  - [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp#L153)
  - [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp#L170)
- Problem: Negative modulo handling yields invalid negative hour values.
- Risk: Incorrect displayed local times.
- Fix:
  - Use canonical normalization: `((h % 24) + 24) % 24`.
  - Update hardcoded offsets to accurate UTC-based differences or explicitly label assumptions.
- Estimated size: `S`
- Acceptance criteria:
  - Output hours are always in `[0,23]`.
  - London/Washington conversions match documented offset rule.

### P2-06: Fix stale output values in cubic equation branch
- Priority: `P2`
- Hotspot: [scr/multiple_equations.cpp](../scr/multiple_equations.cpp#L103)
- Problem: Formats imaginary roots before values are computed.
- Risk: User sees `0.0000` placeholders despite computed roots.
- Fix:
  - Move formatting after `Imagequation(...)` call in that branch.
- Estimated size: `XS`
- Acceptance criteria:
  - Displayed roots match computed values in all branches.

### P2-07: Guard factorial overflow
- Priority: `P2`
- Hotspot: [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L552)
- Problem: Uses `int` and overflows at 13!.
- Risk: Silent incorrect/negative result.
- Fix:
  - Use `long long` and enforce upper bound (e.g., `n <= 20`).
  - Return clear message when input exceeds safe range.
- Estimated size: `S`
- Acceptance criteria:
  - No overflow for allowed range.
  - Out-of-range input is handled gracefully.

---

## P3 (Stabilize and Reduce Future Defects)

### P3-01: Reduce duplication in matrix and complex modules
- Priority: `P3`
- Hotspots:
  - [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp#L61)
  - [scr/Complex_cal.cpp](../scr/Complex_cal.cpp#L128)
- Problem: Repeated parse/format boilerplate across many handlers.
- Risk: Bug-fix fan-out and inconsistent behavior.
- Fix:
  - Add shared helper methods (`ReadMatrices`, `ReadComplexOperands`, `FormatComplexResult`).
- Estimated size: `M`
- Acceptance criteria:
  - Duplication removed from all button handlers.
  - Behavior unchanged under manual test.

### P3-02: Consolidate keypad handlers in Atom dialog
- Priority: `P3`
- Hotspot: [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L463)
- Problem: Dozens of near-identical button handlers.
- Risk: High maintenance cost and easy inconsistency.
- Fix:
  - Route button IDs to one append helper and common update routine.
- Estimated size: `M`
- Acceptance criteria:
  - Fewer handlers with identical behavior preserved.

### P3-03: Isolate expression parser/evaluator into dedicated unit
- Priority: `P3`
- Hotspots:
  - [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L278)
  - [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L340)
- Problem: Core algorithm embedded in large UI class.
- Risk: Hard to test and evolve without UI regressions.
- Fix:
  - Move parser/evaluator into separate `.h/.cpp` utility module.
  - Keep dialog as orchestration only.
- Estimated size: `L`
- Acceptance criteria:
  - Parser builds and runs independent of dialog implementation.
  - Existing calculation behavior remains consistent.

### P3-04: Replace global mutable output in prime factor module
- Priority: `P3`
- Hotspot: [scr/Prime_factor.cpp](../scr/Prime_factor.cpp#L38)
- Problem: Uses global `CString M` as shared mutable state.
- Risk: Hidden coupling and stale-state bugs.
- Fix:
  - Return `CString` from decomposition function; remove global variable.
- Estimated size: `S`
- Acceptance criteria:
  - No file-scope mutable output buffer remains.

### P3-05: Harden external process/document launching
- Priority: `P3`
- Hotspots:
  - [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L681)
  - [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L688)
- Problem: `ShellExecute` uses relative targets without robust error handling.
- Risk: Launch failure or unintended target resolution.
- Fix:
  - Resolve absolute paths from module directory.
  - Check `ShellExecute` return codes and show user message on failure.
- Estimated size: `S`
- Acceptance criteria:
  - Launches succeed from non-project working directory.
  - Failures provide explicit user feedback.

---

## Suggested Execution Order (1-2 Week Window)

1. `P1-02`, `P1-03` (quick correctness/build unblockers)
2. `P1-01` (critical security removal)
3. `P2-02`, `P2-04`, `P2-06` (fast correctness wins)
4. `P2-03`, `P2-05`, `P2-07` (safety hardening)
5. `P2-01` (broad parsing stabilization)
6. `P3-*` refactors for long-term maintainability

## Effort Rollup (Rough)

- `P1`: `M + XS + XS` -> about 2-3 dev days
- `P2`: `L + XS + S + XS + S + XS + S` -> about 6-9 dev days
- `P3`: `M + M + L + S + S` -> about 7-12 dev days

Total rough effort: about 15-24 dev days (single engineer), depending on validation depth and review overhead.
