# Main project

TEMPLATE = subdirs

SUBDIRS = \
    main \
    engine \
    shared \
    sound \
    widgets \
    bundle \
    plugins \
    tests

engine.subdir = src/engine
widgets.subdir = src/widgets
main.subdir = src/main
plugins.subdir = src/plugins
shared.subdir = src/shared
sound.subdir = src/sound

main.depends = shared engine widgets bundle plugins sound
