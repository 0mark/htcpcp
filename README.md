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
- 5 1k
- green LED
- two push buttons
- Wire
- Breadboard

The device  
![The controlling device](http://files.q3t.de/a/get/76164c7904d245fc1fd803b028145b37)

Circuit
![Circuit](https://raw.github.com/0mark/htcpcp/master/circuit.jpg)
Connect Pin N of J1 to
- 1 -> Start brewign
- 2 -> Ready State
- 3 -> Brew State

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

Actually, becaus of a regression in recent Kernels we could not use the Camera on the Raspi, and instead used the Camera on a PC (providing the fill level via http). This  Problem should be resolved in later Kernals.

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
![Omas Teekanne](http://files.q3t.de/a/get/352a530893b39f54e03b20a23bf3fc80)

Server
------
Implemented in Elisp. Implements HTCPCP. Accepts brew command. After a brew is done, and the coffee maker is not refilled, another brew command will result in an error, demanding to call the operator.
Every not supportet action will result in a 418 (hence the teapot casing for the raspy)

First Setup on 30c3
-------------------
![Done!](http://files.q3t.de/a/get/9044fb9a5b54132e47b030be9344532d)

Note the ready state button taped aside the machine. Its labeled "magic". But because of tight time restriction, we had no "more nagic" button anywhere, just a "mundane" reset button on the breadboard.

We brewedaround 6-8 pots of coffee over the time of the congress, of which two where started when we wherent at our assembly, and at least another three by people other than us.
