
mac {
  #- This needs to point to the SDK usually in /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/
  #CONFIG += sdk
  EXPORT_QMAKE_MAC_SDK = macosx15.2
  #QT_MAC_SDK_NO_VERSION_CHECK = 1

  #- Apple removed the AGL framework after SDK 15, but Qt's mkspecs still ask
  #- for it, so the link fails with "framework 'AGL' not found". It cannot be
  #- dropped from here, because qt_lib_gui_private.pri re-adds it to
  #- QMAKE_LIBS_OPENGL after this file is read. The strip_agl target in the
  #- top level Makefile removes it from the generated makefiles instead.

  #- Qt warns that it has only been tested against SDK 15. Building against a
  #- newer SDK works, so silence the four line warning on every qmake run.
  CONFIG += sdk_no_version_check
}
