#pragma once

#include <stdbool.h>
#include <wchar.h>

/// Base units.
/// @see https://en.wikipedia.org/wiki/International_System_of_Units
enum base {
    BaseUnitNone,

#define b(symbol, name) name,
#include "unit.hi"
};

/// Units.
enum unit {
    PresentationUnitNone,
    PresentationUnitUnknown,

#define u(symbol, name, base, scale) name,
#include "unit.hi"
};

/// Convert @c quantity of @c unit to the base unit.
/// @return quantity of @c base_unit.
double unit_to_base_unit(double quantity, enum unit unit, enum base *base_unit);

/// @return Symbol of unit.
const wchar_t *symbol_of_unit(enum unit unit);

/// Render @c quantity of @c base_unit.
/// @return Reference to string, user deallocates.
char *base_unit_render(double quantity, enum base base_unit);

/// @return True if @c base_unit can be rendered as @c unit.
bool can_render_base_unit_as(enum base base_unit, enum unit unit);

/// Render @c quantity of @c base_unit as @c unit.
/// @return Reference to string, user deallocates.
char *base_unit_render_as(double quantity, enum base base_unit, enum unit unit);
