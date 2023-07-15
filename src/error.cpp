#include "error.h"

#include <Rcpp.h>

#include <stdexcept>

std::string ErrorUtils::error_to_string(const Error& error) {
    switch (error) {
        case Error::UNSUPPORTED_VERSION:
            return "Data is encoded with unsupported version";
        case Error::INVALID_PRECISION:
            return "Precision is not supported by encoding";
        case Error::INVALID_ENCODING:
            return "Encoding is corrupt";
        default:
            throw std::invalid_argument(
                "Invalid error code: " +
                std::to_string(static_cast<int>(error)));
    }
}

void ErrorUtils::check_precision(int precision) {
    if (precision < 0 || precision > 15) {
        throw std::out_of_range("Precision " + std::to_string(precision) +
                                " out of range [0, 15]");
    }
}

void ErrorUtils::check_third_dim(int third_dim) {
    if (third_dim < 0 || third_dim > 7) {
        throw std::out_of_range("Third dimension " + std::to_string(third_dim) +
                                " out of range [0, 7]");
    }
}
