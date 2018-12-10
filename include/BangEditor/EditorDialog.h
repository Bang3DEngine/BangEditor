#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Path;
class Scene;
}

using namespace Bang;
namespace BangEditor
{
class ColorPickerReporter;

class EditorDialog
{
public:
    EditorDialog();
    virtual ~EditorDialog();

    static void GetObject(const String &title,
                          Object *resultObject,
                          bool *accepted);

    static void GetAsset(const String &title,
                         const Array<String> &extensions,
                         Path *resultPath,
                         bool *accepted);

    static void GetColor(const String &title,
                         const Color &initialColor,
                         ColorPickerReporter *colorPickerReporter);

private:
    static bool s_accepted;
    static Path s_assetPathResult;
    static Object *s_objectResult;

    static Scene *CreateSearchSceneInto(Scene *scene);
    static Scene *CreateGetObjectSceneInto(Scene *scene);
    static Scene *CreateGetAssetSceneInto(Scene *scene,
                                          const Array<String> &extensions);
    static Scene *CreateGetColorSceneInto(
        Scene *scene,
        const Color &initialColor,
        ColorPickerReporter *colorPickerReporter);
};

class ColorPickerReporter : public GameObject,
                            public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(ColorPickerReporter);

public:
    void SetPickedColor(const Color &color);

    Color GetPickedColor() const;
    bool HasFinished() const;
    bool IsPicking() const;

private:
    Color m_pickedColor = Color::White();
    bool m_isPicking = false;
    bool m_hasFinished = true;

    void SetIsPicking(bool isPicking);
    void SetHasFinished(bool hasFinished);

    friend class EditorDialog;
};
}

#endif  // EDITORDIALOG_H
