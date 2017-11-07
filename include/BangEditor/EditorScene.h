#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Bang/Rect.h"
#include "Bang/Scene.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class GameObject;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Console;
FORWARD class MenuBar;
FORWARD class Explorer;
FORWARD class Inspector;
FORWARD class Hierarchy;
FORWARD class UISceneContainer;

class EditorScene : public Scene
{
public:
    EditorScene();
    virtual ~EditorScene();

    void PreUpdate() override;
    void Update() override;
    void PostUpdate() override;
    virtual void OnResize(int newWidth, int newHeight);

    void RenderOpenScene();
    void SetViewportForOpenScene();

    void SetOpenScene(Scene *openScene);
    Scene *GetOpenScene() const;

    Rect GetOpenSceneRectNDC() const;

    void RenderAndBlitToScreen();
    static EditorScene *GetInstance();

    Console *GetConsole() const;
    Explorer *GetExplorer() const;
    Inspector *GetInspector() const;
    Hierarchy *GetHierarchy() const;

private:
    Console *m_console     = nullptr;
    Explorer *m_explorer   = nullptr;
    Inspector *m_inspector = nullptr;
    Hierarchy *m_hierarchy = nullptr;

    Scene *p_openScene = nullptr;
    UISceneContainer *m_sceneContainer = nullptr;

    MenuBar *m_menuBar = nullptr;
    GameObject *m_mainEditorVL = nullptr;

    Recti m_prevGLViewport = Recti::Zero;

    void SaveGLViewport();
    void LoadGLViewport();

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
