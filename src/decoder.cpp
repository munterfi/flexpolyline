#include "decoder.h"

#include <stdexcept>

using hf::flexpolyline::Polyline;
using hf::flexpolyline::Polyline2d;
using hf::flexpolyline::Polyline3d;
using hf::flexpolyline::polyline_decode;
using Rcpp::CharacterVector;

NumericMatrix Decoder::decode_polyline(const SEXP &encoded)
{
  std::string encoded_str = Rcpp::as<std::string>(encoded);
  Polyline polyline;
  if (auto error = polyline_decode(encoded_str, polyline))
  {
    throw std::invalid_argument("Failed to decode: " + std::to_string(static_cast<uint32_t>(*error)));
  }
  NumericMatrix matrix;
  polyline_to_matrix(polyline, matrix);
  return matrix;
}

std::string Decoder::type3d_to_string(const hf::flexpolyline::Type3d &type)
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

void Decoder::polyline_to_matrix(const Polyline &polyline, NumericMatrix &matrix)
{
  if (std::holds_alternative<Polyline2d>(polyline))
  {
    const Polyline2d &polyline2d = std::get<Polyline2d>(polyline);
    const std::vector<std::tuple<double, double>> &coordinates = polyline2d.coordinates;
    matrix = NumericMatrix(coordinates.size(), 2);
    for (size_t i = 0; i < coordinates.size(); ++i)
    {
      matrix(i, 0) = std::get<0>(coordinates[i]);
      matrix(i, 1) = std::get<1>(coordinates[i]);
    }
    colnames(matrix) = CharacterVector::create("LNG", "LAT");
  }
  else if (std::holds_alternative<Polyline3d>(polyline))
  {
    const Polyline3d &polyline3d = std::get<Polyline3d>(polyline);
    const std::vector<std::tuple<double, double, double>> &coordinates = polyline3d.coordinates;
    matrix = NumericMatrix(coordinates.size(), 3);
    for (size_t i = 0; i < coordinates.size(); ++i)
    {
      matrix(i, 0) = std::get<0>(coordinates[i]);
      matrix(i, 1) = std::get<1>(coordinates[i]);
      matrix(i, 2) = std::get<2>(coordinates[i]);
    }
    colnames(matrix) = CharacterVector::create("LNG", "LAT", type3d_to_string(polyline3d.type3d));
  }
}
