# Xenia Canary — Fable II Performance & Fixes

A tuned Xenia Canary build focused on running **Fable II** well at scaled
resolution. Below is a brief rundown of what changed and what each setting does.
All settings live in `xenia-canary.config.toml`.

## Performance settings

### CPU / JIT (`[CPU]`, `[x64]`)
| Setting | Does |
|---|---|
| `inline_loadclock = true` | Reads the guest timebase inline instead of a function call. Big win for games that poll the clock constantly (Fable II). |
| `elide_e0_check = true` | Skips a memory-range check on stack/TLS accesses — trims instructions from nearly every function. |
| `enable_rmw_context_merging = true` | Fuses read-modify-write sequences into single x86 memory-operand instructions. |
| `align_all_basic_blocks = 16` | Aligns loop heads to 16 bytes — faster loops (helps AMD Zen). |
| `disable_prefetch_and_cachecontrol = true` | Drops Xbox 360-specific prefetch/cache hints; modern CPUs prefetch on their own. |

### Windows (`[Win32]`, `[Kernel]`)
| Setting | Does |
|---|---|
| `enable_rdrand_ntdll_patch = true` | Hot-patches ntdll to skip RDRAND in heap randomization — noticeably lower CPU usage. |
| `ignore_thread_priorities = true` | Ignores guest thread priorities so the host scheduler can do better. |

### GPU readback (`[GPU]`) — the main FPS lever at scaled resolution
| Setting | Does |
|---|---|
| `readback_resolve = "deferred"` | Delivers CPU texture readback without draining the GPU to idle at every fence. |
| `readback_resolve_deferred_lazy = true` | Never eagerly blocks for readback — keeps CPU/GPU overlapping. |
| `readback_resolve_downsample_via_scratch = true` | Avoids shared-memory barriers around scaled readback. |
| `readback_resolve_morph_formats_only = true` / `readback_resolve_max_length` / `readback_resolve_only_dest_bases` | Restrict readback to only the small textures the CPU actually reads (Fable II morphs), skipping the many resolves it never touches. |

### GPU rendering (`[GPU]`)
| Setting | Does |
|---|---|
| `native_2x_msaa = true` | Uses real 2x MSAA instead of emulating it via 4x (less memory). |
| `native_stencil_value_output = true` | Cheaper render-target stencil transfers. |
| `gamma_render_target_as_unorm16 = false` | Cheaper gamma render-target format (less bandwidth) for a slight blend-accuracy trade. |

### Display (`[Display]`)
| Setting | Does |
|---|---|
| `framerate_limit = 165` | Caps FPS to the display refresh. Bounds how far the CPU runs ahead — fixes "high FPS but floaty/laggy camera." Set this to **your** monitor's refresh rate. |
| `postprocess_scaling_and_sharpening` | `cas` = sharpen only (best when already rendering at your target res), `fsr` = upscale + sharpen (best when rendering below target res). |
| `draw_resolution_scale_x/y` | Internal render resolution multiplier (1 = native 720p, 2 = 2x). |

## Fable II fixes

| Setting / change | Fixes |
|---|---|
| `readback_resolve_only_dest_bases = "0x12700000-0x127fffff"` | **Black dog / character morph textures** at scaled resolution — reads back exactly the morph memory region. (Game-version specific; find yours with `log_resolve_readback` if a morph goes black.) |
| `debug_skip_vs_hash` | Skips the dog's glitchy "explode" draws (bad CPU-supplied skinning data). |
| `debug_skip_ps_hash` + `gpu_clamp_fp10_edram_output = true` | **Moon-halo hard-disc bug** — an FP10 HDR render-target blowout. The draw is skipped and FP10 output is clamped. |
| `guard_indirect_call_targets = true` | Turns a **combat hard-crash** (call through a bad function pointer) into a safely-skipped call. |
| `deadlock_event_watchdog = true` | Recovers from a **worker-pool freeze** (a lost event wakeup) instead of a permanent softlock. |
| `cpu_starvation_mitigation = true` | Keeps the game's logic thread alive when the host CPU is busy — fixes **NPCs/characters freezing** while the world keeps running. |

## Config presets

Ready-made configs are in [`configs/`](configs/). To use one, copy it over
`build/bin/Windows/Release/xenia-canary.config.toml` (rename it to that).

| Preset | Resolution | Sharpening | Best for |
|---|---|---|---|
| `xenia-canary.x1-cas.config.toml` | Native (720p) | CAS | Lowest GPU load, crisp at native. |
| `xenia-canary.x1-fsr.config.toml` | Native (720p) | FSR | Native render upscaled to your display. |
| `xenia-canary.x2-cas.config.toml` | 2x | CAS | **Recommended** — sharp 2x with low overhead. |
| `xenia-canary.x2-fsr.config.toml` | 2x | FSR | 2x with FSR sharpening (FSR mostly sharpens here since 2x is already high-res). |

All presets share the same performance settings and fixes above — only the
render scale and sharpener differ.
