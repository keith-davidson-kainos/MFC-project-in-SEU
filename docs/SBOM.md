# Software Bill of Materials (SBOM)
## Atom-Molecule Calculate MFC Application

**Generated:** 24 March 2026  
**Project:** MFC-project-in-SEU / Atom-Molecule Calculator  
**Version:** 2.1  
**Build Target:** Windows (Win32 / x64)  
**Toolchain:** Visual Studio 2019, v142 platform toolset  

---

## Executive Summary

| Metric | Count | Notes |
|--------|-------|-------|
| **Direct Dependencies** | 3 (framework categories) | MFC, Windows Common Controls, C++ Standard Library |
| **Internal Components** | 21 C++ source files | Dialog classes + calculator modules |
| **External Libraries** | 1 (MFC Runtime) | Dynamic linking, platform-provided |
| **Resource Files** | 1 | Wire-format RC2 and bitmap assets |
| **Known Vulnerabilities** | 1 Critical | Hardcoded credentials in source |

---

## Part 1: External Dependencies

### 1.1 Microsoft Foundation Classes (MFC)

| Component | Version | Type | License | Source |
|-----------|---------|------|---------|--------|
| MFC Runtime | VS2019 / v14.2 | Framework Library | Proprietary (MSVC) | Windows SDK / Visual Studio 2019 |
| afxwin.h | VS2019 | Header | Proprietary | Framework.h include |
| afxext.h | VS2019 | Header | Proprietary | Framework.h include |
| afxdisp.h | VS2019 | Header | Proprietary | Framework.h include |
| afxdialogex.h | VS2019 | Header | Proprietary | All dialog .cpp files |
| afxdtctl.h | VS2019 | Header | Proprietary | Framework.h include (IE4 controls) |
| afxcmn.h | VS2019 | Header | Proprietary | Framework.h include (Common controls) |
| afxcontrolbars.h | VS2019 | Header | Proprietary | Framework.h include (Ribbons) |

**Linking:** Dynamic (`mfcd142u.dll` for Debug, `mfc142u.dll` for Release on x64 Unicode configurations)

### 1.2 Windows Runtime & API

| Component | Version | Type | License | Notes |
|-----------|---------|------|---------|-------|
| Windows API (Kernel32, User32, GDI32) | Windows 10+ | System Library | Proprietary (Microsoft) | Implicit via MFC |
| Windows Common Controls v6.0.0.0 | 6.0+ | SxS Manifest Dependency | Proprietary (Microsoft) | Ref: `Microsoft.Windows.Common-Controls` public key token `6595b64144ccf1df` |
| Shell Services (ShellExecute) | Windows 10+ | System API | Proprietary (Microsoft) | Used in [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L681), [scr/Leisure.cpp](../scr/Leisure.cpp) |

### 1.3 C++ Standard Library

| Component | Version | Type | License | Source |
|-----------|---------|------|---------|--------|
| `<stack>` | C++98 | STL Header | Proprietary (MSVC) | [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L11) - infix/postfix parsing |
| `<math.h>` | C (with C++ overloads) | Standard Library | Proprietary (MSVC) | [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp#L12) - math functions |
| `<cmath>` | C++11 | Standard Library | Proprietary (MSVC) | [scr/multiple_equations.cpp](../scr/multiple_equations.cpp#L9) - cubic solver |
| `<iomanip>` | C++98 | Standard Library | Proprietary (MSVC) | [scr/multiple_equations.cpp](../scr/multiple_equations.cpp#L8) - unused in final code |

**CRT Link:** Dynamic (`msvcr120.dll` or similar, depending on configuration)

---

## Part 2: Internal Components

### 2.1 Core Application

| Module | File | Type | Lines | Purpose | Dependencies |
|--------|------|------|-------|---------|--------------|
| Application | [scr/Calculate_atom.cpp](../scr/Calculate_atom.cpp) | Class Impl | ~93 | MFC app entry point, initialization | pch.h, framework.h, Calculate_atomDlg.h |
| Application | [scr/Calculate_atom.h](../scr/Calculate_atom.h) | Class Header | ~20 | CCalculateatomApp class | afxwin.h |

### 2.2 Main UI Dialog

| Module | File | Type | Lines | Purpose | Dependencies |
|--------|------|------|-------|---------|--------------|
| Main Atom Dialog | [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | Class Impl | ~700 | Calculator UI, expression parsing/evaluation | pch.h, framework.h, Calculate_atomDlg.h, Leisure.h, afxdialogex.h, `<stack>`, `<math.h>` |
| Main Atom Dialog | [scr/Calculate_atomDlg.h](../scr/Calculate_atomDlg.h) | Class Header | ~89 | CCalculateatomDlg class | Calculate_molecule.h, integration.h, CLoginDlg.h, Leisure.h |
| About Dialog | (embedded in Calculate_atomDlg.cpp) | Class Impl | ~50 | About box (CAboutDlg) | afxdialogex.h |

### 2.3 Feature Dialogs (Molecule Mode)

| Module | File | Type | Lines | Purpose | Dependencies |
|--------|------|------|-------|---------|--------------|
| Molecule Launcher | [scr/Calculate_molecule.cpp](../scr/Calculate_molecule.cpp) | Class Impl | ~160 | Launches 11 advanced calculator dialogs | pch.h, Calculate_atom.h, Calculate_molecule.h, afxdialogex.h, integration.h, linear.h, multiple_equations.h, Base_conversion.h, Complex_cal.h, Matrix_Cal.h, Time_Conversion.h, Date_Conversion.h, Prime_factor.h, Draw_dlg.h |
| Molecule Launcher | [scr/Calculate_molecule.h](../scr/Calculate_molecule.h) | Class Header | ~35 | CCalculate_molecule dialog class | CDialog, integration.h |

### 2.4 Numeric Calculators

#### Integration / Definite Integral

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Integration | [scr/integration.cpp](../scr/integration.cpp) | Class Impl | ~180 | Numerical integration for 6 functions (x, x², e^x, ln x, sin x, cos x) | Unicode fragility (`_T(result)` bug) |
| Integration | [scr/integration.h](../scr/integration.h) | Class Header | ~32 | Cintegration dialog class | CDialogEx |

#### Linear Equations (up to 4 variables)

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Linear Solver | [scr/linear.cpp](../scr/linear.cpp) | Class Impl | ~160 | Gaussian elimination with partial pivoting | High complexity (CC ~14), single monolithic handler |
| Linear Solver | [scr/linear.h](../scr/linear.h) | Class Header | ~38 | Clinear dialog class | CDialogEx |

#### Polynomial Equations (Cubic, up to 3 variables)

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Cubic Solver | [scr/multiple_equations.cpp](../scr/multiple_equations.cpp) | Class Impl | ~200 | Cubic equation solver with dichotomic refinement & imaginary root computation | Stale output display bug (P2-06); free functions at file scope |
| Cubic Solver | [scr/multiple_equations.h](../scr/multiple_equations.h) | Class Header | ~20 | Cmultiple_equations dialog class | CDialogEx |
| Cubic Solver | (scr/multiple_equations.cpp) | Helper Funcs | ~100 | `oneresult()`, `dichotomy()`, `Imagequation()` | No public interface |

#### Matrix Calculations (2×2 matrices)

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Matrix Calc | [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp) | Class Impl | ~300 | Matrix add/subtract/multiply, determinant, rank, transpose | Multiplication output bug (c12/c21 swap); heavy duplication |
| Matrix Calc | [scr/Matrix_Cal.h](../scr/Matrix_Cal.h) | Class Header | ~40 | CMatrix_Cal dialog class | CDialogEx |

#### Complex Number Operations

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Complex Calc | [scr/Complex_cal.cpp](../scr/Complex_cal.cpp) | Class Impl | ~200 | Complex add/subtract/multiply/divide, modulus, argument | Divide by zero risk; duplication |
| Complex Calc | [scr/Complex_cal.h](../scr/Complex_cal.h) | Class Header | ~40 | CComplex_cal dialog class | CDialogEx |

#### Base Conversion

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Base Conv | [scr/Base_conversion.cpp](../scr/Base_conversion.cpp) | Class Impl | ~65 | Convert decimal to binary/octal/hex/base-32/base-36 | Uses `_itoa_s` (deprecated); `atof` parsing |
| Base Conv | [scr/Base_conversion.h](../scr/Base_conversion.h) | Class Header | ~25 | CBase_conversion dialog class | CDialogEx |

#### Time Conversion

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Time Conv | [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp) | Class Impl | ~200 | Time arithmetic, timezone display (Beijing, Tokyo, London, Washington) | Negative hour display bug; hardcoded UTC offsets |
| Time Conv | [scr/Time_Conversion.h](../scr/Time_Conversion.h) | Class Header | ~25 | CTime_Conversion dialog class | CDialogEx |

#### Date Conversion

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Date Conv | [scr/Date_Conversion.cpp](../scr/Date_Conversion.cpp) | Class Impl | ~150 | Days between dates, current date display | Complex leap-year logic; file-scope date class |
| Date Conv | [scr/Date_Conversion.h](../scr/Date_Conversion.h) | Class Header | ~25 | CDate_Conversion dialog class | CDialogEx |

#### Prime Factorization

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Prime Factor | [scr/Prime_factor.cpp](../scr/Prime_factor.cpp) | Class Impl | ~100 | Factor composite numbers; identify primes | Loop bound bug (misses perfect squares); global mutable state |
| Prime Factor | [scr/Prime_factor.h](../scr/Prime_factor.h) | Class Header | ~20 | CPrime_factor dialog class | CDialogEx |
| Prime Factor | (scr/Prime_factor.cpp) | Helper Func | ~40 | `PrimeDecomposition()` (free function) | Uses global `CString M` |

### 2.5 UI Features

| Module | File | Type | Lines | Purpose | Issues |
|--------|------|------|-------|---------|--------|
| Drawing Mode | [scr/Draw_dlg.cpp](../scr/Draw_dlg.cpp) | Class Impl | ~50 | Drawing dialog stub (launches MFC_DRAW.exe) | Relative path; no implementation |
| Drawing Mode | [scr/Draw_dlg.h](../scr/Draw_dlg.h) | Class Header | ~20 | CDraw_dlg dialog class | CDialogEx |
| Leisure Mode | [scr/Leisure.cpp](../scr/Leisure.cpp) | Class Impl | ~50 | Game mode launcher (launches Mine.exe) | Relative path; no validation |
| Leisure Mode | [scr/Leisure.h](../scr/Leisure.h) | Class Header | ~20 | CLeisure dialog class | CDialogEx |
| Login | [scr/CLoginDlg.cpp](../scr/CLoginDlg.cpp) | Class Impl | ~100 | Credential validation | **CRITICAL:** Hardcoded plaintext credentials |
| Login | [scr/CLoginDlg.h](../scr/CLoginDlg.h) | Class Header | ~20 | CLoginDlg dialog class | CDialogEx |
| Login | [scr/Login.cpp](../scr/Login.cpp) | Class Impl | ~50 | Alternative login UI (unused stub) | Not referenced |
| Login | [scr/Login.h](../scr/Login.h) | Class Header | ~20 | CLogin dialog class (unused) | Not referenced |

#### Obsolete/Unused

| Module | File | Type | Lines | Purpose | Status |
|--------|------|------|-------|---------|--------|
| INT calc | [scr/INT.cpp](../scr/INT.cpp) | Class Impl | ~40 | Integer integration (obsolete) | Project includes but not referenced |
| INT calc | [scr/INT.h](../scr/INT.h) | Class Header | ~20 | CINT dialog class | Project includes but not referenced |
| integrate calc | [scr/integrate.cpp](../scr/integrate.cpp) | Class Impl | ~40 | Integration calc (older implementation) | Project includes but not referenced |
| integrate calc | [scr/integrate.h](../scr/integrate.h) | Class Header | ~20 | Cintegrate dialog class | Project includes but not referenced |

### 2.6 Framework & Configuration

| Module | File | Type | Lines | Purpose |
|--------|------|------|-------|---------|
| Framework | [scr/framework.h](../scr/framework.h) | Header | ~50 | MFC include aggregator + manifest pragma |
| PCH | [scr/pch.h](../scr/pch.h) | Header | ~10 | Precompiled header master |
| PCH | [scr/pch.cpp](../scr/pch.cpp) | Impl | ~1 | PCH creation unit |
| Config | [scr/resource.h](../scr/resource.h) | Header | ~200+ | Resource ID definitions |
| Config | [scr/targetver.h](../scr/targetver.h) | Header | ~10 | Windows target version |
| Resources | [scr/Calculateatom.rc](../scr/Calculateatom.rc) | Resource | ~800+ | Dialog definitions, strings, bitmaps, menu, ribbon manifest |
| Resources | [scr/res/Calculateatom.rc2](../scr/res/Calculateatom.rc2) | Resource | ~50 | Additional wire-format resources |

---

## Part 3: Runtime Dependencies

### 3.1 Dynamic Link Libraries (DLLs)

| DLL | Source | Configuration | Purpose |
|-----|--------|---|---------|
| mfc142u.dll | Visual Studio 2019 / Visual C++ Runtime | x64 Release + Unicode | MFC framework (release) |
| mfcd142u.dll | Visual Studio 2019 / Visual C++ Runtime | x64 Debug + Unicode | MFC framework (debug) |
| mfc142.dll | Visual Studio 2019 / Visual C++ Runtime | Win32 Release + MultiByte | MFC framework (release) |
| mfcd142.dll | Visual Studio 2019 / Visual C++ Runtime | Win32 Debug + MultiByte | MFC framework (debug) |
| msvcr120.dll | Visual Studio 2019 / C Runtime | All configurations | C Runtime Library |
| kernel32.dll | Windows | All configurations | Windows Kernel API |
| user32.dll | Windows | All configurations | Windows User API (dialogs, windows, messages) |
| gdi32.dll | Windows | All configurations | Windows Graphics Device Interface |
| shell32.dll | Windows | All configurations | Shell services (ShellExecute, etc.) |
| advapi32.dll | Windows | All configurations | Advanced API (registry, security) |

### 3.2 Manifest Dependencies

| Dependency | Version | Processor | Public Key | Notes |
|------------|---------|-----------|------------|-------|
| Microsoft.Windows.Common-Controls | 6.0.0.0 | x86 (Win32) | 6595b64144ccf1df | Modern Common Controls (v6) for Win32 |
| Microsoft.Windows.Common-Controls | 6.0.0.0 | x64 | 6595b64144ccf1df | Modern Common Controls (v6) for x64 |
| Microsoft.Windows.Common-Controls | 6.0.0.0 | IA64 | 6595b64144ccf1df | Modern Common Controls (v6) for IA64 (legacy) |

---

## Part 4: Known Vulnerabilities & Issues in Dependencies

| Issue | Component | Severity | Mitigations |
|-------|-----------|----------|-------------|
| Hardcoded credentials | CLoginDlg.cpp | **CRITICAL** | Remove from source; use config-backed hashes |
| Unicode build break | integration.cpp | **HIGH** | Fix `_T(result)` → `result` |
| Deprecated `atof` | All numeric dialogs (~10) | **MEDIUM** | Replace with `_tcstod` + validation |
| Deprecated `_itoa_s` | Base_conversion.cpp | **MEDIUM** | Use standard `std::to_string` or format helpers |
| Silent integer overflow | Calculate_atomDlg.cpp (factorial) | **MEDIUM** | Use `long long` + range guard |

---

## Part 5: License Summary

| License | Count | Components |
|---------|-------|------------|
| Proprietary (Microsoft MSVC/MFC/WinAPI) | ~15 | MFC runtime, CRT, Windows SDK, Windows Common Controls |
| Custom (Educational Project) | 1 | MIT license badge, source in repo |

**Overall**: The application is proprietary Microsoft stack + MIT-licensed source.  
**Redistribution**: Requires MSVC redistributable package for end-user deployment.

---

## Part 6: Build Configuration Matrix

| Configuration | Platform | Character Set | MFC Linking | CRT Linking | MSBuild Target |
|---|---|---|---|---|---|
| Debug\|Win32 | x86 | MultiByte | `mfcd142.dll` (Dynamic) | `msvcr120d.dll` | v142 |
| Release\|Win32 | x86 | Unicode | `mfc142u.dll` (Dynamic) | `msvcr120.dll` | v142 |
| Debug\|x64 | x64 | Unicode | `mfcd142u.dll` (Dynamic) | `msvcr120d.dll` | v142 |
| Release\|x64 | x64 | Unicode | `mfc142u.dll` (Dynamic) | `msvcr120.dll` | v142 |

---

## Part 7: Inventory of Local Source Files

| Type | Count | Total LOC | Average LOC/module |
|------|-------|-----------|-------------------|
| Implementation (.cpp) | 21 | ~2,800 | ~133 |
| Headers (.h) | 20 | ~600 | ~30 |
| Resource (.rc) | 2 | ~900 | ~450 |
| **Total** | **43** | **~4,300** | — |

---

## Notes

1. **No external open-source dependencies** found in direct includes (only MSVC/Windows headers).
2. **All Calculator modules are proprietary** to this educational project.
3. **Resource files** define 13+ dialogs, ~50 bitmap/icon resources, and localized strings (Simplified Chinese).
4. **Build-time dependencies**: Visual Studio 2019 toolchain v142 is hardcoded in `.vcxproj`.
5. **Runtime environment**: Windows 10/11, MFC runtime installed (typically bundled with other Microsoft applications).
