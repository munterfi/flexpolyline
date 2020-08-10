test_that("decode_sf works", {

  # Encoded lines
  n <- 5
  encodedXY <- "BFoz5xJ67i1B1B7PzIhaxL7Y"
  encodedXYZ <- "BlBoz5xJ67i1BU1B7PUzIhaUxL7YU"
  encodedXYM <- as.factor("BlXoz5xJ67i1Bgkh9B1B7Pgkh9BzIhagkh9BxL7Ygkh9B")

  # Decode
  lineXY <- decode_sf(rep(encodedXY, n))
  lineXYZ <- decode_sf(rep(encodedXYZ, n))
  lineXYM <- decode_sf(rep(encodedXYM, n))

  # Test decode_sf()
  expect_s3_class(lineXY, c("sf", "data.frame"), exact = TRUE)
  expect_s3_class(lineXYZ, c("sf", "data.frame"), exact = TRUE)
  expect_s3_class(lineXYM, c("sf", "data.frame"), exact = TRUE)
  expect_true(all(sf::st_geometry_type(lineXY) == "LINESTRING"))
  expect_true(all(sf::st_geometry_type(lineXYZ) == "LINESTRING"))
  expect_true(all(sf::st_geometry_type(lineXYM) == "LINESTRING"))
  expect_equal(nrow(lineXY), n)
  expect_equal(nrow(lineXYZ), n)
  expect_equal(nrow(lineXYM), n)

})
