# Draft Architecture Summary

**Project:** Atom-Molecule Calculate (MFC desktop application)  
**Date:** 24 March 2026  
**Scope:** Current-state architecture based on repository inspection

## (a) System Purpose And Key User Groups

### Purpose
The system is a Windows desktop calculator suite built with MFC that combines:
- Atom mode: standard calculator operations and expression evaluation.
- Molecule mode: advanced math dialogs (linear equations, matrix, complex numbers, date/time conversion, base conversion, prime factorization, integration, cubic equations).
- Utility modes: drawing launcher and leisure/game launcher.

### Key User Groups
- Students and instructors needing quick numeric and symbolic computation in a GUI workflow.
- Course-demo users evaluating a multi-dialog MFC application structure.
- Maintainers/developers extending legacy MFC features on Visual Studio toolchains.

## (b) Major Components/Services With Responsibilities And Boundaries

### 1) Application Bootstrap Layer
- **Files:** `scr/Calculate_atom.cpp`, `scr/Calculate_atom.h`
- **Responsibility:** Initialize MFC runtime, create and run the main dialog.
- **Boundary:** No business logic; delegates all interaction to dialog layer.

### 2) Main Dialog And Expression Engine Layer
- **Files:** `scr/Calculate_atomDlg.cpp`, `scr/Calculate_atomDlg.h`
- **Responsibility:** Primary UI, keypad handling, infix/postfix conversion, evaluation flow, menu entry points to other dialogs.
- **Boundary:** Mixes presentation logic with core algorithm logic (currently coupled).

### 3) Molecule Module Launcher Layer
- **Files:** `scr/Calculate_molecule.cpp`, `scr/Calculate_molecule.h`
- **Responsibility:** Route users from Molecule dialog to specialized calculator dialogs.
- **Boundary:** Navigation/orchestration only; does not own heavy computation itself.

### 4) Specialized Calculator Dialog Layer
- **Files:** `scr/linear.*`, `scr/Matrix_Cal.*`, `scr/Complex_cal.*`, `scr/integration.*`, `scr/multiple_equations.*`, `scr/Base_conversion.*`, `scr/Time_Conversion.*`, `scr/Date_Conversion.*`, `scr/Prime_factor.*`
- **Responsibility:** Domain-specific computation and UI rendering per feature.
- **Boundary:** Each module mostly self-contained but repeats parsing/validation patterns.

### 5) Auxiliary Feature Layer
- **Files:** `scr/Draw_dlg.*`, `scr/Leisure.*`, `scr/CLoginDlg.*`
- **Responsibility:** Launch external drawing/game executables and provide login gating.
- **Boundary:** Integrates with external files/executables via ShellExecute.

### 6) Resource And Platform Layer
- **Files:** `scr/Calculateatom.rc`, `scr/resource.h`, `scr/framework.h`, `scr/pch.h`
- **Responsibility:** Dialog/control definitions, resource IDs, MFC includes, Windows common-controls manifest.
- **Boundary:** Platform/configuration concerns; no domain computation.

## (c) Component Interaction Patterns

### Synchronous Calls (Primary Pattern)
- UI button handlers synchronously call parsing/computation logic and immediately update controls.
- Dialog-to-dialog navigation uses direct MFC dialog creation/show calls.
- Most feature modules execute request/response interactions inside one event handler.

### Event-Driven UI Messaging
- Uses MFC message maps (`BEGIN_MESSAGE_MAP`) to dispatch Windows UI events.
- Events are asynchronous at the OS message-loop level, but handler logic is synchronous per event.

### Async Messaging
- No message broker, queue, or service bus integration found.
- No internal asynchronous job scheduling pattern identified.

### Shared Databases / Persistence
- No database dependency found.
- No persistent domain data store identified; calculations are in-memory per dialog session.

## (d) External Integrations And APIs

- **Windows API / MFC Runtime:** Core windowing, controls, and application runtime.
- **ShellExecute integration:** Launches external files/executables (report document, drawing executable, game executable).
- **Windows Common Controls Manifest:** Declares `Microsoft.Windows.Common-Controls` v6.0.0.0.
- **No external web/service APIs** detected.

## (e) Key Dependencies

### Frameworks And Runtimes
- Microsoft Foundation Classes (MFC), Visual Studio toolchain (v142 currently referenced).
- Microsoft C Runtime libraries (current analysis indicates a CRT mismatch risk around `msvcr120.dll` vs expected `vcruntime140.dll` family).
- Windows system DLLs (`kernel32`, `user32`, `gdi32`, `shell32`, `advapi32`).

### Databases
- None identified.

### Message Brokers
- None identified.

## (f) Known Hotspots, Risks, And Technical Debt Areas

### Security Hotspot
- `scr/CLoginDlg.cpp`: hardcoded plaintext credentials.

### Correctness Hotspots
- `scr/Matrix_Cal.cpp`: multiplication output formula mismatch (`c12/c21` issue).
- `scr/Prime_factor.cpp`: loop-bound issue for perfect squares.
- `scr/Time_Conversion.cpp`: negative modulo handling can produce invalid hour output.
- `scr/integration.cpp`: Unicode build fragility (`_T(result)` usage pattern).

### Maintainability Debt
- Heavy handler duplication in main keypad and multiple numeric dialogs.
- Parsing/validation logic repeated across modules (`atof` usage, low error signaling).
- Expression engine and UI logic tightly coupled in main dialog.
- Mixed legacy configuration footprint and obsolete/unused files in project tree.

### Platform/Dependency Risk
- Toolchain lifecycle risk (VS2019 support window status).
- Runtime consistency risk from CRT configuration mismatch.

## High-Level Component Diagram

Diagram source: [docs/2026-03-24_architecture-component-diagram.mmd](./2026-03-24_architecture-component-diagram.mmd)

Use this standalone Mermaid file for previewing and future edits.

## Assumptions And Constraints

- Current system is Windows-only due to MFC and resource/toolchain dependencies.
- Architecture is monolithic desktop UI with in-process computation modules.
- No service decomposition, network API layer, or persistence layer is currently present.
