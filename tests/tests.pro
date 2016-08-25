TEMPLATE = subdirs

SUBDIRS = \
    gtest \
    gtest_main \
    mainTest \
    sharedTest

mainTest.depends = gtest gtest_main
sharedTest.depends = gtest gtest_main
