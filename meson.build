project('ardent-dust-de', 'c',
  default_options: ['c_std=c11', 'warning_level=2']
)

wlroots = dependency('wlroots-0.20')
wayland_server = dependency('wayland-server')
wayland_client = dependency('wayland-client')
xkbcommon = dependency('xkbcommon')
pixman = dependency('pixman-1')

deps = [
  wlroots,
  wayland_server,
  wayland_client,
  xkbcommon,
  pixman
]

executable('ardent-dust-de',
  sources: [
    'src/compositor/main.c',
    'src/compositor/server.c',
    'src/compositor/output.c',
    'src/compositor/xdg.c',
    'src/compositor/listeners.c',
    'src/compositor/cursor.c'
  ],
  include_directories: include_directories('src/compositor'),
  dependencies: deps,
  c_args: ['-DWLR_USE_UNSTABLE'],
  install: false
)
