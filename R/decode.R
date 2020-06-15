#' Decode a flexible polyline encoded string
#'
#' This function calls \code{hf::polyline_decode} and \code{hf::get_third_dimension}
#' of the C++ implementation of the flexible polyline encoding by HERE. Depending
#' on the dimensions of the encoded line, a two or three dimensional line is decoded.
#'
#' @param encoded character, encoded flexible polyline string.
#' @param encoded crs, coordinate reference system (\code{default = 4326}, WGS84).
#'
#' @return
#' A matrix containing the coordinates of the decoded line.
#'
#' @export
#'
#' @examples
#' # 2d line
#' decode("BFoz5xJ67i1B1B7PzIhaxL7Y")
#'
#' # 3d line
#' decode("BlBoz5xJ67i1BU1B7PUzIhaUxL7YU")
decode <- function(encoded, crs = 4326) UseMethod("decode")

#' @export
decode.character <- function(encoded, crs = 4326) {
  return(sf::st_sfc(lapply(encoded, function(x) {
    sf::st_linestring(decode_coords(x))
  }), crs = crs))
}

#' @export
decode.factor <- function(encoded, crs = 4326) {
  return(decode.character(as.character(encoded)))
}
