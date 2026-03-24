# Code Analysis Report — Atom-Molecule Calculate

> Analysed: 24 March 2026  
> Scope: all `.cpp` source files under `scr/`  
> Toolchain context: Visual Studio 2019, MFC, v142 toolset, Win32 (MultiByte) and x64 (Unicode) configurations

---

## Table of Contents

1. [Summary](#1-summary)
2. [Complexity Metrics](#2-complexity-metrics)
3. [Code Duplication](#3-code-duplication)
4. [Deprecated and Unsafe API Usage](#4-deprecated-and-unsafe-api-usage)
5. [Security Patterns](#5-security-patterns)
6. [Correctness Bugs](#6-correctness-bugs)
7. [Prioritised Remediation Plan](#7-prioritised-remediation-plan)

---

## 1. Summary

| Category | Findings |
|---|---|
| Critical security issues | 1 (hardcoded credentials) |
| High-severity bugs | 4 |
| Medium-severity bugs | 3 |
| Deprecated / unsafe API calls | ~40 across 10 files |
| Significant duplication clusters | 5 |
| Functions exceeding CC 10 | 2 |
| Estimated total source lines | ~2 800 across 21 `.cpp` files |

---

## 2. Complexity Metrics

Cyclomatic complexity (CC) is estimated as 1 + the number of branching statements (`if`, `else if`, `while`, `for`, `case`, `&&`, `||`) per function.

### 2.1 High-complexity functions

| File | Function | Estimated CC | Notes |
|---|---|---|---|
| [scr/linear.cpp](../scr/linear.cpp) | `linear::OnBnClickedButton2` | ~14 | Gaussian elimination + back-substitution all in one handler; nested loops with inner loops, pivot search, row-swap, forward and back passes |
| [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | `CCalculateatomDlg::InfixToSuffix` | ~10 | Infix-to-postfix conversion; multiple nested `if/else if/while/while` blocks; correct algorithm but entirely inline inside a dialog handler |
| [scr/CLoginDlg.cpp](../scr/CLoginDlg.cpp) | `CLoginDlg::OnBnClickedButton1` | ~9 | 7-branch `if/else-if` chain for credential checking |
| [scr/multiple_equations.cpp](../scr/multiple_equations.cpp) | `multiple_equations::OnBnClickedButton1` | ~9 | Cubic equation solver; two nested `if/else` paths with calls to helpers |
| [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | `CCalculateatomDlg::SuffixToResult` | ~8 | Postfix evaluation; `for` + `while` + `if` + `switch` with 5 cases |
| [scr/integration.cpp](../scr/integration.cpp) | `integration::OnBnClickedButton1` | ~7 | Six sequential independent `if` blocks, each with identical structure |

### 2.2 File-level size

| File | Approx. LOC | Dominant concern |
|---|---|---|
| [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | ~700 | 29 trivial button handlers inflate the file |
| [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp) | ~300 | Eight handlers each copy-pasting the same 8-line boilerplate |
| [scr/multiple_equations.cpp](../scr/multiple_equations.cpp) | ~200 | Contains free functions at file scope mixed with dialog class |
| [scr/Complex_cal.cpp](../scr/Complex_cal.cpp) | ~200 | Eight handlers each copy-pasting the same 4-line boilerplate |
| [scr/linear.cpp](../scr/linear.cpp) | ~160 | Single handler doing all work |

---

## 3. Code Duplication

### 3.1 Matrix_Cal — 8-variable extraction block (most severe)

Every one of the eight button handlers in [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp) opens with an identical block:

```cpp
double A11 = atof(a11);
double A12 = atof(a12);
double A21 = atof(a21);
double A22 = atof(a22);
double B11 = atof(b11);
double B12 = atof(b12);
double B22 = atof(b22);
double B21 = atof(b21);
```

This block appears **8 times** verbatim. A private helper `GetMatrices(double A[2][2], double B[2][2])` would eliminate all copies.

### 3.2 Complex_cal — 4-variable extraction block

Each of the eight handlers in [scr/Complex_cal.cpp](../scr/Complex_cal.cpp) duplicates:

```cpp
double R1 = atof(r1);
double R2 = atof(r2);
double I1 = atof(i1);
double I2 = atof(i2);
```

Appears **8 times**. Same remedy: extract to a private helper.

### 3.3 Calculate_atomDlg — 29 digit/operator key handlers

Each of the 29 calculator-key handlers in [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) is structurally identical:

```cpp
void CCalculateatomDlg::OnBnClickedButtonNN()
{
    UpdateData(TRUE);
    m_str = m_str + 'X';
    UpdateData(FALSE);
}
```

The only variation is the character appended. A single `AppendChar(TCHAR ch)` helper plus a `CString` character map keyed on the control ID would reduce 29 functions to 1.

### 3.4 Time_Conversion — normalisation logic duplicated 3 times

The seconds-to-H:M:S normalisation sequence:

```cpp
double sum = 3600 * h + 60 * m + s;
h = sum / 3600;
h = int(h) % 24;
sum = int(sum) % 3600;
m = int(sum / 60);
sum = int(sum) % 60;
s = sum;
```

This block appears independently inside `basiccon`, `OnBnClickedButton2`, and `OnBnClickedButton1` in [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp). `basiccon` exists precisely to avoid this, but the callers then redo the calculation anyway.

### 3.5 integration — radio-button check pattern repeated 6 times

In [scr/integration.cpp](../scr/integration.cpp), the pattern:

```cpp
if (((CButton*)GetDlgItem(IDC_RADIOx))->GetCheck())
{
    double max = atof(integrate_input);
    double min = atof(integrate_input_min);
    // ... compute res ...
    result.Format(_T("%.4f"), res);
    MessageBox(_T(result), _T("The answer"));
}
```

is copy-pasted six times with only the formula body changing. The `atof` conversions and `MessageBox` call belong outside the if-chain; the six `if` blocks should use an array of function pointers or a `switch`.

---

## 4. Deprecated and Unsafe API Usage

### 4.1 `atof` — used ~40 times across 10 files

| Files affected |
|---|
| [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp), [scr/Complex_cal.cpp](../scr/Complex_cal.cpp), [scr/linear.cpp](../scr/linear.cpp), [scr/integration.cpp](../scr/integration.cpp), [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp), [scr/multiple_equations.cpp](../scr/multiple_equations.cpp), [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp), [scr/Base_conversion.cpp](../scr/Base_conversion.cpp), [scr/Prime_factor.cpp](../scr/Prime_factor.cpp) |

`atof` silently returns `0.0` on invalid input and has no error indication. MSVC marks it as deprecated in favour of `strtod` (with an `endptr`) or `_strtod_l`. Empty edit boxes — which are the default state — will silently produce `0.0` without any user feedback.

**Recommended replacement:**
```cpp
double val;
TCHAR* end;
val = _tcstod(fieldCString, &end);
if (end == (LPCTSTR)fieldCString) { /* handle invalid input */ }
```

### 4.2 `_T()` macro applied to a runtime `CString` variable

In [scr/integration.cpp](../scr/integration.cpp):

```cpp
result.Format(_T("%.4f"), res);
MessageBox(_T(result), _T("The answer"));   // BUG
```

`_T(x)` is a **compile-time** macro for string literals. In Unicode builds (x64 configurations) it expands to `L##result`, i.e. it looks for a symbol named `Lresult`, causing a compile error. In MultiByte builds it expands to `result` which works by coincidence. The correct call is:

```cpp
MessageBox(result, _T("The answer"));
```

This means the `integration` dialog **cannot be compiled for x64** as-is.

### 4.3 `ShellExecute` with bare relative paths — no path validation

In [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp):

```cpp
ShellExecute(NULL, "open", "计算机综合课程设计报告.doc", NULL, NULL, SW_SHOWNORMAL);
ShellExecute(NULL, "open", "MFC_DRAW.exe",              NULL, NULL, SW_SHOWNORMAL);
```

In [scr/Leisure.cpp](../scr/Leisure.cpp):

```cpp
ShellExecute(NULL, "open", "Mine.exe", NULL, NULL, SW_SHOWNORMAL);
```

These rely on the process working directory at runtime being the installation folder. If the executable is launched from a different directory (e.g., via a desktop shortcut) the files will not be found without any error message to the user. The return value is also not checked. A minimal improvement is to construct an absolute path from the executable's module path at startup and check the `ShellExecute` return value.

### 4.4 `int` overflow in factorial

In [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp):

```cpp
int i = 1;
int result = 1;
int num = _ttof(m_str);
for (i = 1; i <= num; i++)
    result *= i;
m_str.Format(TEXT("%d"), result);
```

`int` overflows silently at 13! = 1,932,053,504, wrapping to negative values above. No upper-bound check exists. Should use `long long` and add a guard (e.g., `num > 20`).

### 4.5 Global mutable state in `Prime_factor.cpp`

```cpp
CString M("");   // file-scope global

void PrimeDecomposition(int k) { M += ...; }

void Prime_factor::OnBnClickedButton1()
{
    PrimeDecomposition(num);
    MessageBox(M, _T("The answer"));
    M = CString("");   // reset after display
}
```

State accumulated in `M` is the only way `PrimeDecomposition` produces output. If `MessageBox` is never reached (e.g., an exception), `M` retains stale content. The function should return a `CString` instead of writing to a global.

---

## 5. Security Patterns

### 5.1 CRITICAL — Hardcoded plaintext credentials

**File:** [scr/CLoginDlg.cpp](../scr/CLoginDlg.cpp)  
**Severity:** Critical  
**OWASP category:** A07 – Identification and Authentication Failures

Six username/password pairs are hardcoded verbatim in the source:

```cpp
else if (this->m_InUser == "孙寒石"  && this->m_InPassword == "Sun20001225") { ... }
else if (this->m_InUser == "张扬"    && this->m_InPassword == "123456")      { ... }
else if (this->m_InUser == "陶星宇"  && this->m_InPassword == "123456")      { ... }
else if (this->m_InUser == "车旭明"  && this->m_InPassword == "123456")      { ... }
else if (this->m_InUser == "韦伟"    && this->m_InPassword == "0904")        { ... }
else if (this->m_InUser == "1"       && this->m_InPassword == "1")           { ... }
```

Issues:
- The passwords are: a likely date-of-birth (`Sun20001225`), three identical weak passwords (`123456`), a 4-digit likely PIN (`0904`), and a development no-op (`1`/`1`).
- Anyone with read access to the source repository (this is a public GitHub repo at `preminstrel/MFC-project-in-SEU`) can authenticate as any user.
- No hashing, no salting, no lockout mechanism.
- Plain string comparison with `==` is not constant-time, although timing attacks are impractical in a local MFC dialog context.

**Remediation:** For a teaching project that must keep a login screen, store credentials in `config.ini` (which already exists in `scr/`) as bcrypt or PBKDF2 hashes, and read/compare at runtime. At minimum, remove real names and birth-date passwords from the repository and rotate any reused passwords.

### 5.2 No input validation on numeric fields

All numeric fields across all dialogs accept arbitrary strings and pass them directly to `atof`. While the attack surface is local (a desktop app with no network input), any numeric field accepting non-numeric content silently produces `0.0`, which can cause division-by-zero (e.g., `Complex_cal` division with zero denominator `R2²+I2²` when both fields are empty) or silent wrong answers.

**Complex division denominator problem** in [scr/Complex_cal.cpp](../scr/Complex_cal.cpp):

```cpp
rr.Format(_T("%.4f"), (R1*R2 + I1*I2) / (R2*R2 + I2*I2));  // div-by-zero if operand 2 = 0+0i
```

No guard for a zero denominator. MSVC will produce `1.#INF` in the result box.

### 5.3 `ShellExecute` launching executables from relative paths

Already covered in §4.3. In a security context, if an attacker can place a malicious `MFC_DRAW.exe` or `Mine.exe` in the working directory the application runs from, it would be executed with the privileges of the current user. The risk is low in practice (local desktop app, controlled environment) but should be acknowledged.

---

## 6. Correctness Bugs

### 6.1 HIGH — Matrix multiplication result rows are swapped

**File:** [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp), `OnBnClickedButton1`

```cpp
// Actual code
double c11 = A11*B11 + A12*B21;   // ✓ correct
double c12 = A21*B11 + A22*B21;   // ✗ this is the c21 formula
double c21 = A11*B12 + A12*B22;   // ✗ this is the c12 formula
double c22 = A21*B12 + A22*B22;   // ✓ correct
```

c12 and c21 are transposed. Correct formulas:
```
c12 = A11*B12 + A12*B22
c21 = A21*B11 + A22*B21
```

### 6.2 HIGH — `minusjudge` missing braces causes always-append

**File:** [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp)

```cpp
if (c == '(' && a[i + 1] == '-')
    transres += '0';
    transres += c;      // ← NOT inside the if; always executes
```

The intent is to insert a `0` before a leading minus inside brackets (e.g., `(-3)` → `(0-3)`) and then append `c`. Without braces the `transres += c` executes unconditionally, which is actually the correct net effect in most cases — but the function also accesses `a[i + 1]` without a bounds check, causing **out-of-bounds access when `c` is the last character** in the string.

### 6.3 HIGH — Prime factor loop excludes perfect-square factors

**File:** [scr/Prime_factor.cpp](../scr/Prime_factor.cpp)

```cpp
for (int i = 2; i < sqrt(k); i++)   // ← should be <=
```

The strict `<` means the loop body never tests `i == sqrt(k)`. For perfect squares (4 is special-cased but 9, 25, 49, … are not), the remainder at loop exit equals `k` rather than the square root factor.

Example: `k = 9` → loop runs only for `i = 2` (2 < 3), exits, appends `9` → output `9=9` instead of `9=3*3`.

### 6.4 HIGH — Negative hour values in timezone conversion

**File:** [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp)

```cpp
h = h - 7;
h = h % 24;   // C++ % of a negative number is implementation-defined (negative in MSVC)
```

If the Beijing hour is 3, `h - 7 = -4`, and `-4 % 24 = -4` on MSVC. The displayed time will be `-4:mm:ss`.

Fix: `h = ((h % 24) + 24) % 24;`

Additionally, the fixed offsets are wrong:
- Tokyo is UTC+9, Beijing is UTC+8 → offset **+1** ✓
- London is UTC+0 (or UTC+1 BST), Beijing is UTC+8 → offset should be **−8**, not **−7**
- Washington DC is UTC−5 (or UTC−4 EDT), Beijing is UTC+8 → offset should be **−13** (or −12), not **−12**

### 6.5 MEDIUM — `multiple_equations` displays stale values for imaginary roots

**File:** [scr/multiple_equations.cpp](../scr/multiple_equations.cpp)

In the monotone-increasing branch, `x2r`, `x2i` etc. are formatted **before** the call to `Imagequation` that computes `result[1]` and `result[2]`:

```cpp
x2r.Format(_T("%.4f"), result[1]);   // result[1] is 0 here
x2i.Format(_T("%.4f"), result[2]);   // result[2] is 0 here
// ...
Imagequation(result[0], &param[0], &param[1], &result[1], &result[2]);
// result[1] and result[2] are now set, but x2r/x2i already show "0.0000"
```

The format calls must be moved to **after** `Imagequation`.

### 6.6 MEDIUM — Integer factorial uses `int` (overflow at n=13)

Covered in §4.4. The result wraps silently and can become negative with no error to the user.

### 6.7 MEDIUM — `integration.cpp` does not compile on Unicode (x64) build

Covered in §4.2. The `MessageBox(_T(result))` lines will fail to compile under the Unicode character set used by the x64 configurations.

---

## 7. Prioritised Remediation Plan

| Priority | Issue | File(s) | Effort |
|---|---|---|---|
| P1 | Remove hardcoded credentials from source | [CLoginDlg.cpp](../scr/CLoginDlg.cpp) | Low |
| P1 | Fix `_T(result)` → `result` (x64 build failure) | [integration.cpp](../scr/integration.cpp) | Trivial |
| P1 | Fix matrix multiplication formula (c12/c21 swap) | [Matrix_Cal.cpp](../scr/Matrix_Cal.cpp) | Trivial |
| P2 | Fix negative timezone hours (`% 24` with addend) | [Time_Conversion.cpp](../scr/Time_Conversion.cpp) | Trivial |
| P2 | Fix prime factor loop (`<` → `<=`) | [Prime_factor.cpp](../scr/Prime_factor.cpp) | Trivial |
| P2 | Fix minusjudge bounds check and braces | [Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | Low |
| P2 | Fix imaginary roots: move Format calls after Imagequation | [multiple_equations.cpp](../scr/multiple_equations.cpp) | Low |
| P2 | Guard complex division against zero denominator | [Complex_cal.cpp](../scr/Complex_cal.cpp) | Low |
| P3 | Replace `atof` with `strtod`/`_tcstod` + error handling | All numeric dialogs | Medium |
| P3 | Replace global `M` with return value in PrimeDecomposition | [Prime_factor.cpp](../scr/Prime_factor.cpp) | Low |
| P3 | Use `long long` for factorial + input bound check | [Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | Low |
| P3 | Abstract repeated `atof` extraction into helpers | [Matrix_Cal.cpp](../scr/Matrix_Cal.cpp), [Complex_cal.cpp](../scr/Complex_cal.cpp) | Medium |
| P4 | Consolidate 29 digit/operator handlers into one | [Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | Medium |
| P4 | Extract `InfixToSuffix`/`SuffixToResult` to a separate unit | [Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | Medium |
| P4 | Resolve absolute path for ShellExecute targets | [Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp), [Leisure.cpp](../scr/Leisure.cpp) | Low |
| P4 | Fix timezone offsets to match actual UTC values | [Time_Conversion.cpp](../scr/Time_Conversion.cpp) | Trivial |
