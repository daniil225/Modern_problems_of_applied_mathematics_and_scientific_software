project('Module2_LR1', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'warning_level=3',
    'buildtype=debug'
  ]
)

current_buildtype = get_option('buildtype')
fs = import('fs')
scripts_dir = meson.current_source_dir() / 'scripts'
deps_dir = meson.current_source_dir() / 'deps'
vcpkg_dir = deps_dir / 'vcpkg'
triplet = get_option('triplet')
if triplet == ''
  error('set triplet option')
endif
vcpkg_installed = vcpkg_dir / 'installed' / triplet
vcpkg_include = vcpkg_installed / 'include'
vcpkg_lib_dir = ''
if current_buildtype == 'debug'
  vcpkg_lib_dir = vcpkg_installed / 'debug' / 'lib'
elif current_buildtype == 'release'
  vcpkg_lib_dir = vcpkg_installed / 'lib'
endif

# vcpkg install
if not fs.exists(vcpkg_dir)
  out = run_command(
    scripts_dir / 'install-vcpkg.sh',
    check: true,
    env: ['DEPS_DIR=@0@'.format(deps_dir)],
  ).stdout()
  message(out)
else
  message('vcpkg olready exists')
endif

# vcpkg install matplot++ library
if not fs.exists(vcpkg_include / 'matplot')
  out = run_command(
    scripts_dir / 'load-matplot.sh',
    check: true,
    env: ['VCPCK_DIR=@0@'.format(vcpkg_dir)],
  ).stdout()
  message(out)
else
  message('Matplot++ already exists')
endif

matplot_dep = declare_dependency(
  compile_args: ['-I' + vcpkg_include],
  link_args: [
    '-L' + vcpkg_lib_dir,
    '-lmatplot',
    '-lnodesoup'
  ],
)

# Boost
boost_dep = dependency('boost', required:true)

# tabulate
tabulate_dep = dependency('tabulate', required: true)


# collect dep's
dep = [
  boost_dep,
  matplot_dep,
  tabulate_dep,
]


inc_dir = [
  include_directories('include'),
]

src = [
    'main.cpp',
]

executable(
    'Module2_LR1',
    sources: [
        src
    ],
    dependencies : dep,
    include_directories: inc_dir,
)