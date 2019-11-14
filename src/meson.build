subdir('bitStream')
subdir('encoder')

file_utils_dep = declare_dependency(include_directories : include_directories('.'))

compress_exe = executable('compress.cpp.executable',
    sources : ['compress.cpp'],
    dependencies : [cxxopts_dep, file_utils_dep, hc_node_dep, hc_tree_dep, hc_tree2_dep])

uncompress_exe = executable('uncompress.cpp.executable',
    sources : ['uncompress.cpp'],
    dependencies : [cxxopts_dep, file_utils_dep, hc_node_dep, hc_tree_dep, hc_tree2_dep])