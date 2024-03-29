#ifndef FLEXPOLYLINE_DECODER_H
#define FLEXPOLYLINE_DECODER_H

#include "hf/flexpolyline.h"

#include <Rcpp.h>

using hf::flexpolyline::Polyline;
using hf::flexpolyline::Type3d;
using Rcpp::NumericMatrix;
using Rcpp::String;

class Decoder {
public:
    NumericMatrix decode(const SEXP &encoded);

    String get_third_dimension(const SEXP &encoded);

private:
    static std::string type3d_to_string(const Type3d &type);

    void polyline_to_matrix(const Polyline &polyline, NumericMatrix &matrix);
};

#endif // FLEXPOLYLINE_DECODER_H
