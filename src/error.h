#ifndef FLEXPOLYLINE_ERROR_H
#define FLEXPOLYLINE_ERROR_H

#include "hf/flexpolyline.h"

using hf::flexpolyline::Error;

class ErrorUtils {
   public:
    static std::string error_to_string(const Error& error);
    static void check_precision(int precision);
    static void check_third_dim(int third_dim);
};

#endif /* FLEXPOLYLINE_ERROR_H */
