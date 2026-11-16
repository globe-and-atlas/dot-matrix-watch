---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:37:54-06:00"
---
# Build watchface
Run `node watchface/settings.test.js` and `python3 -m pytest tests/ -q`.
`python3 execution/build.py` clean-builds production, excluding DOT_* flags.
`python3 execution/verify.py` checks the resulting EmeryPBW.
Fixtures require `pebble clean`, `DOT_FIXTURE=1 pebble build` from watchface.
Capture only from the root-owned shared Emery emulator. Never publish fixture artifacts.
