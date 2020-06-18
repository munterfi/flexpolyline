#include <Rcpp.h>
#include <sstream>
#include <vector>
#include <tuple>
#include <iostream>
#include "hf/flexpolyline.h"
using namespace Rcpp;

//' Decode a flexible polyline encoded string
//'
//' This function calls \code{hf::polyline_decode} and \code{hf::get_third_dimension}
//' of the C++ implementation of the flexible polyline encoding by HERE. Depending
//' on the dimensions of the encoded line, a two or three dimensional line is decoded.
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

  // Convert from R SEXP to std::string
  std::string encoded_str = Rcpp::as<std::string>(encoded);

  // Initialize decoder
  std::vector<std::tuple<double, double, double>> polyline;
  auto res = hf::polyline_decode(
    encoded_str, [&polyline](double lat, double lng, double z) {
    polyline.push_back(std::make_tuple(lat, lng, z));
  });

  // Check valid encoding
  if (!res) {
    stop("Invalid encoding.");
  }

  // Extract third dimension type
  const char * dim_name[] = {
    "ABSENT", "LEVEL", "ALTITUDE", "ELEVATION",
    "RESERVED1", "RESERVED2", // Should not be used...
    "CUSTOM1", "CUSTOM2"
  };
  hf::ThirdDim thrd = hf::get_third_dimension(encoded_str);
  int index = static_cast<std::underlying_type<hf::ThirdDim>::type>(thrd);

  // Get line coordinates
  size_t n = polyline.size();
  NumericMatrix coords(n, 2 + !!index);

  if (!!index) {

    // 3d case (index > 0)
    for (size_t i = 0; i < n; ++i) {
      coords( i, 0 ) = std::get<1>(polyline[i]);
      coords( i, 1 ) = std::get<0>(polyline[i]);
      coords( i, 2 ) = std::get<2>(polyline[i]);
    }
    colnames(coords) = CharacterVector({"LNG", "LAT", dim_name[index]});

  } else {

    // 2d case, third dimension ABSENT (index == 0)
    for (size_t i = 0; i < n; ++i) {
      coords( i, 0 ) = std::get<1>(polyline[i]);
      coords( i, 1 ) = std::get<0>(polyline[i]);
    }
    colnames(coords) = CharacterVector({"LNG", "LAT"});

  }

  return coords;

}

//' Encode a line in the flexible polyline encoding format
//'
//' This function calls \code{hf::polyline_encode} of the C++ implementation of
//' the flexible polyline encoding by HERE. Depending on the dimensions of the
//' input coordinates, a two or three dimensional line is encoded.
//'
//' @param line matrix, coordinates of the line in 2d or 3d (column order: LNG, LAT, DIM3).
//' @param precision integer, precision to use in encoding (between 0 and 15, \code{default=5}).
//' @param third_dim integer, type of the third dimension (0: ABSENT, 1: LEVEL, 2: ALTITUDE, 3: ELEVATION, 4, 6: CUSTOM1, 7: CUSTOM2, \code{default=3}).
//' @param third_dim_precision integer, precision to use in encoding for the third dimension (between 1 and 15, \code{default=5}).
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
String encode(NumericMatrix line, int precision = 5,
              int third_dim = 3, int third_dim_precision = 5) {

  String encoded;
  size_t n = line.rows();

  if (line.cols() == 2) {

    // 2d case: Set third dimension to ABSENT and third dimension precision to 0
    std::vector<std::pair<double, double>> input;
    for (size_t i = 0; i < n; ++i) {
      input.push_back(
        std::make_pair(line( i, 1 ), line( i, 0 ))
      );
    }
    encoded = hf::polyline_encode(input, precision, hf::ThirdDim::ABSENT, 0);

  } else if (line.cols() == 3) {

    // 3d case: Use third dimension with third dimension precision
    std::vector<std::tuple<double, double, double>> input;
    for (size_t i = 0; i < n; ++i) {
      input.push_back(
        std::make_tuple(line( i, 1 ), line( i, 0 ), line( i, 2 ))
      );
    }
    encoded = hf::polyline_encode(
      input, precision, static_cast<hf::ThirdDim>(third_dim), third_dim_precision
    );

  } else {

    stop("Invalid input dimensions.");

  }

  return encoded;

}

