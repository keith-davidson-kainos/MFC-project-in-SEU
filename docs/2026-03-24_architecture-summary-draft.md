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

## SBOM-Based Dependency Upgrade Shortlist And Ordering

### High-Value Upgrade Candidates (2-3)

| Priority | Dependency Candidate | Current State (SBOM) | Why High-Value |
|---|---|---|---|
| 1 | MSVC/MFC toolchain stack (v142 -> v143) | Visual Studio 2019 toolset + MFC 14.2 family | Current toolchain is past lifecycle support; highest risk reduction and unlocks supported compiler/runtime servicing. |
| 2 | CRT runtime alignment (`msvcr120.dll` -> `vcruntime140*.dll`/UCRT) | SBOM flags `msvcr120.dll` mismatch against v142 expectations | Eliminates runtime mismatch risk and reduces ABI inconsistency across modules. |
| 3 | Windows platform baseline + SDK (Windows 10 legacy baseline -> supported Windows 11/modern Windows 10 build policy) | SBOM/risk notes indicate Windows 10 22H2 lifecycle has ended | Reduces platform EOL exposure and aligns deployment policy with supported OS patch channels. |

### Impact Analysis By Candidate

#### 1) MSVC/MFC toolchain stack (v142 -> v143)

- Breaking API changes (practical):
	- Compiler conformance and diagnostics are stricter in newer MSVC toolsets; legacy code patterns that compiled under older defaults may fail or warn-as-error.
	- Mixed-toolset binary coupling risk: modules built with older toolset/runtime should not be mixed arbitrarily with newly rebuilt binaries.
- Deprecated features removed:
	- Legacy/obsolete CRT assumptions tied to old redistributables are no longer a safe baseline for a supported toolchain posture.
	- Existing deprecated numeric-conversion usage in code (`atof`, `_itoa_s` hotspots) becomes higher-risk under modern hardening goals.
- Minimum runtime/platform requirement:
	- Build environment must move to a supported Visual Studio toolchain generation (VS2022 class).
	- Deployment should standardize on the corresponding Microsoft Visual C++ 2015-2022 runtime family.

#### 2) CRT runtime alignment (`msvcr120.dll` -> `vcruntime140*.dll`/UCRT)

- Breaking API changes (practical):
	- ABI/runtime boundary changes mean all native modules should be rebuilt consistently against the same CRT family.
	- Passing CRT-owned resources (allocation/file handles/locale state) across mixed CRT boundaries is unsafe and can cause defects.
- Deprecated features removed:
	- Reliance on VS2013-era CRT deployment (`msvcr120`) should be treated as legacy and removed from linker/deployment assumptions.
- Minimum runtime/platform requirement:
	- Requires packaging/installing the modern VC++ redistributable family used by the retargeted build.
	- Keep project runtime support policy at Windows 10/11 (or stricter) to avoid unsupported legacy combinations.

#### 3) Windows platform baseline + SDK policy refresh

- Breaking API changes (practical):
	- Raising minimum supported OS/version policy can intentionally drop very old Windows builds from support matrix.
	- Any legacy behavior relying on older shell/runtime quirks must be re-verified under modern platform defaults.
- Deprecated features removed:
	- EOL Windows servicing baseline (notably older Windows 10 channels) should be removed from deployment target matrix.
- Minimum runtime/platform requirement:
	- Define and document supported OS matrix explicitly (recommended: fully supported Windows 11 and supported Windows 10 channels only).
	- Align SDK selection and CI build images with that support matrix.

### Dependency Graph And Upgrade Ordering

#### Runtime Dependency Graph (simplified)

1. Application modules (`Calculate_atom.exe` + dialogs) depend on MFC runtime/toolset outputs.
2. MFC runtime/toolset outputs depend on a consistent CRT/UCRT family.
3. CRT/UCRT and MFC runtime ultimately depend on Windows OS APIs and supported platform baseline.

#### Ordered Upgrade Path (what must happen first)

1. Establish supported platform policy and SDK baseline (supported Windows build matrix).
2. Retarget toolchain from VS2019/v142 to VS2022/v143 with full solution rebuild.
3. Align CRT linkage/deployment to one modern runtime family (`vcruntime140*`/UCRT); remove `msvcr120` assumptions.
4. Re-validate MFC/UI modules and external launch paths (`ShellExecute`) under the new runtime + platform matrix.

Upgrade dependency constraints:

- Upgrading toolchain (step 2) requires platform/SDK decision (step 1) to avoid retarget churn.
- Upgrading CRT alignment (step 3) depends on toolchain retarget completion (step 2), because linker/runtime family is toolchain-driven.
- Functional verification (step 4) depends on steps 2 and 3 being complete to test real production-like binaries.
