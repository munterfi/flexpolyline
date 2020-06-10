test_that("Cpp binding to 'flexpolyline.h' en- and decodes correctly", {

  # Read and preprocess test data
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

  # Original lines
  org <- parse_test_examples(
    readLines(
      system.file("test/original.txt", package="flexpolyline")
    )
  )

  # Decoded lines
  dec <- parse_test_examples(
    readLines(
      system.file("test/round_half_even/decoded.txt", package="flexpolyline")
    )
  )

  # Encoded lines
  enc <- readLines(
    system.file("test/round_half_even/encoded.txt", package="flexpolyline")
  )

  # Test encoding
  expect_equal(
    any(
      sapply(1:length(enc), function(i) {

        # Omit reserved dimensions
        if (org[[i]]$third_dim %in% c(4, 5)) return(FALSE)

        # Encode
        encoded <- encode(
          line = org[[i]]$coords,
          precision = org[[i]]$precision,
          third_dim = org[[i]]$third_dim,
          third_dim_precision = org[[i]]$third_dim_precision
        )

        # Test equality
        enc[i] != encoded

      })
    ),
    FALSE
  )

  # Test decoding
  expect_equal(
    any(
      sapply(1:length(dec), function(i) {

        # Omit reserved dimensions and encoding with precision higher than 7
        if (
          is.na(dec[[i]]$third_dim_precision) |
          is.na(dec[[i]]$precision)
        ) return(FALSE)
        if (
          dec[[i]]$third_dim %in% c(4, 5) |
          dec[[i]]$third_dim_precision > 7 |
          dec[[i]]$precision > 7
        ) return(FALSE)

        # Encode
        decoded <- decode(
          enc[i]
        )

        # Test equality
        any(dec[[i]]$coords != decoded)

      })
    ),
    FALSE
  )

})
