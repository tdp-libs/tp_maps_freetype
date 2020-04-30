#ifndef tp_maps_freetype_FreeTypeFont_h
#define tp_maps_freetype_FreeTypeFont_h

#include "tp_maps_freetype/Globals.h"

#include "tp_maps/Font.h"

namespace tp_maps_freetype
{

//##################################################################################################
enum class LoadFrom
{
  File,
  Resource,
  Data
};

//##################################################################################################
class TP_MAPS_FREETYPE_SHARED_EXPORT FreeTypeFont : public tp_maps::Font
{
public:
  //################################################################################################
  /*!
  \param loadFrom how to load the data.
  \param data depending on loadFrom this could be a file path, resource path, or binary data.
  */
  FreeTypeFont(LoadFrom loadFrom, const std::string& data);

  //################################################################################################
  ~FreeTypeFont() override;

  //################################################################################################
  void prepareGlyph(char16_t character, const std::function<void(const tp_maps::Glyph&)>& addGlyph) const override;

  //################################################################################################
  float lineHeight() const override;

private:
  struct Private;
  friend struct Private;
  Private* d;
};
}

#endif
