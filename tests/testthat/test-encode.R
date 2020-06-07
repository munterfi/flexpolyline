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
    c(8.69821, 50.10228, 10,
      8.69567, 50.10201, 20,
      8.69150, 50.10063, 30,
      8.68752, 50.09878, 40),
    ncol = 3, byrow = TRUE
  )

  # Encode
  encoded2d <- encode(line2d)
  encoded3d <- encode(line3d)

  # Test
  expect_is(encoded2d, "character")
  expect_is(encoded3d, "character")

})
