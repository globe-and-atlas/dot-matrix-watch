---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:46:00-06:00"
---
# Dot Matrix
A Pebble Time2 watchface with original circular-dot time, a compact dot date and a centered dotted battery display.

- Six presets: Mint, Amber, Ice, Rose, White and Paper.
- Custom background, digits/date and battery/divider colors.
- Watch-default,12-hour or24-hour time. Explicit AM/PM in12-hour mode.
- Date Off, weekday/month/day, ISO, US or EU.
- Real battery percentage plus four bars.
- Minute-only updates.

## Build
`make test`
`make build`
`make verify`

Output: dist/Dot-Matrix-0.1.0.pbw. Includes Emery, Chalk, and Gabbro.
Native captures live in media/. This version has not been published.


## Round-compatible release — 0.1.1

Dot Matrix includes native layouts for Pebble Time2 (Emery200×228), original Time Round (Chalk180×180), and Round2 (Gabbro260×260). The rectangular layout and existing settings identities are preserved.

Build: `python3 execution/build.py`, then `python3 execution/verify.py`. Local bundle: `dist/Dot-Matrix-0.1.1.pbw`. Native round previews: `media/round/`. Repeatable fixture/capture instructions: [round compatibility QA](knowledge/procedural/round_compatibility.md).

This update is [publicly available](https://apps.repebble.com/3f9c7df9e7924d4abe6be703) with round screenshots. Public PBW bytes and screenshot pixels verify; physical-watch validation remains separate.
