#include "parse.h"

#include <assert.h>
#include <locale.h>
#include <math.h>

/// Fuzzy compare.
static bool fcmp(double x, double y)
{
    return fabs(x - y) < 0.000001;
}

/// Parse @c input and expect @c quantity of @c base_unit.
static void test_parse(const wchar_t *input, double quantity, enum base base_unit)
{
    wchar_t *p = NULL;
    struct value v = parse_quantity_and_unit(input, &p);
    assert(fcmp(quantity, v.quantity));
    assert(base_unit == v.base_unit);
}

int main(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");

    {
        wchar_t *p = NULL;
        struct value v = parse_quantity_and_unit(L"  42  ", &p);
        assert(fcmp(v.quantity, 42));
        assert(v.input_unit == PresentationUnitNone);
        assert(v.base_unit == BaseUnitNone);
    }

    {
        wchar_t *p = NULL;
        struct value v = parse_quantity_and_unit(L"  42  foo  ", &p);
        assert(fcmp(v.quantity, 42));
        assert(v.input_unit == PresentationUnitUnknown);
        assert(v.base_unit == BaseUnitNone);
    }

    test_parse(L"1000mm",         1, BaseUnitMetre);
    test_parse(L" 100cm",         1, BaseUnitMetre);
    test_parse(L"   1m",          1, BaseUnitMetre);
    test_parse(L"   0.001km",     1, BaseUnitMetre);
    test_parse(L"   1.093613yd",  1, BaseUnitMetre);
    test_parse(L"   3.280839ft",  1, BaseUnitMetre);
    test_parse(L"   3.280839'\"", 1, BaseUnitMetre);
    test_parse(L"  39.370078in",  1, BaseUnitMetre);

    test_parse(L"   0.621371192mi", 1000, BaseUnitMetre);
    test_parse(L"   1km",           1000, BaseUnitMetre);

    test_parse(L"2000000mg",        2, BaseUnitKilogram);
    test_parse(L"   2000g ",        2, BaseUnitKilogram);
    test_parse(L"      2kg",        2, BaseUnitKilogram);
    test_parse(L"      4.409245lb", 2, BaseUnitKilogram);
    test_parse(L"     70.547931oz", 2, BaseUnitKilogram);

    test_parse(L"300K",     300, BaseUnitKelvin);
    test_parse(L" 26.85°C", 300, BaseUnitKelvin);
    test_parse(L" 80.33°F", 300, BaseUnitKelvin);

    test_parse(L"1000Pa",              1000, DerivedUnitPressurePascal);
    test_parse(L"  10hPa",             1000, DerivedUnitPressurePascal);
    test_parse(L"   1kPa",             1000, DerivedUnitPressurePascal);
    test_parse(L"   7.5006157585mmHg", 1000, DerivedUnitPressurePascal);
    test_parse(L"   0.2952998307inHg", 1000, DerivedUnitPressurePascal);
}
