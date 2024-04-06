numbers_c = custom_target(
  'numbers_c',
  command: [leg, '@INPUT@'],
  input: ['numbers.leg'],
  capture: true,
  output: 'numbers.c',
)

numbers = executable(
  'numbers',
  [numbers_c],
)
