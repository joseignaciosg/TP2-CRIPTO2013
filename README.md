TP2-CRIPTO2013
==============

#ESTEGANOGRAFÍA

#Para compilar, situarse en el directorio src y hacer make stegobmp.

Extraction Usage:<br>
--extract                : to extract hidden message<br>
-p bitmapfile            : root of the image with the hidden message<br>
--out outfile            : name of the extracted hidden message<br>
--steg  <LSB1|LSB4|LSBE>  : mode of extraction<br>
<br>
Embedding Usage:<br>
--embed                  : to embed hidden message<br>
-p bitmapfile            : root of the image whehe the message should be embedded<br>
--out outfile            : name of the resulting image<br>
--in outfile             : message to hide<br>
--steg  <LSB1|LSB4|LSBE>  : mode of embedding<br>
<br>
Optional Parameters for encryption/decryption<br>
-a <aes128|aes192|aes256|des> : encryption mode<br>
-m <ecb|cfb|ofb|cbc>          : encryption block mode<br>
--pass password               : password<br>
<br><br>
#Ejemplo de extracción<br>
./test --extract -p ../files/example/sherlock1.bmp --out out --steg LSB1<br>
<br>
#Ejemplo de embebimiento<br>
./test --embed -p test.bmp --out out.bmp --in 1 --steg LSB1<br>
<br>
<br>
#Para encontrar la solución a la enigma:<br>
cd test/grupo4<br>
../../src/stegobmp --extract -p hugo5.bmp --out out_hugo5 --steg LSB4<br>
../../src/stegobmp --extract -p medianocheenparis1.bmp --out out_medianocheenparis1 --steg LSBE<br>
cp out_hugo5.png out_hugo5.zip<br>
unzip out_hugo5.zip<br>
have a look at the file unzipped and the .png<br>
xxd lifeofpi.bmp | tail -n 5<br>
../../src/stegobmp --extract -p medianocheenparis1.bmp --out out_medianocheenparis1 --steg LSBE<br>
../../src/stegobmp --extract -p eclipse.bmp --out out --steg LSB1 --pass camaleon -m ecb -a des<br>
<br>
El archivo oculto se encuentra en forma de video en el archivo : out.wmv<br>

