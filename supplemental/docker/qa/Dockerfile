FROM ubuntu:xenial

ARG DB_NAME
ARG DB_LOGIN
ARG DB_PASSWORD
ARG DB_HOST
ARG BUILD_REPO

ARG GIT_FOLDER=/backend

LABEL maintainer="ivan.kuchin@gmail.com"
LABEL org.opencontainers.image.source="https://github.com/IvanKuchin/__BUILD_REPO__"


###########################################
# check if all mandatory args are defined #
###########################################
RUN [ -z "$BUILD_REPO" ] && echo "BUILD_REPO is required" && exit 1 || true
RUN echo repository is $BUILD_REPO

# following two commands suppose to be reused from cache
RUN apt update &&  apt install --no-install-recommends --no-install-suggests -y git ca-certificates

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

#####################
# locale generation #
#####################
RUN apt-get install --no-install-recommends --no-install-suggests -y locales
RUN locale-gen ru_RU.utf8
RUN locale-gen en_US.utf8
RUN update-locale
RUN export LC_ALL=en_US.utf8

#############################
# libwkhtmltox installation #
#############################
RUN apt-get install --no-install-recommends --no-install-suggests -y wget
RUN wget https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.trusty_amd64.deb
RUN apt install --no-install-recommends --no-install-suggests -y ./wkhtmltox_0.12.5-1.trusty_amd64.deb

###############
# pull PI src #
###############
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
RUN cd build \
    && cmake \
       -DPROJECT_NAME=__BUILD_REPO__ \
       -DMAIN_SUBDOMAIN=www \
       -DIMAGE_SUBDOMAIN=images \
       .. \
    && make -j2

###########################
# test connectivity to DB #
###########################
# RUN apt install --no-install-recommends --no-install-suggests -y mysql-client
# RUN mysqladmin -u $DB_LOGIN -p$DB_PASSWORD -h $DB_HOST ping

################
# run QA tests #
################
# RUN cd build && ctest --output-on-failure

###################
# DB installation #
###################
# RUN ["/bin/bash", "-c", "debconf-set-selections <<< 'mysql-server mysql-server/root_password password $DB_PASSWORD'"]
# RUN ["/bin/bash", "-c", "debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password $DB_PASSWORD'"]
# RUN debconf-show mysql-server
# RUN apt-get install --no-install-recommends --no-install-suggests -y mysql-server

###########
# DB init #
###########
# RUN mysql -h localhost -P 3306 -u $DB_LOGIN -p$DB_PASSWORD
# RUN ["/bin/bash", "-c", "mysql -u $DB_LOGIN -p$DB_PASSWORD <<< 'CREATE DATABASE $DB_NAME;'"]
# RUN echo CREATE USER $DB_LOGIN; | mysql -u $DB_LOGIN -p$DB_PASSWORD
# RUN echo GRANT ALL ON $DB_NAME.* to '$DB_LOGIN'@'localhost' identified by '$DB_PASSWORD'; | mysql  -u $DB_LOGIN -p$DB_PASSWORD
# COPY ./supplemental/docker/qa/sql /tmp/
# RUN mysql -u $DB_LOGIN -p$DB_PASSWORD $DB_NAME < /tmp/sql

############################################################
## forward request and error logs to docker log collector ##
############################################################
#RUN ln -sf /dev/stdout /var/log/nginx/access.log \
# && ln -sf /dev/stderr /var/log/nginx/error.log

##########################
# DB tables provisioning #
##########################
# RUN apt install -f --no-install-recommends --no-install-suggests -y mysql-client
# RUN chmod +x ./supplemental/docker/qa/run.sh

STOPSIGNAL SIGTERM

CMD ["/bin/bash", "-c", "cd ./build && ctest --output-on-failure"]
