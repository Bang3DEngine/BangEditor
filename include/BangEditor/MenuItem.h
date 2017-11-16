#ifndef MENUITEM_H
#define MENUITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/List.h"
#include "Bang/UIButton.h"
#include "Bang/GameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UIVerticalLayout;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuItem : public GameObject,
                 public IUIButtonListener
{
    GAMEOBJECT(MenuItem)

public:
    // GameObject
    void Update() override;

    void AddSeparator();
    void AddItem(MenuItem *childItem);
    MenuItem* AddItem(const String &text);

    UITextRenderer *GetText() const;
    UIButton *GetButton() const;

    bool MustDisplayChildren() const;

protected:
    MenuItem() = default;
    MenuItem(bool topItem);
    virtual ~MenuItem();

private:
    UIButton *m_button = nullptr;
    UITextRenderer *m_text = nullptr;

    List<MenuItem*> m_childrenItems;
    GameObject *m_childrenContainer = nullptr;
    UIVerticalLayout *m_childrenContainerVL = nullptr;

    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUITEM_H
