# Precision Build Snapping

F9 toggles snapping; enabled snapping reaches 30 cm for belts and machines.

## Install

Install [Moon Sweepers Mod Loader](https://github.com/swooping-mods/moon-sweepers-mod-loader) first.

Download [precise-build-snapping-0.1.0.msmod](content/precise-build-snapping-0.1.0.msmod), import it with **Install mod package**, then select the mod and its required frameworks.

## Controls

- F9 toggles snapping; enabled snapping reaches 30 cm for belts and machines.
- Implementation candidate; gameplay and co-op testing deferred.

## Compatibility

Version 0.1.0. Targets the pinned Steam Windows x64 IL2CPP build of Moon Sweepers (Steam app 5335510). On Linux, the game runs through Proton. Experimental: back up saves before using save-affecting mods.

## Build

Run `python3 build.py --check` to verify source hashes, then `python3 build.py` to write a build under `.local/build/`. Requires installed Clang, lld-link and llvm-dlltool.

## Credits

See [CREDITS.txt](CREDITS.txt) and [third-party notices](vendored/THIRD_PARTY_NOTICES.md).
