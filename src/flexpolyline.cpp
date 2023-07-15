#include "decoder.h"
#include "encoder.h"

#include <Rcpp.h>

using Rcpp::CharacterVector;
using Rcpp::NumericMatrix;
using Rcpp::String;

//' Decode a flexible polyline encoded string
//'
//' This function calls \code{hf::polyline_decode} and
//' \code{hf::get_third_dimension} of the C++ implementation of the flexible
//' polyline encoding by HERE. Depending on the dimensions of the encoded line,
//' a two or three-dimensional line is decoded.
//'
//' @param encoded character, encoded flexible polyline string.
//'
//' @return
//' A matrix containing the coordinates of the decoded line.
//'
//' @export
//'
//' @examples
//' # 2d line
//' decode("BFoz5xJ67i1B1B7PzIhaxL7Y")
//'
//' # 3d line
//' decode("BlBoz5xJ67i1BU1B7PUzIhaUxL7YU")
// [[Rcpp::export]]
NumericMatrix decode(SEXP encoded) {
    Decoder decoder;
    return decoder.decode(encoded);
}

//' Encode a line in the flexible polyline encoding format
//'
//' This function calls \code{hf::polyline_encode} of the C++ implementation of
//' the flexible polyline encoding by HERE. Depending on the dimensions of the
//' input coordinates, a two or three-dimensional line is encoded.
//'
//' @param line matrix, coordinates of the line in 2d or 3d (column order: LNG,
//' LAT, DIM3).
//' @param precision integer, precision to use in encoding (between 0 and 15,
//' \code{default=5}).
//' @param third_dim integer, type of the third dimension (0: ABSENT, 1: LEVEL,
//' 2: ALTITUDE, 3: ELEVATION, 4, 6: CUSTOM1, 7: CUSTOM2, \code{default=3}).
//' @param third_dim_precision integer, precision to use in encoding for the
//' third dimension (between 1 and 15, \code{default=5}).
//'
//' @return
//' The line as string in the flexible polyline encoding format.
//'
//' @export
//'
//' @examples
//' # 2D
//' line2d <- matrix(
//'   c(8.69821, 50.10228,
//'     8.69567, 50.10201,
//'     8.69150, 50.10063,
//'     8.68752, 50.09878),
//'   ncol = 2, byrow = TRUE
//' )
//' encode(line2d)
//'
//' # 3D
//' line3d <- matrix(
//'   c(8.69821, 50.10228, 10,
//'     8.69567, 50.10201, 20,
//'     8.69150, 50.10063, 30,
//'     8.68752, 50.09878, 40),
//'   ncol = 3, byrow = TRUE
//' )
//' encode(line3d)
// [[Rcpp::export]]
String encode(NumericMatrix line, int precision = 5, int third_dim = 3,
              int third_dim_precision = 5) {
    Encoder encoder(precision, third_dim_precision, third_dim);
    return encoder.encode(line);
}

//' Get third dimension of a flexible polyline encoded string
//'
//' This function calls \code{hf::get_third_dimension} of the C++ implementation
//' of the flexible polyline encoding by HERE and return the type of the third
//' dimension.
//'
//' @param encoded character, encoded flexible polyline string.
//'
//' @return
//' A string describing the third dimension.
//'
//' @export
//'
//' @examples
//' # 2d line
//' get_third_dimension("BFoz5xJ67i1B1B7PzIhaxL7Y")
//'
//' # 3d line
//' get_third_dimension("BlBoz5xJ67i1BU1B7PUzIhaUxL7YU")
// [[Rcpp::export]]
String get_third_dimension(SEXP encoded) {
    Decoder decoder;
    return decoder.get_third_dimension(encoded);
}
