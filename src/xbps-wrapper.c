#include "xbps-wrapper.h"
#include "output.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>

// Helper: execute command and return exit status
static int exec_cmd(const char *cmd, char *const argv[]) {
    // Show the command being executed (in verbose mode)
    output_command(cmd, argv);
    
    pid_t pid = fork();
    if (pid == -1) {
        output_error("Failed to fork process");
        return EXIT_FAILURE;
    }
    
    if (pid == 0) {
        // Child process - execvp will inherit stdin/stdout/stderr
        execvp(cmd, argv);
        // If execvp returns, it failed
        perror(cmd);
        exit(EXIT_FAILURE);
    }
    
    // Parent process
    int status;
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    
    return EXIT_FAILURE;
}

int xbps_install(int argc, char **argv, bool yes) {
    if (argc < 1) {
        output_error("No packages specified");
        return EXIT_FAILURE;
    }
    
    // Build: xbps-install [-y] pkg1 pkg2 ...
    int arg_count = yes ? 3 : 2;
    char **cmd_argv = malloc((arg_count + argc) * sizeof(char *));
    cmd_argv[0] = "xbps-install";
    int idx = 1;
    if (yes) {
        cmd_argv[idx++] = "-y";
    }
    for (int i = 0; i < argc; i++) {
        cmd_argv[idx + i] = argv[i];
    }
    cmd_argv[idx + argc] = NULL;
    
    int ret = exec_cmd("xbps-install", cmd_argv);
    free(cmd_argv);
    
    return ret;
}

int xbps_remove(int argc, char **argv, bool purge, bool yes) {
    if (argc < 1) {
        output_error("No packages specified");
        return EXIT_FAILURE;
    }
    
    // Build: xbps-remove [-y] -R [-Rn] pkg1 pkg2 ...
    int base_args = purge ? 2 : 1;
    int arg_count = yes ? (base_args + 1) : base_args;
    char **cmd_argv = malloc((arg_count + argc + 1) * sizeof(char *));
    cmd_argv[0] = "xbps-remove";
    int idx = 1;
    if (yes) {
        cmd_argv[idx++] = "-y";
    }
    cmd_argv[idx++] = purge ? "-Rn" : "-R";
    for (int i = 0; i < argc; i++) {
        cmd_argv[idx + i] = argv[i];
    }
    cmd_argv[idx + argc] = NULL;
    
    int ret = exec_cmd("xbps-remove", cmd_argv);
    free(cmd_argv);
    
    return ret;
}

int xbps_reinstall(int argc, char **argv, bool yes) {
    if (argc < 1) {
        output_error("No packages specified");
        return EXIT_FAILURE;
    }
    
    // Build: xbps-install [-y] -f pkg1 pkg2 ...
    int arg_count = yes ? 3 : 2;
    char **cmd_argv = malloc((arg_count + argc) * sizeof(char *));
    cmd_argv[0] = "xbps-install";
    int idx = 1;
    if (yes) {
        cmd_argv[idx++] = "-y";
    }
    cmd_argv[idx++] = "-f";
    for (int i = 0; i < argc; i++) {
        cmd_argv[idx + i] = argv[i];
    }
    cmd_argv[idx + argc] = NULL;
    
    int ret = exec_cmd("xbps-install", cmd_argv);
    free(cmd_argv);
    
    return ret;
}

int xbps_autoremove(bool yes) {
    // Build: xbps-remove [-y] -o
    if (yes) {
        char *cmd_argv[] = {"xbps-remove", "-y", "-o", NULL};
        return exec_cmd("xbps-remove", cmd_argv);
    } else {
        char *cmd_argv[] = {"xbps-remove", "-o", NULL};
        return exec_cmd("xbps-remove", cmd_argv);
    }
}

int xbps_search(const char *query) {
    if (!query || !query[0]) {
        output_error("No search query specified");
        return EXIT_FAILURE;
    }
    
    char *cmd_argv[] = {"xbps-query", "-Rs", (char *)query, NULL};
    return exec_cmd("xbps-query", cmd_argv);
}

int xbps_show(const char *pkgname) {
    if (!pkgname || !pkgname[0]) {
        output_error("No package name specified");
        return EXIT_FAILURE;
    }
    
    char *cmd_argv[] = {"xbps-query", "-R", (char *)pkgname, NULL};
    return exec_cmd("xbps-query", cmd_argv);
}

int xbps_list_installed(void) {
    char *cmd_argv[] = {"xbps-query", "-l", NULL};
    return exec_cmd("xbps-query", cmd_argv);
}

int xbps_list_upgradable(void) {
    char *cmd_argv[] = {"xbps-query", "-Su", NULL};
    return exec_cmd("xbps-query", cmd_argv);
}

int xbps_update(bool yes) {
    // Build: xbps-install [-y] -S
    if (yes) {
        char *cmd_argv[] = {"xbps-install", "-y", "-S", NULL};
        return exec_cmd("xbps-install", cmd_argv);
    } else {
        char *cmd_argv[] = {"xbps-install", "-S", NULL};
        return exec_cmd("xbps-install", cmd_argv);
    }
}

int xbps_upgrade(bool yes) {
    // Build: xbps-install [-y] -u
    if (yes) {
        char *cmd_argv[] = {"xbps-install", "-y", "-u", NULL};
        return exec_cmd("xbps-install", cmd_argv);
    } else {
        char *cmd_argv[] = {"xbps-install", "-u", NULL};
        return exec_cmd("xbps-install", cmd_argv);
    }
}

int xbps_sync(bool yes) {
    // Build: xbps-install [-y] -Syu
    if (yes) {
        char *cmd_argv[] = {"xbps-install", "-y", "-Syu", NULL};
        return exec_cmd("xbps-install", cmd_argv);
    } else {
        char *cmd_argv[] = {"xbps-install", "-Syu", NULL};
        return exec_cmd("xbps-install", cmd_argv);
    }
}

int xbps_clean(bool yes) {
    // Build: xbps-remove [-y] -O
    if (yes) {
        char *cmd_argv[] = {"xbps-remove", "-y", "-O", NULL};
        return exec_cmd("xbps-remove", cmd_argv);
    } else {
        char *cmd_argv[] = {"xbps-remove", "-O", NULL};
        return exec_cmd("xbps-remove", cmd_argv);
    }
}

void xbps_free_pkg_info(pkg_info_t *info) {
    if (!info) return;
    free(info->name);
    free(info->version);
    free(info->description);
    free(info->status);
}
