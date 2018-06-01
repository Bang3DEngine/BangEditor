#ifndef UIINPUTFILE_H
#define UIINPUTFILE_H

#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UIInputText;
FORWARD class UIInputNumber;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputFile : public GameObject,
                    public EventEmitter<IEventsValueChanged>,
                    public EventListener<IEventsDragDrop>
{
    GAMEOBJECT_EDITOR(UIInputFile);

public:
    virtual void SetPath(const Path &path);
    void SetExtensions(const Array<String> &extensions);

    Path GetPath() const;
    UIInputText *GetInputText() const;
    const Array<String>& GetExtensions() const;

protected:
	UIInputFile();
    virtual ~UIInputFile();

    // IEventsDragDrop
    virtual void OnDragStarted(EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDragUpdate(EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable) override;

private:
    Path m_path = Path("undef"); // Set to empty in constructor
    Array<String> m_extensions;

    UIButton *p_searchButton = nullptr;
    UIInputText *p_pathInputText = nullptr;
    UIButton *p_openFileInInspectorButton = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTFILE_H

