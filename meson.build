project('ardent-dust-de', 'c',
  version: '0.1',
  default_options: [warning_level=2]
)

gtk4 = dependency('gtk4')
layer_shell = dependency('gtk4-layer-shell-0')

executable('ardent-dust-de',
  'src/main.c,
  dependencies: [
    gtk4,
    layer_shell,
  ],
  install: false
)
