#project('ct-core', 'cpp')

source= [
  'platform/platform_win32.cpp',
  'platform/platform_nix.cpp',
  'memory/allocators/dynamic_allocator.cpp',
  'memory/ct_memory.cpp',
]

inc = include_directories('.')

lib_ct_core = shared_library('ct-core',
  source,
  include_directories : inc,
  cpp_pch: 'pch/pch.h',
  cpp_args: '-DCT_EXPORT=1',
  install : true)

lib_ct_core_dep = declare_dependency(
    include_directories : inc,
    link_with : lib_ct_core)
