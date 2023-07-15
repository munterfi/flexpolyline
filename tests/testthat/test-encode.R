test_that("decode works", {
  # Skip tests on CRAN due to platform specific rounding at high precision
  skip_on_cran()

  org <- load_original()
  enc <- load_encoded()

  # Test encoding
  expect_equal(
    any(
      vapply(seq_along(enc), function(i) {
        # Avoid NA values in input
        third_dim <- org[[i]]$third_dim
        third_dim_precision <- org[[i]]$third_dim_precision
        encoded <- encode(
          line = org[[i]]$coords,
          precision = org[[i]]$precision,
          third_dim = ifelse(is.na(third_dim), 0, third_dim),
          third_dim_precision = ifelse(is.na(third_dim_precision), 0, third_dim_precision)
        )
        enc[i] != encoded
      }, logical(1))
    ),
    FALSE
  )

  # Input checks
  expect_error(encode(matrix(1, 2, 4)), "Invalid input dimensions \\(2 x 4\\)", class = "std::invalid_argument")
  expect_error(encode(matrix(1, 2, 3), precision = -1), "Precision -1 out of range \\[0, 15\\]", class = "std::out_of_range")
  expect_error(encode(matrix(1, 2, 3), third_dim = -1), "Third dimension -1 out of range \\[0, 7\\]", class = "std::out_of_range")
  expect_error(encode(matrix(1, 2, 3), third_dim_precision = -1), "Precision -1 out of range \\[0, 15\\]", class = "std::out_of_range")
})
