FROM gitpod/workspace-full

USER gitpod

RUN sudo apt-get -q update && \
    sudo apt-get install -yq gdb vim libasan6 libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev git cmake clang libssl-dev zlib1g-dev && \
    sudo rm -rf /var/lib/apt/lists/*