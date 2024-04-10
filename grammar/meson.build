grammar_c = custom_target(
  'grammar_c',
  command: [leg, '@INPUT@'],
  input: ['grammar.leg'],
  capture: true,
  output: 'grammar.c',
)

common_src = files([
  'list.c',
  'grammar_objects.c',
  'wonkyregex.c',
])

grammar = executable(
  'grammar',
  common_src + [grammar_c],
  link_args: '-lpcre2-8',
)

test_get_tree = executable(
  'test_get_tree',
  common_src + ['test_get_tree.c', 'grammar_get_tree.c'],
  link_args: '-lpcre2-8',
)

thing = custom_target(
  'thing',
  command: [grammar],
  feed: true,
  input: ['grammar.leg'],
  capture: true,
  output: 'thing',
)

thing2 = custom_target(
  'thing2',
  command: [test_get_tree],
  capture: true,
  output: 'thing2',
)

diff = custom_target(
  'diff',
  command: ['diff', '@INPUT@'],
  input: [thing, thing2],
  capture: true,
  output: 'diff',
)
