#' Decode a flexible polyline encoded string
#'
#' @param encoded character, encoded flexible polyline string.
#'
#' @return
#' A matrix containing the coordinates.
#'
#' @export
#'
#' @examples
#' # 2d line
#' decode("BFoz5xJ67i1B1B7PzIhaxL7Y")
#'
#' # 3d line
#' decode("BlBoz5xJ67i1BU1B7PUzIhaUxL7YU")
#' # routes$geometry[1] %>% decode() %>% st_linestring() %>% st_sfc(crs = 4326) %>% mapview()
decode <- function(encoded) {
  return(cpp_decode(encoded))
}
