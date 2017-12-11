#ifndef UIINPUTVECTOR_H
#define UIINPUTVECTOR_H

#include "Bang/Array.h"
#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class UIInputNumber;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputVector : public GameObject,
                      public IValueChangedListener,
                      public EventEmitter<IValueChangedListener>
{
    GAMEOBJECT_EDITOR(UIInputVector);

public:
    void SetLabelText(const String &text);
    void SetSize(int size);

    void Set(const Vector2 &v);
    void Set(const Vector3 &v);
    void Set(const Vector4 &v);
    void Set(int i, float v);

    float Get(int i) const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;

    bool HasFocus() const;
    void SetPreferredWidth(int width);

    void OnValueChanged(Object *object) override;

protected:
    UIInputVector();
    UIInputVector(const String &labelText, int size);
    virtual ~UIInputVector();

private:
    Array<UIInputNumber*> m_inputNumbers;

    UILabel *p_label = nullptr;

};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTVECTOR_H

