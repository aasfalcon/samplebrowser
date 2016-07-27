TEMPLATE = subdirs

SUBDIRS = \
    main \
    engine \
    widgets \
    bundle \
    plugins

engine.subdir = src/engine
widgets.subdir = src/widgets
main.subdir = src/main
bundle.subdir = bundle
plugins.subdir = src/plugins

main.depends = engine widgets bundle plugins
