TEMPLATE = subdirs

SUBDIRS = \
    gtest \
    gtest_main \
    mainTest \
    sharedTest \
    soundTest

mainTest.depends = gtest gtest_main
sharedTest.depends = gtest gtest_main
