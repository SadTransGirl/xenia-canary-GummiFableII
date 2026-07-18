<h1 align="center">Xenia Canary — Fable II (GummiFableII build)</h1>

A tuned fork of [Xenia Canary](https://github.com/xenia-canary/xenia-canary)
focused on running **Fable II** smoothly at scaled resolution. It ships with
performance tuning, Fable II-specific bug fixes, ready-made config presets, and
the game patches already set up.

> This is an unofficial personal build. For general Xenia usage, the
> [Xenia Canary wiki](https://github.com/xenia-canary/xenia-canary/wiki) still
> applies.

---
### Gummi's Disclaimer:

## This was a pure passion project, Something for my mental health more than anything.

## This isn't a replacement or anything like that for xenia or the WIP Recomp.

## If any of the Xenia Dev's have issues with this just let me know and I will take it down. No issues.

## I just wanted to have a more enjoyable version of Fable-II.

## I used GOTY no TU1. So other versions might not work.


### Enjoy. -Gummi.


## What's in this build

- **Performance tuning** — CPU/JIT, Windows, and GPU-readback settings tuned so
  Fable II holds framerate at 2x resolution.
- **Fable II fixes** — black-dog/morph textures, the moon-halo disc bug, a combat
  hard-crash, a worker-pool freeze, and NPC-freeze mitigations.
- **Config presets** — native/2x resolution × CAS/FSR sharpening (see below).
- **Patches** — Fable II (GOTY / Platinum Edition) patches in `patches/`.

A full, setting-by-setting explanation of every change is in
[CHANGES.md](CHANGES.md). Read that if you want to know *why* each setting is on.

---

## Requirements

- Windows 10/11, 64-bit.
- A GPU with Direct3D 12 (or Vulkan) support.
- **Your own legally-dumped copy of Fable II.** No game files are included here,
  and none will be provided — see the disclaimer at the bottom.

---

## Getting it running

1. Download the release `.zip` from the
   [Releases](https://github.com/SadTransGirl/xenia-canary-GummiFableII/releases)
   page.
2. Extract it to a folder of its own (e.g. `C:\Games\XeniaFableII\`). You should
   have:
   - `xenia_canary.exe`
   - `xenia-canary.config.toml`  ← the active config (2x + CAS by default)
   - `xenia-canary.x1-fsr.config.toml`  ← alternate preset (native + FSR)
   - `patches\`  ← Fable II patches, already in place
3. Run **`xenia_canary.exe`** and open your Fable II dump
   (**File → Open**, or drag the game onto the window).

That's it — the included config and patches are picked up automatically.


## Choosing a config preset

Presets live in [`configs/`](configs/). **To switch presets, copy the one you
want next to `xenia_canary.exe` and rename it to `xenia-canary.config.toml`**
(replace the existing one). Xenia loads that file on startup.

| Preset | Render resolution | Sharpening | Best for |
|---|---|---|---|
| `xenia-canary.x2-cas.config.toml` | 2× | CAS | **Recommended / default** — sharp 2× with low overhead. |
| `xenia-canary.x1-fsr.config.toml` | Native (720p) | FSR | Native render upscaled to your display; lighter GPU load. |

Both presets share the same performance settings and fixes — only the render
scale and the sharpener differ. The default `xenia-canary.config.toml` in the
release is the x2-cas preset.

---

## Patches

The `patches/` folder contains the Fable II patch files:

- `4D5307F1 - Fable II (GOTY_Platinum Edition).patch.toml`
- `4D5307F1 - Fable II (GOTY_Platinum Edition, TU1).patch.toml`

They're enabled by default (`apply_patches = true` in the config). Keep the
`patches\` folder next to `xenia_canary.exe`.

---

## Troubleshooting

- **A character/dog goes black at 2× res** — this is the morph-readback region,
  which can be game-version specific. Enable `log_resolve_readback` in the config
  to find your version's address range, then set `readback_resolve_only_dest_bases`
  accordingly. Details in [CHANGES.md](CHANGES.md#fable-ii-fixes).
- **Stutter / low FPS at 2×** — try the `x1-fsr` preset (native render, upscaled).

---

## Credits

- Built on [Xenia Canary](https://github.com/xenia-canary/xenia-canary) and the
  original [Xenia](https://github.com/xenia-project/xenia) project — all credit
  for the emulator itself goes to their contributors.
- Fable II tuning, fixes, and presets in this fork by
  [SadTransGirl](https://github.com/SadTransGirl).

---

## Disclaimer

The goal of this project is to experiment, research, and educate on the topic of
emulation. **It is not for enabling illegal activity.** You must own a legal copy
of any game you run. No game files, keys, or copyrighted material are distributed
with this build.
