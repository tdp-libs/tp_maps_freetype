#ifndef tp_maps_freetype_Globals_h
#define tp_maps_freetype_Globals_h

#include "tp_utils/Globals.h"

#if defined(TP_MAPS_FREETYPE_LIBRARY)
#  define TP_MAPS_FREETYPE_SHARED_EXPORT TP_EXPORT
#else
#  define TP_MAPS_FREETYPE_SHARED_EXPORT TP_IMPORT
#endif

//##################################################################################################
//! Support for rendering fonts in tp_maps using freetype.
namespace tp_maps_freetype
{

//##################################################################################################
const char* ftErrorMessage(int error);

}

#endif
