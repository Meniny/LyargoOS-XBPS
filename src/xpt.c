#include "common.h"
#include "output.h"
#include "xbps-wrapper.h"
#include <stdio.h>
#include <string.h>

static void usage(void) {
    printf("xpt - LyargoOS Linux XBPS Package Manager Wrapper (apt-style interface)\n\n");
    printf("Usage: xpt <command> [options] [packages...]\n\n");
    printf("Commands:\n");
    printf("  install <pkg...>     Install packages\n");
    printf("  remove <pkg...>      Remove packages\n");
    printf("  purge <pkg...>       Remove packages and configuration\n");
    printf("  autoremove           Remove unnecessary dependencies\n");
    printf("  reinstall <pkg...>   Reinstall packages\n");
    printf("  search <query>       Search for packages\n");
    printf("  show <pkg>           Show package information\n");
    printf("  list                 List installed packages\n");
    printf("  update               Update repository index\n");
    printf("  upgrade              Upgrade packages\n");
    printf("  sync                 Update and upgrade (shortcut)\n");
    printf("  clean                Clean package cache\n");
    printf("\nOptions:\n");
    printf("  -h, --help           Show this help\n");
    printf("  -V, --version        Show version\n");
    printf("  -v, --verbose        Show executed commands\n");
    printf("  -y, --yes            Assume yes to all questions\n");
}

static void version(void) {
    printf("xpt 0.1.0 - LyargoOS Linux XBPS Package Manager Wrapper (apt-style)\n");
}

int main(int argc, char **argv) {
    // Initialize color support
    output_init_colors();
    
    if (argc < 2) {
        usage();
        return EXIT_FAILURE;
    }
    
    // Check for global flags
    int cmd_start = 1;
    bool yes = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            output_set_verbose(true);
            // Remove this argument from argv
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            i--;
        } else if (strcmp(argv[i], "-y") == 0 || strcmp(argv[i], "--yes") == 0) {
            yes = true;
            // Remove this argument from argv
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            i--;
        }
    }
    
    if (argc < 2) {
        usage();
        return EXIT_FAILURE;
    }
    
    const char *cmd = argv[cmd_start];
    
    // Handle global options
    if (strcmp(cmd, "-h") == 0 || strcmp(cmd, "--help") == 0) {
        usage();
        return EXIT_SUCCESS;
    }
    
    if (strcmp(cmd, "-V") == 0 || strcmp(cmd, "--version") == 0) {
        version();
        return EXIT_SUCCESS;
    }
    
    // Handle commands
    if (strcmp(cmd, "install") == 0) {
        return xbps_install(argc - 2, argv + 2, yes);
    }
    
    if (strcmp(cmd, "remove") == 0) {
        return xbps_remove(argc - 2, argv + 2, false, yes);
    }
    
    if (strcmp(cmd, "purge") == 0) {
        return xbps_remove(argc - 2, argv + 2, true, yes);
    }
    
    if (strcmp(cmd, "autoremove") == 0) {
        return xbps_autoremove(yes);
    }
    
    if (strcmp(cmd, "reinstall") == 0) {
        return xbps_reinstall(argc - 2, argv + 2, yes);
    }
    
    if (strcmp(cmd, "search") == 0) {
        if (argc < 3) {
            output_error("search requires a query");
            return EXIT_FAILURE;
        }
        return xbps_search(argv[2]);
    }
    
    if (strcmp(cmd, "show") == 0) {
        if (argc < 3) {
            output_error("show requires a package name");
            return EXIT_FAILURE;
        }
        return xbps_show(argv[2]);
    }
    
    if (strcmp(cmd, "list") == 0) {
        return xbps_list_installed();
    }
    
    if (strcmp(cmd, "update") == 0) {
        return xbps_update(yes);
    }
    
    if (strcmp(cmd, "upgrade") == 0) {
        return xbps_upgrade(yes);
    }
    
    if (strcmp(cmd, "sync") == 0) {
        return xbps_sync(yes);
    }
    
    if (strcmp(cmd, "clean") == 0) {
        return xbps_clean(yes);
    }
    
    output_error("Unknown command: %s", cmd);
    usage();
    return EXIT_FAILURE;
}
