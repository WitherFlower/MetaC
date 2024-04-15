pcre2 = dependency('libpcre2-8')

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
  'objects.c',
  'dict.c',
])

grammar = executable(
  'grammar',
  common_src + [grammar_c],
  dependencies: [pcre2]
)

# TODO: FAIRE MARCHER CE TRUC
# grammar_get_tree_c = custom_target(
#   'grammar_get_tree_c',
#   command: [grammar],
#   input: ['raw.leg'],
#   output: ['grammar_get_tree.c'],
#   feed: true,
# )

test_get_tree = executable(
  'test_get_tree',
  common_src + ['test_get_tree.c', 'grammar_get_tree.c'],
  dependencies: [pcre2]
)

exec_tree = executable(
  'exec_tree',
  common_src + ['exec_tree.c', 'grammar_get_tree.c'],
  dependencies: [pcre2],
)

thing = custom_target(
  'thing',
  command: [grammar],
  feed: true,
  input: ['raw.leg'],
  capture: true,
  output: 'thing',
)

thing2 = custom_target(
  'thing2',
  command: [test_get_tree],
  capture: true,
  output: 'thing2',
)

test_list = executable(
  'test_list',
  common_src + ['test_list.c'],
  dependencies: [pcre2],
)

test('list', test_list)

# diff = custom_target(
#   'diff',
#   command: ['diff', '@INPUT@'],
#   input: [thing, thing2],
#   capture: true,
#   output: 'diff',
# )
