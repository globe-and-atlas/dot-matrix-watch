---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:37:54-06:00"
---
# Dot Matrix native watchface

## Contract
- Surface: native Emery200×228 clock matching selected DotMatrix concept.
- Execution: build.py, verify.py, capture_native.py.
- Artifact: dist/Dot-Matrix-0.1.0.pbw and native previews.
- No store publication requested in this step.
- Never read .env; never commit .tmp or SDK lock files.
- Root serializes shared emulator use.

## Validation contract
- [x] Native time consists of discrete circular dots.
- [x] Time ink is horizontally centered.
- [x] Default palette is mint on black.
- [x] Six preset palettes render.
- [x] Custom colors render.
- [x] Watch-default time format follows the watch preference.
- [x] Forced12-hour time has an AM/PM indicator.
- [x] Forced24-hour time shows00 at midnight.
- [x] Date Off hides date text.
- [x] Word date fits the screen.
- [x] ISO date fits the screen.
- [x] US date fits the screen.
- [x] EU date fits the screen.
- [x] Battery displays actual charge.
- [x] Battery group is centered at0%,82%,100%.
- [x] Settings survive native relaunch.
- [x] Clay Save sends normalized settings.
- [x] Clay Cancel preserves settings.
- [x] Host tests pass.
- [x] Production build passes identity verification.
- [x] Fresh verifier reviews source and native outputs.

## Master's brief
Primary good: Clarity through a recognizable, readable electronic display.
Stone removed: original5×7 dot glyphs need no external font dependencies.
Human mark: Daniel judges whether native dot texture captures the selected render.
Non Finito: physical display appearance and store publication remain separate.
Structural weakness: concept circles can become muddy on a200px screen.
Myth under test:5px clock dots with8px horizontal pitch retain distinct shapes; reject overlap or unreadable1/7/0/8 at native scale.
Misericord: optical centering, date width and settings cancellation receive equal care.
S.D.G.: Give PebbleTime2 owners clear time with a distinctive dot display.
Feynman gap: native appearance verified; physical-watch appearance remains untested.

## Completion
-25nativefixturecases plus2watch-defaultpreferencecases passed.
- Native settings persisted pixel-identically after relaunch.
- Clean production installed and live-clock/battery screenshot observed.
- Fresh independent source/native review passed.
- No store publication or remote push performed.

## Round compatibility — 2026-11-06
- [x] Emery remains a build target.
- [x] Chalk is a build target.
- [x] Gabbro is a build target.
- [x] The UUID is preserved.
- [x] Existing settings key mappings are preserved.
- [x] Native Chalk layout passes visual review.
- [x] Native Gabbro layout passes visual review.
- [x] Clean production PBW contains three native binaries.
- [x] Existing host/settings checks pass.
- [x] Fresh independent review passes.
No store publication is authorized by this compatibility request.

## Store round update — 2026-11-06
- [x] Publish the verified three-platform production update.
- [x] Preserve the current animated preview.
- [x] Add native screenshots for Chalk.
- [x] Add native screenshots for Gabbro.
- [x] Verify the public download matches the production artifact.
- [x] Verify public round screenshot pixels.
- [x] Fresh independent public-proof review passes.
