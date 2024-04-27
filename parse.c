#include "parse.h"

#include <errno.h>
#include <string.h>
#include <wctype.h>

struct lookup {
    /// Labels may use American or British English.
    /// Labels may use plurals.
    /// Labels may refer to SI or non-SI units in common use.
    const wchar_t *label;
    /// Unit.
    enum unit unit;
};

/// Lookup table used for parsing user description of a unit.
static struct lookup labels[] = {
    { L"", PresentationUnitNone },

    { L"km", PresentationUnitKilometre },
    { L"kilometre", PresentationUnitKilometre },
    { L"kilometres", PresentationUnitKilometre },
    { L"kilometer", PresentationUnitKilometre },
    { L"kilometers", PresentationUnitKilometre },

    { L"m", PresentationUnitMetre },
    { L"metre", PresentationUnitMetre },
    { L"metres", PresentationUnitMetre },
    { L"meter", PresentationUnitMetre },
    { L"meters", PresentationUnitMetre },

    { L"cm", PresentationUnitCentimetre },
    { L"centimetre", PresentationUnitCentimetre },
    { L"centimetres", PresentationUnitCentimetre },
    { L"centimeter", PresentationUnitCentimetre },
    { L"centimeters", PresentationUnitCentimetre },

    { L"mm", PresentationUnitMillimetre },
    { L"millimetre", PresentationUnitMillimetre },
    { L"millimetres", PresentationUnitMillimetre },
    { L"millimeter", PresentationUnitMillimetre },
    { L"millimeters", PresentationUnitMillimetre },

    // Not SI, but commonly used.
    { L"mi", PresentationUnitMile },
    { L"mile", PresentationUnitMile },
    { L"miles", PresentationUnitMile },

    // Not SI, but commonly used.
    { L"yd", PresentationUnitYard },
    { L"yds", PresentationUnitYard },
    { L"yard", PresentationUnitYard },
    { L"yards", PresentationUnitYard },

    // Not SI, but commonly used.
    { L"'", PresentationUnitFeet },
    { L"ft", PresentationUnitFeet },
    { L"feet", PresentationUnitFeet },
    { L"foot", PresentationUnitFeet },

    // Not SI, but commonly used.
    { L"'\"", PresentationUnitFeetAndInches },

    // Not SI, but commonly used.
    { L"\"", PresentationUnitInch },
    { L"in", PresentationUnitInch },
    { L"inch", PresentationUnitInch },
    { L"inches", PresentationUnitInch },

    { L"kg", PresentationUnitKilogram },
    { L"kilogram", PresentationUnitKilogram },
    { L"kilograms", PresentationUnitKilogram },

    { L"g", PresentationUnitGram },
    { L"gram", PresentationUnitGram },
    { L"grams", PresentationUnitGram },

    { L"mg", PresentationUnitMilligram },
    { L"milligram", PresentationUnitMilligram },
    { L"milligrams", PresentationUnitMilligram },

    // Not SI, but commonly used.
    { L"lb", PresentationUnitPound },
    { L"lbs", PresentationUnitPound },
    { L"pound", PresentationUnitPound },
    { L"pounds", PresentationUnitPound },

    // Not SI, but commonly used.
    { L"oz", PresentationUnitOunce },
    { L"ounce", PresentationUnitOunce },
    { L"ounces", PresentationUnitOunce },

    { L"K", PresentationUnitKelvin },
    // Less common.
    { L"°K", PresentationUnitKelvin },
    { L"Kelvin", PresentationUnitKelvin },
    { L"degrees Kelvin", PresentationUnitKelvin },

    { L"°C", PresentationUnitDegreesCelsius },
    { L"'C", PresentationUnitDegreesCelsius },
    { L"°Celsius", PresentationUnitDegreesCelsius },
    { L"'Celsius", PresentationUnitDegreesCelsius },
    { L"degrees Celsius", PresentationUnitDegreesCelsius },

    { L"°F", PresentationUnitDegreesFahrenheit },
    { L"'F", PresentationUnitDegreesFahrenheit },
    { L"°Fahrenheit", PresentationUnitDegreesFahrenheit },
    { L"'Fahrenheit", PresentationUnitDegreesFahrenheit },
    { L"degrees Fahrenheit", PresentationUnitDegreesFahrenheit },

    { L"Pa", PresentationUnitPascal },
    { L"pascal", PresentationUnitPascal },

    { L"hPa", PresentationUnitHectoPascal },
    { L"hectopascal", PresentationUnitHectoPascal },
    { L"hectopascals", PresentationUnitHectoPascal },

    { L"kPa", PresentationUnitKiloPascal },
    { L"kilopascal", PresentationUnitKiloPascal },
    { L"kilopascals", PresentationUnitKiloPascal },

    { L"mmHg", PresentationUnitMillimetreMercury },
    { L"millimeter of mercury", PresentationUnitMillimetreMercury },
    { L"millimeters of mercury", PresentationUnitMillimetreMercury },

    { L"inHg", PresentationUnitInchesMercury },
    { L"in Hg", PresentationUnitInchesMercury },
    { L"\"Hg", PresentationUnitInchesMercury },
    { L"\" Hg", PresentationUnitInchesMercury },
    { L"inch of mercury", PresentationUnitInchesMercury },
    { L"inches of mercury", PresentationUnitInchesMercury },
};

/// @return True if end of word.
static bool is_eow(const wchar_t *s)
{
    return !s || !*s || iswspace(*s) || iswdigit(*s);
}

/// @return True if string @c s matches, or begins with, string @c w.
static bool accept_word(wchar_t *s, const wchar_t *w, wchar_t **p)
{
    if (!wcsncmp(s, w, wcslen(w))) {
        s += wcslen(w);

        if (is_eow(s)) {
            if (p) {
                *p = s;
            }
            return true;
        }
    }

    return false;
}

enum unit parse_unit_label(wchar_t *s, wchar_t **p)
{
    // Skip whitespace.
    while (iswspace(*s)) {
        s++;
    }

    *p = s;

    if (!*s) {
        return PresentationUnitNone;
    }

    size_t candidate = 0;
    size_t candidates = 0;
    size_t length = 0;

    for (size_t i = 0; i < sizeof(labels) / sizeof(*labels); ++i) {
        if (accept_word(s, labels[i].label, NULL)) {
            // Find longest match.
            if (wcslen(labels[i].label) > length) {
                candidate = i;
                candidates++;;
                length = wcslen(labels[i].label);
            }
        }
    }

    if (candidates) {
        accept_word(s, labels[candidate].label, p);
        return labels[candidate].unit;
    }

    return PresentationUnitUnknown;
}

struct value parse_quantity_and_unit(const wchar_t *s, wchar_t **p)
{
    struct value v;
    memset(&v, 0, sizeof(v));
    v.quantity = wcstod(s, p);

     if (*p != s) {
        v.has_quantity = true;

        v.input_unit = parse_unit_label(*p, p);

        v.quantity = unit_to_base_unit(v.quantity, v.input_unit, &v.base_unit);

        if (v.input_unit == PresentationUnitFeet) {
            // Exceptionally, we make a second conversion for potential inches.
            struct value w = parse_quantity_and_unit(*p, p);

            if (w.input_unit == PresentationUnitNone) {
                // Assume that Y in X'Y is inches.
                w.input_unit = PresentationUnitInch;
                w.quantity = unit_to_base_unit(w.quantity, PresentationUnitInch, &w.base_unit);
            }

            if (w.input_unit == PresentationUnitInch) {
                v.quantity += w.quantity;
            }
        }
    }

    return v;
}
