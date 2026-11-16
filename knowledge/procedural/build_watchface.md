---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:46:00-06:00"
---
# Native build and QA
- Run `make test`, `make build`, `make verify` from repo root.
- Production build strips DOT_* environment values and cleans prior objects.
- Fixture build from watchface: `pebble clean`, then `DOT_FIXTURE=1 pebble build`.
- Root-owned shared emulator only: install fixture and inspect foreground before capture.
- `python3 execution/capture_native.py --monitor PORT` checks native dot centers, dot gaps, ink centering, palette, date/AMPM presence and battery-group centering.
- Fixture AppMessage11000hour/11001minute/11004battery, compiled out of production. Date fixture Thu2026-11-05.
- Restore/verify clean production before copying to release; screenshots under media retain fixed fixture time, not a claim about real-time device behavior.
