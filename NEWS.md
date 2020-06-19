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
