test_that("decode works", {

  # Encoded lines
  encoded2d <- "BFoz5xJ67i1B1B7PzIhaxL7Y"
  encoded3d <- "BlBoz5xJ67i1BU1B7PUzIhaUxL7YU"

  # Decode
  line2d <- decode(encoded2d)
  line3d <- decode(encoded3d)

  # Test
  expect_is(line2d, "matrix")
  expect_is(line3d, "matrix")

})
