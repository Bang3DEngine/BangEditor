#ifndef UIINPUTTEXTURE_H
#define UIINPUTTEXTURE_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/ResourceHandle.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFileWithPreview.h"

namespace Bang
{
class Path;
class Texture2D;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class UIInputTexture : public UIInputFileWithPreview
{
public:
    UIInputTexture();
    virtual ~UIInputTexture() override;

    virtual RH<Texture2D> GetPreviewTextureFromPath(const Path &path) override;
};
}

#endif  // UIINPUTTEXTURE_H
