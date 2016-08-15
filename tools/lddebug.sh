#/bin/sh

export LD_DEBUG=all
LD_LIBRARY_PATH=/tmp/wexplorer.build/src/plugins /tmp/wexplorer.build/src/wexplorer 2> /tmp/wexplorer.build/lddebug.log
export LD_DEBUG=
