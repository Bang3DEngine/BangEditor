#ifndef EDITSCENEGAMEOBJECTS_H
#define EDITSCENEGAMEOBJECTS_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/GizmosManager.h"

namespace Bang
{
class Scene;
}

using namespace Bang;
namespace BangEditor
{
class EditorCamera;
class EditorFloor;
class GizmosManager;

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
    GizmosManager *GetGizmosManager() const;

    static EditSceneGameObjects *GetInstance();

private:
    EditorFloor *m_editorFloor = nullptr;
    EditorCamera *m_editorCamera = nullptr;
    GizmosManager *m_gizmosManager = nullptr;

    friend class EditorCamera;
};
}

#endif  // EDITSCENEGAMEOBJECTS_H
