# Repository Guidelines

## Project Structure & Module Organization
- `src/`: C++17 Qt5 sources. Key modules: `mainwindow.*` (UI, search, shortcuts), `templatemanager.*` (YAML load/save), `clipboardhandler.*` (copy/paste via X11), `keyboardhandler.*` (key handling), `main.cpp` (bootstrap).
- `config/`: default `templates.yaml` shipped and installed to `share/clip-template/`.
- `build/`: out-of-source build artifacts (create locally).
- Top-level: `CMakeLists.txt`, `README.md`, this guide.

## Build, Test, and Development Commands
- Configure: `mkdir -p build && cd build && cmake ..` — generates build files.
- Build: `make -j$(nproc)` — compiles the `clip-template` binary.
- Run (from build): `./clip-template` — launches the app.
- Install (optional): `sudo make install` — installs binary and default config.
- Configure with type: `cmake .. -DCMAKE_BUILD_TYPE=Release` — optimized builds.

## Coding Style & Naming Conventions
- Language: C++17 with Qt5 (AUTOMOC/AUTOUIC enabled).
- Indentation: 4 spaces; braces on new line for functions/classes.
- Naming: Classes `UpperCamelCase` (e.g., `MainWindow`); methods/functions `lowerCamelCase` (e.g., `filterTemplates`); members prefixed `m_` (e.g., `m_templateList`); files lowercase (e.g., `mainwindow.cpp`).
- Includes: prefer local headers with quotes; group Qt/system first, then project headers.
- Formatting: if available, run `clang-format -i src/*.{h,cpp}` (Qt style is preferred); keep diffs minimal and focused.

## Testing Guidelines
- Current repo has no test suite. For new logic, add tests under `tests/` using Qt Test or GoogleTest; wire into CMake and `ctest`.
- Name tests `test_<area>.cpp`; aim for fast, deterministic cases. If UI-heavy, factor logic into testable classes.
- Run: `ctest` from the `build/` directory (after you add tests).

## Commit & Pull Request Guidelines
- Commits: concise, imperative subject; include context in the body. Conventional Commits are welcome (e.g., `feat: add YAML shortcut parsing`).
- PRs: include a clear description, linked issues, reproduction/verification steps, and screenshots or short clips for UI changes.
- Scope: one focused change per PR; update `README.md` or `config/templates.yaml` if behavior or defaults change.

## Security & Configuration Tips
- Config precedence: `~/.config/clip-template/templates.yaml` → `/usr/share/clip-template/templates.yaml` → `./config/templates.yaml`.
- Avoid hardcoding absolute paths; prefer Qt APIs and keep YAML parsing robust against malformed input.
