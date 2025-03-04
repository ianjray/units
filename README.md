# units
Unit Conversion

## Example

```shell
$ units "" "1" "2kg" "3 kg cm" "4 foo" "5 foo kg" "6 kg foo"
Missing quantity.
Missing input unit.
Missing output unit.
Cannot convert to 'cm'
Unknown input unit 'foo'.
Unknown input unit 'foo kg'.
Unknown output unit 'foo'.

$ units "16 km miles" "20 kilograms lb" "373 K 'C" "101325 Pa mmHg" "42 ° rad"
16 km is 9.94194 mi
20 kg is 44.0925 lb
373 K is 99.85 °C
101325 Pa is 760 mmHg
42 ° is 0.733038 rad
```

# Code Generation Notes

A macro file [unit.hi](unit.hi) is used to describe units and the relationship to base units.

Files [unit.head.c](unit.head.c) and [unit.body.c](unit.body.c) are preprocessed and used to create `unit.c`.
This approach is used to make code coverage checking work nicely with the macro expansions.
