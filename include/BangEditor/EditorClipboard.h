#ifndef EDITORCLIPBOARD_H
#define EDITORCLIPBOARD_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Path.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Component;
class GameObject;
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class EditorClipboard
{
public:
    EditorClipboard() = default;
    virtual ~EditorClipboard() = default;

    static void CopyComponent(Component *component);
    static Component *GetCopiedComponent();
    static bool HasCopiedComponent();
    static void ClearCopiedComponent();

    static void CopyGameObject(GameObject *gameObject);
    static GameObject *GetCopiedGameObject();
    static bool HasCopiedGameObject();
    static void ClearCopiedGameObject();

    static void CopyPath(const Path &path);
    static const Path &GetCopiedPath();
    static bool HasCopiedPath();
    static void ClearCopiedPath();

private:
    Component *m_copiedComponent = nullptr;
    GameObject *m_copiedGameObject = nullptr;
    Path m_copiedPath = Path::Empty();

    static EditorClipboard *GetInstance();
};
}  // namespace BangEditor

#endif  // EDITORCLIPBOARD_H
