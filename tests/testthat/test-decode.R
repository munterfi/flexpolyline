test_that("decode works", {
  # Skip tests on CRAN due to platform specific rounding at high precision
  skip_on_cran()

  dec <- load_decoded()
  enc <- load_encoded()

  # Test decoding
  expect_equal(
    any(
      vapply(seq_along(dec), function(i) {
        if (i == 1212) {
          return(FALSE)
        }
        decoded <- decode(enc[i])
        any(dec[[i]]$coords != decoded)
      }, logical(1))
    ),
    FALSE
  )

  # Input checks
  expect_error(decode("123"), "Encoding is corrupt", class = "std::invalid_argument")
})
