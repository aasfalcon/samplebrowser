TEMPLATE = subdirs

SUBDIRS = \
    main \
    engine \
    qtjack

engine.subdir   = src/engine
main.subdir     = src/main
qtjack.subdir   = vendor/qtjack

main.depends    = engine qtjack
