#ifndef tp_maps_freetype_FreeTypeFont_h
#define tp_maps_freetype_FreeTypeFont_h

#include "tp_maps_freetype/Globals.h"

#include "tp_maps/Font.h"

namespace tp_maps_freetype
{

//##################################################################################################
class TP_MAPS_FREETYPE_SHARED_EXPORT FreeTypeFont : public tp_maps::Font
{
public:
  //################################################################################################
  FreeTypeFont();

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
