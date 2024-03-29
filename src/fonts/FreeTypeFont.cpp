#include "tp_maps_freetype/fonts/FreeTypeFont.h"

#include "tp_utils/Resources.h"
#include "tp_utils/DebugUtils.h"
#include "tp_utils/TPPixel.h"

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
FreeTypeFont::FreeTypeFont(LoadFrom loadFrom, const std::string& data, int pointSize):
  d(new Private())
{
  {
    auto error = FT_Init_FreeType(&d->library);
    if(error)
    {
      tpWarning() << "Failed to initialize free type: " << ftErrorMessage(error);
    }
  }

  switch(loadFrom)
  {
  case LoadFrom::File: //---------------------------------------------------------------------------
  {
    auto error = FT_New_Face(d->library,
                             data.c_str(),
                             0,
                             &d->face);
    if(error == FT_Err_Unknown_File_Format)
      tpWarning() << "The font file could be opened and read, but it appears that its font format is unsupported.";
    else if(error)
      tpWarning() << "New face error: " << ftErrorMessage(error);

    break;
  }

  case LoadFrom::Resource: //-----------------------------------------------------------------------
  {
    tp_utils::Resource resource = tp_utils::resource(data);

    if(resource.data && resource.size>0)
    {
      auto error = FT_New_Memory_Face(d->library,
                                      reinterpret_cast<const FT_Byte*>(resource.data),
                                      FT_Long(resource.size),
                                      0,
                                      &d->face);
      if(error)
        tpWarning() << "New face error: " << ftErrorMessage(error);
    }
    else
      tpWarning() << "Failed to load resource: " << data;
    break;
  }

  case LoadFrom::Data: //---------------------------------------------------------------------------
  {
    auto error = FT_New_Memory_Face(d->library,
                                    reinterpret_cast<const FT_Byte*>(data.c_str()),
                                    FT_Long(data.size()),
                                    0,
                                    &d->face);
    if(error)
      tpWarning() << "New face error: " << ftErrorMessage(error);

    break;
  }
  }

  {
    auto error = FT_Set_Char_Size(d->face,      // handle to face object
                                  0,            // char_width in 1/64th of points
                                  pointSize*64, // char_height in 1/64th of points
                                  200,          // horizontal device resolution
                                  200 );        // vertical device resolution
    if(error)
    {
      tpWarning() << "Set char size error: " << ftErrorMessage(error);
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
    tpWarning() << "Failed to load glyph: " << ftErrorMessage(error);
    return;
  }

  if(d->face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
  {
    if(auto error = FT_Render_Glyph(d->face->glyph, FT_RENDER_MODE_NORMAL); error)
    {
      tpWarning() << "Failed to render glyph: " << ftErrorMessage(error);
      return;
    }
  }

  std::vector<TPPixel> glyphData;
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
      dst.r = 255;
      dst.g = 255;
      dst.b = 255;
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
