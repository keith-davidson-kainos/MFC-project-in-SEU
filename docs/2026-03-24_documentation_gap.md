# Documentation Gap Matrix

**Date:** 24 March 2026  
**Project:** Atom-Molecule Calculate (MFC-project-in-SEU)

## Gap Matrix

| Checklist Item | Status | Last Updated (Discoverable) | One-Line Quality Note | Evidence |
|---|---|---|---|---|
| (a) Architecture overview | Partial | 2026-03-24 | High-level structure exists, but no single canonical architecture doc with system boundaries, runtime flow, and diagrams. | [README.md](../README.md), [index.html](../index.html), [.github/copilot-instructions.md](../.github/copilot-instructions.md), [docs/SBOM.md](./SBOM.md) |
| (b) Component/service descriptions | Present | 2026-03-24 | Component inventory is strong (modules, purpose, dependencies), but lacks ownership, lifecycle, and interface contracts. | [docs/SBOM.md](./SBOM.md) |
| (c) API documentation | Absent | N/A | No API surface documentation (endpoints, callable interfaces, contracts, or usage examples) was found. | [README.md](../README.md), [scr](../scr) |
| (d) Data model/schema docs | Absent | N/A | No data model, schema, or persistent storage structure documentation is present. | [README.md](../README.md), [scr](../scr) |
| (e) Deployment and infrastructure notes | Partial | 2026-03-24 | Build/runtime prerequisites are documented, but no end-to-end deployment packaging/release or environment topology guide exists. | [.github/copilot-instructions.md](../.github/copilot-instructions.md), [docs/SBOM.md](./SBOM.md), [docs/sbom-risk-analysis.md](./sbom-risk-analysis.md), [docs/2026-03-24_action-plan.md](./2026-03-24_action-plan.md) |
| (f) Runbooks and operational procedures | Absent | N/A | No incident, troubleshooting, recovery, or operational runbooks were found. | [docs](./), [README.md](../README.md) |
| (g) Onboarding guide | Partial | 2022-06-03 (project intro), 2026-03-24 (build notes) | Newcomers get basic context and build hints, but no step-by-step setup/first-task workflow. | [README.md](../README.md), [.github/copilot-instructions.md](../.github/copilot-instructions.md) |
| (h) ADRs or decision records | Absent | N/A | No Architecture Decision Records or equivalent decision log was identified. | [docs](./), [README.md](../README.md) |
| (i) Test strategy and coverage notes | Partial | 2026-03-24 | There are statements about lack of automated tests and some acceptance checks, but no formal test strategy, test pyramid, or coverage baseline. | [.github/copilot-instructions.md](../.github/copilot-instructions.md), [docs/remediation-backlog.md](./remediation-backlog.md), [docs/code-analysis.md](./code-analysis.md) |

## Summary

- Present: 1 of 9
- Partial: 4 of 9
- Absent: 4 of 9

## Discoverable Last-Updated Dates Used

- [README.md](../README.md): 2022-06-03
- [index.html](../index.html): 2022-06-03
- [Atom-Molecule_Calculate_doc.pdf](../Atom-Molecule_Calculate_doc.pdf): 2022-06-03
- [report.pdf](../report.pdf): 2022-06-03
- [slides.pdf](../slides.pdf): 2022-06-03
- [docs/code-analysis.md](./code-analysis.md): 2026-03-24
- [docs/remediation-backlog.md](./remediation-backlog.md): 2026-03-24
- [docs/SBOM.md](./SBOM.md): 2026-03-24
- [docs/sbom-risk-analysis.md](./sbom-risk-analysis.md): 2026-03-24
- [docs/2026-03-24_action-plan.md](./2026-03-24_action-plan.md): 2026-03-24
- [.github/copilot-instructions.md](../.github/copilot-instructions.md): 2026-03-24
