#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ColorPickerReporter;

class EditorDialog
{
public:
	EditorDialog();
	virtual ~EditorDialog();

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

    static Scene* CreateGetAssetSceneInto(Scene *scene,
                                          const Array<String> &extensions);
    static Scene* CreateGetColorSceneInto(Scene *scene,
                                          const Color &initialColor,
                                          ColorPickerReporter *colorPickerReporter);
};


class ColorPickerReporter : public GameObject,
                            public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(ColorPickerReporter);

public:
    void SetPickedColor(const Color &color);
    void SetHasFinished(bool hasFinished);

    Color GetPickedColor() const;
    bool HasFinished() const;

private:
    Color m_pickedColor = Color::White;
    bool m_hasFinished = true;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORDIALOG_H

