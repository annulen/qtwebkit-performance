TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += common

# tests
TESTNAMES = loading painting scrolling cycler layout \
            graphicspainting graphicsscrolling

for(tname,TESTNAMES) {
  SUBDIRS += tests/$${tname}/tst_$${tname}.pro
}

!symbian: {
# reductions
!lessThan(QT_MINOR_VERSION, 6): SUBDIRS += reductions/loading_standalone/tst_loading_standalone.pro

# host tools
SUBDIRS += host-tools/http_server host-tools/mirror
}

symbian: {
load(data_caging_paths)

TARGET.CAPABILITY = All -TCB -DRM -AllFiles 
RSS_RULES ="group_name=\"QtBenchmarks\";"

vendorinfo = \
    "; Localised Vendor name" \
    "%{\"Nokia, Qt\"}" \
    " " \
    "; Unique Vendor name" \
    ":\"Nokia, Qt\"" \
    " "

default_deployment.pkg_prerules += vendorinfo

for(tname,TESTNAMES) {
  testsexe.sources += tst_$${tname}.exe
}

testsexe.path = c:PROGRAMS_DIR

for(tname,TESTNAMES) {
  testrsc.sources += $${EPOCROOT}$$HW_ZDIR$$APP_RESOURCE_DIR/tst_$${tname}.rsc
}
testrsc.path = c:$$APP_RESOURCE_DIR

for(tname,TESTNAMES) {
  testregrsc.sources += $${EPOCROOT}$$HW_ZDIR$$REG_RESOURCE_IMPORT_DIR/tst_$${tname}_reg.rsc
}
testregrsc.path = $$REG_RESOURCE_IMPORT_DIR

exists(perfurls.txt) {
testurls.sources = perfurls.txt
testurls.path = c:/data/Others
}

DEPLOYMENT += testrsc testregrsc testsexe testurls
}
