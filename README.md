
## OSX Installation
Install and locate the library.
```
brew install freetype
sudo /usr/libexec/locate.updatedb
locate ft2build.h
```
The locate command will return the path to the library, add this path to your projects top level project.inc file.
```
INCLUDEPATHS += /usr/local/Cellar/freetype/2.9.1/include/freetype2/
LIBRARYPATHS += /usr/local/Cellar/freetype/2.9.1/lib/

```

## Windows Installation
Download a release from:
* https://download.savannah.gnu.org/releases/freetype/

For example:
* https://download.savannah.gnu.org/releases/freetype/ft291.zip
 
Unpack them somewhere sensible for example ```C:\FreeType``` then clone the dlls from the git repo:
```
cd /c/FreeType/
git clone https://github.com/ubawurinna/freetype-windows-binaries.git

```

You will need paths something like this:
```
INCLUDEPATHS += C:\FreeType\freetype-2.9.1\include
LIBRARYPATHS += C:\FreeType\freetype-windows-binaries\win64

```

## Android Installation
Build a toolchain:
```
/home/tom/Android/Sdk/ndk/21.0.6113669/build/tools/make-standalone-toolchain.sh \
    --platform=android-16 \
    --install-dir=/home/tom/projects/external/android-ndk-16


```

Download a release:
* https://download.savannah.gnu.org/releases/freetype/

Build:
```
tar xf freetype-2.10.0.tar.bz2 
cd freetype-2.10.0/

export PATH=/home/tom/projects/external/android-ndk-16/bin:$PATH
export CFLAGS="-std=gnu99 -fPIE -fPIC"
export LDFLAGS="-pie"
./configure \
    --host=arm-linux-androideabi \
    --prefix=/freetype \
    --without-zlib \
    --without-bzip2 \
    --without-png \
    --without-harfbuzz

make -j40
mkdir ../install/
make install DESTDIR=`realpath ../install/`

```

You will need paths something like this:
```
INCLUDEPATHS += /home/tom/projects/external/android-freetype2/install/freetype/include/freetype2
LIBRARYPATHS += /home/tom/projects/external/android-freetype2/install/freetype/lib

```

#x86
```
./configure \
    --host=x86-linux-androideabi \
    --prefix=/freetype \
    --without-zlib \
    --without-bzip2 \
    --without-png \
    --without-harfbuzz

make -j40
mkdir ../install_x86/
make install DESTDIR=`realpath ../install_x86/`

```

#arm64-v8a
```
export target_host=aarch64-linux-android
export AR=$target_host-ar
export AS=$target_host-as
export CC=$target_host-gcc
export CXX=$target_host-g++
export LD=$target_host-ld
export STRIP=$target_host-strip

./configure \
    --build=BUILD \
    --host=$target_host \
    --prefix=/freetype \
    --without-zlib \
    --without-bzip2 \
    --without-png \
    --without-harfbuzz

make -j40
mkdir ../install_arm64-v8a/
make install DESTDIR=`realpath ../install_arm64-v8a/`

```
