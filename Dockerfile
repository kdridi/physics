FROM fedora:32
LABEL maintainer="Karim DRIDI <karim.dridi@gmail.com>"

RUN dnf install -y gdb vim libasan-static libasan libXinerama-devel libXrandr-devel libXcursor-devel libXi-devel libXinerama-devel
RUN dnf install -y git cmake clang openssl-devel zlib-devel
RUN dnf install -y libasan-static libasan