test_that("decode works", {

  # Encoded lines
  n <- 5
  encoded2d <- "BFoz5xJ67i1B1B7PzIhaxL7Y"
  encoded3d <- "BlBoz5xJ67i1BU1B7PUzIhaUxL7YU"

  # Decode
  line2d <- decode_sf(rep(encoded2d, n))
  line3d <- decode_sf(rep(encoded3d, n))

  # Test decode_sf()
  expect_s3_class(line2d, c("sf", "data.frame"), exact = TRUE)
  expect_s3_class(line3d, c("sf", "data.frame"), exact = TRUE)
  expect_true(all(sf::st_geometry_type(line2d) == "LINESTRING"))
  expect_true(all(sf::st_geometry_type(line3d) == "LINESTRING"))
  expect_equal(nrow(line2d), n)
  expect_equal(nrow(line3d), n)

})
