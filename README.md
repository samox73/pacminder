# pacminder
Pacminder is a simple helper for you not to forget to update your system every
once in a while.

## Installation

#### Linux Arch: Install from the AUR
For Linux Arch users there is a package in the AUR. Just execute

```
yay -S pacminder
```


#### Manual install from Source

Clone this repository, cd into it and create a build directory

```
git clone https://github.com/samox73/pacminder.git
cd pacminder
mkdir build
```
then enter the build directory, run cmake, execute the resulting Makefile and
install as super user:

```
cd build
cmake ..
make
sudo make install
```
