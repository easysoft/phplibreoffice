#!/bin/sh
system=`cat /etc/issue`
c1=`echo "$system" | grep -i "debian"`
c2=`echo "$system" | grep -i "ubuntu"`
pack_name=""
if test -n "$c1" || test -n "$c2" ; then
#use deb
libre_sdk="http://mirrors.ustc.edu.cn/tdf/libreoffice/stable/6.0.5/deb/x86_64/LibreOffice_6.0.5_Linux_x86-64_deb_sdk.tar.gz"
libre="http://mirrors.ustc.edu.cn/tdf/libreoffice/stable/6.0.5/deb/x86_64/LibreOffice_6.0.5_Linux_x86-64_deb.tar.gz"
#liblang="http://mirrors.ustc.edu.cn/tdf/libreoffice/stable/6.0.5/deb/x86_64/LibreOffice_6.0.5_Linux_x86-64_deb_langpack_zh-CN.tar.gz"

#install libreoffice
#wget "$libre"
tar -zxvf "LibreOffice_6.0.5_Linux_x86-64_deb.tar.gz"
dpkg -i LibreOffice_6.0.5.2_Linux_x86-64_deb/DEBS/*.deb

#install lang
#wget "$liblang"
#tar -zxvf "LibreOffice_6.0.5_Linux_x86-64_deb_langpack_zh-CN.tar.gz"
#dpkg -i LibreOffice_6.0.5.2_Linux_x86-64_deb_langpack_zh-CN/DEBS/*.deb

#install libreoffice sdk
#wget "$libre_sdk"
tar -zxvf "LibreOffice_6.0.5_Linux_x86-64_deb_sdk.tar.gz"
dpkg -i LibreOffice_6.0.5.2_Linux_x86-64_deb_sdk/DEBS/*.deb
pack_name="apt"
apt-get update
else
#use rpm
libre="http://mirrors.ustc.edu.cn/tdf/libreoffice/stable/6.0.5/rpm/x86_64/LibreOffice_6.0.5_Linux_x86-64_rpm.tar.gz"
libre_sdk="http://mirrors.ustc.edu.cn/tdf/libreoffice/stable/6.0.5/rpm/x86_64/LibreOffice_6.0.5_Linux_x86-64_rpm_sdk.tar.gz"

#install libreoffice
wget "$libre"
tar -zxvf "LibreOffice_6.0.5_Linux_x86-64_rpm.tar.gz"
rpm -i LibreOffice_6.0.5.2_Linux_x86-64_rpm/RPMS/*.rpm
#install libreoffice sdk
wget "$libre_sdk"
tar -zxvf "LibreOffice_6.0.5_Linux_x86-64_rpm_sdk.tar.gz"
rpm -i LibreOffice_6.0.5.2_Linux_x86-64_rpm_sdk/RPMS/*.rpm
pack_name="yum"

fi

cd /opt/libreoffice6.0/program/
for i in $(ls *.so*); do ln -s /opt/libreoffice6.0/program/$i /usr/lib/$i; done
cd --

"$pack_name"  install -y zip
"$pack_name"  install -y expect
"$pack_name"  install -y default-jre

#add URE_MORE_TYPES
cat >/opt/libreoffice6.0/program/unorc <<EOF
[Bootstrap]
URE_INTERNAL_LIB_DIR=\${ORIGIN}
URE_INTERNAL_JAVA_DIR=\${ORIGIN}/classes
URE_INTERNAL_JAVA_CLASSPATH=\${URE_MORE_JAVA_TYPES}
URE_MORE_TYPES=file:///opt/libreoffice6.0/sdk/../program/types/offapi.rdb
UNO_TYPES=\${ORIGIN}/types.rdb \${URE_MORE_TYPES}
UNO_SERVICES=\${ORIGIN}/services.rdb \${URE_MORE_SERVICES}
EOF


/usr/bin/expect <<EOF #!/usr/bin/expect
spawn /opt/libreoffice6.0/sdk/setsdkenv_unix
send "\n"
send "\n"
send "\n"
send "\n"
send "\n"
send "\n"
send "\n"
send "\n"
send "\n"
send "nohup soffice \"--accept=socket,host=localhost,port=3536;urp;StarOffice.ServiceManager\" &\r"
expect eof
EOF