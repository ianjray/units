#include "parse.h"
#include "unit.h"

#include <assert.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/// Fuzzy compare.
static bool fcmp(double x, double y)
{
    return fabs(x - y) < 0.000001;
}

/// Test that @c unit_to_base_unit produces expected output.
static void test_unit_to_base_unit(double quantity, enum unit unit, double expected_quantity, enum base expected_base_unit)
{
    enum base base_unit;
    double v = unit_to_base_unit(quantity, unit, &base_unit);
    assert(base_unit == expected_base_unit);
    assert(fcmp(v, expected_quantity));
}

/// Test that @c actual matches @c expected.
static void expect(char *actual, const char *expected)
{
    if (expected == NULL) {
        assert(!actual);
    } else {
        assert(actual && expected);
        assert(!strcmp(actual, expected));
        free(actual);
    }
}

int main(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");

    test_unit_to_base_unit(42, PresentationUnitNone, 42, BaseUnitNone);

    test_unit_to_base_unit(1000, PresentationUnitMillimetre, 1, BaseUnitMetre);
    test_unit_to_base_unit(100, PresentationUnitCentimetre, 1, BaseUnitMetre);
    test_unit_to_base_unit(1, PresentationUnitMetre, 1, BaseUnitMetre);
    test_unit_to_base_unit(1, PresentationUnitKilometre, 1000, BaseUnitMetre);

    test_unit_to_base_unit(1.093613, PresentationUnitYard, 1, BaseUnitMetre);
    test_unit_to_base_unit(3.280839, PresentationUnitFeet, 1, BaseUnitMetre);
    test_unit_to_base_unit(3.280839, PresentationUnitFeetAndInches, 1, BaseUnitMetre);
    test_unit_to_base_unit(39.370078, PresentationUnitInch, 1, BaseUnitMetre);
    test_unit_to_base_unit(0.621371192, PresentationUnitMile, 1000, BaseUnitMetre);

    test_unit_to_base_unit(2000000, PresentationUnitMilligram, 2, BaseUnitKilogram);
    test_unit_to_base_unit(2000, PresentationUnitGram, 2, BaseUnitKilogram);
    test_unit_to_base_unit(2, PresentationUnitKilogram, 2, BaseUnitKilogram);
    test_unit_to_base_unit(4.409245, PresentationUnitPound, 2, BaseUnitKilogram);
    test_unit_to_base_unit(70.547931, PresentationUnitOunce, 2, BaseUnitKilogram);

    test_unit_to_base_unit(300, PresentationUnitKelvin, 300, BaseUnitKelvin);
    test_unit_to_base_unit(26.85, PresentationUnitDegreesCelsius, 300, BaseUnitKelvin);
    test_unit_to_base_unit(80.33, PresentationUnitDegreesFahrenheit, 300, BaseUnitKelvin);

    test_unit_to_base_unit(1000, PresentationUnitPascal, 1000, DerivedUnitPressurePascal);
    test_unit_to_base_unit(10, PresentationUnitHectoPascal, 1000, DerivedUnitPressurePascal);
    test_unit_to_base_unit(1, PresentationUnitKiloPascal, 1000, DerivedUnitPressurePascal);
    test_unit_to_base_unit(7.5006157585, PresentationUnitMillimetreMercury, 1000, DerivedUnitPressurePascal);
    test_unit_to_base_unit(0.2952998307, PresentationUnitInchesMercury, 1000, DerivedUnitPressurePascal);

    test_unit_to_base_unit(M_PI, PresentationUnitRadian, M_PI, DerivedUnitAngleRadian);
    test_unit_to_base_unit(180, PresentationUnitDegree, M_PI, DerivedUnitAngleRadian);

    assert(NULL == symbol_of_unit(PresentationUnitNone));
    assert(NULL == symbol_of_unit(PresentationUnitUnknown));

    assert(!wcscmp(symbol_of_unit(PresentationUnitMillimetre), L"mm"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitCentimetre), L"cm"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitMetre), L"m"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitKilometre), L"km"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitMile), L"mi"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitYard), L"yd"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitFeet), L"ft"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitFeetAndInches), L"'\""));
    assert(!wcscmp(symbol_of_unit(PresentationUnitInch), L"in"));

    assert(!wcscmp(symbol_of_unit(PresentationUnitMilligram), L"mg"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitGram), L"g"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitKilogram), L"kg"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitPound), L"lb"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitOunce), L"oz"));

    assert(!wcscmp(symbol_of_unit(PresentationUnitKelvin), L"K"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitDegreesCelsius), L"°C"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitDegreesFahrenheit), L"°F"));

    assert(!wcscmp(symbol_of_unit(PresentationUnitPascal), L"Pa"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitHectoPascal), L"hPa"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitKiloPascal), L"kPa"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitMillimetreMercury), L"mmHg"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitInchesMercury), L"inHg"));

    assert(!wcscmp(symbol_of_unit(PresentationUnitRadian), L"rad"));
    assert(!wcscmp(symbol_of_unit(PresentationUnitDegree), L"°"));


    expect(base_unit_render(42, BaseUnitNone),              NULL);
    expect(base_unit_render(42, BaseUnitMetre),             "42 m");
    expect(base_unit_render(42, BaseUnitKilogram),          "42 kg");
    expect(base_unit_render(42, BaseUnitKelvin),            "42 K");
    expect(base_unit_render(42, DerivedUnitPressurePascal), "42 Pa");
    expect(base_unit_render(42, DerivedUnitAngleRadian),    "42 rad");


    assert(!can_render_base_unit_as(BaseUnitMetre, PresentationUnitNone));
    assert(!can_render_base_unit_as(BaseUnitMetre, PresentationUnitUnknown));

    assert(!can_render_base_unit_as(BaseUnitMetre, PresentationUnitMilligram));

    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitMillimetre));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitCentimetre));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitMetre));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitKilometre));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitYard));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitFeetAndInches));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitFeet));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitInch));
    assert(can_render_base_unit_as(BaseUnitMetre, PresentationUnitMile));

    assert(!can_render_base_unit_as(BaseUnitKilogram, PresentationUnitMillimetre));

    assert(can_render_base_unit_as(BaseUnitKilogram, PresentationUnitMilligram));
    assert(can_render_base_unit_as(BaseUnitKilogram, PresentationUnitGram));
    assert(can_render_base_unit_as(BaseUnitKilogram, PresentationUnitKilogram));
    assert(can_render_base_unit_as(BaseUnitKilogram, PresentationUnitPound));
    assert(can_render_base_unit_as(BaseUnitKilogram, PresentationUnitOunce));

    assert(!can_render_base_unit_as(BaseUnitKelvin, PresentationUnitPascal));

    assert(can_render_base_unit_as(BaseUnitKelvin, PresentationUnitKelvin));
    assert(can_render_base_unit_as(BaseUnitKelvin, PresentationUnitDegreesCelsius));
    assert(can_render_base_unit_as(BaseUnitKelvin, PresentationUnitDegreesFahrenheit));

    assert(!can_render_base_unit_as(DerivedUnitPressurePascal, PresentationUnitKelvin));

    assert(can_render_base_unit_as(DerivedUnitPressurePascal, PresentationUnitPascal));
    assert(can_render_base_unit_as(DerivedUnitPressurePascal, PresentationUnitHectoPascal));
    assert(can_render_base_unit_as(DerivedUnitPressurePascal, PresentationUnitKiloPascal));
    assert(can_render_base_unit_as(DerivedUnitPressurePascal, PresentationUnitMillimetreMercury));
    assert(can_render_base_unit_as(DerivedUnitPressurePascal, PresentationUnitInchesMercury));

    assert(!can_render_base_unit_as(DerivedUnitAngleRadian, PresentationUnitMetre));

    assert(can_render_base_unit_as(DerivedUnitAngleRadian, PresentationUnitRadian));
    assert(can_render_base_unit_as(DerivedUnitAngleRadian, PresentationUnitDegree));


    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitNone),          NULL);

    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitMillimetre),    "1000 mm");
    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitCentimetre),     "100 cm");
    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitMetre),            "1 m");
    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitKilometre),        "0.001 km");
    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitYard),             "1.09361 yd");
    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitFeetAndInches),    "3 ' 3.37008 \"");
    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitFeet),             "3.28084 ft");
    expect(base_unit_render_as(1, BaseUnitMetre, PresentationUnitInch),            "39.3701 in");

    expect(base_unit_render_as(1000, BaseUnitMetre, PresentationUnitMile),          "0.621371 mi");
    expect(base_unit_render_as(1000, BaseUnitMetre, PresentationUnitKilometre),     "1 km");

    expect(base_unit_render_as(2, BaseUnitKilogram, PresentationUnitMilligram), "2e+06 mg");
    expect(base_unit_render_as(2, BaseUnitKilogram, PresentationUnitGram),       "2000 g");
    expect(base_unit_render_as(2, BaseUnitKilogram, PresentationUnitKilogram),      "2 kg");
    expect(base_unit_render_as(2, BaseUnitKilogram, PresentationUnitPound),         "4.40925 lb");
    expect(base_unit_render_as(2, BaseUnitKilogram, PresentationUnitOunce),        "70.5479 oz");

    expect(base_unit_render_as(300, BaseUnitKelvin, PresentationUnitKelvin),            "300 K");
    expect(base_unit_render_as(300, BaseUnitKelvin, PresentationUnitDegreesCelsius),     "26.85 °C");
    expect(base_unit_render_as(300, BaseUnitKelvin, PresentationUnitDegreesFahrenheit),  "80.33 °F");

    expect(base_unit_render_as(1000, DerivedUnitPressurePascal, PresentationUnitPascal),            "1000 Pa");
    expect(base_unit_render_as(1000, DerivedUnitPressurePascal, PresentationUnitHectoPascal),         "10 hPa");
    expect(base_unit_render_as(1000, DerivedUnitPressurePascal, PresentationUnitKiloPascal),           "1 kPa");
    expect(base_unit_render_as(1000, DerivedUnitPressurePascal, PresentationUnitMillimetreMercury),    "7.50062 mmHg");
    expect(base_unit_render_as(1000, DerivedUnitPressurePascal, PresentationUnitInchesMercury),        "0.2953 inHg");

    expect(base_unit_render_as(3.1415926536, DerivedUnitAngleRadian, PresentationUnitRadian), "3.14159 rad");
    expect(base_unit_render_as(3.1415926536, DerivedUnitAngleRadian, PresentationUnitDegree), "180 °");
}
