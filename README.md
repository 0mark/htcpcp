htcpcp
======
HTPCPC implementation for the 30c3.

AVR
---
Start brewing: PB2 (Pull to ground)  
Ready state: PB0 (high=true)  
Brewing state: PB4 (high=true)  

```bash
avrdude -cusbasp -p t13 -e -U flash:w:ctrl.hex
```

Partlist
- Attiny13
- Relais Schrack RT42006 (8A, 250VAC)
- BC547
- 4 1k
- red & green LED
- two push buttons
- Wire
- Breadboard

The device  
![The controlling device](https://files.q3t.de/a/get/1a3665a5277a68314ee4b73f74570abc)

Kamera
------
Modell: Intel CS430  
Modul: gspca_spca50

Capture a frame with fswebcam:  
```bash
LD_PRELOAD=/usr/lib/v4l2convert.so fswebcam -d /dev/video1 img.jpg
```
This camera uses some abscure color format, hence the preloaded library.
  
Read the current coffee level:  
```bash
export LD_PRELOAD=/usr/lib/v4l2convert.so
fswebcam -d /dev/video1 img.jpg
convert img.jpg -crop 50x210+200+55 img_crop.jpg
convert img_crop.jpg -grayscale rec709luma img_crop_gray.jpg
convert img_crop_gray.jpg -threshold 20% img_crop_bw.txt
c=$(cat img_crop_bw.txt | wc -l)
bc=$(cat img_crop_bw.txt | grep black -c)
echo $(( $bc*100/$c ))
```
What? Easy: Take image, crop center of pot, grayscale, threshold, count black pixels and all pixels. Ratio is coffee level. Yes dudes, thats advanced image recognition!

Raspi
-----
Using GPIO X:
 - first activate 
   /sys/class/gpio/export
 - set direction (in|out)
   /sys/class/gpio/gpioX/direction
 - read or set value
   /sys/class/gpio/gpioX/value
 - set active_low (needed for bin 17)
   /sys/class/gpio/gpioX/active_low

GPIO Belegung:  
- 17: start brew (pull to high)
- 18: brew state (high if brewing in progress)
- 4: ready state (high if ready)
if both are low: Call the operator (brewing done, refill needed)

The power supply comes directly from the GPIO Header (5V from power supply are routed through). Mind that the device may need around 100mA.

Coffee maker
----------
Lend from Iso, a secondary replacement device is available  
Brew time: ca 7.5 minutes (8 in Firmware, to be safe)

Tea pot
-------
Grannys Teapot
![Omas Teekanne](https://files.q3t.de/a/get/352a530893b39f54e03b20a23bf3fc80)

Server
------
Implemented in Elisp. Implements HTCPCP. Accepts brew command. After a brew is done, and the coffee maker is not refilled, another brew command will result in an error, demanding to call the operator.
Every not supportet action will result in a 418 (hence the teapot casing for the raspy)

First Setup on 30c3
-------------------
![Done!](https://files.q3t.de/a/get/9044fb9a5b54132e47b030be9344532d)
