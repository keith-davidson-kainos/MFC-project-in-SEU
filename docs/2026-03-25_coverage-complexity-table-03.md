# Coverage And Complexity Reassessment Table

**Date:** 25 March 2026  
**Scope:** `scr/*.cpp` plus `scr/DateConversionCore/*.cpp` and `scr/MatrixCore/*.cpp`  
**Test Run Basis:** CMake + CTest on macOS (`DateConversionCoreTests`)

## Coverage Summary

- Measured line coverage (`DateConversionCore.cpp`): **100.00%** (60/60 lines executed)
- Measured branch coverage (`DateConversionCore.cpp`): **100.00%** (42/42 branches executed)
- Branches taken at least once (`DateConversionCore.cpp`): **85.71%** (36/42)
- Measured line coverage (`MatrixCore.cpp`): **100.00%** (183/183 lines executed)
- Measured branch coverage (`MatrixCore.cpp`): **100.00%** (186/186 branches executed)
- Branches taken at least once (`MatrixCore.cpp`): **69.89%** (130/186)
- Untested production modules in this run: treated as **0.00%** line coverage
- Estimated overall production line coverage (lower-bound): **7.79%**
  - Calculation: executed lines 243 / total production LOC 3121

## Method

- Coverage measured with instrumented CMake build (`--coverage`) and `gcov` output for covered core modules.
- Complexity score formula: `1 + count(if, else if, for, while, case, &&, ||)`.
- Complexity density: complexity score per 100 LOC.

## Module Ranking (Coverage First, Then Complexity)

| Rank | Module | Coverage | LOC | Complexity Score | Complexity Density (/100 LOC) |
|---|---|---:|---:|---:|---:|
| 1 | [scr/MatrixCore/MatrixCore.cpp](../scr/MatrixCore/MatrixCore.cpp) | 100.00% | 313 | 52 | 16.61 |
| 2 | [scr/DateConversionCore/DateConversionCore.cpp](../scr/DateConversionCore/DateConversionCore.cpp) | 100.00% | 100 | 23 | 23.00 |
| 3 | [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | 0.00% | 693 | 45 | 6.49 |
| 4 | [scr/CLoginDlg.cpp](../scr/CLoginDlg.cpp) | 0.00% | 103 | 16 | 15.53 |
| 5 | [scr/multiple_equations.cpp](../scr/multiple_equations.cpp) | 0.00% | 203 | 16 | 7.88 |
| 6 | [scr/linear.cpp](../scr/linear.cpp) | 0.00% | 180 | 15 | 8.33 |
| 7 | [scr/Calculate_molecule.cpp](../scr/Calculate_molecule.cpp) | 0.00% | 161 | 11 | 6.83 |
| 8 | [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp) | 0.00% | 278 | 10 | 3.60 |
| 9 | [scr/Calculate_atom.cpp](../scr/Calculate_atom.cpp) | 0.00% | 107 | 7 | 6.54 |
| 10 | [scr/integration.cpp](../scr/integration.cpp) | 0.00% | 141 | 7 | 4.96 |
| 11 | [scr/Prime_factor.cpp](../scr/Prime_factor.cpp) | 0.00% | 78 | 6 | 7.69 |
| 12 | [scr/Complex_cal.cpp](../scr/Complex_cal.cpp) | 0.00% | 214 | 5 | 2.34 |
| 13 | [scr/Date_Conversion.cpp](../scr/Date_Conversion.cpp) | 0.00% | 89 | 3 | 3.37 |
| 14 | [scr/Base_conversion.cpp](../scr/Base_conversion.cpp) | 0.00% | 75 | 2 | 2.67 |
| 15 | [scr/Draw_dlg.cpp](../scr/Draw_dlg.cpp) | 0.00% | 38 | 1 | 2.63 |
| 16 | [scr/integrate.cpp](../scr/integrate.cpp) | 0.00% | 41 | 1 | 2.44 |
| 17 | [scr/Leisure.cpp](../scr/Leisure.cpp) | 0.00% | 50 | 1 | 2.00 |
| 18 | [scr/INT.cpp](../scr/INT.cpp) | 0.00% | 34 | 1 | 2.94 |
| 19 | [scr/Login.cpp](../scr/Login.cpp) | 0.00% | 34 | 1 | 2.94 |
| 20 | [scr/pch.cpp](../scr/pch.cpp) | 0.00% | 5 | 1 | 20.00 |
| 21 | [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp) | 0.00% | 184 | 1 | 0.54 |

## Notes

- This reassessment supersedes `-02` with current MatrixCore and DateConversionCore coverage results.
- Core logic extracted for tests now provides two fully covered production modules.
- MFC dialog modules remain at 0% in this CMake+CTest path because tests target portable core modules, not UI handlers.
- `pch.cpp` complexity density is not meaningful due to tiny file size.
