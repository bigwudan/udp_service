gcc -lpthread -I/usr/include/mysql/  -L/usr/lib64/mysql/ -lmysqlclient -lz udp_pthread.c stack_data.c mysql_connect.c -o udp_pthread

http://blog.csdn.net/u010246789/article/details/52382632

yum install mysql-server mysql-devel mysql

wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm
rpm -ivh mysql-community-release-el7-5.noarch.rpm
yum install mysql-server
 service mysqld start或者/etc/init.d/mysqld start



 ulimit -u 1111
 4700