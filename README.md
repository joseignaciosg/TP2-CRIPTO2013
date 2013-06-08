TP2-CRIPTO2013
==============

ESTEGANOGRAFÍA

Para compilar, situarse en el directorio src y hacer make stegobmp.

Extraction Usage:
--extract                : to extract hidden message
-p bitmapfile            : root of the image with the hidden message
--out outfile            : name of the extracted hidden message
--steg  <LSB1|LSB4|LSBE>  : mode of extraction

Embedding Usage:
--embed                  : to embed hidden message
-p bitmapfile            : root of the image whehe the message should be embedded
--out outfile            : name of the resulting image
--in outfile             : message to hide
--steg  <LSB1|LSB4|LSBE>  : mode of embedding

Optional Parameters for encryption/decryption
-a <aes128|aes192|aes256|des> : encryption mode
-m <ecb|cfb|ofb|cbc>          : encryption block mode
--pass password               : password
   
Ejemplo de extracción
./test --extract -p ../files/example/sherlock1.bmp --out out --steg LSB1


Ejemplo de embebimiento
./test --embed -p test.bmp --out out.bmp --in 1 --steg LSB1


Para encontrar la solución a la enigma:
cd test/grupo4
../../src/stegobmp --extract -p hugo5.bmp --out out_hugo5 --steg LSB4
../../src/stegobmp --extract -p medianocheenparis1.bmp --out out_medianocheenparis1 --steg LSBE
cp out_hugo5.png out_hugo5.zip
unzip out_hugo5.zip
# have a look at the file unzipped and the .png
xxd lifeofpi.bmp | tail -n 5
../../src/stegobmp --extract -p medianocheenparis1.bmp --out out_medianocheenparis1 --steg LSBE
../../src/stegobmp --extract -p eclipse.bmp --out out --steg LSB1 --pass camaleon -m ecb -a des

El archivo oculto se encuentra en forma de video en el archivo : out.wmv

