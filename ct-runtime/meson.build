
source= [
  'core/engine.cpp',
]

inc = include_directories('.')
lib_ct_runtime = shared_library('ct-runtime',
  source,
  include_directories : inc,
  cpp_args: '-DCT_EXPORT=1',
  dependencies : [lib_ct_core_dep],
  install : true)

lib_ct_runtime_dep = declare_dependency(
    include_directories : inc,
    link_with : lib_ct_runtime)
