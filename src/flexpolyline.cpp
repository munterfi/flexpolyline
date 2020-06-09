#include <Rcpp.h>
#include <sstream>
#include <vector>
#include <tuple>
#include <iostream>
#include "hf/flexpolyline.h"
using namespace Rcpp;

// [[Rcpp::export]]
NumericMatrix cpp_decode(SEXP encoded) {

  // Convert from R SEXP to std::string
  std::string encoded_str = Rcpp::as<std::string>(encoded);

  // Initialize decoder
  std::vector<std::tuple<double, double, double>> polyline;
  auto res = hf::polyline_decode(
    encoded_str, [&polyline](double lat, double lng, double z) {
    polyline.push_back({lat, lng, z});
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

// [[Rcpp::export]]
String cpp_encode(NumericMatrix line, int precision,
                  int third_dim, int third_dim_precision) {

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

