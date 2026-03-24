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
- No automated test suite is present in this repository. When changing behavior, prefer small targeted code changes and explain what should be verified manually in the affected dialog.

## Conventions
- Many dialogs are created lazily with `GetSafeHwnd()` checks and `Create(MAKEINTRESOURCE(...))` before `ShowWindow(1)`. Keep that pattern when adding dialog-launch behavior.
- Before editing dialog controls or adding commands, inspect both the implementation file and `scr/Calculateatom.rc` so message handlers, menu items, and control IDs stay aligned.
- The project mixes Unicode and MultiByte settings across configurations in `scr/Calculate_atom.vcxproj`. Avoid broad encoding changes unless the task explicitly requires them.
- Some features launch external files or executables with `ShellExecute`, for example the report document and `Mine.exe`. When modifying those areas, preserve relative-path expectations unless you are explicitly fixing packaging.