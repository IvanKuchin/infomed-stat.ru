#!/bin/bash

chown -R mysql:mysql /var/lib/mysql /var/run/mysqld \

cd \
&& service mysql start \
&& mysql -proot < pre.sql \
&& mysql -proot timecard_su < sql \
&& git clone --single-branch -b development https://github.com/IvanKuchin/timecard.su.git \
&& cd timecard.su \
&& mkdir build \
&& cd build \
&& cmake .. \
&& make \
&& make test 
