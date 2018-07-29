php libreoffice support
====

This is a copy of [Puno v0.9](https://sourceforge.net/projects/puno/) which appears to have not been updated for quite some time. 

Installation Guide
==================
apt-get update
./install.sh

phpize
./configure
make install

Q&A
==================
report "Binary URP bridge disposed during call"
`nohup /opt/libreoffice6.0/program/soffice "--accept=socket,host=localhost,port=3536;urp;StarOffice.ServiceManager" &`


