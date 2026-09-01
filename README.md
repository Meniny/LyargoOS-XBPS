# LyargoOS XBPS Package Tools

Two XBPS package manager frontends providing apt-style and pacman-style command line interfaces.

## Tools

### xpt - apt-style

```bash
xpt install pkg       # Install packages
xpt remove pkg        # Remove packages
xpt purge pkg         # Remove packages and configuration
xpt autoremove        # Remove unnecessary dependencies
xpt reinstall pkg     # Reinstall packages
xpt search query      # Search for packages
xpt show pkg          # Show package information
xpt list              # List installed packages
xpt update            # Update repository index
xpt upgrade           # Upgrade packages
xpt sync              # Update and upgrade (shortcut)
xpt clean             # Clean package cache
```

Options:
- `-v, --verbose` — Show executed xbps commands
- `-y, --yes` — Skip confirmation prompts

### xman - pacman-style

```bash
xman -S pkg           # Install packages
xman -R pkg           # Remove packages
xman -Rn pkg          # Remove packages and configuration
xman -Rs              # Remove unnecessary dependencies
xman -Sf pkg          # Reinstall packages
xman -Ss query        # Search for packages
xman -Qi pkg          # Show package information
xman -Q               # List installed packages
xman -Sy              # Update repository index
xman -Su              # Upgrade packages
xman -Syu             # Sync and upgrade (shortcut)
```

Options:
- `-v, --verbose` — Show executed xbps commands
- `--noconfirm` — Skip confirmation prompts

## Build

```bash
make
```

## Install

```bash
sudo make install
```

Installs to `/usr/local/bin` by default. Override with `PREFIX`:

```bash
sudo make install PREFIX=/usr
```

## Uninstall

```bash
sudo make uninstall
```

## Features

- **Colored output** — Symbol markers (✓ ✗ → ⚠)
- **Transparent** — `-v` shows the actual xbps commands being executed
- **Interactive confirmation** — Passes through xbps native prompts
- Respects `NO_COLOR` environment variable
- Shared core codebase

## Examples

```bash
# Install a package (shows xbps confirmation prompt)
xpt install firefox

# Show the executed command
xpt -v install firefox
# → install firefox
# $ xbps-install firefox
# ... xbps output ...

# Skip confirmation
xpt -y install firefox
xman --noconfirm -S firefox

# Search for a package
xpt search editor
xman -Ss editor

# Sync and upgrade
xpt sync
xman -Syu
```

## Command Mapping

| Action | xpt (apt) | xman (pacman) | XBPS command |
|--------|-----------|---------------|--------------|
| Install | `install` | `-S` | `xbps-install` |
| Remove | `remove` | `-R` | `xbps-remove -R` |
| Purge | `purge` | `-Rn` | `xbps-remove -Rn` |
| Autoremove | `autoremove` | `-Rs` | `xbps-remove -o` |
| Reinstall | `reinstall` | `-Sf` | `xbps-install -f` |
| Search | `search` | `-Ss` | `xbps-query -Rs` |
| Info | `show` | `-Qi` | `xbps-query -R` |
| List | `list` | `-Q` | `xbps-query -l` |
| Update | `update` | `-Sy` | `xbps-install -S` |
| Upgrade | `upgrade` | `-Su` | `xbps-install -u` |
| Sync | `sync` | `-Syu` | `xbps-install -Syu` |
| Clean | `clean` | `-Sc` | `xbps-remove -O` |

## Project Structure

```
lyargoos-xpm/
├── Makefile
├── README.md
├── include/
│   ├── common.h          # Common definitions
│   ├── output.h          # Output formatting
│   └── xbps-wrapper.h    # XBPS wrapper
├── src/
│   ├── output.c          # Color/formatting implementation
│   ├── xbps-wrapper.c    # XBPS command wrapper
│   ├── xpt.c             # apt-style frontend
│   └── xman.c            # pacman-style frontend
└── build/                # Build output (gitignored)
```

## License

Same as the LyargoOS project.
