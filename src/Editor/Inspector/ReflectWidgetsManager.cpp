#include "BangEditor/ReflectWidgetsManager.h"

#include "Bang/GameObjectFactory.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"

#include "BangEditor/InspectorWidget.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputVector.h"

using namespace Bang;
using namespace BangEditor;

void ReflectWidgetsManager::UpdateWidgetsFromReflection(
    const ReflectStruct &reflectStruct,
    InspectorWidget *inspectorWidget)
{
    if (reflectStruct != m_previousReflectedStruct)
    {
        // Clear old reflect widgets (if any)
        for (GameObject *widget : GetWidgets())
        {
            inspectorWidget->RemoveWidget(widget);
        }

        // Clear structures
        m_widgets.Clear();
        m_widgetToReflectedVar.Clear();
        m_varNameToWidget.Clear();

        // Add widgets to structures
        for (const ReflectVariable &reflVar : reflectStruct.GetVariables())
        {
            GameObject *widgetToAdd = nullptr;
            String widgetName = reflVar.GetName();
            if (reflVar.GetVariant().GetType() == Variant::Type::FLOAT ||
                reflVar.GetVariant().GetType() == Variant::Type::DOUBLE ||
                reflVar.GetVariant().GetType() == Variant::Type::INT ||
                reflVar.GetVariant().GetType() == Variant::Type::UINT)
            {
                UIInputNumber *inputNumber =
                    GameObjectFactory::CreateUIInputNumber();
                if (reflVar.GetVariant().GetType() == Variant::Type::FLOAT ||
                    reflVar.GetVariant().GetType() == Variant::Type::DOUBLE)
                {
                    inputNumber->SetValue(reflVar.GetInitValue().GetFloat());
                }
                else
                {
                    inputNumber->SetValue(reflVar.GetInitValue().GetInt());
                }

                if (reflVar.GetVariant().GetType() == Variant::Type::INT ||
                    reflVar.GetVariant().GetType() == Variant::Type::UINT)
                {
                    inputNumber->SetDecimalPlaces(0);
                    if (reflVar.GetVariant().GetType() == Variant::Type::UINT)
                    {
                        inputNumber->SetMinValue(0.0f);
                    }
                }
                inputNumber
                    ->EventEmitter<IEventsValueChanged>::RegisterListener(
                        inspectorWidget);
                widgetToAdd = inputNumber->GetGameObject();
            }
            else if (reflVar.GetVariant().GetType() == Variant::Type::BOOL)
            {
                UICheckBox *checkBox = GameObjectFactory::CreateUICheckBox();
                checkBox->SetChecked(reflVar.GetInitValue().GetBool());
                checkBox->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widgetToAdd = checkBox->GetGameObject();
            }
            else if (reflVar.GetVariant().GetType() == Variant::Type::STRING)
            {
                UIInputText *inputText = GameObjectFactory::CreateUIInputText();
                inputText->GetText()->SetContent(
                    reflVar.GetInitValue().GetString());
                inputText->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widgetToAdd = inputText->GetGameObject();
            }
            else if (reflVar.GetVariant().GetType() == Variant::Type::VECTOR2 ||
                     reflVar.GetVariant().GetType() == Variant::Type::VECTOR3 ||
                     reflVar.GetVariant().GetType() == Variant::Type::VECTOR4 ||
                     reflVar.GetVariant().GetType() ==
                         Variant::Type::QUATERNION)
            {
                int numComps = 4;
                if (reflVar.GetVariant().GetType() == Variant::Type::VECTOR2)
                {
                    numComps = 2;
                }
                else if (reflVar.GetVariant().GetType() ==
                         Variant::Type::VECTOR3)
                {
                    numComps = 3;
                }
                UIInputVector *inputVec = new UIInputVector();
                inputVec->SetSize(numComps);
                inputVec->Set(reflVar.GetInitValue().GetVector4());
                inputVec->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widgetToAdd = inputVec;
            }
            else if (reflVar.GetVariant().GetType() == Variant::Type::COLOR)
            {
                UIInputColor *inputColor = new UIInputColor();
                inputColor->SetColor(reflVar.GetInitValue().GetColor());
                inputColor->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widgetToAdd = inputColor;
            }

            if (widgetToAdd)
            {
                m_varNameToWidget.Add(reflVar.GetName(), widgetToAdd);
                m_widgetToReflectedVar.Add(widgetToAdd, reflVar);
                m_widgets.PushBack(widgetToAdd);
            }
        }

        // Add reflect widgets
        for (GameObject *widget : GetWidgets())
        {
            String name = GetWidgetToReflectedVar().Get(widget).GetName();
            inspectorWidget->AddWidget(name, widget);
        }

        m_previousReflectedStruct = reflectStruct;
    }
}

void ReflectWidgetsManager::UpdateWidgetsContentFromMeta(const MetaNode &meta)
{
    for (const auto &pair : meta.GetAttributes())
    {
        const String &attrName = pair.first;
        const MetaAttribute &metaAttr = pair.second;

        auto it = GetVarNameToWidget().Find(attrName);
        if (it != GetVarNameToWidget().End())
        {
            GameObject *widget = it->second;
            if (UIInputVector *inputVec = DCAST<UIInputVector *>(widget))
            {
                inputVec->Set(metaAttr.Get<Vector4>());
            }
            else if (UIInputColor *inputColor = DCAST<UIInputColor *>(widget))
            {
                inputColor->SetColor(metaAttr.Get<Color>());
            }
            else if (UIInputNumber *inputNumber =
                         widget->GetComponent<UIInputNumber>())
            {
                inputNumber->SetValue(SCAST<float>(metaAttr.Get<double>()));
            }
            else if (UIInputText *inputText =
                         widget->GetComponent<UIInputText>())
            {
                inputText->GetText()->SetContent(metaAttr.Get<String>());
            }
            else if (UICheckBox *inputCheckBox =
                         widget->GetComponent<UICheckBox>())
            {
                inputCheckBox->SetChecked(metaAttr.Get<bool>());
            }
        }
    }
}

String GetStringValueFromWidget(GameObject *widget)
{
    String value = "";
    if (UIInputVector *inputVec = DCAST<UIInputVector *>(widget))
    {
        switch (inputVec->GetSize())
        {
            case 2: value = String::ToString(inputVec->GetVector2()); break;

            case 3: value = String::ToString(inputVec->GetVector3()); break;

            default: value = String::ToString(inputVec->GetVector4()); break;
        }
    }
    else if (UIInputColor *inputColor = DCAST<UIInputColor *>(widget))
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
                                 SCAST<int>(inputNumber->GetDecimalPlaces()));
    }
    else if (UIInputText *inputText = widget->GetComponent<UIInputText>())
    {
        value = inputText->GetText()->GetContent();
    }
    return value;
}

MetaNode ReflectWidgetsManager::GetMetaFromWidget(GameObject *widget) const
{
    MetaNode meta;
    auto it = GetWidgetToReflectedVar().Find(widget);
    if (it == GetWidgetToReflectedVar().End())
    {
        // Try finding widget parent...
        it = GetWidgetToReflectedVar().Find(widget->GetParent());
    }

    if (it != GetWidgetToReflectedVar().End())
    {
        const ReflectVariable &reflVar = it->second;
        String value = GetStringValueFromWidget(widget);
        meta.Set(reflVar.GetName(), value);
    }
    return meta;
}

MetaNode ReflectWidgetsManager::GetMetaFromWidgets() const
{
    MetaNode meta;
    for (const auto &it : GetWidgetToReflectedVar())
    {
        GameObject *widget = it.first;
        const ReflectVariable &reflVar = it.second;
        String value = GetStringValueFromWidget(widget);
        meta.Set(reflVar.GetName(), value);
    }
    return meta;
}

const Map<GameObject *, ReflectVariable>
    &ReflectWidgetsManager::GetWidgetToReflectedVar() const
{
    return m_widgetToReflectedVar;
}

const Map<String, GameObject *> &ReflectWidgetsManager::GetVarNameToWidget()
    const
{
    return m_varNameToWidget;
}

const Array<GameObject *> &ReflectWidgetsManager::GetWidgets() const
{
    return m_widgets;
}
