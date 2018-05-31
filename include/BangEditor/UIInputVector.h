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
                      public EventListener<IValueChangedListener>,
                      public EventEmitter<IValueChangedListener>
{
    GAMEOBJECT_EDITOR(UIInputVector);

public:
    void SetSize(int size);

    void Set(const Vector2 &v);
    void Set(const Vector3 &v);
    void Set(const Vector4 &v);
    void Set(int i, float v);

    float Get(int i) const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;

    const Array<UIInputNumber*> GetInputNumbers() const;

    bool HasFocus() const;

    void OnValueChanged(Object *object) override;

protected:
    UIInputVector();
    UIInputVector(int size);
    virtual ~UIInputVector();

private:
    Array<UIInputNumber*> m_inputNumbers;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTVECTOR_H

