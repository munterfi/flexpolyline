load_original <- function() {
  return(
    parse_test_examples(
      readLines(
        system.file("test/original.txt", package = "flexpolyline")
      )
    )
  )
}

load_decoded <- function() {
  return(
    parse_test_examples(
      readLines(
        system.file("test/round_half_up/decoded.txt", package = "flexpolyline")
      )
    )
  )
}

load_encoded <- function() {
  return(
    readLines(
      system.file("test/round_half_up/encoded.txt", package = "flexpolyline")
    )
  )
}

parse_test_examples <- function(input) {
  third_dim_names <- c(
    "ABSENT", "LEVEL", "ALTITUDE", "ELEVATION", "", "", "CUSTOM1", "CUSTOM2"
  )
  lapply(strsplit(input, ";"), function(x) {
    # Get header part of input
    header <-
      as.numeric(
        unlist(strsplit(stringr::str_remove_all(x[[1]], "[{( )]"), ","))
      )
    # Get polyline part of input
    line <-
      unlist(strsplit(stringr::str_remove_all(x[[2]], "[\\[\\] (}]"), ")"))
    coords <- do.call(
      "rbind",
      lapply(line, function(x) {
        coords <- strsplit(x, ",")
        coords <- sapply(coords, as.numeric)
        coords[!is.na(coords)]
      })
    )
    # Reorder and rename coords
    n_col <- ncol(coords)
    if (n_col == 2) {
      coords <- matrix(c(coords[, 2], coords[, 1]), ncol = n_col)
      colnames(coords) <- c("LNG", "LAT")
    } else {
      coords <- matrix(c(coords[, 2], coords[, 1], coords[, 3]), ncol = n_col)
      colnames(coords) <- c("LNG", "LAT", third_dim_names[header[3] + 1])
    }
    # Create R list
    list(
      precision = header[1],
      third_dim_precision = header[2],
      third_dim = header[3],
      coords = coords
    )
  })
}
