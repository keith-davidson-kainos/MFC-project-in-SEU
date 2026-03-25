# Coverage And Complexity Reassessment Table

**Date:** 25 March 2026  
**Scope:** `scr/*.cpp` plus `scr/DateConversionCore/*.cpp`  
**Test Run Basis:** CMake + CTest on macOS (`DateConversionCoreTests`)  

## Coverage Summary

- Measured line coverage (`DateConversionCore.cpp`): **61.67%** (37/60 lines executed)
- Measured branch coverage (`DateConversionCore.cpp`): **71.43%** (30/42 branches executed)
- Untested production modules in this run: treated as **0.00%** line coverage
- Estimated overall production line coverage (lower-bound): **1.33%**
  - Calculation: executed lines 37 / total production LOC 2783

## Method

- Coverage measured with instrumented CMake build (`--coverage`) and `gcov` output for `DateConversionCore.cpp`.
- Complexity score formula: `1 + count(if, else if, for, while, case, &&, ||)`.
- Complexity density: complexity score per 100 LOC.

## Module Ranking (Coverage First, Then Complexity)

| Rank | Module | Coverage | LOC | Complexity Score | Complexity Density (/100 LOC) |
|---|---|---:|---:|---:|---:|
| 1 | [scr/DateConversionCore/DateConversionCore.cpp](../scr/DateConversionCore/DateConversionCore.cpp) | 61.67% | 100 | 23 | 23.00 |
| 2 | [scr/Calculate_atomDlg.cpp](../scr/Calculate_atomDlg.cpp) | 0.00% | 693 | 45 | 6.49 |
| 3 | [scr/CLoginDlg.cpp](../scr/CLoginDlg.cpp) | 0.00% | 103 | 16 | 15.53 |
| 4 | [scr/multiple_equations.cpp](../scr/multiple_equations.cpp) | 0.00% | 203 | 16 | 7.88 |
| 5 | [scr/linear.cpp](../scr/linear.cpp) | 0.00% | 180 | 15 | 8.33 |
| 6 | [scr/Matrix_Cal.cpp](../scr/Matrix_Cal.cpp) | 0.00% | 253 | 13 | 5.14 |
| 7 | [scr/Calculate_molecule.cpp](../scr/Calculate_molecule.cpp) | 0.00% | 161 | 11 | 6.83 |
| 8 | [scr/Calculate_atom.cpp](../scr/Calculate_atom.cpp) | 0.00% | 107 | 7 | 6.54 |
| 9 | [scr/integration.cpp](../scr/integration.cpp) | 0.00% | 141 | 7 | 4.96 |
| 10 | [scr/Prime_factor.cpp](../scr/Prime_factor.cpp) | 0.00% | 78 | 6 | 7.69 |
| 11 | [scr/Complex_cal.cpp](../scr/Complex_cal.cpp) | 0.00% | 214 | 5 | 2.34 |
| 12 | [scr/Date_Conversion.cpp](../scr/Date_Conversion.cpp) | 0.00% | 89 | 3 | 3.37 |
| 13 | [scr/Base_conversion.cpp](../scr/Base_conversion.cpp) | 0.00% | 75 | 2 | 2.67 |
| 14 | [scr/Draw_dlg.cpp](../scr/Draw_dlg.cpp) | 0.00% | 38 | 1 | 2.63 |
| 15 | [scr/integrate.cpp](../scr/integrate.cpp) | 0.00% | 41 | 1 | 2.44 |
| 16 | [scr/Leisure.cpp](../scr/Leisure.cpp) | 0.00% | 50 | 1 | 2.00 |
| 17 | [scr/INT.cpp](../scr/INT.cpp) | 0.00% | 34 | 1 | 2.94 |
| 18 | [scr/Login.cpp](../scr/Login.cpp) | 0.00% | 34 | 1 | 2.94 |
| 19 | [scr/pch.cpp](../scr/pch.cpp) | 0.00% | 5 | 1 | 20.00 |
| 20 | [scr/Time_Conversion.cpp](../scr/Time_Conversion.cpp) | 0.00% | 184 | 1 | 0.54 |

## Notes

- This reassessment is a true improvement over the previous baseline: one production module now has measured executable coverage.
- `Date_Conversion.cpp` remains 0% in this run because current tests target `DateConversionCore` directly (logic-level testing), not MFC dialog handlers.
- `pch.cpp` complexity density is not meaningful due to tiny file size.
