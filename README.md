php libreoffice support
====

bring uno api to php5, and make is easy to use, fork from Puno

Installation Guide
==================
./install.sh

phpize
./configure
make install

TODO
==================
php7 support

Q&A
==================
if report error "Binary URP bridge disposed during call" <br\>
run cmd `nohup /opt/libreoffice6.0/program/soffice "--accept=socket,host=localhost,port=3536;urp;StarOffice.ServiceManager" &`


