test_that("decode_sf works", {

  # Encoded lines
  n <- 5
  encoded_xyz <- "B1Voz5xJ67i1Bgkh9B"
  encoded_xy <- "BFoz5xJ67i1B1B7PlU9yB"
  encoded_xym <- as.factor("BlXoz5xJ67i1Bgkh9B1B7Pgkh9BzIhagkh9BqK-pB_ni6D")

  # Decode
  point_xyz <- decode_sf(rep(encoded_xyz, n))
  line_xy <- decode_sf(rep(encoded_xy, n))
  poly_xym <- decode_sf(rep(encoded_xym, n))

  # Test decode_sf()
  expect_s3_class(point_xyz, c("sf", "data.frame"), exact = TRUE)
  expect_s3_class(line_xy, c("sf", "data.frame"), exact = TRUE)
  expect_s3_class(poly_xym, c("sf", "data.frame"), exact = TRUE)
  expect_s3_class(decode_sf(c("BlXoz5xJ67i1Bgkh9B", "BlXoz5xJ67i1Bgkh9B1B7Pgkh9BzIhagkh9B", "BlXoz5xJ67i1Bgkh9B1B7Pgkh9BzIhagkh9BqK-pB_ni6D")), c("sf", "data.frame"), exact = TRUE)
  expect_true(all(sf::st_geometry_type(point_xyz) == "POINT"))
  expect_true(all(sf::st_geometry_type(line_xy) == "LINESTRING"))
  expect_true(all(sf::st_geometry_type(poly_xym) == "POLYGON"))
  expect_equal(nrow(point_xyz), n)
  expect_equal(nrow(line_xy), n)
  expect_equal(nrow(poly_xym), n)
})
