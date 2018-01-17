#ifndef UITABHEADER_H
#define UITABHEADER_H

#include "Bang/String.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabHeader;
class ITabHeaderListener : public IEventListener
{
public:
    virtual void OnTabHeaderClicked(UITabHeader *header) = 0;
};

class UITabHeader : public GameObject,
                    public EventEmitter<ITabHeaderListener>
{
    GAMEOBJECT_EDITOR(UITabHeader);

public:
	UITabHeader();
	virtual ~UITabHeader();

    // GameObject
    void Update() override;

    void SetInForeground(bool inForeground);

    void SetTitle(const String &title);
    const String &GetTitle() const;

private:
    const Color ForegroundColor = Color::Zero;
    const Color BackgroundColor = Color::DarkGray.WithValue(1.3f);

    String m_title = "Tab";
    Color m_currentHeaderColor = Color::Zero;

    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UITextRenderer *p_titleText = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UITABHEADER_H

