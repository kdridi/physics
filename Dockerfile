FROM epitechcontent/epitest-docker
LABEL maintainer="Karim DRIDI <karim.dridi@gmail.com>"

RUN dnf install -y cloc file jq emacs gdb vim libasan-static libasan man-pages man-pages-fr --setopt='tsflags='
RUN dnf install -y meson ninja-build inotify-tools freealut-devel htop moreutils doxygen libXinerama-devel libXrandr-devel libXcursor-devel libXi-devel libXinerama-devel

RUN mandb
