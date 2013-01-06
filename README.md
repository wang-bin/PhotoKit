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

Get the binary
--------------

I have built PhotoKit for many platforms, you can download them from sourceforge: https://sourceforge.net/projects/photokit/files

Including deb packages for Nokia N9 and N900, dmg for Mac OS X, and zip for Windows.

You can build PhotoKit yourself for linux, and building a deb is very easy, just "make deb"

You can find PhotoKit for OS2 there: http://svn.netlabs.org/qtapps/wiki/QT4%20Graphics

Build
------
1. Clone the project including submodules:
  
  git clone --recursive git://github.com/wang-bin/PhotoKit.git

  git  submodule foreach git checkout master
  
2. qmake -r "BUILD_DIR=/your/build/dir" [path/to/pro]

3. make -j4

  The binaries is in $BUILD_DIR/bin

4. Build package

  building debian package is supported now(tested for ubuntu 12.04, 8.04). Use the following command

  make deb

5. Update source code

  git pull

  git submodule update


NOTE: If you are using QtCreator to build the project, you should go to Projects->Build Steps->qmake->Additional arguments, add "BUILD_DIR=your/buid/dir".


Screenshot
-------

![Alt text](https://github.com/downloads/wang-bin/PhotoKit/Screenshot-854x480.png "screenshot")
