#' Wrapper function for decoding to simple features
#'
#' A wrapper function for \code{\link{decode}} that converts the input polylines
#' to simple feature geometries of the sf package.
#' This function calls \code{hf::polyline_decode} and \code{hf::get_third_dimension}
#' of the C++ implementation of the flexible polyline encoding by HERE. Depending
#' on the dimensions of the encoded line, a two or three dimensional line is decoded.
#'
#' @note
#' The function returns a sf object, therefore the input set of polylines must
#' be of consistent dimension (e.g XY or XYZ) to meet the requirements of the
#' constructor of an sf object. For mixed dimensions use the \code{\link{decode}}
#' function directly.
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
#' decode_sf("BFoz5xJ67i1B1B7PzIhaxL7Y")
#'
#' # 3d line
#' decode_sf("BlBoz5xJ67i1BU1B7PUzIhaUxL7YU")
decode_sf <- function(encoded, crs = 4326) {
  UseMethod("decode_sf", encoded)
}

#' @export
decode_sf.character <- function(encoded, crs = 4326) {

  dim3 <- character(length(encoded))
  ind3 <- 2
  sfdi <- "XY"

  geom <- sf::st_sfc(
    lapply(1:length(encoded), function(x) {
      m <- decode(encoded[[x]])
      d3 <- colnames(m)[3]
      if (is.na(d3)) {
        dim3[x] <<- "ABSENT"
        ind3 <<- 2
        sfdi <<- "XY"
      } else {
        dim3[x] <<- d3
        ind3 <<- 3
        if (d3 %in% c("LEVEL", "ALTITUDE", "ELEVATION")) {
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
decode_sf.factor <- function(encoded, crs = 4326) {
  return(decode_sf.character(as.character(encoded)))
}
