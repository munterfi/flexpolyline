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
