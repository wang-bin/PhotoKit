PhotoKit
==============

PhotoKit is a photo browser with impressive 3D effects and many slide effects. 

Features:
* 3D effects
* Slide effects (my another project NextEffect).
* Multi-touch (e.g. on N9)
* EXIF information.
* Share images to sina weibo. (for Chinese users)
* Online images with Google image search.
* Build debian package (make deb).


PhotoKit can not manager photos now, just viewing. It will be possible in the future.


The UI of PhotoKit is based on Qt graphics framework (except filedialog), so you will have the same experience on all platforms. On N9, it only supports landscape mode because the limitation of QWidget. At the begining, i just use many QWidget based components, but they are very ugly on Harmattan, so I have to wrote these components using graphics framework, e.g. button, textinput and dialog. The work is hard, but the result is good.

Build
------
1. Clone the project including submodules:
  
  git clone --recursive git://github.com/wang-bin/PhotoKit.git

  git  submodule foreach git checkout master
  
2. BUILD_DIR=/your/build/dir qmake -r 

  or qmake -r  to build in $PWD/out

  or cd build_dir ; qmake /path/to/PhotoKit.pro

3. make -j4

  The binaries is in $BUILD_DIR/bin

4. Build package

  building debian package is supported now(tested for ubuntu 12.04). Use the following command

  make deb

5. Update source code

  git pull

  git submodule update

Screenshot
-------

![Alt text](https://github.com/downloads/wang-bin/PhotoKit/Screenshot-854x480.png "screenshot")
