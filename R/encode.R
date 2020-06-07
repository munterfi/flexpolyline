#' Encode a line in flexible polyline encoding
#'
#' @param line matrix, coordinates of the line in 2d or 3d.
#' @param precision integer, precision to use in encoding (between 1 and 15; only valid results until 7!).
#'
#' @return
#' The line as encoded string.
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
encode <- function(line, precision = 5) {
  return(cpp_encode(line, precision))
}


