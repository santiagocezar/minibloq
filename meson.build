project('minibloq', 'cpp',
  version : '0.1',
  default_options : ['warning_level=0', 'cpp_std=c++17'])

wx = dependency('wxwidgets', modules : ['all'])
usb = dependency('libusb')
lager = dependency('Lager')
immer = dependency('Immer')
zug = dependency('Zug')
fmt = dependency('fmt')

subdir('src')

res = include_directories(
  'data/Resources'
)

# install_subdir(
#   'data/Bin/Minibloq',
#   install_dir : get_option('datadir') / 'minibloq',
#   exclude_directories : [
#     '_drivers',
#     '_lang',
#     '_tools',
#   ],
#
#   strip_directory : true)

executable('minibloq',
           sources,
           install : true,
           include_directories : [include, res],
           dependencies : [wx, usb, lager, immer, zug, fmt])
