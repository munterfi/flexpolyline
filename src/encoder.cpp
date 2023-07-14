#include "encoder.h"

#include <stdexcept>

using hf::flexpolyline::Polyline;
using hf::flexpolyline::Polyline2d;
using hf::flexpolyline::Polyline3d;
using hf::flexpolyline::polyline_encode;
using Rcpp::CharacterVector;

Encoder::Encoder(int precision, int third_dim_precision, int third_dim)
    : precision(precision), third_dim_precision(third_dim_precision), third_dim(third_dim)
{
}

std::string Encoder::encode_polyline(const NumericMatrix &matrix)
{
  std::string encoded;
  Polyline polyline;
  matrix_to_polyline(matrix, polyline);
  if (auto error = hf::flexpolyline::polyline_encode(polyline, encoded))
  {
    throw std::invalid_argument("Failed to encode: " + std::to_string(static_cast<uint32_t>(*error)));
  }
  return encoded;
}

void Encoder::matrix_to_polyline(const NumericMatrix &matrix, Polyline &polyline)
{
  if (matrix.cols() == 3)
  {
    std::vector<std::tuple<double, double, double>> coordinates;
    matrix_to_vector(matrix, coordinates);
    polyline = Polyline3d{
        std::move(coordinates),
        *hf::flexpolyline::Precision::from_u32(precision),
        *hf::flexpolyline::Precision::from_u32(third_dim_precision),
        static_cast<hf::flexpolyline::Type3d>(third_dim)};
  }
  else
  {
    std::vector<std::tuple<double, double>> coordinates;
    matrix_to_vector(matrix, coordinates);
    polyline = Polyline2d{
        std::move(coordinates), *hf::flexpolyline::Precision::from_u32(precision)};
  }
}

void Encoder::matrix_to_vector(const NumericMatrix &matrix, std::vector<std::tuple<double, double>> &result)
{
  size_t n = matrix.nrow();
  result.reserve(n);
  for (size_t i = 0; i < n; i++)
  {
    result.emplace_back(matrix(i, 0), matrix(i, 1));
  }
}

void Encoder::matrix_to_vector(const NumericMatrix &matrix, std::vector<std::tuple<double, double, double>> &result)
{
  size_t n = matrix.nrow();
  result.reserve(n);
  for (size_t i = 0; i < n; i++)
  {
    result.emplace_back(matrix(i, 0), matrix(i, 1), matrix(i, 2));
  }
}
