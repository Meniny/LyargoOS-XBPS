#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdbool.h>

// Color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_DIM     "\033[2m"

// Output functions
void output_info(const char *fmt, ...);
void output_success(const char *fmt, ...);
void output_warn(const char *fmt, ...);
void output_error(const char *fmt, ...);
void output_header(const char *fmt, ...);
void output_item(const char *prefix, const char *name, const char *desc);
void output_progress(const char *action, const char *target);
void output_command(const char *cmd, char *const argv[]);

// Terminal detection
bool is_color_supported(void);
void output_init_colors(void);

// Verbose mode
void output_set_verbose(bool verbose);
bool output_is_verbose(void);

#endif // OUTPUT_H
