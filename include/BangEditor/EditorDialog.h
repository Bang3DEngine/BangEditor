#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "BangMath/Color.h"
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
class NavigatorItem;
class ColorPickerReporter;

class EditorDialog
{
public:
    EditorDialog();
    virtual ~EditorDialog();

    static void GetObject(const String &title,
                          ClassIdType acceptedClassId,
                          ClassIdType acceptedClassIdEnd,
                          Object **resultObject,
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

    static void CreateSearchSceneInto(
        Scene *scene,
        const Array<String> &tabNames,
        const Array<Array<NavigatorItem *>> &tabNavItems,
        std::function<void(NavigatorItem *)> OnNavigatorItemSelected);
    static void CreateGetObjectSceneInto(Scene *scene,
                                         ClassIdType acceptedClassIdBegin,
                                         ClassIdType acceptedClassIdEnd,
                                         GameObject *baseGameObject);
    static void CreateGetAssetSceneInto(Scene *scene,
                                        const Array<String> &extensions);
    static void CreateGetColorSceneInto(
        Scene *scene,
        const Color &initialColor,
        ColorPickerReporter *colorPickerReporter);
};

class ColorPickerReporter : public GameObject,
                            public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(ColorPickerReporter);

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
