#!/usr/bin/sh
cmake --build ./build
cp ./build/photobooth_process /usr/bin/
cp ./button.py /usr/bin/
cp ./shutter_button.service /lib/systemd/system/
cp ./photobooth_liveprocess.service /lib/systemd/system/
