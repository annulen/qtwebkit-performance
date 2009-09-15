TEMPLATE = subdirs

# benchmarks
SUBDIRS = loading/tst_loading.pro painting/tst_painting.pro scrolling/tst_scrolling.pro

# no auto tests
SUBDIRS += robustness mirror launcher

# host tools
SUBDIRS += http_server memusagestat
