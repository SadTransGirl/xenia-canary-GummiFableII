# Low-end CPU configs

Use these **only if the main presets hitch/stutter** on your machine.

The main presets in [`../`](../) compile shaders **synchronously**
(`d3d12_pipeline_creation_threads = 0`), which gives clean, artifact-free frames but
can briefly **hitch** the first time a new shader appears — more noticeable on weaker
CPUs.

These configs are the same four presets with **async** shader compilation
(`d3d12_pipeline_creation_threads = -1`), so there's **no hitch**. The trade-off is that
async mode briefly shows cold-cache artifacts (vegetation blowout / flicker / the "red
dog") while shaders compile, so these re-enable the mitigations to keep it minimal:

- `gamma_decode_pwl_resolve = false`
- `resolve_check_number_format = true`

The flicker still tapers off as your shader cache warms up (it's a first-visit thing).

## How to use

Copy the variant you want next to `xenia_canary.exe` and rename it to
`xenia-canary.config.toml` (replacing the existing one).

| Config | Resolution | Sharpening |
|---|---|---|
| `xenia-canary.x2-cas.config.toml` | 2× | CAS |
| `xenia-canary.x1-fsr.config.toml` | Native (720p) | FSR |
| `xenia-canary.x1-cas.config.toml` | Native (720p) | CAS |
| `xenia-canary.x2-fsr.config.toml` | 2× | FSR |
