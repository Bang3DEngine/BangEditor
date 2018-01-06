#ifndef UISCENECONTAINER_H
#define UISCENECONTAINER_H

#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneContainer : public GameObject
{
    GAMEOBJECT_EDITOR(UISceneContainer);

public:
    void BeforeChildrenRender(RenderPass renderPass) override;
    void AfterChildrenRender(RenderPass renderPass) override;

    Rect GetImageScreenRectNDC() const;
    void SetSceneImageTexture(Texture2D *texture);

protected:
    UISceneContainer();
    virtual ~UISceneContainer();

private:
    GL::BlendFactor m_prevBlendFactorColorSrc, m_prevBlendFactorColorDst;
    GL::BlendFactor m_prevBlendFactorAlphaSrc, m_prevBlendFactorAlphaDst;
    bool m_wasBlendingEnabled = false;

    UIImageRenderer *p_sceneImg = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // SUBSCENE_H
