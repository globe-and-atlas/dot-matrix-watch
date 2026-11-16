---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-05T21:46:00-06:00"
---
# Errors —2026-11-05
## Relative execution path from SDK directory
A root-relative helper command was invoked from watchface, so it did not run. Product builds continued independently. Correct path use is an absolute Path for helpers; SDKcommands retain watchface cwd. Final root checks are rerun explicitly before completion.
## Shared emulator isolation
The root-owned emulator flash was backed up before graceful reset for the new face. This prevents the known stale-foreground issue from becoming false native screenshot evidence. No other emulator driver ran concurrently.
