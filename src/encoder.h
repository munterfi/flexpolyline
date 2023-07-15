#ifndef FLEXPOLYLINE_ENCODER_H
#define FLEXPOLYLINE_ENCODER_H

#include "hf/flexpolyline.h"

#include <Rcpp.h>

using hf::flexpolyline::Polyline;
using Rcpp::NumericMatrix;
using Rcpp::String;

class Encoder
{
public:
  Encoder(int precision, int third_dim_precision, int third_dim);
  String encode(const NumericMatrix &matrix);

private:
  void matrix_to_polyline(const NumericMatrix &matrix, Polyline &polyline);
  static void matrix_to_vector(const NumericMatrix &matrix, std::vector<std::tuple<double, double>> &result);
  static void matrix_to_vector(const NumericMatrix &matrix, std::vector<std::tuple<double, double, double>> &result);

  int precision;
  int third_dim_precision;
  int third_dim;
};

#endif // FLEXPOLYLINE_ENCODER_H
