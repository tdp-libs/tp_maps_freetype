#include "tp_maps_freetype/fonts/FreeTypeFont.h"

#include "tp_maps/textures/BasicTexture.h"

#include "tp_utils/DebugUtils.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tp_maps_freetype
{

//##################################################################################################
struct FreeTypeFont::Private
{
  FT_Library  library;
  FT_Face     face;
};

//##################################################################################################
FreeTypeFont::FreeTypeFont():
  d(new Private())
{
  {
    auto error = FT_Init_FreeType(&d->library);
    if ( error )
    {
      tpDebug() << "Failed to initialize free type: " << ftErrorMessage(error);
    }
  }

  {
    auto error = FT_New_Face(d->library,
                             "/usr/share/fonts/liberation/LiberationSans-Bold.ttf",
                             0,
                             &d->face);
    if(error == FT_Err_Unknown_File_Format)
    {
      tpDebug() << "The font file could be opened and read, but it appears that its font format is unsupported.";
    }
    else if ( error )
    {
      tpDebug() << "New face error: " << ftErrorMessage(error);
    }
  }

  {
    auto error = FT_Set_Char_Size(d->face,    /* handle to face object           */
                                  0,       /* char_width in 1/64th of points  */
                                  4*64,    /* char_height in 1/64th of points */
                                  200,     /* horizontal device resolution    */
                                  200 );   /* vertical device resolution      */
    if ( error )
    {
      tpDebug() << "Set char size error: " << ftErrorMessage(error);
    }
  }
}

//##################################################################################################
FreeTypeFont::~FreeTypeFont()
{
  if(d->face)
    FT_Done_Face(d->face);

  if(d->library)
    FT_Done_FreeType(d->library);

  delete d;
}

//##################################################################################################
void FreeTypeFont::prepareGlyph(char16_t character, const std::function<void(const tp_maps::Glyph&)>& addGlyph) const
{
  if(!d->face)
    return;

  auto glyphIndex = FT_Get_Char_Index(d->face, FT_ULong(character));
  if(!glyphIndex)
    return;

  if(auto error = FT_Load_Glyph(d->face, glyphIndex, FT_LOAD_DEFAULT); error)
  {
    tpDebug() << "Failed to load glyph: " << ftErrorMessage(error);
    return;
  }

  if(d->face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
  {
    if(auto error = FT_Render_Glyph(d->face->glyph, FT_RENDER_MODE_NORMAL); error)
    {
      tpDebug() << "Failed to render glyph: " << ftErrorMessage(error);
      return;
    }
  }

  std::vector<tp_maps::Pixel> glyphData;
  tp_maps::Glyph glyph;
  glyph.w             = int(d->face->glyph->bitmap.width);
  glyph.h             = int(d->face->glyph->bitmap.rows);
  glyph.kerningWidth  = float(d->face->glyph->metrics.horiAdvance)/64.0f;
  glyph.leftBearing   = float(d->face->glyph->bitmap_left);
  glyph.topBearing    = float(d->face->glyph->bitmap_top);
  glyph.bottomBearing = -(float(glyph.h) - float(d->face->glyph->bitmap_top));

  glyphData.resize(size_t(glyph.w*glyph.h));

  size_t rMax = size_t(glyph.h);
  size_t rr = rMax-1;
  size_t iMax = size_t(glyph.w);
  for(size_t r=0; r<rMax; r++)
  {
    size_t srcAdd = r * size_t(glyph.w);
    size_t dstAdd = (rr-r) * size_t(glyph.w);
    for(size_t i=0; i<iMax; i++)
    {
      auto src = d->face->glyph->bitmap.buffer[srcAdd+i];
      auto& dst = glyphData[dstAdd+i];
      dst.r = 0;
      dst.g = 0;
      dst.b = 0;
      dst.a = src;
    }
  }
  glyph.data = glyphData.data();

  addGlyph(glyph);
}

//##################################################################################################
float FreeTypeFont::lineHeight() const
{
  return 10.0f;
}

}
