#ifndef EDITSCENEGAMEOBJECTS_H
#define EDITSCENEGAMEOBJECTS_H

#include "Bang/Bang.h"

#include "BangEditor/SelectionGizmosManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorFloor;
FORWARD class EditorCamera;
FORWARD class SelectionGizmosManager;

class EditSceneGameObjects
{
public:
	EditSceneGameObjects();
    virtual ~EditSceneGameObjects();

    void Update();

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    EditorFloor *GetEditorFloor() const;
    EditorCamera *GetEditorCamera() const;
    SelectionGizmosManager *GetSelectionGizmosManager() const;

    static EditSceneGameObjects *GetInstance();

private:
    EditorFloor *m_editorFloor = nullptr;
    EditorCamera *m_editorCamera = nullptr;
    SelectionGizmosManager *m_selectionGizmosManager = nullptr;

    friend class EditorCamera;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITSCENEGAMEOBJECTS_H

