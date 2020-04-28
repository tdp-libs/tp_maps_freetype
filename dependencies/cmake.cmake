
if(ANDROID)
  set(FREETYPE_ROOT "/home/tom/projects/external/android-freetype2/android-freetype2/")
  list(APPEND TP_LIBRARIES "-L${FREETYPE_ROOT}/debug/jni/${ANDROID_ABI}/")
else()
endif()

list(APPEND TP_LIBRARIES "-lfreetype")
