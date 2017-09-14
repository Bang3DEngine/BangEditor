#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "BangEditor/BangEditor.h"

#include "Bang/Scene.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class UIGameObject;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorScene : public Scene
{
public:
    EditorScene();
    virtual ~EditorScene();

    // Scene
    virtual void Update() override;
    virtual void ParentSizeChanged() override;

    void SetOpenScene(Scene *openScene);

    Scene *GetOpenScene() const;

private:
    Scene *p_openScene = nullptr;
    UIGameObject *m_openSceneContainer = nullptr;
    UIImageRenderer *m_openSceneImage  = nullptr;
    Texture2D *m_openSceneRenderTexture = nullptr;

    void RenderEditor();

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
