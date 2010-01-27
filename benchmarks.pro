TEMPLATE = subdirs

# tests
SUBDIRS = tests/loading/tst_loading.pro \
          tests/painting/tst_painting.pro \
          tests/scrolling/tst_scrolling.pro \
          tests/cycler/tst_cycler.pro \

# reductions
!lessThan(QT_MINOR_VERSION, 6): SUBDIRS += reductions/loading_standalone/tst_loading_standalone.pro

# host tools
SUBDIRS += host-tools/http_server host-tools/mirror
