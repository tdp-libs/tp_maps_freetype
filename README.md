
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
INCLUDEPATH  += C:\FreeType\freetype-2.9.1\include
LIBRARYPATHS += C:\FreeType\freetype-windows-binaries\win64

```
