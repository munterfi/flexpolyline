test_that("encode works", {

  # 2d line
  line2d <- matrix(
    c(8.69821, 50.10228,
      8.69567, 50.10201,
      8.69150, 50.10063,
      8.68752, 50.09878),
    ncol = 2, byrow = TRUE
  )

  # 3d line
  line3d <- matrix(
    c(8.69821, 50.10228, 10.11111,
      8.69567, 50.10201, 20.22222,
      8.69150, 50.10063, 30.33333,
      8.68752, 50.09878, 40.44444),
    ncol = 3, byrow = TRUE
  )

  # Test encode_sf()
  expect_error(
    encode_sf(sf::st_point(c(1,2,3))),
    "Invalid geometry type 'POINT' of input, only 'LINESTRING' is supported."
  )

  ## sfg
  line2d_sfg <- sf::st_linestring(line2d)
  line3d_sfg <- sf::st_linestring(line3d)
  expect_type(encode_sf(line2d_sfg), "character")
  expect_type(encode_sf(line3d_sfg), "character")

  ## sfc
  line2d_sfc <- sf::st_as_sfc(list(line2d_sfg, line2d_sfg), crs = 4326)
  line3d_sfc <- sf::st_as_sfc(list(line3d_sfg, line3d_sfg), crs = 4326)
  expect_type(encode_sf(line2d_sfc), "character")
  expect_type(encode_sf(line3d_sfc), "character")

  ## sf
  line2d_sf <- sf::st_as_sf(line2d_sfc)
  line3d_sf <- sf::st_as_sf(line3d_sfc)
  expect_type(encode_sf(line2d_sf), "character")
  expect_type(encode_sf(line3d_sf), "character")

})
