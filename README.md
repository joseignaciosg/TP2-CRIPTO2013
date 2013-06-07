TP2-CRIPTO2013
==============

ESTEGANOGRAFÍA

Forma de invocación
Estraction Usage:
--extract                : to extract hidden message
-p bitmapfile            : root of the image with the hidden messahe
--out outfile            : name of the extracted hidden message
-steg  <LSB1|LSB4|LSBE>  : mode of estraction

Embedding Usage: \n");
--embed                  : to embed hidden message
-p bitmapfile            : root of the image whehe the message should be embedded
--out outfile            : name of the resulting image
--in outfile             : message to hide
-steg  <LSB1|LSB4|LSBE>  : mode of embedding


Ejemplo de extracción
./test --extract -p ../files/example/sherlock1.bmp --out out --steg LSB1


Ejemplo de embebimiento
./test --embed -p test.bmp --out out.bmp --in 1 --steg LSB1
