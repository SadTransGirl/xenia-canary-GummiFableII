<h1 align="center">Xenia Canary — Fable II (GummiFableII build)</h1>

A tuned fork of [Xenia Canary](https://github.com/xenia-canary/xenia-canary)
focused on running **Fable II** smoothly at scaled resolution. It ships with
performance tuning, Fable II-specific bug fixes, ready-made config presets, and
the game patches already set up.

> This is an unofficial personal build. For general Xenia usage, the
> [Xenia Canary wiki](https://github.com/xenia-canary/xenia-canary/wiki) still
> applies.

Discord For Help/Support: https://discord.gg/3zE3Ey2TH7
### There seems to be some confusion about this build, I made this for pure testing purposes. This so we have something more stable to test with while work happens on the recomp. THIS ISN'T A REPLACEMENT IN ANYWAY. -Gummi.

### VVV Please read my disclaimer below. Thank you. VVV
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

## Setup

### Step 1 — Download & extract

1. Download the release `.zip` from the
   [Releases](https://github.com/SadTransGirl/xenia-canary-GummiFableII/releases) page.
2. **Extract it into its own empty folder** (e.g. `C:\Games\XeniaFableII\`) — don't
   run it from inside the zip, and don't drop it in `Downloads` next to other files.

After extracting you should see:

```
XeniaFableII\
├─ xenia_canary.exe              ← the emulator
├─ xenia-canary.config.toml      ← active config (2× + CAS — used by default)
├─ xenia-canary.x1-fsr.config.toml   ← alternate preset (native 720p + FSR)
├─ patches\                      ← Fable II patches (already set up)
└─ low-end-cpu\                  ← fallback configs for weaker CPUs
```

### Step 2 — Launch and load your game

1. Run **`xenia_canary.exe`**.
2. **File → Open** your Fable II dump (or drag the game file onto the window).

That's the whole setup — the config and patches load automatically. You do **not**
need to configure anything for a normal playthrough.

> **A brief pause the first time you enter an area is normal** — that's shaders
> compiling. On the default config it's a quick hitch/pause; on the `low-end-cpu`
> configs it's a brief flicker instead. Either way it clears within a moment and won't
> happen in that spot again. If the hitching is too much on a weaker CPU, see
> [Which config should I use?](#which-config-should-i-use) below.

### Which config should I use?

The default (`xenia-canary.config.toml`) is right for most people. Only swap if one of
these fits you better:

| Your situation | Use | 
|---|---|
| **Most PCs** — just want it to work | Keep the default (2× + CAS) |
| **Weaker/older GPU**, or slowdown in crowds | `xenia-canary.x1-fsr.config.toml` (native 720p + FSR) |
| **Weaker CPU** — the game hitches/stutters | a config from `low-end-cpu\` (see its README) |

**How to swap a config:** copy the one you want next to `xenia_canary.exe`, delete the
old `xenia-canary.config.toml`, and **rename your chosen file to
`xenia-canary.config.toml`**. Xenia loads that file on startup. (Advanced: you can also
launch with `xenia_canary.exe --config "some-other.config.toml"` to test without
renaming.)

---

## Config presets (reference)

All presets live in [`configs/`](configs/). Full table:

| Preset | Render resolution | Sharpening | Best for |
|---|---|---|---|
| `xenia-canary.x2-cas.config.toml` | 2× | CAS | **Recommended / default** — sharp 2× with low overhead. |
| `xenia-canary.x1-fsr.config.toml` | Native (720p) | FSR | Native render upscaled to your display; lighter GPU load. |

Both presets share the same performance settings and fixes — only the render
scale and the sharpener differ. The default `xenia-canary.config.toml` in the
release is the x2-cas preset.

### Shader compilation (why these presets look clean)

These presets compile shaders **synchronously** (`d3d12_pipeline_creation_threads = 0`).
That's the single setting that fixes the cold-cache artifacts Fable II is prone to —
vegetation blowout, the "red dog", and first-launch flicker. In the default (async)
mode Xenia *skips* draws whose shader is still compiling, which briefly reveals the
raw render buffer (that's the flicker/blowout). Synchronous compile waits instead, so
every frame is correct. On a capable CPU the wait is invisible.

### Low-end CPUs

Synchronous compile can cause a brief **hitch/stutter** the first time a new shader
appears, which is more noticeable on weaker CPUs. If that bothers you, use the configs
in [`configs/low-end-cpu/`](configs/low-end-cpu/) — same presets, but with async shader
compilation (no hitching) plus the gamma/number-format workarounds re-enabled to keep
the async flicker to a minimum. Swap one in the same way (copy over
`xenia-canary.config.toml`). The flicker still tapers off as your shader cache warms up.

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
- **Flickering on the ground / surfaces** — make sure
  `readback_resolve_drain_on_fence = true` in your config. It's `true` in all of
  these presets by default. Setting it `false` trades a little more performance
  for the risk of stale resolved data, which shows up as surface flicker in
  Fable II — leave it `true` unless you know you need the extra speed.
- **Stutter / low FPS at 2×** — try the `x1-fsr` preset (native render, upscaled).
- **Hitching when new things appear (weaker CPUs)** — that's synchronous shader
  compilation. Switch to a [`configs/low-end-cpu/`](configs/low-end-cpu/) preset (async).
- **Brief flicker/blowout on first entering an area (async / low-end presets only)** —
  cold shader cache; it clears as those shaders compile and won't happen there again.
- **Slowdown with lots of NPCs on screen** — usually GPU-bound at 2×. Drop to the
  `x1-fsr` preset (native render) for a large GPU saving in crowd scenes.

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
