FROM ubuntu:xenial as base

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN=www
ARG IMAGE_SUBDOMAIN=images
ARG ROOTDIR=/home/httpd/www
ARG IMAGEROOTDIR=/home/httpd/images
ARG LOCAL_INSTALL_DIR=/usr/local/share/www

ARG GIT_FOLDER=/backend

LABEL maintainer="ivan.kuchin@gmail.com"
LABEL org.opencontainers.image.source="https://github.com/IvanKuchin/${BUILD_REPO}"

###########################################
# check if all mandatory args are defined #
###########################################
RUN [ -z "$BUILD_REPO" ] && echo "BUILD_REPO is required" && exit 1 || true
RUN echo repository is $BUILD_REPO

###########################
# install basic utilities #
###########################
RUN apt update &&  apt install --no-install-recommends --no-install-suggests -y git ca-certificates

####################
# install timezone #
####################
RUN apt-get install --no-install-recommends --no-install-suggests -y tzdata
RUN cp /usr/share/zoneinfo/Europe/Moscow /etc/localtime
RUN echo "Europe/Moscow" > /etc/timezone
RUN date

##############################
# install additional locales #
##############################
RUN apt-get install --no-install-recommends --no-install-suggests -y locales gettext
RUN locale-gen ru_RU.utf8
RUN locale-gen en_US.utf8
RUN update-locale
RUN export LC_ALL=en_US.utf8

########################
# install dependencies #
########################
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

#############################
# libwebsocket installation #
#############################
RUN git clone -b v2.2.0 --depth 1 https://github.com/warmcat/libwebsockets.git /libwebsockets
WORKDIR /libwebsockets
RUN mkdir build
RUN cd build \
    && cmake -DLWS_WITHOUT_DAEMONIZE:BOOL=FALSE -DLWS_WITH_STATIC=1 -DLWS_WITH_SHARED=1 -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/share/libwebsockets/ .. \
    && make install

############
# pull src #
############
COPY . ${GIT_FOLDER}
WORKDIR ${GIT_FOLDER}
RUN git clone --depth 1 https://github.com/IvanKuchin/backend_pi.git ./src/pi

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
        -DMAIN_SUBDOMAIN=${MAIN_SUBDOMAIN} \
        -DIMAGE_SUBDOMAIN=${IMAGE_SUBDOMAIN} \
        -DROOTDIR=${ROOTDIR} \
        -DIMAGEROOTDIR=${IMAGEROOTDIR} \
        -DLOCAL_INSTALL_DIR=${LOCAL_INSTALL_DIR} \
        .. && \
    make -j2

############################################################################################


FROM base as app

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR

#######################
# install mail sender #
#######################
RUN apt-get install --no-install-recommends --no-install-suggests -y ssmtp

#########################
# configure mail sender #
#########################



###################
# install apache2 #
###################
RUN apt-get install --no-install-recommends --no-install-suggests -y apache2

#####################
# configure apache2 #
#####################
RUN git clone --depth 1 https://github.com/IvanKuchin/${BUILD_REPO}-ui.git ${ROOTDIR}
RUN cp -R ${ROOTDIR}/supplemental/apache2 /etc
RUN mkdir ${ROOTDIR}/logs/
RUN mkdir -p ${IMAGEROOTDIR}

RUN a2enmod cgi
RUN a2enmod rewrite
RUN a2dissite `ls /etc/apache2/sites-enabled/`
RUN a2ensite *www
RUN a2enmod expires

EXPOSE 80

####################################################################
# install from source                                              #
# moved to a separate step to avoid error in cloning UI repository #
####################################################################
RUN cd build && make install

####################################################################
# change folders to proper owner                                   #
####################################################################
RUN chown -R www-data:www-data ${ROOTDIR}
RUN chown -R www-data:www-data ${IMAGEROOTDIR}
RUN chown -R www-data:www-data ${LOCAL_INSTALL_DIR}

############################################################
## forward request and error logs to docker log collector ##
############################################################
#RUN ln -sf /dev/stdout /var/log/nginx/access.log \
# && ln -sf /dev/stderr /var/log/nginx/error.log

STOPSIGNAL SIGTERM

CMD ["apachectl", "-D", "FOREGROUND", ""]


############################################################################################


FROM base as images

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR

###################
# install apache2 #
###################
RUN apt-get install --no-install-recommends --no-install-suggests -y apache2

RUN echo 1

#####################
# configure apache2 #
#####################
RUN git clone --depth 1 https://github.com/IvanKuchin/${BUILD_REPO}-ui.git ${ROOTDIR}
RUN cp -R ${ROOTDIR}/supplemental/apache2 /etc
RUN mkdir -p ${IMAGEROOTDIR}/cgi-bin/
RUN mkdir -p ${IMAGEROOTDIR}/html/
RUN mkdir -p ${IMAGEROOTDIR}/logs/

RUN a2enmod rewrite
RUN a2enmod expires
RUN a2enmod headers
RUN a2dismod cgi

RUN a2dissite `ls /etc/apache2/sites-enabled/`
RUN a2ensite *images

EXPOSE 80

####################################################################
# change folders to proper owner                                   #
####################################################################
RUN chown -R www-data:www-data ${IMAGEROOTDIR}

####################################################################
# remove unnecessary folders                                       #
####################################################################
RUN rm -rf ${LOCAL_INSTALL_DIR}
RUN rm -rf ${ROOTDIR}
RUN rm -rf ${GIT_FOLDER}

############################################################
## forward request and error logs to docker log collector ##
############################################################
#RUN ln -sf /dev/stdout /var/log/nginx/access.log \
# && ln -sf /dev/stderr /var/log/nginx/error.log

STOPSIGNAL SIGTERM

CMD ["apachectl", "-D", "FOREGROUND", ""]



############################################################################################


FROM app as cron

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR

##############################
# install and configure cron #
##############################
RUN apt-get install --no-install-recommends --no-install-suggests -y cron
RUN cp ${LOCAL_INSTALL_DIR}/cron/cron_config /etc/cron.d/www
RUN chmod 644 /etc/cron.d/www
RUN chown root:root /etc/cron.d/www

CMD ["cron", "-f"]

############################################################################################


FROM app as chat

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR

WORKDIR ${LOCAL_INSTALL_DIR}/chat
CMD ["./chat-server"]
