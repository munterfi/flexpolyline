#include "encoder.h"

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "error.h"

using hf::flexpolyline::Polyline;
using hf::flexpolyline::Polyline2d;
using hf::flexpolyline::Polyline3d;
using hf::flexpolyline::polyline_encode;
using Rcpp::CharacterVector;
using Rcpp::String;

Encoder::Encoder(int precision, int third_dim_precision, int third_dim)
        : precision(precision), third_dim_precision(third_dim_precision), third_dim(third_dim) {
    ErrorUtils::check_precision(precision);
    ErrorUtils::check_precision(third_dim_precision);
    ErrorUtils::check_third_dim(third_dim);
}

String Encoder::encode(const NumericMatrix &matrix) {
    std::string encoded;
    Polyline polyline;
    matrix_to_polyline(matrix, polyline);
    if (auto error = polyline_encode(polyline, encoded)) {
        throw std::invalid_argument(ErrorUtils::error_to_string(*error));
    }
    return String(encoded);
}

void Encoder::matrix_to_polyline(const NumericMatrix &matrix, Polyline &polyline) {
    if (matrix.cols() == 3) {
        std::vector <std::tuple<double, double, double>> coordinates;
        matrix_to_vector(matrix, coordinates);
        polyline = Polyline3d{
                std::move(coordinates),
                *hf::flexpolyline::Precision::from_u32(precision),
                *hf::flexpolyline::Precision::from_u32(third_dim_precision),
                static_cast<hf::flexpolyline::Type3d>(third_dim)};
    } else if (matrix.cols() == 2) {
        std::vector <std::tuple<double, double>> coordinates;
        matrix_to_vector(matrix, coordinates);
        polyline = Polyline2d{
                std::move(coordinates), *hf::flexpolyline::Precision::from_u32(precision)};
    } else {
        throw std::invalid_argument(
                "Invalid input dimensions (" + std::to_string(matrix.rows()) + " x " + std::to_string(matrix.cols()) +
                ")");
    }
}

void Encoder::matrix_to_vector(const NumericMatrix &matrix, std::vector <std::tuple<double, double>> &result) {
    size_t n = matrix.nrow();
    result.reserve(n);
    for (size_t i = 0; i < n; i++) {
        result.emplace_back(matrix(i, 1), matrix(i, 0));
    }
}

void Encoder::matrix_to_vector(const NumericMatrix &matrix, std::vector <std::tuple<double, double, double>> &result) {
    size_t n = matrix.nrow();
    result.reserve(n);
    for (size_t i = 0; i < n; i++) {
        result.emplace_back(matrix(i, 1), matrix(i, 0), matrix(i, 2));
    }
}
