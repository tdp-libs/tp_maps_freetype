
if(ANDROID)
  get_filename_component(FREETYPE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../android-freetype2" ABSOLUTE)
  list(APPEND TP_LIBRARIES "-L${FREETYPE_ROOT}/release/jni/${ANDROID_ABI}/")
  list(APPEND TP_INCLUDEPATHS "${FREETYPE_ROOT}/include")
else()
endif()

list(APPEND TP_LIBRARIES "-lfreetype")
