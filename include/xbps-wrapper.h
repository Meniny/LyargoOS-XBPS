#ifndef XBPS_WRAPPER_H
#define XBPS_WRAPPER_H

#include <stdbool.h>

// Package info structure
typedef struct {
    char *name;
    char *version;
    char *description;
    char *status;  // installed, available, etc.
} pkg_info_t;

// Core operations
int xbps_install(int argc, char **argv, bool yes);
int xbps_remove(int argc, char **argv, bool purge, bool yes);
int xbps_reinstall(int argc, char **argv, bool yes);
int xbps_autoremove(bool yes);

// Search and query
int xbps_search(const char *query);
int xbps_show(const char *pkgname);
int xbps_list_installed(void);
int xbps_list_upgradable(void);

// Repository operations
int xbps_update(bool yes);
int xbps_upgrade(bool yes);
int xbps_sync(bool yes);

// Maintenance
int xbps_clean(bool yes);

// Utility
void xbps_free_pkg_info(pkg_info_t *info);

#endif // XBPS_WRAPPER_H
