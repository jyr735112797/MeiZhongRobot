#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/ubuntu/mz_ws/src/rbx1/rbx1_vision"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/ubuntu/mz_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/ubuntu/mz_ws/install/lib/python2.7/dist-packages:/home/ubuntu/mz_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/ubuntu/mz_ws/build" \
    "/usr/bin/python" \
    "/home/ubuntu/mz_ws/src/rbx1/rbx1_vision/setup.py" \
    build --build-base "/home/ubuntu/mz_ws/build/rbx1/rbx1_vision" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/ubuntu/mz_ws/install" --install-scripts="/home/ubuntu/mz_ws/install/bin"
