ARG BUILD_REPO
ARG MAIN_SUBDOMAIN=www
ARG IMAGE_SUBDOMAIN=images
ARG ROOTDIR=/home/httpd/www
ARG IMAGEROOTDIR=/home/httpd/images
ARG LOCAL_INSTALL_DIR=/usr/local/share/www
ARG BUILD_RELEASE_NUMBER=0.0.1
ARG GIT_FOLDER=/backend

FROM ubuntu:jammy AS base

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR
ARG BUILD_RELEASE_NUMBER
ARG GIT_FOLDER

LABEL maintainer="ivan.kuchin@gmail.com"
LABEL org.opencontainers.image.source="https://github.com/IvanKuchin/${BUILD_REPO}"

###########################################
# check if all mandatory args are defined #
###########################################
RUN [ -z "$BUILD_REPO" ] && echo "BUILD_REPO is required" && exit 1 || true
RUN echo repository is $BUILD_REPO

############################
# make apt non-interactive #
############################
ENV DEBIAN_FRONTEND=noninteractive
ENV DEBCONF_NONINTERACTIVE_SEEN=true

####################
# preseed timezone #
####################
RUN echo tzdata tzdata/Areas select Europe > /tmp/preseed.txt \
 && echo tzdata tzdata/Zones/Europe select Moscow >> /tmp/preseed.txt \
 && debconf-set-selections /tmp/preseed.txt

###########################
# install basic utilities #
###########################
RUN apt update \
    && apt install --no-install-recommends --no-install-suggests -y \
           ca-certificates \
           tzdata \
           locales \
           gettext \
    && rm -rf /var/lib/apt/lists/*

##############################
# install additional locales #
##############################
#RUN apt-get install --no-install-recommends --no-install-suggests -y locales gettext 
#    && rm -rf /var/lib/apt/lists/*
RUN locale-gen ru_RU.utf8
RUN locale-gen en_US.utf8
RUN update-locale LC_ALL=en_US.UTF8
# RUN export LC_ALL=en_US.utf8

############################################################################################


FROM base AS build

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR
ARG BUILD_RELEASE_NUMBER
ARG GIT_FOLDER

########################
# install dependencies #
########################
RUN apt update \
    && apt-get install --no-install-recommends --no-install-suggests -y \ 
        build-essential \
        file \
        cmake \
        ffmpeg \
        gsfonts \
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
        pkg-config \
        git \
        wget \
        cron

#############################
# libwebsocket installation #
#############################
#RUN git clone -b v2.2.0 --depth 1 https://github.com/warmcat/libwebsockets.git /libwebsockets
#WORKDIR /libwebsockets
#RUN mkdir build \
# && cd build \
# && cmake -DLWS_WITHOUT_DAEMONIZE:BOOL=FALSE -DLWS_WITH_STATIC=1 -DLWS_WITH_SHARED=1 -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/share/libwebsockets/ .. \
# && make -j2 install && make clean

######################
# libxl installation #
######################
RUN if test -f "./libxl-3.8.4.0/lib64/libxl.so"; then \
       cp ./libxl-3.8.4.0/lib64/libxl.so /usr/local/lib/ \
       && ldconfig; \
    fi

#############################
# libwkhtmltox installation #
#############################
RUN if test -d "./wkhtmltox"; then \
       wget https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.trusty_amd64.deb \
       && apt install --no-install-recommends --no-install-suggests -y ./wkhtmltox*.deb; \
    fi

##################################
# install mail sender && apache2 #
##################################
RUN apt-get install --no-install-recommends --no-install-suggests -y ssmtp apache2

#########################
# configure mail sender #
#########################
RUN sed -i "s/#rewriteDomain=/rewriteDomain=${BUILD_REPO}/g" /etc/ssmtp/ssmtp.conf

############
# pull src #
############
WORKDIR ${GIT_FOLDER}
RUN git clone --depth 1 https://github.com/IvanKuchin/backend_pi.git ./src/pi
COPY . .

##################
# save commit id #
##################
RUN git rev-parse HEAD > ./supplemental/git/git_commit_id

#####################
# configure apache2 #
#####################
#RUN git clone --depth 1 https://github.com/IvanKuchin/${BUILD_REPO}-ui.git ${ROOTDIR}
#RUN git -C ${ROOTDIR} rev-parse HEAD > ${ROOTDIR}/cgi-bin/templates/git_commit_id     # it is used in user account to show current commit_id
RUN cp -R ${GIT_FOLDER}/supplemental/apache2 /etc
RUN mkdir -p ${IMAGEROOTDIR}
RUN mkdir -p ${ROOTDIR}/logs/

RUN a2enmod cgi \
&& a2enmod rewrite \
&& a2enmod expires \
&& a2enconf mod_expires \
&& a2dissite `ls /etc/apache2/sites-enabled/` \
&& a2ensite *www

##################### 
# build from source #
##################### 
RUN mkdir build
RUN cd build \ 
    && cmake \
        -DPROJECT_NAME=${BUILD_REPO} \
        -DMAIN_SUBDOMAIN=${MAIN_SUBDOMAIN} \
        -DIMAGE_SUBDOMAIN=${IMAGE_SUBDOMAIN} \
        -DROOTDIR=${ROOTDIR} \
        -DIMAGEROOTDIR=${IMAGEROOTDIR} \
        -DLOCAL_INSTALL_DIR=${LOCAL_INSTALL_DIR} \
        -DBUILD_RELEASE_NUMBER=${BUILD_RELEASE_NUMBER} \
        .. \
    && make -j2 package \
    && make -j2 install

# EXPOSE 80

####################################################################
# change folders to proper owner                                   #
####################################################################
#RUN chown -R www-data:www-data ${ROOTDIR} \
# && chown -R www-data:www-data ${IMAGEROOTDIR} \
# && chown -R www-data:www-data ${LOCAL_INSTALL_DIR}

############################################################
## forward request and error logs to docker log collector ##
############################################################
#RUN ln -sf /dev/stdout /var/log/nginx/access.log \
# && ln -sf /dev/stderr /var/log/nginx/error.log

# STOPSIGNAL SIGTERM

# CMD ["apachectl", "-D", "FOREGROUND"]

############################################################################################

FROM build AS app

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR
ARG GIT_FOLDER

WORKDIR ${GIT_FOLDER}

############
# pull src #
############
# RUN git clone --depth 1 https://github.com/IvanKuchin/backend_pi.git ./src/pi
# COPY . ${GIT_FOLDER}
# WORKDIR ${GIT_FOLDER}

##################### 
# build from source #
##################### 
# RUN mkdir build
# RUN cd build \ 
#     && cmake \
#         -DPROJECT_NAME=${BUILD_REPO} \
#         -DMAIN_SUBDOMAIN=${MAIN_SUBDOMAIN} \
#         -DIMAGE_SUBDOMAIN=${IMAGE_SUBDOMAIN} \
#         -DROOTDIR=${ROOTDIR} \
#         -DIMAGEROOTDIR=${IMAGEROOTDIR} \
#         -DLOCAL_INSTALL_DIR=${LOCAL_INSTALL_DIR} \
#         -DBUILD_RELEASE_NUMBER=${BUILD_RELEASE_NUMBER} \
#         .. \
#     && make -j2 install

############################################################
## forward request and error logs to docker log collector ##
############################################################
RUN ln -sf /dev/stdout ${ROOTDIR}/logs/access.log \
 && ln -sf /dev/stderr ${ROOTDIR}/logs/error.log

####################################################################
# change folders to proper owner                                   #
####################################################################
RUN chown -R www-data:www-data ${ROOTDIR} \
 && chown -R www-data:www-data ${IMAGEROOTDIR} \
 && chown -R www-data:www-data ${LOCAL_INSTALL_DIR}

EXPOSE 80

CMD ["apachectl", "-D", "FOREGROUND"]

############################################################################################

FROM build AS uploaders

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR
ARG GIT_FOLDER

WORKDIR ${GIT_FOLDER}

####################
# copy from build  #
####################
# COPY --from=build ${GIT_FOLDER}/build/${BUILD_REPO}-infra.deb ./
# COPY --from=build ${GIT_FOLDER}/build/${BUILD_REPO}-uploaders.deb ./
# COPY --from=build ${GIT_FOLDER}/supplemental/apache2/ /etc/apache2/

####################
# install binaries #
####################
# RUN apt update \
#     && apt-get install --no-install-recommends --no-install-suggests -y \
#                apache2 \
#     && apt install  --no-install-recommends --no-install-suggests -y ffmpeg \
#     && apt install  --no-install-recommends --no-install-suggests -y ./*.deb \
#     && apt autoremove -y software-properties-common \
#     && rm -rf /var/lib/apt/lists/*

#########################
# configure mail sender #
#########################
#RUN sed -i "s/#rewriteDomain=/rewriteDomain=${BUILD_REPO}/g" /etc/ssmtp/ssmtp.conf

#####################
# configure apache2 #
#####################
# RUN mkdir -p ${IMAGEROOTDIR}
# RUN mkdir ${ROOTDIR}/logs/

# RUN a2enmod cgi \
#     && a2enmod rewrite \
#     && a2enmod expires \
#     && a2enconf mod_expires \
#     && a2dissite `ls /etc/apache2/sites-enabled/` \
#     && a2ensite *www

############################################################
## forward request and error logs to docker log collector ##
############################################################
RUN ln -sf /dev/stdout ${ROOTDIR}/logs/access.log \
 && ln -sf /dev/stderr ${ROOTDIR}/logs/error.log

####################################################################
# change folders to proper owner                                   #
####################################################################
RUN chown -R www-data:www-data ${ROOTDIR} \
 && chown -R www-data:www-data ${IMAGEROOTDIR} \
 && chown -R www-data:www-data ${LOCAL_INSTALL_DIR}

EXPOSE 80

CMD ["apachectl", "-D", "FOREGROUND"]

############################################################################################


FROM build AS cron

ARG BUILD_REPO
ARG MAIN_SUBDOMAIN
ARG IMAGE_SUBDOMAIN
ARG ROOTDIR
ARG IMAGEROOTDIR
ARG LOCAL_INSTALL_DIR
ARG GIT_FOLDER

WORKDIR ${GIT_FOLDER}

####################
# copy from build  #
####################
#COPY --from=build ${GIT_FOLDER}/build/${BUILD_REPO}-infra.deb ./
#COPY --from=build ${GIT_FOLDER}/build/${BUILD_REPO}-cron.deb ./

##############################
# install and configure cron #
##############################
RUN cp ${LOCAL_INSTALL_DIR}/cron/cron_config /etc/cron.d/www
RUN chmod 644 /etc/cron.d/www \
    && chown root:root /etc/cron.d/www

####################################################################
# change folders to proper owner                                   #
####################################################################
RUN chown -R www-data:www-data ${LOCAL_INSTALL_DIR}

#############################
# pre-create empty log-file #
#############################
RUN touch ${LOCAL_INSTALL_DIR}/log/backend.log
RUN chown www-data:www-data ${LOCAL_INSTALL_DIR}/log/backend.log

CMD ["sh", "-c", "cron && tail -f /usr/local/share/www/log/backend.log"]
# CMD ["cron", "-f"]

############################################################################################
