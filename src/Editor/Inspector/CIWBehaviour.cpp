#include "BangEditor/CIWBehaviour.h"

#include <map>
#include <utility>
#include <vector>

#include "Bang/Assert.h"
#include "Bang/BPReflectedStruct.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/Behaviour.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/MetaAttribute.h"
#include "Bang/Serializable.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputVector.h"

namespace Bang {
class IEventsValueChanged;
template <class > class EventListener;
}  // namespace Bang


USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWBehaviour::CIWBehaviour()
{
}

CIWBehaviour::~CIWBehaviour()
{
}

void CIWBehaviour::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWBehaviour");
    SetTitle("Behaviour");

    SetLabelsWidth(80);
}

void CIWBehaviour::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    SetTitle(GetBehaviour()->GetClassName());

    MetaNode behaviourMeta;
    GetBehaviour()->ExportMeta(&behaviourMeta);
    m_ciwBehaviourHelper.UpdateWidgetsFromMeta(behaviourMeta);
}


void CIWBehaviour::OnComponentSet()
{
    ASSERT(GetBehaviour());

    // Clear
    for (GameObject *widget : m_ciwBehaviourHelper.GetWidgets())
    {
        RemoveWidget(widget);
    }

    // Add
    BPReflectedStruct reflectStruct = GetBehaviour()->GetReflectionInfo();
    m_ciwBehaviourHelper.RecreateWidgetsFromReflection(reflectStruct, this);
    for (GameObject *widget : m_ciwBehaviourHelper.GetWidgets())
    {
        String name = m_ciwBehaviourHelper.GetWidgetToReflectedVar().
                      Get(widget).GetName();
        AddWidget(name, widget);
    }
}

void CIWBehaviour::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    MetaNode meta = m_ciwBehaviourHelper.GetMetaFromWidgets();
    GetBehaviour()->ImportMeta(meta);
}

Behaviour *CIWBehaviour::GetBehaviour() const
{
    return SCAST<Behaviour*>(GetComponent());
}


// ===========================================================================

void CIWBehaviourHelper::RecreateWidgetsFromReflection(
                    const BPReflectedStruct &reflectStruct,
                    EventListener<IEventsValueChanged> *listener)
{
    // Clear
    m_widgets.Clear();
    m_widgetToReflectedVar.Clear();
    m_varNameToWidget.Clear();

    // Add
    for (const BPReflectedVariable &reflVar : reflectStruct.GetVariables())
    {
        GameObject *widgetToAdd = nullptr;
        String widgetName = reflVar.GetName();
        if (reflVar.IsOfType(BangPreprocessor::VarTypeFloat)  ||
            reflVar.IsOfType(BangPreprocessor::VarTypeDouble) ||
            reflVar.IsOfType(BangPreprocessor::VarTypeInt))
        {
            UIInputNumber *inputNumber =
                                GameObjectFactory::CreateUIInputNumber();
            inputNumber->SetValue(
                        String::ToFloat(reflVar.GetVariableInitValue()) );
            if (reflVar.IsOfType(BangPreprocessor::VarTypeInt))
            {
                inputNumber->SetDecimalPlaces(0);
            }
            inputNumber->EventEmitter<IEventsValueChanged>::
                         RegisterListener(listener);
            widgetToAdd = inputNumber->GetGameObject();
        }
        else if (reflVar.IsOfType(BangPreprocessor::VarTypeBool))
        {
            UICheckBox *checkBox = GameObjectFactory::CreateUICheckBox();
            checkBox->SetChecked(
                    (reflVar.GetVariableInitValue().EqualsNoCase("True")) );
            checkBox->EventEmitter<IEventsValueChanged>::
                      RegisterListener(listener);
            widgetToAdd = checkBox->GetGameObject();
        }
        else if (reflVar.IsOfType(BangPreprocessor::VarTypeString))
        {
            UIInputText *inputText = GameObjectFactory::CreateUIInputText();
            inputText->GetText()->SetContent(reflVar.GetVariableInitValue());
            inputText->EventEmitter<IEventsValueChanged>::
                       RegisterListener(listener);
            widgetToAdd = inputText->GetGameObject();
        }
        else if (reflVar.IsOfType(BangPreprocessor::VarTypeVector2) ||
                 reflVar.IsOfType(BangPreprocessor::VarTypeVector3) ||
                 reflVar.IsOfType(BangPreprocessor::VarTypeVector4) ||
                 reflVar.IsOfType(BangPreprocessor::VarTypeQuaternion))
        {
            int numComps = 4;
            if (reflVar.IsOfType(BangPreprocessor::VarTypeVector2))
            {
                numComps = 2;
            }
            else if (reflVar.IsOfType(BangPreprocessor::VarTypeVector3))
            {
                numComps = 3;
            }
            UIInputVector *inputVec = GameObject::Create<UIInputVector>();
            inputVec->SetSize(numComps);
            inputVec->Set( Vector4::Zero );
            inputVec->EventEmitter<IEventsValueChanged>::
                      RegisterListener(listener);
            widgetToAdd = inputVec;
        }
        else if (reflVar.IsOfType(BangPreprocessor::VarTypeColor))
        {
            UIInputColor *inputColor = GameObject::Create<UIInputColor>();
            inputColor->SetColor( Color::White );
            inputColor->EventEmitter<IEventsValueChanged>::
                        RegisterListener(listener);
            widgetToAdd = inputColor;
        }

        m_varNameToWidget.Add(reflVar.GetName(), widgetToAdd);
        m_widgetToReflectedVar.Add(widgetToAdd, reflVar);
        m_widgets.PushBack(widgetToAdd);
    }
}

void CIWBehaviourHelper::UpdateWidgetsFromMeta(const MetaNode &meta)
{
    for (const auto &pair : meta.GetAttributes())
    {
        const String &attrName = pair.first;
        const MetaAttribute &metaAttr = pair.second;

        auto it = GetVarNameToWidget().Find(attrName);
        if (it != GetVarNameToWidget().End())
        {
            GameObject *widget = it->second;
            if (UIInputVector *inputVec = DCAST<UIInputVector*>(widget))
            {
                inputVec->Set(metaAttr.Get<Vector4>());
            }
            else if (UIInputColor *inputColor = DCAST<UIInputColor*>(widget))
            {
                inputColor->SetColor(metaAttr.Get<Color>());
            }
            else if (UIInputNumber *inputNumber = widget->GetComponent<UIInputNumber>())
            {
                inputNumber->SetValue( metaAttr.Get<double>() );
            }
            else if (UIInputText *inputText = widget->GetComponent<UIInputText>())
            {
                inputText->GetText()->SetContent(metaAttr.Get<String>());
            }
            else if (UICheckBox *inputCheckBox = widget->GetComponent<UICheckBox>())
            {
                inputCheckBox->SetChecked( metaAttr.Get<bool>() );
            }
        }
    }
}

String GetStringValueFromWidget(GameObject *widget)
{
    String value = "";
    if (UIInputVector *inputVec = DCAST<UIInputVector*>(widget))
    {
        switch (inputVec->GetSize())
        {
            case 2:
                value = String::ToString( inputVec->GetVector2() );
            break;

            case 3:
                value = String::ToString( inputVec->GetVector3() );
            break;

            default:
                value = String::ToString( inputVec->GetVector4() );
            break;
        }
    }
    else if (UIInputColor *inputColor = DCAST<UIInputColor*>(widget))
    {
        value = String::ToString(inputColor->GetColor());
    }
    else if (UICheckBox *inputCheckBox = widget->GetComponent<UICheckBox>())
    {
        value = inputCheckBox->IsChecked() ? "True" : "False";
    }
    else if (UIInputNumber *inputNumber = widget->GetComponent<UIInputNumber>())
    {
        value = String::ToString(inputNumber->GetValue(),
                                 inputNumber->GetDecimalPlaces());
    }
    else if (UIInputText *inputText = widget->GetComponent<UIInputText>())
    {
        value = inputText->GetText()->GetContent();
    }
    return value;
}

MetaNode CIWBehaviourHelper::GetMetaFromWidgets() const
{
    MetaNode meta;
    for (const auto &it : GetWidgetToReflectedVar())
    {
        GameObject *widget = it.first;
        const BPReflectedVariable &reflVar = it.second;
        String value = GetStringValueFromWidget(widget);
        meta.Set(reflVar.GetName(), value);
    }
    return meta;
}

const Map<GameObject *, BPReflectedVariable> &CIWBehaviourHelper::GetWidgetToReflectedVar() const
{
    return m_widgetToReflectedVar;
}

const Map<String, GameObject *> &CIWBehaviourHelper::GetVarNameToWidget() const
{
    return m_varNameToWidget;
}

const Array<GameObject *> &CIWBehaviourHelper::GetWidgets() const
{
    return m_widgets;
}
