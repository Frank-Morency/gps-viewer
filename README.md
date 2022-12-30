# gps-viewer
gps data line viewer

Open /dev/ttyUSB0
Configure port speed to 4800 bauds
Read nema sentences and print them on screen (could also be piped to a file) 

/*if you want to pipe all sentence:
exemple: ./prog_name ALL | your_file.txt
only one type of sentence run it as in:
exemple: ./prog_name \$GPGGA | your_file.txt
this will only take $GPGGA sentence on pipe.

If you dont put any argument you have to select at runtime*/