# pacminder
Pacminder is a simple helper for you not to forget to update pacman every once
in a while. 

To install it is simple. Clone this repository, cd into it and create a build directory
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
