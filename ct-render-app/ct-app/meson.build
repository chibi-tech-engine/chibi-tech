inc = include_directories('.')

render_demo_app = executable('ct-render-demo',
  'ct-render-demo-entry.cpp',
  include_directories : inc,
  link_with : [lib_ct_core, lib_ct_runtime],
  dependencies : [lib_ct_core_dep, lib_ct_runtime_dep],
  install : true)
