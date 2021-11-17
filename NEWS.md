# flexpolyline 0.2.5

* Changed workflow from git flow to trunc-based development. Removed develop branch. New features are merged directly into `master` and releases are tagged after CRAN has accepted the package submission, which triggers the documentation build.

# flexpolyline 0.2.4

* Adjust URLs to GitHub account due to renaming munterfinger to @munterfi.

# flexpolyline 0.2.3

* Skip encoding and decoding validation tests on CRAN due to minor deviations in the results due to rounding (half up vs. half even) on different platforms (e.g. Apple M1) at high precision values (closes [#49](https://github.com/munterfi/flexpolyline/issues/49))

# flexpolyline 0.2.2

* Fix wrong integer shift resulting in lost bits for precision values greater than 7 (see [heremaps/flexible-polyline#36](https://github.com/heremaps/flexible-polyline/issues/36), closes [#44](https://github.com/munterfi/flexpolyline/issues/44)).
* Fix CRAN note for a specified lazy data statement without data directory (closes [#43](https://github.com/munterfi/flexpolyline/issues/43)).
* Use **styler** package and use `tyler::tidyverse_style()`to format the package.
* Solve **lintr** issues, except from line length issues (limit of 80 characters).

# flexpolyline 0.2.1

* Skip input tests of C++ binding on CRAN, closes #35.
* Added contribution guidelines, code of conduct and issue templates.

# flexpolyline 0.2.0

* Fix clang range-loop-analysis warning on macOS in `flexpolyline.h` (Apple clang version 12.0.0).
* Support for geometry types `"POLYGON"` and `"POINT"` in `encode_sf()` and `decode_sf()`, closes #31.
* Added functions to get (`get_third_dimension()`) and set (`set_third_dimension()`) the third dimension type of a flexible polyline encoded string.
* Sign in to CodeFactor.io and add badge to continuously track code quality.
* Use exception classes when throwing an exception in C++.
* Improve coverage of tests.

# flexpolyline 0.1.1

* Add ORCID to author field in `DESCRIPTION`.
* Limit the encoding check in the C++ binding test to 7 digits.
* Use explicit type casts before left shifting and reassigning (`x <<= y`) to avoid UBSAN runtime error *'left shift of negative value'* in `flexpolyline.h`.

# flexpolyline 0.1.0

First release of the **flexpolyline** package, which provides a binding to the
[C++ implementation](https://github.com/heremaps/flexible-polyline/tree/master/cpp) of the
flexible polyline encoding by [HERE](https://github.com/heremaps/flexible-polyline).
The flexible polyline encoding is a lossy compressed representation of a list of
coordinate pairs or coordinate triples. The encoding is achieved by:

(1) Reducing the decimal digits of each value;
(2) encoding only the offset from the previous point;
(3) using variable length for each coordinate delta; and
(4) using 64 URL-safe characters to display the result.

The felxible polyline encoding is a variant of the [Encoded Polyline Algorithm Format](https://developers.google.com/maps/documentation/utilities/polylinealgorithm) by Google.

**License:**

* The **flexpolyline** R package is licensed under GNU GPL v3.0.
* The C++ implementation by HERE Europe B.V. is licensed under MIT.
