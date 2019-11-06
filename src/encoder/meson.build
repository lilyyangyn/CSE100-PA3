hc_node_dep = declare_dependency(include_directories : include_directories('.'))

hc_tree = library('hc_tree', sources : ['HCTree.hpp', 'HCTree.cpp'], 
    dependencies : [bit_input_stream_dep, bit_output_stream_dep, hc_node_dep])
hc_tree_dep = declare_dependency(include_directories : include_directories('.'), 
    link_with : hc_tree,
    dependencies : [bit_input_stream_dep, bit_output_stream_dep])