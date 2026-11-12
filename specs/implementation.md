# Precision Build Snapping: behavior and implementation

Describes the retained payload **0.1.0**. The behavior prose describes the retained payload; current candidate source and metadata are identified below. Gameplay qualification is a separate requirement.


<!-- source-only-sync -->
## Current source candidate

Source version **0.1.0** is saved from workshop revision `ca32bbf79ce0840d5166a232bdb3a9fdcd761254`. The retained installable payload remains **0.1.0** with its original bytes. This source update does not rebuild or requalify that payload. The current source and metadata define the candidate; gameplay, rendered behavior and co-op remain unqualified for rebuilt bytes.

The authored behavior and lifecycle descriptions below are retained from the published payload. They are not new qualification evidence for the source candidate. The integration table below follows the current candidate metadata.
<!-- /source-only-sync -->

## Behavior

F9 toggles snapping with a default 30 cm reach for belts and machines. The pinned ConveyorPorts hook clamps the native search radius; disabled snapping clears its endpoint list. SnapMachine runs the original through a recursion guard and rejects candidate origins displaced beyond the allowed reach; when disabled it clears outputs and returns null so native free-placement fallback applies.

## State and lifecycle

The INI supplies enabled state, key and bounded reach. Toggle state is session-local; no new saved structure data is written. Checked native method tokens/entry shapes gate hook installation. Foundation grid and railing controls remain separate native behavior.

## Integration and configuration

No additional feature-mod runtime dependency is declared. Included shared headers are source reuse, not extra DLL dependencies.

The [module metadata](../source/mods/precise-build-snapping/mod.json) owns the version and declared integration contract. Native modules start at `MoonLab_ModStart`. Game/Unity integrations target the pinned Steam Windows x64 IL2CPP build.

Packaged defaults in [settings.ini](../source/mods/precise-build-snapping/settings.ini); supported bounds and interpretation remain in the linked parser.

| Section | Setting | Default |
|---|---|---|
| `[settings]` | `ToggleKey` | `120` |
| `[settings]` | `Enabled` | `1` |
| `[settings]` | `ReachCentimetres` | `30` |

## Implementation sources

- [mod.c](../source/mods/precise-build-snapping/mod.c)
- [config.h](../source/mods/precise-build-snapping/config.h)
- [building.h](../source/mods/precise-build-snapping/building.h)

## Verification boundary

No arbitrary precision placement editor or guarantee for other exclusive snapping hooks. Placement preview, actual port fit and combined construction mods require native-game qualification.

`python3 build.py --check` verifies the exported source hashes. It does not execute the mod or qualify the behaviors above. Installation and independent build prerequisites are in the [README](../README.md).
