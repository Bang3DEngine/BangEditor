#ifndef EDITORAPPLICATION_H
#define EDITORAPPLICATION_H

#include "BangEditor/BangEditor.h"

#include "Bang/Application.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN
USING_NAMESPACE_BANG

class EditorApplication : public Application
{
public:
    EditorApplication(int argc, char **argv,
                      const Path &engineRootPath = Path::Empty);
    virtual ~EditorApplication();

    virtual void CreateWindow() override;
    virtual int MainLoop() override;
    virtual bool MainLoopIteration() override;

    virtual void UpdateScene() override;
    virtual void RenderScene() override;
    virtual void OnResize(int newWidth, int newHeight) override;

private:
    Texture2D *m_openSceneRenderTexture = nullptr;
    UIImageRenderer *m_openSceneImage = nullptr;
    Scene *m_openScene = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORAPPLICATION_H
