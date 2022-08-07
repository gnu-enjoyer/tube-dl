FROM debian:bullseye AS base-image

RUN apt-get update
RUN useradd --system --create-home tubedl

FROM gcc:11.2.0-bullseye as compile-image
RUN apt-get update
RUN apt-get install -y --no-install-recommends libssl-dev libjsoncpp-dev cmake
RUN useradd --system --create-home tubedl
USER tubedl
WORKDIR /home/tubedl

COPY --chown=tubedl . .
RUN mkdir build
WORKDIR /home/tubedl/build
RUN cmake ..
RUN cmake --build . --target tubedl

FROM base-image AS production-image

USER tubedl
WORKDIR /home/tubedl

COPY --from=compile-image /home/tubedl/build/tubedl /home/tubedl/tubedl
