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

$ units "16 km miles" "20 kilograms lb" "373 K 'C" "101325 Pa mmHg"
16 km is 9.94194 mi
20 kg is 44.0925 lb
373 K is 99.85 °C
101325 Pa is 760 mmHg

$ units "42 ° rad"
42 ° is 0.733038 rad

$ units "180 lbs kg" "1 t tn" "1 t imperial ton"
180 lb is 81.6466 kg
1 t is 1.10231 tn
1 t is 0.984206 long ton

$ units "80 psi bar"
80 psi is 5.51581 bar
```

# Code Generation Notes

A macro file [unit.hi](unit.hi) is used to describe units and the relationship to base units.

Files [unit.head.c](unit.head.c) and [unit.body.c](unit.body.c) are preprocessed and used to create `unit.c`.
This approach is used to make code coverage checking work nicely with the macro expansions.
