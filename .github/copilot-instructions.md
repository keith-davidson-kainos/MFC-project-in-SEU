# Project Guidelines

## Code Style
- This workspace is a Visual C++ MFC application under `scr/`, not a typical `src/` layout. Start from `scr/Calculate_atom.sln` and `scr/Calculate_atom.vcxproj` when you need build or file-inclusion context.
- Match the existing C++/MFC style in touched files: tabs for indentation, PascalCase dialog/class names, MFC types such as `CString`, `BOOL`, and `CDialogEx`, and message-map macros such as `BEGIN_MESSAGE_MAP`.
- Most source comments and UI strings are Simplified Chinese. Preserve existing language in the file you edit and avoid rewriting unrelated strings.
- This project uses precompiled headers. New `.cpp` files should include `pch.h` first and should be added to `scr/Calculate_atom.vcxproj` and `scr/Calculate_atom.vcxproj.filters` if they are meant to build.

## Architecture
- `scr/Calculate_atom.cpp` initializes the MFC application and launches the main dialog defined in `scr/Calculate_atomDlg.cpp` and `scr/Calculate_atomDlg.h`.
- `scr/Calculate_atomDlg.*` is the main “Atom” calculator UI. It also owns menu actions that switch into other dialogs such as molecule mode, leisure mode, and drawing mode.
- `scr/Calculate_molecule.cpp` acts as the launcher for the advanced calculators. Most feature dialogs live as paired `.cpp` and `.h` files in `scr/`, for example `integration.*`, `Matrix_Cal.*`, `Time_Conversion.*`, and `Prime_factor.*`.
- UI layout and control IDs are defined in `scr/Calculateatom.rc` and `scr/resource.h`. Handler code, dialog classes, and resource IDs must stay in sync.

## Build And Test
- Build on Windows with Visual Studio 2019 and the MFC toolchain configured by `scr/Calculate_atom.sln`.
- Preferred build entry points:
  - Open `scr/Calculate_atom.sln` in Visual Studio and build `Debug|Win32`, `Debug|x64`, `Release|Win32`, or `Release|x64`.
  - Or use MSBuild on Windows for scripted builds, for example: `msbuild scr/Calculate_atom.sln /p:Configuration=Debug /p:Platform=x64`
- Do not assume the project can be built or run on macOS or Linux. MFC, Windows resources, and the Visual Studio project format are Windows-specific.

## Tests Directory And Unit Testing Approach
- Automated unit tests live under `tests/Calculate_atom.Tests/` and are built by the root `CMakeLists.txt` into the `DateConversionCoreTests` executable.
- Keep test code portable and independent of MFC UI types. Put computation and parsing logic into plain C++ modules (for example under `scr/DateConversionCore/` or `scr/MatrixCore/`) and test those modules directly.
- Add one `*Tests.cpp` file per module/feature in `tests/Calculate_atom.Tests/`, include `pch.h`, and register cases with GoogleTest.
- Unit tests should cover:
  - Public function behavior and key branches.
  - Error handling for invalid, null, empty, and out-of-range inputs where applicable.
  - Boundary conditions and representative numeric edge cases.
- Prefer deterministic assertions (`EXPECT_EQ`, `EXPECT_NEAR`) with explicit expected values; avoid UI-level automation in this test target.
- Run the test workflow from repository root:
  - `cmake -S . -B build`
  - `cmake --build build`
  - `ctest --test-dir build --output-on-failure`
- When introducing a new testable core module, wire both production and test files in `CMakeLists.txt`; if the module is also used by the MFC app, add corresponding entries to `scr/Calculate_atom.vcxproj` and `scr/Calculate_atom.vcxproj.filters`.
- For MFC dialog changes that remain UI-only, document manual verification steps in the PR/summary even when unit tests are not applicable.

## Conventions
- Many dialogs are created lazily with `GetSafeHwnd()` checks and `Create(MAKEINTRESOURCE(...))` before `ShowWindow(1)`. Keep that pattern when adding dialog-launch behavior.
- Before editing dialog controls or adding commands, inspect both the implementation file and `scr/Calculateatom.rc` so message handlers, menu items, and control IDs stay aligned.
- The project mixes Unicode and MultiByte settings across configurations in `scr/Calculate_atom.vcxproj`. Avoid broad encoding changes unless the task explicitly requires them.
- Some features launch external files or executables with `ShellExecute`, for example the report document and `Mine.exe`. When modifying those areas, preserve relative-path expectations unless you are explicitly fixing packaging.