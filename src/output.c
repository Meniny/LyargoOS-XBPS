#include "output.h"
#include "common.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

static bool color_enabled = true;
static bool verbose_mode = false;

bool is_color_supported(void) {
    // Check if stdout is a terminal
    if (!isatty(STDOUT_FILENO)) {
        return false;
    }
    
    // Check NO_COLOR environment variable
    const char *no_color = getenv("NO_COLOR");
    if (no_color && no_color[0] != '\0') {
        return false;
    }
    
    return true;
}

void output_init_colors(void) {
    color_enabled = is_color_supported();
}

void output_set_verbose(bool verbose) {
    verbose_mode = verbose;
}

bool output_is_verbose(void) {
    return verbose_mode;
}

static const char *colorize(const char *color, const char *text) {
    static char buffer[4096];
    if (!color_enabled) {
        snprintf(buffer, sizeof(buffer), "%s", text);
    } else {
        snprintf(buffer, sizeof(buffer), "%s%s%s", color, text, COLOR_RESET);
    }
    return buffer;
}

void output_info(const char *fmt, ...) {
    va_list args;
    printf("%s ", colorize(COLOR_BLUE, "→"));
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void output_success(const char *fmt, ...) {
    va_list args;
    printf("%s ", colorize(COLOR_GREEN, "✓"));
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void output_warn(const char *fmt, ...) {
    va_list args;
    fprintf(stderr, "%s ", colorize(COLOR_YELLOW, "⚠"));
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void output_error(const char *fmt, ...) {
    va_list args;
    fprintf(stderr, "%s ", colorize(COLOR_RED, "✗"));
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void output_header(const char *fmt, ...) {
    va_list args;
    printf("\n%s", COLOR_BOLD);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("%s\n", COLOR_RESET);
}

void output_item(const char *prefix, const char *name, const char *desc) {
    UNUSED(prefix);
    printf("  %s%-20s%s %s\n", 
           COLOR_CYAN, name, COLOR_RESET, desc ? desc : "");
}

void output_progress(const char *action, const char *target) {
    printf("%s %s %s\n", 
           colorize(COLOR_BLUE, "→"),
           action,
           colorize(COLOR_CYAN, target));
}

void output_command(const char *cmd, char *const argv[]) {
    if (!verbose_mode) return;
    
    fprintf(stderr, "%s", COLOR_DIM);
    fprintf(stderr, "$ %s", cmd);
    for (int i = 1; argv[i]; i++) {
        fprintf(stderr, " %s", argv[i]);
    }
    fprintf(stderr, "%s\n", COLOR_RESET);
}
