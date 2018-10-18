#ifndef UIINPUTFILE_H
#define UIINPUTFILE_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsDragDrop;
class IEventsValueChanged;
class UIButton;
class UIInputNumber;
class UIInputText;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile : public GameObject,
                    public EventEmitter<IEventsValueChanged>,
                    public EventListener<IEventsDragDrop>
{
    GAMEOBJECT_EDITOR(UIInputFile);

public:
    UIInputFile();

    virtual void SetPath(const Path &path);
    void SetExtensions(const Array<String> &extensions);

    Path GetPath() const;
    UIInputText *GetInputText() const;
    const Array<String> &GetExtensions() const;

protected:
    virtual ~UIInputFile() override;

    // IEventsDragDrop
    virtual void OnDragStarted(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDragUpdate(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable) override;

private:
    Path m_path = Path("undef");  // Set to empty in constructor
    Array<String> m_extensions;

    UIButton *p_searchButton = nullptr;
    UIInputText *p_pathInputText = nullptr;
    UIButton *p_openFileInInspectorButton = nullptr;
};
}

#endif  // UIINPUTFILE_H
