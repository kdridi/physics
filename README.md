# Come Out & Play

Please try this game on [Brayan's dedicated server](http://146.59.150.207:9090/)

# Next Steps

- step 1 : the player can rename himself
- step 2 : establish a leaderboard
- step 3 : determine who is scoring
- step 4 : reset the game every 5 minutes

# Roadmap

- launch the game fullscreen on mobile devices
- use cookies to save the name player
- use google/facebook oauth identification
- serialize the leaderboard on diskfile
- serialize the leaderboard on cloud database

# Compilation and launch with docker

```sh
git clone https://github.com/kdridi/physics.git
cd physics/
git submodule update --init --recursive
docker build -t kdridi/ggj2021 .
docker run --rm --name ggj2021 -it -v $PWD:/mnt -w /mnt -p 9090:9090 -p 9091:9091 kdridi/ggj2021 bash
scripts/install-Linux.sh
scripts/XX-makefile.sh
```

# Compilation and launch with fedora

```sh
sudo dnf install -y gdb vim libasan-static libasan libXinerama-devel libXrandr-devel libXcursor-devel libXi-devel libXinerama-devel git cmake clang openssl-devel zlib-devel

git clone https://github.com/kdridi/physics.git
cd physics/
git submodule update --init --recursive

scripts/install-Linux.sh
scripts/XX-makefile.sh

git pull origin master
make -C system/Linux/makefile && system/Linux/makefile/GGJ2021
```

# Compilation and launch with ubuntu

```sh
sudo apt install -y gdb vim libasan6 libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev git cmake clang libssl-dev zlib1g-dev

git clone https://github.com/kdridi/physics.git
cd physics/
git submodule update --init --recursive

scripts/install-Linux.sh
scripts/XX-makefile.sh

git pull origin master
make -C system/Linux/makefile && system/Linux/makefile/GGJ2021
```

# Compilation and launch with MacOS

```sh
brew install cmake

git clone https://github.com/kdridi/physics.git
cd physics/
git submodule update --init --recursive

scripts/install-MacOC.sh
scripts/XX-makefile.sh

git pull origin master
make -C system/Linux/makefile && system/Linux/makefile/GGJ2021
```

# Compilation and development with MacOS/XCode

```sh
brew install cmake

git clone https://github.com/kdridi/physics.git
cd physics/
git submodule update --init --recursive

scripts/install-MacOC.sh
scripts/XX-xcode.sh
```

# Development on gitpod

Please try [this link](https://gitpod.io/#https://github.com/kdridi/physics)
