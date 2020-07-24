FROM ubuntu:xenial

LABEL maintainer="ikuchin@ikuchin.com"

ENV VERSION   0.1


#RUN debconf-set-selections <<< 'mysql-server mysql-server/root_password password root'
#RUN debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password root'
RUN echo mysql-server mysql-server/root_password password root | debconf-set-selections
RUN echo mysql-server mysql-server/root_password_again password root | debconf-set-selections 
RUN apt-get update \
	&& apt-get install --no-install-recommends --no-install-suggests -y software-properties-common \
	&& add-apt-repository -y ppa:jonathonf/ffmpeg-3 \
	&& apt update \
	&& apt-get install --no-install-recommends --no-install-suggests -y graphicsmagick-libmagick-dev-compat libgraphicsmagick++-dev libwebp-dev libmaxminddb-dev ffmpeg git libmysqlclient-dev libssl-dev libavutil-dev libavcodec-dev libavformat-dev libcurl4-openssl-dev build-essential cmake pkg-config mysql-server libhpdf-dev

COPY my_script.sh /root/
COPY sql /root/
COPY pre.sql /root/
RUN  chmod +x /root/my_script.sh

# forward request and error logs to docker log collector
#RUN ln -sf /dev/stdout /var/log/nginx/access.log \
#	&& ln -sf /dev/stderr /var/log/nginx/error.log

STOPSIGNAL	SIGTERM

CMD ["/root/my_script.sh"]
