htcpcp
======

HTPCPC gedängel für den 30c3

AVR:
Start brewing: PB2 (Pull to ground)
Ready state: PB0 (high=true)
Brewing state: PB4 (high=true)

avrdude -cusbasp -p t13 -e -U flash:w:ctrl.hex

Teile:
Atttiny13
Schrack RT42006 (8A, 250VAC)
BC547
4 1k
Rote/Grüne LED
Taster
Kabel
Breadboard



  Kamera:
Modell: Intel CS430
Modul: gspca_spca505
Bild Aufnemen mit fswebcam:
  LD_PRELOAD=/usr/lib/v4l2convert.so fswebcam -d /dev/video1 img.jpg
Füllstand auslesen (LD_PRELOAD muss gesetzt sein!):
  i=voll; fswebcam -d /dev/video1 $i.jpg; convert ${i}.jpg -crop 50x140+200+90 ${i}_crop.jpg; convert ${i}_crop.jpg -grayscale rec709luma ${i}_crop_gray.jpg; convert ${i}_crop_gray.jpg -threshold 20% ${i}_crop_bw.txt; c=$(cat ${i}_crop_bw.txt | wc -l); bc=$(cat ${i}_crop_bw.txt | grep black -c); echo $(( $bc*100/$c ))


  Raspi:
GPIOs ansprechen:
 - aktivieren:
   echo "4" > /sys/class/gpio/export
 - Richtung
   /sys/class/gpio/gpioX/direction
 - Wert setzen/lesen
   /sys/class/gpio/gpioX/value
 - 17 muss auf aktive_low gesetzt werden
   echo "1" > /sys/class/gpio/gpio17/active_low

GPIO Belegung:
17: start brew (Auf high ziehen, wenn ready startet die Maschine)
18: brew state (Wenn high läuft die Maschine gerade)
4: ready state (Wenn high kann die Maschine gestartet werden)
Wenn beide low: Call the operator

Die Stromversorgung kommt direkt über den GPIO Header (die 5V vom Netzteil
werden durchgeschleift). Das Netzteil sollte also ca 100ma mehr liefern können
als der Raspi selber benötigt.
