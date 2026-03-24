# Dependency Upgrade Action Plan
**Date:** 24 March 2026  
**Status:** CRITICAL ISSUES DETECTED  
**Urgency:** Immediate action required on 2 items

---

## Dependency RAG Assessment

| Dependency | Current Version | EOL Status | Days Past EOL | RAG | Priority | Action |
|------------|---|---|---|:---:|:---:|---|
| **Visual Studio 2019 v142** | v142 (2019) | 13 Oct 2025 | 163 days | 🔴 RED | **CRITICAL** | **IMMEDIATE:** Migrate to VS2022 v143 or later |
| **C Runtime (msvcr120.dll)** | v12.0 (VS2013) | 20 July 2018 | ~2,800 days | 🔴 RED | **CRITICAL** | **TODAY:** Fix `.vcxproj` linker; use vcruntime140.dll (v14.0+) |
| **MFC Runtime** | v14.2 (VS2019) | 13 Oct 2025 | 163 days | 🔴 RED | **HIGH** | Resolve as part of VS2019 → VS2022 migration |
| **Windows 10** | Latest (22H2) | 8 Oct 2025 | 168 days | 🟠 AMBER | MEDIUM | Document Windows 11 as preferred target; maintain legacy support if feasible |
| **Windows 11** | Latest (23H2) | 10 Oct 2026 | +201 days | 🟢 GREEN | LOW | No action; official preferred platform; ~6 months remaining support |
| **C++ Standard Library** | C++98 | Perpetual | N/A | 🟢 GREEN | LOW | No EOL risk; consider C++11/14 upgrade for modern code |
| **Windows Common Controls** | v6.0.0.0 | Platform-provided | N/A | 🟢 GREEN | LOW | No action; manifest-deployed SxS assembly |
| **MIT License (Source)** | MIT | Perpetual | N/A | 🟢 GREEN | LOW | Permissive; include LICENSE in distribution |

---

## Critical Issues Summary

### 🔴 Issue 1: CRT Version Mismatch (P0)
- **Current State:** Project links `msvcr120.dll` (Visual Studio 2013), mismatched to v142 (VS2019)
- **Root Cause:** Stale `.vcxproj` configuration; likely from old template
- **Risk:** Build artifacts may fail at runtime; security patches for old CRT no longer issued
- **Fix Effort:** < 1 hour
- **Resolution:** 
  1. Edit `.vcxproj` and remove explicit `msvcr120.dll` from linker inputs
  2. Allow MSBuild to auto-select `vcruntime140.dll` for v142 toolset
  3. Rebuild and verify output DLL imports

### 🔴 Issue 2: Toolchain End of Support (P1)
- **Current State:** VS2019 v142 extended support ended 13 Oct 2025 (163 days ago)
- **Risk:** No security patches; no continued vendor support; new Windows updates may cause compatibility issues
- **Support Timeline:** VS2022 v143 has ~18 months remaining support (Oct 2027 estimated)
- **Fix Effort:** 2–3 days (testing + validation)
- **Resolution:**
  1. Retarget project from v142 to v143 in `.vcxproj`
  2. Test compilation on Debug/Release × Win32/x64
  3. Verify runtime behavior unchanged
  4. Update CI/CD build scripts

---

## Recommended Timeline

| Phase | Target | Start | Duration | Deliverable |
|-------|--------|-------|----------|---|
| **Phase 0** | Fix CRT mismatch (P0) | **NOW** | < 1h | Updated `.vcxproj`; validated build |
| **Phase 1** | VS2022 migration (P1) | This week | 2–3d | v143 toolset retargeting; test suite pass |
| **Phase 2** | Publish support matrix | Next sprint | 1d | Documentation: Windows 11 preferred, Windows 10 legacy |
| **Phase 3** | Plan modernization (optional) | Q2 2026 | Ongoing | C++11/14 adoption; potential WinUI 3 evaluation |

---

## Compliance Status

✅ **Green Items:**
- No copyleft exposure (GPL/LGPL/AGPL)
- MIT license compatible with proprietary wrapper
- Windows Common Controls v6.0.0.0 SxS manifest correct
- Platform-provided dependencies (Windows API) have no external version risk

⚠️ **Amber Items:**
- Windows 10 past EOL (users on 22H2 no longer receiving OS security updates)
- Recommend upgrade path to Windows 11

❌ **Red Items (Action Required):**
- CRT version mismatch must be fixed before next release
- Toolchain EOL requires planning for migration within 6 months

---

## Risk Assessment (Current → Post-Migration)

| Metric | Before Fix | After Fix | Impact |
|--------|---|---|---|
| **Toolchain Support Remaining** | 0 months (past EOL) | ~18 months (v143) | Stability + security |
| **CRT Vendor Support** | Ancient (v12.0 EOL 2018) | Current (v14.0+ EOL 2027) | Runtime correctness |
| **Deployment Risk** | HIGH (mismatched runtimes) | LOW (coherent v142/v143 stack) | Reliability |
| **Security Posture** | EXPOSED (no patches) | MAINTAINED (vendor support) | Compliance |

---

## Next Steps

**Today:**
- [ ] Verify current `.vcxproj` CRT linker configuration
- [ ] Document findings + plan fix

**This Week:**
- [ ] Apply CRT fix and validate rebuild
- [ ] Initiate VS2022 migration planning

**Next Sprint:**
- [ ] Complete VS2022 v143 retargeting and testing
- [ ] Publish updated support matrix documentation

---

**Owner:** Development Lead  
**Review Date:** 30 June 2026  
**Escalation:** If CRT fix or VS2022 migration blocked, notify Platform Owner
