#include "unit.h"

#include <stdbool.h>
#include <wchar.h>

/// Parse unit label in @c s.
/// @return unit
/// @return @c p is updated to point to tail of @c s after the matching unit label.
enum unit parse_unit_label(wchar_t *s, wchar_t **p);

struct value {
    /// The parsed unit label.
    enum unit input_unit;

    /// True if @c quantity is valid.
    bool has_quantity;
    double quantity;
    enum base base_unit;
};

/// Parse quantity and unit label in @c s.
/// @return value
struct value parse_quantity_and_unit(const wchar_t *s, wchar_t **p);
