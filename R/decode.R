#' BLA
#'
#' @param encoded character, encoded flexible polyline string.
#'
#' @return
#' String
#' @export
#'
#' @examples
#' # 2D
#' decode("BFoz5xJ67i1B1B7PzIhaxL7Y")
#'
#' # 3D
#' decode("BlBoz5xJ67i1BU1B7PUzIhaUxL7YU")
#' # routes$geometry[1] %>% decode() %>% st_linestring() %>% st_sfc(crs = 4326) %>% mapview()
decode <- function(encoded) {
  return(cpp_decode(encoded))
}
