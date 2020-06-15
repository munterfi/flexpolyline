#' Encode a line in the flexible polyline encoding format
#'
#' This function calls \code{hf::polyline_encode} of the C++ implementation of
#' the flexible polyline encoding by HERE. Depending on the dimensions of the
#' input coordinates, a two or three dimensional line is encoded.
#'
#' @param line matrix, coordinates of the line in 2d or 3d (column order: LNG, LAT, DIM3).
#' @param precision integer, precision to use in encoding (between 0 and 15, \code{default=5}).
#' @param third_dim integer, type of the third dimension (0: ABSENT, 1: LEVEL, 2: ALTITUDE, 3: ELEVATION, 4, 6: CUSTOM1, 7: CUSTOM2, \code{default=3}).
#' @param third_dim_precision integer, precision to use in encoding for the third dimension (between 1 and 15, \code{default=precision}).
#'
#' @return
#' The line as string in the flexible polyline encoding format.
#'
#' @export
#'
#' @examples
#' # 2D
#' line2d <- matrix(
#'   c(8.69821, 50.10228,
#'     8.69567, 50.10201,
#'     8.69150, 50.10063,
#'     8.68752, 50.09878),
#'   ncol = 2, byrow = TRUE
#' )
#' encode(line2d)
#'
#' # 3D
#' line3d <- matrix(
#'   c(8.69821, 50.10228, 10,
#'     8.69567, 50.10201, 20,
#'     8.69150, 50.10063, 30,
#'     8.68752, 50.09878, 40),
#'   ncol = 3, byrow = TRUE
#' )
#' encode(line3d)
encode <- function(line, precision = 5, third_dim = 3,
                   third_dim_precision = precision)
  UseMethod("encode")

#' @export
encode.sf <- function(line, precision = 5, third_dim = 3,
                      third_dim_precision = precision) {
  encoded <- sapply(sf::st_geometry(line), function(x) {
    encode_coords(
      sf::st_coordinates(x)[, c(1:2)],
      precision, third_dim, third_dim_precision
    )
  })
  return(encoded)
}

#' @export
encode.list <- function(line, precision = 5, third_dim = 3,
                        third_dim_precision = precision) {
  if(any(!sapply(line, is.matrix))) stop("Input not of type 'matrix array'.")
  encoded <- sapply(line, function(x) {
    encode_coords(x, precision, third_dim, third_dim_precision)
  })
  return(encoded)
}

#' @export
encode.matrix <- function(line, precision = 5, third_dim = 3,
                          third_dim_precision = precision) {
  encoded <- encode_coords(line, precision, third_dim, third_dim_precision)
  return(encoded)
}
