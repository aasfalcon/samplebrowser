# Main project

TEMPLATE = subdirs

SUBDIRS = \
    main \
    engine \
    sound \
    widgets \
    bundle \
    plugins

engine.subdir = src/engine
widgets.subdir = src/widgets
main.subdir = src/main
bundle.subdir = bundle
plugins.subdir = src/plugins
sound.subdir = src/sound

main.depends = engine widgets bundle plugins sound
