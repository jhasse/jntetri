FROM emscripten/emsdk:3.1.42
RUN apt-get update && apt-get install -y \
  ninja-build \
  && rm -rf /var/lib/apt/lists/*
RUN cd /tmp \
  && curl --silent -O -L https://archives.boost.io/release/1.82.0/source/boost_1_82_0.tar.bz2 \
  && tar --wildcards -xf *.tar.bz2 'boost_1_*_0/boost' \
  && rm boost_*.tar.bz2
