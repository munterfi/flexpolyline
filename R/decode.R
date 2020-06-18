#' Decode a flexible polyline encoded string
#'
#' A wrapper function for \link{decode_coords} that converts the input polylines
#' to simple feature geometries of the sf package.
#' This function calls \code{hf::polyline_decode} and \code{hf::get_third_dimension}
#' of the C++ implementation of the flexible polyline encoding by HERE. Depending
#' on the dimensions of the encoded line, a two or three dimensional line is decoded.
#'
#' @note
#' The function returns a sf object, therefore the input set of polylines must
#' be of consistent dimension (e.g XY or XYZ) to meet the requirements of the constructor of an sf object.
#' For mixed dimensions use the \link{decode_coords} function directly.
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
decode <- function(encoded, crs = 4326) UseMethod("decode", encoded)

#' @export
decode.character <- function(encoded, crs = 4326) {

  dim3 <- character(length(encoded))
  ind3 <- 2
  sfdi <- "XY"

  geom <- sf::st_sfc(
    lapply(1:length(encoded), function(x) {
      m <- decode_coords(encoded[[x]])
      d3 <- colnames(m)[3]
      if (is.na(d3)) {
        dim3[x] <<- "ABSENT"
        ind3 <<- 2
        sfdi <<- "XY"
      } else {
        dim3[x] <<- d3
        ind3 <<- 3
        if (d3 %in% c("LEVEL", "ALTITUDE", "ELEVATION")) {
          message(d3)
          sfdi <<- "XYZ"
        } else {
          sfdi <<- "XYM"
        }
      }
      sf::st_linestring(m, dim = sfdi)
    }),
    crs = crs
  )

  dim3[is.na(dim3)] <- "ABSENT"

  return(
    sf::st_as_sf(
      data.frame(
        id = seq(1, length(encoded)),
        dim3 = dim3,
        geometry = geom
      )
    )
  )
}

#' @export
decode.factor <- function(encoded, crs = 4326) {
  return(decode.character(as.character(encoded)))
}
