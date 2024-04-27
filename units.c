#include "parse.h"
#include "unit.h"

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @return wide string of @c arg.
static wchar_t *str_to_wcs(const char *arg)
{
    wchar_t *wcs = NULL;

    if (arg) {
        size_t len = strlen(arg);

        wcs = (wchar_t *)malloc(sizeof(wchar_t) * len);
        if (wcs) {
            size_t ret = mbstowcs(wcs, arg, len);
            if (ret == (size_t)-1) {
                free(wcs);
            }
        }
    } else {
        errno = EINVAL;
    }

    return wcs;
}

/// Convert @c arg of the form "quantity input-unit output-unit".
static void convert(const wchar_t *arg)
{
    wchar_t *p = NULL;

    struct value v = parse_quantity_and_unit(arg, &p);

    if (!v.has_quantity) {
        fprintf(stderr, "Missing quantity.\n");
        return;
    }

    char *input = base_unit_render_as(v.quantity, v.base_unit, v.input_unit);
    if (!input) {
        if (*p) {
            fprintf(stderr, "Unknown input unit '%ls'.\n", p);
        } else {
            fprintf(stderr, "Missing input unit.\n");
        }
        return;
    }

    enum unit output_unit = parse_unit_label(p, &p);

    if (!can_render_base_unit_as(v.base_unit, output_unit)) {
        if (symbol_of_unit(output_unit)) {
            fprintf(stderr, "Cannot convert to '%ls'\n", symbol_of_unit(output_unit));
            free(input);
            return;
        }
    }

    char *output = base_unit_render_as(v.quantity, v.base_unit, output_unit);
    if (!output) {
        if (*p) {
            fprintf(stderr, "Unknown output unit '%ls'.\n", p);
        } else {
            fprintf(stderr, "Missing output unit.\n");
        }
        free(input);
        return;
    }

    printf("%s is %s\n", input, output);
    free(input);
    free(output);
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "en_US.UTF-8");

    while (--argc > 0) {
        wchar_t *arg = str_to_wcs(*++argv);
        if (arg) {
            convert(arg);
            free(arg);
        }
    }
}
