# Build
FROM debian:latest

WORKDIR /app/back
COPY ./back /app/back

RUN apt-get update && apt-get upgrade && apt-get install -y cmake gcc g++
RUN cmake -G"Unix Makefiles" -S. -B./build  \
  -DCMAKE_BUILD_TYPE:STRING=Release         \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE
RUN cmake --build ./build --config Release --target all

EXPOSE 8080

# Run
WORKDIR /app/back/build/src
ENTRYPOINT [ "./eePARS_app" ]