# flexpolyline 0.1.1

* Add ORCID to author field in DESCRIPTION.
* Also limit the encoding check in the C++ binding test to 7 digits.
* Use explicit type casts instead of `delta <<= 1'` to avoid UBSAN runtime error 'left shift of negative value' in `flexpolyline.h`.

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
