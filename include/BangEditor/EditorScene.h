#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Bang/Rect.h"
#include "Bang/Scene.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/IEditorOpenSceneListener.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class GameObject;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Editor;
FORWARD class Console;
FORWARD class MenuBar;
FORWARD class Explorer;
FORWARD class Inspector;
FORWARD class Hierarchy;
FORWARD class ProjectManager;
FORWARD class UISceneContainer;
FORWARD class EditorApplication;

class EditorScene : public Scene,
                    public EventEmitter<IEditorOpenSceneListener>
{
    GAMEOBJECT_EDITOR(EditorScene);

public:
    void Update() override;
    virtual void OnResize(int newWidth, int newHeight);

    void RenderOpenScene();
    void SetViewportForOpenScene();

    void SetOpenScene(Scene *openScene);
    Scene *GetOpenScene() const;

    Rect GetOpenSceneScreenRectNDC() const;

    void RenderAndBlitToScreen();

    Editor *GetEditor() const;
    Console *GetConsole() const;
    Explorer *GetExplorer() const;
    Inspector *GetInspector() const;
    Hierarchy *GetHierarchy() const;
    ProjectManager *GetProjectManager() const;

protected:
    EditorScene();
    virtual ~EditorScene();

private:
    Editor *m_editor = nullptr;
    ProjectManager *m_projectManager = nullptr;

    Console *m_console     = nullptr;
    Explorer *m_explorer   = nullptr;
    Inspector *m_inspector = nullptr;
    Hierarchy *m_hierarchy = nullptr;

    Scene *p_openScene = nullptr;
    UISceneContainer *m_sceneContainer = nullptr;
    UITextRenderer *m_fpsText = nullptr;

    MenuBar *m_menuBar = nullptr;
    GameObject *m_mainEditorVL = nullptr;

    Recti m_prevGLViewport = Recti::Zero;

    void Init();

    void BindOpenScene();
    void UnBindOpenScene();
    void PushGLViewport();
    void PopGLViewport();

    friend class BangEditor::EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
