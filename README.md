ExifMover
=========

Just a small project to freshen up my Qt knowledge (and learn git *gg*)... :o)

Goal of this tiny project is to have a directory with lots of JPEGs from my digicam in it.
They are to be sorted (i.e. moved) in subdirectories of the format YYYY-MM-DD.

For now, libexif is used to read the creation timestamps from the JPEG files. This means ExifMover cannot be compiled under MS Windows or OS X without installed libexif.

Please also note I am using QtCreator for development of ExifMover.
Therefore the object files and binary output is currently put to ../ExifMover-build-desktop

