# Documentation Gap Register (One-Page)

**Date:** 24 March 2026  
**Scope:** Standard documentation categories for investigation readiness  
**Priority Scale:** High = needed to proceed with investigation; Medium = need soon; Low = can wait

| Area | Current State | Priority | Suggested Owner | Recommended Next Activity |
|---|---|---|---|---|
| Architecture overview | Partial | **High** | Architecture Lead [name] | Create Architecture Summary: context, module boundaries, runtime flow, and one diagram of Atom->Molecule interactions. |
| Component/service descriptions | Present (quality gaps) | Medium | Module Maintainers [name] | Enrich component catalog with ownership, lifecycle, dependencies, and failure modes per dialog/module. |
| API documentation | Absent | **High** | Technical Lead [name] | Produce Interface Notes: document callable interfaces, message handlers, input/output contracts, and examples. |
| Data model/schema docs | Absent | **High** | Data/Domain Owner [name] | Create Data Model Note: input fields, transient structures, parsing assumptions, and result-format conventions. |
| Deployment and infrastructure notes | Partial | Medium | Build/Release Owner [name] | Write Deployment Guide: supported OS/toolchain matrix, packaging steps, runtime DLL requirements, and verification checklist. |
| Runbooks and operational procedures | Absent | **High** | Operations Owner [name] | Create Operations Runbook: startup checks, common failures, ShellExecute path issues, recovery/rollback, and escalation path. |
| Onboarding guide | Partial | Medium | Team Lead [name] | Build New-Joiner Guide: local setup, build/run steps, key files map, first-change workflow, and PR expectations. |
| ADRs / decision records | Absent | Low | Architecture Lead [name] | Start ADR log with first 3 records: MFC choice, Windows-only target, and toolchain migration decision (v142->v143). |
| Test strategy and coverage notes | Partial | Medium | QA/Test Owner [name] | Publish Test Strategy: manual smoke suites by dialog, defect-risk-based regression set, and coverage baseline policy. |

## Prioritized Execution Order

1. High: Architecture overview
2. High: API documentation
3. High: Data model/schema docs
4. High: Runbooks and operational procedures
5. Medium: Deployment and infrastructure notes
6. Medium: Onboarding guide
7. Medium: Test strategy and coverage notes
8. Medium: Component/service description quality uplift
9. Low: ADRs / decision records

## Suggested 2-Week Delivery Cadence

- Week 1: Close all High items (draft + review + publish).
- Week 2: Close Medium items; open ADR log and capture initial decisions.

## Definition of Done (per item)

- Stored under `docs/` with clear owner and review date.
- Linked from `README.md` as part of a documentation index.
- Contains at least one worked example relevant to this MFC project.
