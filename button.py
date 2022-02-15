import RPi.GPIO as GPIO
import os

cam_cmd = "gphoto2 --capture-image-and-download"

GPIO.setmode(GPIO.BCM)
GPIO.setup(21, GPIO.IN, pull_up_down=GPIO.PUD_UP)
while 1:
    if not GPIO.input(21):
        print("button pressed!")
        os.system(cam_cmd)
GPIO.cleanup()
