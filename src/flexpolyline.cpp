#include <Rcpp.h>
#include <sstream>
#include <vector>
#include <tuple>
#include <iostream>
#include "hf/flexpolyline.h"
using namespace Rcpp;

// [[Rcpp::export]]
NumericMatrix cpp_decode(String encoded) {

  std::vector<std::tuple<double, double, double>> polyline;
  auto res = hf::polyline_decode(
    encoded, [&polyline](double lat, double lng, double z) {
    polyline.push_back({lat, lng, z});
  });

  if (!res) {
    stop("Invalid encoding.");
  }

  int n = polyline.size();
  NumericMatrix coords( n , 3 );

  for (size_t i = 0; i < n; ++i) {
    double lat = std::get<0>(polyline[i]);
    double lng = std::get<1>(polyline[i]);
    double z = std::get<2>(polyline[i]);
    coords( i, 0 ) = lng;
    coords( i, 1 ) = lat;
    coords( i, 2 ) = z;
  }

  return coords;

}

// [[Rcpp::export]]
String cpp_encode(NumericMatrix line, int precision) {

  String encoded;

  if (line.cols() == 2) {

    std::vector<std::pair<double, double>> input;
    for (size_t i = 0; i < line.rows(); ++i) {
      input.push_back(
        std::make_pair(line( i, 1 ), line( i, 0 ))
      );
    }
    encoded = hf::polyline_encode(input, precision, hf::ThirdDim::ABSENT);

  } else if (line.cols() == 3) {

    std::vector<std::tuple<double, double, double>> input;
    for (size_t i = 0; i < line.rows(); ++i) {
      input.push_back(
        std::make_tuple(line( i, 1 ), line( i, 0 ), line( i, 2 ))
      );
    }
    encoded = hf::polyline_encode(input, precision, hf::ThirdDim::ALTITUDE);

  } else {

    stop("Invalid input dimensions.");

  }

  return encoded;

}

