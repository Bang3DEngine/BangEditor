#ifndef UIINPUTVECTOR_H
#define UIINPUTVECTOR_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class IEventsValueChanged;
}  // namespace Bang

NAMESPACE_BANG_BEGIN
FORWARD class UIInputNumber;
FORWARD class UILabel;

NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputVector : public GameObject,
                      public EventListener<IEventsValueChanged>,
                      public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UIInputVector);

public:
    void SetSize(int size);

    void Set(const Vector2 &v);
    void Set(const Vector3 &v);
    void Set(const Vector4 &v);
    void Set(int i, float v);
    void SetMinValue(const Vector4 &minValue);
    void SetMaxValue(const Vector4 &maxValue);
    void SetMinMaxValues(const Vector4 &minValue,
                         const Vector4 &maxValue);

    float Get(int i) const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;

    int GetSize() const;

    const Array<UIInputNumber*> GetInputNumbers() const;

    bool HasFocus() const;

    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

protected:
    UIInputVector();
    UIInputVector(int size);
    virtual ~UIInputVector();

private:
    int m_size = 4;
    Array<UIInputNumber*> m_inputNumbers;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTVECTOR_H

