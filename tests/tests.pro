TEMPLATE = subdirs

SUBDIRS = \
    gtest \
    gtest_main \
    main_test \
    shared_test \
    sound_test

main_test.depends = gtest gtest_main
shared_test.depends = gtest gtest_main
sound_test.depends = gtest gtest_main
