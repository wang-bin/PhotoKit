PhotoKit
==============

PhotoKit is a photo browser with impressive 3D effects and many slide effects. Support viewing EXIF information.
You can share the image to sina weibo easily with it.
You can also view the online images with Google image search.
PhotoKit can not manager photos now, just viewing. It will be possible in the future.


The UI of PhotoKit is based on Qt graphics framework (except filedialog), so you will have the same experience on all platforms. On N9, it only supports landscape mode because the limitation of QWidget. At the begining, i just use many QWidget based components, but they are very ugly on Harmattan, so I have to wrote these components using graphics framework, e.g. button, textinput and dialog. The work is hard, but the result is good.

Build
------
1. clone the project including submodules:
  
  git clone --recursive git://github.com/wang-bin/PhotoKit.git

  git  submodule foreach git checkout master
  
2. BUILD_DIR=/your/build/dir qmake -r
3. make -j4

The binaries is in $BUILD_DIR/bin

Screenshot
-------

![Alt text](https://github.com/wang-bin/PhotoKit/raw/master/screenshot/Screenshot-854x480.png "desktop version")
