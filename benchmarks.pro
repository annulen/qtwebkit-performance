TEMPLATE = subdirs

# benchmarks
SUBDIRS = loading/tst_loading.pro painting/tst_painting.pro

# no auto tests
SUBDIRS += robustness mirror launcher

# host tools
SUBDIRS += http_server
