double unit_to_base_unit(double quantity, enum unit unit, enum base *base_unit)
{
    double X = quantity;

    switch (unit) {
        case PresentationUnitNone:
        case PresentationUnitUnknown:
            *base_unit = BaseUnitNone;
            break;

#define u(symbol, name, base, scale)            case name : *base_unit = base ; X *= scale  ; break ;
#define c(symbol, name, base, tobase, frombase) case name : *base_unit = base ; X  = tobase ; break ;
#include "unit.hi"
    }

    return X;
}

const wchar_t *symbol_of_unit(enum unit unit)
{
    switch (unit) {
        case PresentationUnitNone:
        case PresentationUnitUnknown:
            break;

#define u(symbol, name, base, scale)            case name : return symbol ;
#define c(symbol, name, base, tobase, frombase) case name : return symbol ;
#include "unit.hi"
    }

    return NULL;
}

char *base_unit_render(double quantity, enum base base_unit)
{
    char *s = NULL;

    switch (base_unit) {
        case BaseUnitNone:
            break;

#define b(symbol, name) case name : asprintf(&s, "%g %ls", quantity, symbol) ; break ;
#include "unit.hi"
    }

    return s;
}

bool can_render_base_unit_as(enum base base_unit, enum unit unit)
{
    switch (unit) {
        case PresentationUnitNone:
        case PresentationUnitUnknown:
            break;

#define u(symbol, name, base, scale)            case name : return base_unit == base;
#define c(symbol, name, base, tobase, frombase) case name : return base_unit == base;
#include "unit.hi"
    }

    return false;
}

char *base_unit_render_as(double quantity, enum base base_unit, enum unit unit)
{
    char *s = NULL;

    double X = quantity;

    switch (unit) {
        case PresentationUnitNone:
        case PresentationUnitUnknown:
            break;

#define u(symbol, name, base, scale)            case name : if (base_unit == base) { X /= scale    ; asprintf(&s, "%g %ls", X, symbol); } break ;
#define c(symbol, name, base, tobase, frombase) case name : if (base_unit == base) { X  = frombase ; asprintf(&s, "%g %ls", X, symbol); } break ;
#include "unit.hi"
    }

    if (unit == PresentationUnitFeetAndInches) {
        // Exception.
        const double ScaleFractionalFeetToInch = 12;
        double y = fmod(X, 1) * ScaleFractionalFeetToInch;
        free(s);
        asprintf(&s, "%ld ' %g \"", (long)X, y);
    }

    return s;
}
