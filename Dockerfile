FROM ubuntu

RUN apt-get update -qq
RUN apt-get install -yq \
    git \
    curl \
    make \
    g++ \
    clang++-3.6
RUN sed -i '120d' /usr/include/c++/4.8/cstdio
RUN ln -s /usr/bin/clang++-3.6 /usr/bin/clang
RUN ln -s /usr/bin/clang++-3.6 /usr/bin/clang++

RUN curl -L \
    -o /boost.tar.bz2 \
    http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.bz2/download
RUN tar xf /boost.tar.bz2
RUN rm /boost.tar.bz2
WORKDIR boost_1_58_0/
RUN ./bootstrap.sh --with-toolset=clang --with-libraries=program_options
RUN ./b2 --link=static --runtime-link=static
RUN ln -s /boost_1_58_0/stage/lib/libboost_program_options.a /usr/lib/x86_64-linux-gnu/
RUN ln -s /boost_1_58_0/boost/ /usr/include/x86_64-linux-gnu/

WORKDIR /
RUN curl -o /ncurses.tar.gz \
    http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.9.tar.gz
RUN tar xf /ncurses.tar.gz
RUN rm /ncurses.tar.gz
WORKDIR /ncurses-5.9
RUN CC=clang CXX=clang++ ./configure
RUN make || ln -s /ncurses-5.9/include/* /usr/include/x86_64-linux-gnu/
RUN ln -s /ncurses-5.9/lib/* /usr/lib/x86_64-linux-gnu/

WORKDIR /
