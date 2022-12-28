# gps-viewer
gps data line viewer

Open /dev/ttyUSB0
Configure port speed to 4800 bauds
Read nema sentences and print them on screen (could also be piped to a file) 

If you run the program without any argument, every sentence will show on screen
if you want to print only one type of sentence run it as in exemple: "name of prog" \$GPGGA
this will only show $GPGGA sentence on screen