#include <Rcpp.h>
#include <sstream>
#include <vector>
#include <tuple>
#include <iostream>
#include "hf/flexpolyline.h"
using namespace Rcpp;

// Stop if not TRUE
void assert_true(bool value) {
  if (!value) {
    stop("Assert failed.");
  }
}

// [[Rcpp::export]]
NumericMatrix cpp_decode(String encoded) {

  std::vector<std::tuple<double, double, double>> polyline;
  auto res = hf::polyline_decode(encoded, [&polyline](double lat, double lng, double z) {
    polyline.push_back({lat, lng, z});
  });
  assert_true(res);

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
String cpp_encode(NumericMatrix line) {

  String encoded;

  if (line.cols() == 2) {

    Rcout << "2 Cols";
    Rcout << '\n';

    std::vector<std::pair<double, double>> input{{
      {50.1022829, 8.6982122},
      {50.1020076, 8.6956695},
      {50.1006313, 8.6914960},
      {50.0987800, 8.6875156},
      }};

    encoded = hf::polyline_encode(input, 5, hf::ThirdDim::ABSENT);

  } else if (line.cols() == 3) {

    Rcout << "3 Cols";
    Rcout << '\n';

    std::vector<std::tuple<double, double, double>> input{{
      {50.1022829, 8.6982122, 10},
      {50.1020076, 8.6956695, 20},
      {50.1006313, 8.6914960, 30},
      {50.0987800, 8.6875156, 40},
    }};

    encoded = hf::polyline_encode(input, 5, hf::ThirdDim::ALTITUDE);

  } else {

    stop("Invalid input dimensions.");

  }

  return encoded;

}

