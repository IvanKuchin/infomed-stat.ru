FROM ubuntu:xenial

LABEL maintainer="ivan.kuchin@gmail.com"
LABEL org.opencontainers.image.source="https://github.com/IvanKuchin/__BUILD_REPO__"
				     
# following two commands suppose to be reused from cache
RUN apt update &&  apt install --no-install-recommends --no-install-suggests -y git ca-certificates

##############################
# install additional locales #
##############################
RUN apt-get install --no-install-recommends --no-install-suggests -y locales gettext
RUN locale-gen ru_RU.utf8
RUN locale-gen en_US.utf8
RUN update-locale
RUN export LC_ALL=en_US.utf8

#########################################
# libwebsocket copy from prev container #
#########################################

##########################
# copy from libwebsocket #
##########################
# COPY --from=libwebsocket /usr/local/share/libwebsockets/ /usr/local/share/libwebsockets/

COPY . /backend
WORKDIR /backend
RUN git clone --depth 1 https://github.com/IvanKuchin/backend_pi.git ./src/pi
RUN git clone --depth 1 https://github.com/IvanKuchin/__BUILD_REPO__-ui.git /home/httpd/www

RUN apt-get install --no-install-recommends --no-install-suggests -y software-properties-common
RUN add-apt-repository -y ppa:jonathonf/ffmpeg-4
RUN apt update
RUN apt-get install --no-install-recommends --no-install-suggests -y \ 
        build-essential \
        cmake \
        ffmpeg \
        graphicsmagick-libmagick-dev-compat \
        libavcodec-dev \
        libavformat-dev \
        libavutil-dev \
        libcurl4-openssl-dev \
        libgraphicsmagick++-dev \
        libhpdf-dev \
        libmaxminddb-dev \
        libmysqlclient-dev \
        libssl-dev \
        libwebp-dev \
        libarchive-dev \
        rapidjson-dev \
        pkg-config

#############################
# libwkhtmltox installation #
#############################
RUN apt-get install --no-install-recommends --no-install-suggests -y wget
RUN wget https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.trusty_amd64.deb
RUN dpkg -i  wkhtmltox_0.12.5-1.trusty_amd64.deb || true     # trick to force dpkg return successfull return code
RUN apt install -f --no-install-recommends --no-install-suggests -y

##################
# save commit id #
##################
RUN git rev-parse HEAD > ./supplemental/git/git_commit_id

##################### 
# build from source #
##################### 
RUN mkdir build
RUN cd build && \
    cmake \
        -DMAIN_SUBDOMAIN=www \
        -DIMAGE_SUBDOMAIN=images \
        -DROOTDIR=/home/httpd/www \
        -DIMAGEROOTDIR=/home/httpd/images \
        -DLOCAL_INSTALL_DIR=/usr/local/share/www \
        .. && \
    make -j2 && \
    make install

###################
# install apache2 #
###################
RUN git clone --depth 1 https://github.com/IvanKuchin/__BUILD_REPO__-ui.git /home/httpd/www
RUN mkdir /home/httpd/www/logs/
RUN cp -R /home/httpd/www/supplemental/apache2 /etc

RUN a2enmod cgi
RUN a2dissite `ls /etc/apache2/sites-enabled/`
RUN a2ensite *www
RUN a2enmod expires

EXPOSE 80

############################################################
## forward request and error logs to docker log collector ##
############################################################
#RUN ln -sf /dev/stdout /var/log/nginx/access.log \
# && ln -sf /dev/stderr /var/log/nginx/error.log

STOPSIGNAL SIGTERM

CMD ["apachectl", "-D", "FOREGROUND"]
