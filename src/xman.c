#include "common.h"
#include "output.h"
#include "xbps-wrapper.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>

static void usage(void) {
    printf("xman - LyargoOS Linux XBPS Package Manager Wrapper (pacman-style interface)\n\n");
    printf("Usage: xman <operation> [options] [packages...]\n\n");
    printf("Operations:\n");
    printf("  -S, --sync           Install packages\n");
    printf("  -R, --remove         Remove packages\n");
    printf("  -Q, --query          Query installed packages\n");
    printf("  -h, --help           Show this help\n");
    printf("  -V, --version        Show version\n");
    printf("  -v, --verbose        Show executed commands\n");
    printf("      --noconfirm      Skip confirmation prompts\n");
    printf("\nSync options (-S):\n");
    printf("  -s, --search         Search for packages\n");
    printf("  -y, --refresh        Update repository index\n");
    printf("  -u, --sysupgrade     Upgrade packages\n");
    printf("  -f, --force          Reinstall packages\n");
    printf("\nRemove options (-R):\n");
    printf("  -n, --nosave         Remove configuration files\n");
    printf("  -s, --recursive      Remove unnecessary dependencies\n");
    printf("\nQuery options (-Q):\n");
    printf("  -l, --list           List installed packages\n");
    printf("  -i, --info           Show package information\n");
    printf("  -u, --upgrades       List upgradable packages\n");
    printf("\nExamples:\n");
    printf("  xman -S pkg          Install package\n");
    printf("  xman -Sy             Update index\n");
    printf("  xman -Su             Upgrade packages\n");
    printf("  xman -Syu            Sync and upgrade\n");
    printf("  xman -Ss query       Search for package\n");
    printf("  xman -R pkg          Remove package\n");
    printf("  xman -Rn pkg         Remove with config\n");
    printf("  xman -Q              List installed\n");
    printf("  xman -Qi pkg         Show package info\n");
}

static void version(void) {
    printf("xman 0.1.0 - LyargoOS Linux XBPS Package Manager Wrapper (pacman-style)\n");
}

int main(int argc, char **argv) {
    // Initialize color support
    output_init_colors();
    
    if (argc < 2) {
        usage();
        return EXIT_FAILURE;
    }
    
    // Check for global flags
    bool noconfirm = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            output_set_verbose(true);
            // Remove this argument from argv
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            i--;
        } else if (strcmp(argv[i], "--noconfirm") == 0) {
            noconfirm = true;
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
    
    // Parse operation
    char *op = argv[1];
    
    if (strcmp(op, "-h") == 0 || strcmp(op, "--help") == 0) {
        usage();
        return EXIT_SUCCESS;
    }
    
    if (strcmp(op, "-V") == 0 || strcmp(op, "--version") == 0) {
        version();
        return EXIT_SUCCESS;
    }
    
    // Sync operation (-S)
    if (strcmp(op, "-S") == 0 || strcmp(op, "--sync") == 0) {
        bool search = false, refresh = false, upgrade = false, force = false;
        int opt;
        
        optind = 2;
        static struct option long_opts[] = {
            {"search", no_argument, 0, 's'},
            {"refresh", no_argument, 0, 'y'},
            {"sysupgrade", no_argument, 0, 'u'},
            {"force", no_argument, 0, 'f'},
            {0, 0, 0, 0}
        };
        
        while ((opt = getopt_long(argc, argv, "syuf", long_opts, NULL)) != -1) {
            switch (opt) {
                case 's': search = true; break;
                case 'y': refresh = true; break;
                case 'u': upgrade = true; break;
                case 'f': force = true; break;
            }
        }
        
        // Combined operations
        if (refresh && upgrade) {
            return xbps_sync(noconfirm);
        }
        if (refresh) {
            return xbps_update(noconfirm);
        }
        if (upgrade) {
            return xbps_upgrade(noconfirm);
        }
        if (search) {
            if (optind >= argc) {
                output_error("search requires a query");
                return EXIT_FAILURE;
            }
            return xbps_search(argv[optind]);
        }
        if (force) {
            return xbps_reinstall(argc - optind, argv + optind, noconfirm);
        }
        
        // Default: install
        return xbps_install(argc - optind, argv + optind, noconfirm);
    }
    
    // Remove operation (-R)
    if (strcmp(op, "-R") == 0 || strcmp(op, "--remove") == 0) {
        bool nosave = false, recursive = false;
        int opt;
        
        optind = 2;
        static struct option long_opts[] = {
            {"nosave", no_argument, 0, 'n'},
            {"recursive", no_argument, 0, 's'},
            {0, 0, 0, 0}
        };
        
        while ((opt = getopt_long(argc, argv, "ns", long_opts, NULL)) != -1) {
            switch (opt) {
                case 'n': nosave = true; break;
                case 's': recursive = true; break;
            }
        }
        
        if (recursive) {
            return xbps_autoremove(noconfirm);
        }
        
        return xbps_remove(argc - optind, argv + optind, nosave, noconfirm);
    }
    
    // Query operation (-Q)
    if (strcmp(op, "-Q") == 0 || strcmp(op, "--query") == 0) {
        bool list = false, info = false, upgrades = false;
        int opt;
        
        optind = 2;
        static struct option long_opts[] = {
            {"list", no_argument, 0, 'l'},
            {"info", no_argument, 0, 'i'},
            {"upgrades", no_argument, 0, 'u'},
            {0, 0, 0, 0}
        };
        
        while ((opt = getopt_long(argc, argv, "liu", long_opts, NULL)) != -1) {
            switch (opt) {
                case 'l': list = true; break;
                case 'i': info = true; break;
                case 'u': upgrades = true; break;
            }
        }
        
        if (list) {
            return xbps_list_installed();
        }
        if (info) {
            if (optind >= argc) {
                output_error("info requires a package name");
                return EXIT_FAILURE;
            }
            return xbps_show(argv[optind]);
        }
        if (upgrades) {
            return xbps_list_upgradable();
        }
        
        // Default: list
        return xbps_list_installed();
    }
    
    output_error("Unknown operation: %s", op);
    usage();
    return EXIT_FAILURE;
}
