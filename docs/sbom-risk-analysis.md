# SBOM Risk & Lifecycle Analysis Report

> Generated: 24 March 2026  
> Basis: SBOM from previous analysis  
> Scope: Atom-Molecule Calculate MFC Application

---

## Executive Summary

| Category | Finding | Risk Level |
|----------|---------|-----------|
| **EOL / Lifecycle** | Toolchain (VS2019) entering extended support phase; OS baseline (Windows 10) near EOL | **HIGH** |
| **Version Conflicts** | No transitive conflicts detected; closed Microsoft stack provides version coherence | **LOW** |
| **License Risk** | Proprietary stack + MIT source; no copyleft exposure | **LOW** |

---

## Part 1: End-of-Life & Lifecycle Status

### 1.1 Visual Studio 2019 v142 Toolset

| Aspect | Details | Status |
|--------|---------|--------|
| **Release Date** | 23 April 2019 | Released 6+ years ago |
| **Mainstream Support** | 10 May 2024 (ended) | **PAST DEADLINE** |
| **Extended Support** | 13 October 2025 | **APPROACHING EOL** (~6 months) |
| **Current Date** | 24 March 2026 | **PAST EXTENDED SUPPORT** |
| **Action Required** | **URGENT**: Plan upgrade to VS2022 or migrate to newer toolchain | **CRITICAL** |

**Details:**
- VS2019 extended support ended 13 October 2025.
- The project builds with v142 platform toolset (hardcoded in `.vcxproj`).
- After EOL, security patches and support are discontinued.

**Implications:**
- New builds on modern Windows 10/11 may encounter warnings or incompatibilities.
- Security vulnerabilities in toolchain/runtime are no longer addressed.
- Recommend: Target VS2022 v143 or later as soon as feasible.

### 1.2 Windows 10 (Runtime / Build Environment)

| Aspect | Details | Status |
|--------|---------|--------|
| **Baseline Version** | Windows 10 (documented minimum in framework.h) | ~5+ years old |
| **Version 22H2** | Released October 2023 | Latest |
| **Support End (22H2)** | 8 October 2025 | **PAST** (as of 24 March 2026) |
| **Windows 11** | Released October 2021 | Preferred |
| **Windows 11 Support** | 10 October 2025 (22H2); 10 October 2026 (23H2) | 12+ months remaining |
| **Recommendation** | Target Windows 11 for new end-user deployments | **MEDIUM** |

**Details:**
- The `.vcxproj` doesn't explicitly constraint to Windows 10, but the header comment and oldest CI/CD likely targets Windows 10.
- Windows 10 22H2 reached end of servicing ~5 months ago (October 2025).
- Windows 11 has longer support windows.

**Implications:**
- Users on end-of-life Windows 10 builds will not receive further security updates.
- Recommend documenting Windows 11 as the preferred target in deployment docs.

### 1.3 MFC Runtime DLLs

| DLL | Version | Tied To | Status | Lifecycle | Notes |
|-----|---------|---------|--------|-----------|-------|
| mfc142u.dll | v14.2 (VS2019) | Visual Studio 2019 | **PAST EXTENDED SUPPORT** | Ends 13 Oct 2025 | Matching toolchain |
| mfcd142u.dll | v14.2 (VS2019) | Visual Studio 2019 | **PAST EXTENDED SUPPORT** | Ends 13 Oct 2025 | Debug variant; same lifecycle |
| msvcr120.dll | CRT v12.0 | Visual Studio 2013 | **VERY OLD** | Ended July 2018 | Ancient; should be vcruntime140.dll |

**ISSUE DETECTED:** `msvcr120.dll` is the Visual Studio 2013 C Runtime, not VS2019. The correct CRT for v142 is `vcruntime140.dll` (VS2015+). This mismatch suggests either:
- Stale/incorrect project configuration.
- Unintended binding to very old CRT.
- Configuration inconsistency across build profiles.

**Action:** Verify `.vcxproj` CRT version and align to match v142 toolset (should pull `vcruntime140.dll` or later).

### 1.4 C++ Standard Library Headers

| Component | Standard | Status | Modern Equivalent | Recommendation |
|-----------|----------|--------|-------------------|---|
| `<stack>` | C++98 | Legacy | C++98+ compatible | No action; STL is evergreen |
| `<math.h>` | C89 | Legacy | `<cmath>` (C++98+) | Prefer `<cmath>` for consistency |
| `<cmath>` | C++98 | Modern (for project) | C++11+ overloads | Compatible; keep |
| `<iomanip>` | C++98 | Legacy | C++11+ equivalent | Remove if unused (P3-05) |

**Status:** C++98 is old but well-supported perpetually. No EOL risk here, though modernizing to C++11 or later would improve code quality.

---

## Part 2: Transitive Dependency Version Conflicts

### 2.1 Dependency Graph

```
Calculate_atom.exe
├─ MFC Runtime (v142)
│  ├─ msvcr120.dll (CRT v12.0) [MISMATCH]
│  └─ Windows Common Controls (v6.0.0.0)
│     └─ kernel32.dll, user32.dll, gdi32.dll
├─ C++ STL (C++98)
│  └─ msvcr120.dll (shared with MFC)
└─ Windows API (Windows 10+)
   └─ advapi32.dll, shell32.dll
```

### 2.2 Conflict Detection

| Conflict | Type | Severity | Resolution |
|----------|------|----------|-----------|
| **CRT Version Mismatch** | Major | **HIGH** | `msvcr120.dll` (Visual Studio 2013) vs. expected `vcruntime140.dll` (VS2015+) |
| **MFC/CRT Coupling** | Expected | Low | Both v142 from VS2019; aligned |
| **Windows Common Controls v6.0.0.0** | Manifest Dep | Low | Published public key token matches; no conflict |
| **C++ STL on Multiple Headers** | Normal | Low | `<stack>`, `<math.h>`, `<cmath>` all coexist without conflict; linker selects one CRT |

### 2.3 Priority Issue: CRT Version Mismatch

**Root Cause Hypothesis:**
- The `.vcxproj` was created/templated with VS2013 defaults and later retargeted to v142 without fully updating the CRT linker input.
- OR the build system is picking up an old CRT from the system PATH or lib directories.

**Verification Steps:**
1. Open `.vcxproj` in a text editor and search for `msvcr120` or `vcruntime`.
2. Check project properties → Linker → Input → Additional Dependencies.
3. Verify `/link` options in the build command.

**Remediation:**
- Update `.vcxproj` to remove explicit `msvcr120.dll` linker input.
- Rely on implicit linking through `#pragma comment(lib, "...")` or MSBuild's automatic CRT detection for v142.
- Rebuild and verify output DLL imports `vcruntime140.dll`.

### 2.4 No Other Conflicts Detected

All other dependencies are either:
- **Framework-level** (MFC provides a closed, versioned ecosystem).
- **OS-level** (Windows API is backwards-compatible within major version).
- **STL-level** (C++98 headers are stable and conflict-free).

---

## Part 3: License Analysis & Risk Assessment

### 3.1 License Inventory

| Component | License | Type | Proprietary Context Risk | Notes |
|-----------|---------|------|-------------------------|-------|
| MFC Runtime (mfc142*.dll) | Proprietary (Microsoft) | Closed-source | **None** | Commercial use via MSVC license |
| CRT (msvcr120.dll, vcruntime140.dll) | Proprietary (Microsoft) | Closed-source | **None** | Redistributable with app |
| Windows API (kernel32, user32, gdi32, shell32, advapi32) | Proprietary (Microsoft) | OS-provided | **None** | Platform cost; Windows desktop license required for end-user |
| Windows Common Controls v6.0.0.0 | Proprietary (Microsoft) | SxS Assembly | **None** | Platform-provided, SxS-deployable |
| C++ STL (`<stack>`, `<math.h>`, `<cmath>`, `<iomanip>`) | Proprietary (MSVC) | Header library | **None** | Redistributable; no copyleft |
| Application Source Code (*.cpp, *.h, *.rc) | **MIT License** | Open-source | **None** | Permissive; compatible with proprietary wrapper |

### 3.2 Copyleft Risk Assessment

| License Family | Risk in Proprietary Context | Found in SBOM | Action |
|---|---|---|---|
| **GPL/LGPL** (Copyleft) | **HIGH** — Triggers source disclosure requirement | ✗ Not found | N/A |
| **AGPL** (Copyleft) | **CRITICAL** — Network copyleft | ✗ Not found | N/A |
| **Mozilla Public License** (Weak copyleft) | **MEDIUM** — File-level copyleft | ✗ Not found | N/A |
| **MIT/BSD/Apache 2.0** (Permissive) | **NONE** — No requirements | ✓ MIT source code | ✓ Safe |
| **Proprietary** (Closed) | **NONE** — License holder decides | ✓ MSVC stack | ✓ Safe |

**Conclusion:** No copyleft exposure. The project is safe to redistribute as proprietary closed-source with optional source availability.

### 3.3 MIT License Implications

**Current Status:**
- Application source code carries MIT license (visible in repo badge and LICENSE file).
- MIT is **permissive**: allows commercial use, modification, and redistribution with minimal conditions.

**Compliance Requirements (MIT):**
1. Include a copy of the MIT license in binary distribution or documentation.
2. Acknowledge original authors (already in repo).
3. No modification restrictions.

**Recommendation:**
- Include LICENSE file in the app's shipped documentation folder.
- Display in "About" or Help menu (currently not done).

---

## Part 4: Version Conflict Resolution & Upgrade Path

### 4.1 Immediate Actions (P0)

| Issue | Component | Action | Effort |
|-------|-----------|--------|--------|
| **CRT mismatch** | msvcr120.dll vs. v142 | Verify and correct `.vcxproj` linker settings | XS |
| **VS2019 EOL** | v142 toolset | Plan migration to VS2022 v143 | M-L (testing/validation required) |

### 4.2 Short-term (Next 6-12 months)

| Item | Action | Rationale |
|------|--------|-----------|
| **Modernize C++ standard** | Adopt C++11 or C++14 in project settings | Enables safer idioms, better performance, modern library features |
| **Update Windows target** | Document Windows 11 as preferred; maintain Windows 10 support if feasible | Long-term platform viability |
| **Audit CRT usage** | Ensure all new code links only `vcruntime140.dll` or later | Avoid future EOL surprises |

### 4.3 Long-term Roadmap (12+ months)

| Phase | Target | Benefit |
|-------|--------|---------|
| **Phase 1** | Migrate to VS2022 v143 + C++14 | ~24 months extended support vs. immediate EOL |
| **Phase 2** | Consider modern GUI framework (WinUI 3 / .NET MAUI) | Move away from legacy MFC; gain modern features |
| **Phase 3** | Containerize for CI/CD on Azure / GitHub Actions | Decouple build from developer machines; reproducible builds |

---

## Part 5: Risk Heat Map

| Dimension | Current State | Risk | Timeline | Action Required |
|-----------|---------------|------|----------|-----------------|
| **Toolchain EOL** | VS2019 v142 past extended support | 🔴 **CRITICAL** | Immediate | Upgrade to VS2022 v143 |
| **CRT Version** | Mismatch (v12.0 vs. expected v14.0+) | 🔴 **HIGH** | This release | Fix `.vcxproj` linker config |
| **OS Baseline** | Windows 10 22H2 past EOL | 🟠 **MEDIUM** | 12 months | Target Windows 11; document support matrix |
| **Dependency Coherence** | All first-order deps aligned (Microsoft stack) | 🟢 **LOW** | N/A | Monitor for future transitive additions |
| **License Compliance** | MIT source + proprietary wrapper | 🟢 **LOW** | N/A | Include LICENSE in distribution |
| **Copyleft Exposure** | None detected | 🟢 **LOW** | N/A | Continue avoiding GPL/LGPL/AGPL |

---

## Part 6: Recommendations & Next Steps

### Immediate (This Sprint)

1. **Fix CRT mismatch** in `.vcxproj`:
   ```xml
   <!-- Remove or fix any references to msvcr120.dll -->
   <!-- Verify linker Input uses vcruntime140.dll or later -->
   ```
   Effort: < 1 hour. Impact: Correctness, security.

2. **Plan VS2022 migration**:
   - Evaluate v143 toolset compatibility with existing code.
   - Test compilation and runtime on target platforms.
   Effort: 2-3 days. Impact: ~24 months of extended support lifecycle.

### Short-term (Next Release)

3. **Publish support matrix**:
   - Officially document Windows 11 as preferred; Windows 10 as legacy support.
   - Specify minimum CRT version (`vcruntime140.dll` 14.0+).

4. **Modernize C++ standard** (if codebase review shows benefit):
   - Migrate to C++11/14 for new code.
   - Consider refactoring hotspots (parser, solver modules) with modern idioms.

### Long-term (12+ months)

5. **Strategic toolchain evolution**:
   - Evaluate .NET 6+/WinUI 3 as alternative MFC migration path.
   - Assess containerized CI/CD (GitHub Actions / Azure Pipelines).

---

## Appendix: Detailed EOL Dates

| Product | Current Date | EOL Date | Days Until EOL | Status |
|---------|---|---|---|---|
| VS2019 v142 Extended Support | 24 Mar 2026 | 13 Oct 2025 | -163 days | **PAST EOL** |
| Windows 10 22H2 | 24 Mar 2026 | 8 Oct 2025 | -168 days | **PAST EOL** |
| Windows 10 1809 | 24 Mar 2026 | 12 Nov 2024 | -503 days | **VERY OLD** |
| Windows 11 22H2 (initial) | 24 Mar 2026 | 10 Oct 2025 | -166 days | **PAST EOL** |
| Windows 11 23H2 | 24 Mar 2026 | 10 Oct 2026 | 201 days | **ACTIVE** (~6 months) |
| VS2022 v143 | 24 Mar 2026 | Oct 2027 (Estimate) | ~550 days | **ACTIVE** (~18 months) |

---

## Compliance Checklist

- [x] All external dependencies catalogued
- [x] No GPL/LGPL/AGPL copyleft detected
- [x] MIT license on source code (permissive; safe)
- [x] Proprietary Microsoft stack (no licensing conflicts)
- [x] Transitive dependency conflicts: **1 detected** (CRT mismatch)
- [ ] **ACTION REQUIRED**: Fix CRT version mismatch
- [ ] **ACTION REQUIRED**: Plan VS2019 → VS2022 migration
- [ ] **ACTION REQUIRED**: Document Windows 11 as preferred target
