#include "hf/flexpolyline.h"

#include <Rcpp.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using namespace Rcpp;

std::string to_string(const hf::flexpolyline::Type3d &type)
{
  switch (type)
  {
  case hf::flexpolyline::Type3d::LEVEL:
    return "LEVEL";
  case hf::flexpolyline::Type3d::ALTITUDE:
    return "ALTITUDE";
  case hf::flexpolyline::Type3d::ELEVATION:
    return "ELEVATION";
  case hf::flexpolyline::Type3d::RESERVED1:
    return "RESERVED1";
  case hf::flexpolyline::Type3d::RESERVED2:
    return "RESERVED2";
  case hf::flexpolyline::Type3d::CUSTOM1:
    return "CUSTOM1";
  case hf::flexpolyline::Type3d::CUSTOM2:
    return "CUSTOM2";
  default:
    throw std::invalid_argument("Invalid enum value: " + std::to_string(static_cast<int>(type)));
  }
}

void parse_polyline(const hf::flexpolyline::Polyline2d &polyline, NumericMatrix &matrix)
{
  const std::vector<std::tuple<double, double>> &coordinates = polyline.coordinates;
  matrix = NumericMatrix(coordinates.size(), 2);
  for (size_t i = 0; i < coordinates.size(); ++i)
  {
    matrix(i, 0) = std::get<0>(coordinates[i]);
    matrix(i, 1) = std::get<1>(coordinates[i]);
  }
  colnames(matrix) = CharacterVector::create("LNG", "LAT");
}

void parse_polyline(const hf::flexpolyline::Polyline3d &polyline, NumericMatrix &matrix)
{
  const std::vector<std::tuple<double, double, double>> &coordinates = polyline.coordinates;
  matrix = NumericMatrix(coordinates.size(), 3);
  for (size_t i = 0; i < coordinates.size(); ++i)
  {
    matrix(i, 0) = std::get<0>(coordinates[i]);
    matrix(i, 1) = std::get<1>(coordinates[i]);
    matrix(i, 2) = std::get<2>(coordinates[i]);
  }
  colnames(matrix) = CharacterVector::create("LNG", "LAT", to_string(polyline.type3d));
}

//' Decode a flexible polyline encoded string
//'
//' This function calls \code{hf::polyline_decode} and
//' \code{hf::get_third_dimension} of the C++ implementation of the flexible
//' polyline encoding by HERE. Depending on the dimensions of the encoded line,
//' a two or three dimensional line is decoded.
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
NumericMatrix decode(SEXP encoded)
{
  std::string encoded_str = Rcpp::as<std::string>(encoded);
  hf::flexpolyline::Polyline polyline;
  if (auto error = hf::flexpolyline::polyline_decode(encoded_str, polyline))
  {
    throw std::invalid_argument("Failed to decode: " + std::to_string(static_cast<uint32_t>(*error)));
  }
  NumericMatrix matrix;
  if (std::holds_alternative<hf::flexpolyline::Polyline2d>(polyline))
  {
    parse_polyline(std::get<hf::flexpolyline::Polyline2d>(polyline), matrix);
  }
  else
  {
    parse_polyline(std::get<hf::flexpolyline::Polyline3d>(polyline), matrix);
  }
  return matrix;
}

void matrix2d_to_vector(const Rcpp::NumericMatrix &matrix, std::vector<std::tuple<double, double>> &result)
{
  size_t n = matrix.nrow();
  result.reserve(n);
  for (size_t i = 0; i < n; i++)
  {
    result.emplace_back(matrix(i, 0), matrix(i, 1));
  }
}

void matrix3d_to_vector(const Rcpp::NumericMatrix &matrix, std::vector<std::tuple<double, double, double>> &result)
{
  size_t n = matrix.nrow();
  result.reserve(n);
  for (size_t i = 0; i < n; i++)
  {
    result.emplace_back(matrix(i, 0), matrix(i, 1), matrix(i, 2));
  }
}

//' Encode a line in the flexible polyline encoding format
//'
//' This function calls \code{hf::polyline_encode} of the C++ implementation of
//' the flexible polyline encoding by HERE. Depending on the dimensions of the
//' input coordinates, a two or three dimensional line is encoded.
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
              int third_dim_precision = 5)
{
  hf::flexpolyline::Polyline polyline;
  std::string encoded;
  if (line.rows() == 3)
  {
    std::vector<std::tuple<double, double, double>> coordinates;
    matrix3d_to_vector(line, coordinates);
    polyline = hf::flexpolyline::Polyline3d{
        std::move(coordinates),
        *hf::flexpolyline::Precision::from_u32(precision),
        *hf::flexpolyline::Precision::from_u32(third_dim_precision),
        static_cast<hf::flexpolyline::Type3d>(third_dim)};
  }
  else
  {
    std::vector<std::tuple<double, double>> coordinates;
    matrix2d_to_vector(line, coordinates);
    polyline = hf::flexpolyline::Polyline2d{
        std::move(coordinates), *hf::flexpolyline::Precision::from_u32(precision)};
  }
  if (auto error = polyline_encode(polyline, encoded))
  {
    throw std::invalid_argument("Failed to encode: " + std::to_string(static_cast<uint32_t>(*error)));
  }
  return encoded;
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
std::string get_third_dimension(SEXP encoded)
{
  return "test";
}

//' Set third dimension of a flexible polyline encoded string
//'
//' This function decodes the flexible polyline encoded line, changes the third
//' dimension and encodes the line again.
//'
//' @note
//' The precision is not read from the header of the encoded line. Therefore it
//' must be provided as a parameter for re-encoding.
//'
//' @param encoded character, encoded flexible polyline string.
//' @param third_dim_name character, name of the third dimension to set (ABSENT,
//' LEVEL, ALTITUDE, ELEVATION, CUSTOM1, CUSTOM2).
//' @param precision integer, precision to use in encoding (between 0 and 15,
//' \code{default=5}).
//' @param third_dim_precision integer, precision to use in encoding for the
//' third dimension (between 1 and 15, \code{default=5}).
//'
//' @return
//' The line with the new third dimension as string in the flexible polyline
//' encoding format.
//'
//' @export
//'
//' @examples
//' # 2d line (nothing happens...)
//' set_third_dimension("BFoz5xJ67i1B1B7PzIhaxL7Y", "ELEVATION")
//'
//' # 3d line
//' set_third_dimension("BlBoz5xJ67i1BU1B7PUzIhaUxL7YU", "ELEVATION")
// [[Rcpp::export]]
std::string set_third_dimension(SEXP encoded, SEXP third_dim_name,
                                int precision = 5,
                                int third_dim_precision = 5)
{
  return "test";
}
