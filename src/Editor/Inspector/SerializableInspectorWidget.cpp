#include "BangEditor/SerializableInspectorWidget.h"

#include "Bang/GameObjectFactory.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/Serializable.h"
#include "Bang/UIButton.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "Bang/UISlider.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/InspectorWidget.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputObject.h"
#include "BangEditor/UIInputVector.h"

using namespace Bang;
using namespace BangEditor;

SerializableInspectorWidget::SerializableInspectorWidget()
{
}

SerializableInspectorWidget::~SerializableInspectorWidget()
{
}

void SerializableInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();
}

void SerializableInspectorWidget::UpdateFromReference()
{
    InspectorWidget::UpdateFromReference();

    ReflectStruct reflectStruct = GetReflectableReflectStruct();
    UpdateReflectWidgetsFromReflection(reflectStruct, this);

    if (GetSerializable())
    {
        MetaNode reflectableMeta = GetSerializable()->GetMeta();
        UpdateWidgetsContentFromMeta(reflectableMeta);
    }
}

void SerializableInspectorWidget::SetSerializable(Serializable *serializable)
{
    if (serializable != GetSerializable())
    {
        p_serializable = serializable;

        if (GetSerializable())
        {
            SetTitle(GetSerializable()->GetClassName());

            EventListener<IEventsValueChanged>::SetReceiveEvents(false);
            OnReflectableSet();
            EventListener<IEventsValueChanged>::SetReceiveEvents(true);
        }
    }
}

void SerializableInspectorWidget::UpdateReflectWidgetsFromReflection(
    const ReflectStruct &reflectStruct,
    InspectorWidget *inspectorWidget)
{
    if (!reflectStruct.EqualsWithoutValue(m_previousReflectStruct))
    {
        int totalLabelsWidth = GetLabelsWidth();

        // Add widgets to structures
        Array<String> widgetNamesToAdd;
        Array<GameObject *> widgetsToAdd;
        for (const ReflectVariable &reflVar : reflectStruct.GetVariables())
        {
            bool needsToRecreateVar = true;
            GameObject *prevWidget = nullptr;
            const String varName = reflVar.GetName();
            if (m_varNameToReflectWidget.ContainsKey(varName))
            {
                prevWidget = m_varNameToReflectWidget.Get(varName);
                ASSERT(m_reflectWidgetToReflectVar.ContainsKey(prevWidget));

                const ReflectVariable &previousReflVar =
                    m_reflectWidgetToReflectVar.Get(prevWidget);
                needsToRecreateVar =
                    NeedsToRecreateWidget(previousReflVar, reflVar);

                const bool destroyWidget = needsToRecreateVar;
                m_reflectWidgets.Remove(prevWidget);
                m_reflectWidgetToReflectVar.Remove(prevWidget);
                m_varNameToReflectWidget.Remove(varName);
                inspectorWidget->RemoveWidget(prevWidget, destroyWidget);
            }

            if (!reflVar.GetHints().GetIsShown())
            {
                continue;
            }

            ASSERT(needsToRecreateVar == (prevWidget == nullptr));

            GameObject *widget = nullptr;
            const Variant::Type variantType = reflVar.GetVariant().GetType();
            if (variantType == Variant::Type::FLOAT ||
                variantType == Variant::Type::DOUBLE ||
                variantType == Variant::Type::INT ||
                variantType == Variant::Type::UINT)
            {
                if (reflVar.GetHints().GetIsEnum() ||
                    reflVar.GetHints().GetIsEnumFlags())
                {
                    UIComboBox *enumInput =
                        (needsToRecreateVar
                             ? GameObjectFactory::CreateUIComboBox()
                             : prevWidget->GetComponent<UIComboBox>());
                    ASSERT(enumInput);

                    enumInput->SetMultiCheck(
                        reflVar.GetHints().GetIsEnumFlags());

                    const auto &enumFields =
                        reflectStruct.GetEnumFields(reflVar.GetName());
                    if (enumFields.Size() >= 1)
                    {
                        for (const auto &enumFieldNameValuePair : enumFields)
                        {
                            const String &enumFieldName =
                                enumFieldNameValuePair.first;
                            uint enumFieldValue = enumFieldNameValuePair.second;
                            enumInput->AddItem(enumFieldName,
                                               SCAST<int>(enumFieldValue));
                        }
                        enumInput->EventEmitter<IEventsValueChanged>::
                            RegisterListener(inspectorWidget);
                    }
                    widget = enumInput->GetGameObject();
                }
                else
                {
                    UIInputNumber *inputNumber = nullptr;
                    if (reflVar.GetHints().GetIsSlider())
                    {
                        UISlider *slider =
                            (needsToRecreateVar
                                 ? GameObjectFactory::CreateUISlider()
                                 : prevWidget->GetComponent<UISlider>());
                        ASSERT(slider);

                        slider->SetMinMaxValues(
                            reflVar.GetHints().GetMinValue().x,
                            reflVar.GetHints().GetMaxValue().x);
                        slider->SetValue(reflVar.GetInitValue().GetFloat());

                        inputNumber = slider->GetInputNumber();

                        slider->EventEmitter<IEventsValueChanged>::
                            RegisterListener(inspectorWidget);
                        widget = slider->GetGameObject();
                    }
                    else
                    {
                        inputNumber =
                            (needsToRecreateVar
                                 ? GameObjectFactory::CreateUIInputNumber()
                                 : prevWidget->GetComponent<UIInputNumber>());
                        ASSERT(inputNumber);

                        inputNumber->SetMinMaxValues(
                            reflVar.GetHints().GetMinValue().x,
                            reflVar.GetHints().GetMaxValue().x);
                        inputNumber->SetValue(
                            reflVar.GetInitValue().GetFloat());

                        inputNumber->EventEmitter<IEventsValueChanged>::
                            RegisterListener(inspectorWidget);
                        widget = inputNumber->GetGameObject();
                    }

                    inputNumber->SetStep(reflVar.GetHints().GetStepValue());
                    inputNumber->SetBlocked(reflVar.GetHints().GetIsBlocked());

                    if (variantType == Variant::Type::INT ||
                        variantType == Variant::Type::UINT)
                    {
                        inputNumber->SetDecimalPlaces(0);
                    }
                }
            }
            else if (variantType == Variant::Type::BOOL)
            {
                if (reflVar.GetHints().GetIsButton())
                {
                    UIButton *button =
                        (needsToRecreateVar
                             ? GameObjectFactory::CreateUIButton(
                                   reflVar.GetName())
                             : prevWidget->GetComponent<UIButton>());
                    ASSERT(button);

                    button->AddClickedCallback([reflVar]() {
                        reflVar.GetSetter()(Variant::From(true));
                    });
                    button->SetBlocked(reflVar.GetHints().GetIsBlocked());
                    widget = button->GetGameObject();
                }
                else
                {
                    UICheckBox *checkBox =
                        GameObjectFactory::CreateUICheckBox();
                    checkBox->SetChecked(reflVar.GetInitValue().GetBool());
                    checkBox
                        ->EventEmitter<IEventsValueChanged>::RegisterListener(
                            inspectorWidget);
                    widget = checkBox->GetGameObject();
                }
            }
            else if (variantType == Variant::Type::GUID)
            {
                UIInputFile *inputFile =
                    (needsToRecreateVar ? new UIInputFile()
                                        : SCAST<UIInputFile *>(prevWidget));
                ASSERT(inputFile);

                inputFile->SetShowPreview(true);
                inputFile->SetPath(Path::Empty());
                inputFile->SetZoomable(
                    reflVar.GetHints().GetIsZoomablePreview());
                inputFile->SetExtensions(reflVar.GetHints().GetExtensions());
                inputFile->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widget = inputFile;
            }
            else if (variantType == Variant::Type::OBJECT_PTR)
            {
                UIInputObject *inputObject =
                    (needsToRecreateVar ? new UIInputObject()
                                        : SCAST<UIInputObject *>(prevWidget));
                ASSERT(inputObject);

                inputObject->SetAcceptedClassIdBeginAndEnd(
                    reflVar.GetHints().GetObjectPtrClassIdBegin(),
                    reflVar.GetHints().GetObjectPtrClassIdEnd());
                inputObject
                    ->EventEmitter<IEventsValueChanged>::RegisterListener(
                        inspectorWidget);
                String objClassStr =
                    reflVar.GetHints().GetObjectPtrClass().Replace("*", "");

                inputObject->SetPreviewIcon(
                    EditorTextureFactory::GetComponentIcon(objClassStr),
                    EditorTextureFactory::GetComponentIconTint(objClassStr));

                widget = inputObject;
            }
            else if (variantType == Variant::Type::PATH)
            {
                UIInputFile *inputFile =
                    (needsToRecreateVar ? new UIInputFile()
                                        : SCAST<UIInputFile *>(prevWidget));
                ASSERT(inputFile);

                inputFile->SetShowPreview(false);
                inputFile->SetPath(Path::Empty());
                inputFile->SetExtensions(reflVar.GetHints().GetExtensions());
                inputFile->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                inputFile->GetInputText()->SetBlocked(
                    reflVar.GetHints().GetIsBlocked());
                widget = inputFile;
            }
            else if (variantType == Variant::Type::STRING)
            {
                UIInputText *inputText =
                    (needsToRecreateVar
                         ? GameObjectFactory::CreateUIInputText()
                         : prevWidget->GetComponent<UIInputText>());
                ASSERT(inputText);

                inputText->GetText()->SetContent(
                    reflVar.GetInitValue().GetString());
                inputText->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                inputText->SetBlocked(reflVar.GetHints().GetIsBlocked());
                widget = inputText->GetGameObject();
            }
            else if (variantType == Variant::Type::VECTOR2 ||
                     variantType == Variant::Type::VECTOR3 ||
                     variantType == Variant::Type::VECTOR4 ||
                     variantType == Variant::Type::VECTOR2i ||
                     variantType == Variant::Type::VECTOR3i ||
                     variantType == Variant::Type::VECTOR4i ||
                     variantType == Variant::Type::QUATERNION)
            {
                const bool isInt = (variantType == Variant::Type::VECTOR2i ||
                                    variantType == Variant::Type::VECTOR3i ||
                                    variantType == Variant::Type::VECTOR4i);
                int numComps = 4;
                if (variantType == Variant::Type::VECTOR2 ||
                    variantType == Variant::Type::VECTOR2i)
                {
                    numComps = 2;
                }
                else if (variantType == Variant::Type::VECTOR3 ||
                         variantType == Variant::Type::VECTOR3i)
                {
                    numComps = 3;
                }

                UIInputVector *inputVec =
                    (needsToRecreateVar ? new UIInputVector()
                                        : SCAST<UIInputVector *>(prevWidget));
                ASSERT(inputVec);

                inputVec->SetBlocked(reflVar.GetHints().GetIsBlocked());
                inputVec->SetSize(numComps);
                inputVec->Set(
                    isInt ? Vector4(reflVar.GetInitValue().GetVector4i())
                          : reflVar.GetInitValue().GetVector4());
                inputVec->SetStep(Vector4(reflVar.GetHints().GetStepValue()));
                inputVec->SetMinValue(reflVar.GetHints().GetMinValue());
                inputVec->SetMaxValue(reflVar.GetHints().GetMaxValue());
                for (UIInputNumber *inputNum : inputVec->GetInputNumbers())
                {
                    inputNum->SetDecimalPlaces(isInt ? 0 : 3);
                }
                inputVec->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widget = inputVec;
            }
            else if (variantType == Variant::Type::COLOR)
            {
                UIInputColor *inputColor =
                    (needsToRecreateVar ? new UIInputColor()
                                        : SCAST<UIInputColor *>(prevWidget));
                ASSERT(inputColor);

                inputColor->SetColor(reflVar.GetInitValue().GetColor());
                inputColor->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widget = inputColor;
            }

            if (widget)
            {
                widgetsToAdd.PushBack(widget);
                widgetNamesToAdd.PushBack(varName);
                m_varNameToReflectWidget.Add(varName, widget);
                m_reflectWidgetToReflectVar.Add(widget, reflVar);
                m_reflectWidgets.PushBack(widget);
            }
        }

        // Add reflect widgets
        for (uint i = 0; i < widgetsToAdd.Size(); ++i)
        {
            const String &name = widgetNamesToAdd[i];
            GameObject *widget = widgetsToAdd[i];
            const bool needsLabel = (!widget->GetComponent<UIButton>());
            if (needsLabel)
            {
                inspectorWidget->AddWidget(name, widget);
            }
            else
            {
                inspectorWidget->AddWidget(widget);
            }

            if (UILabel *label =
                    inspectorWidget->GetWidgetToLabel().Get(widget))
            {
                label->GetText()->CalculateLayout(Axis::HORIZONTAL);
                int labelWidth = label->GetText()->GetPreferredSize().x + 10;
                totalLabelsWidth = Math::Max(labelWidth, totalLabelsWidth);
            }
        }
        SetLabelsWidth(totalLabelsWidth);

        m_previousReflectStruct = reflectStruct;
    }
}

void SerializableInspectorWidget::UpdateWidgetsContentFromMeta(
    const MetaNode &meta)
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
            else if (UIInputFile *inputFile = DCAST<UIInputFile *>(widget))
            {
                if (inputFile->GetShowPreview())
                {
                    GUID guid = metaAttr.Get<GUID>();
                    Path inputFilePath = MetaFilesManager::GetFilepath(guid);
                    inputFile->SetPath(inputFilePath);
                }
                else
                {
                    Path inputFilePath = metaAttr.Get<Path>();
                    inputFile->SetPath(inputFilePath);
                }
            }
            else if (UIInputObject *inputObject =
                         DCAST<UIInputObject *>(widget))
            {
                String str = metaAttr.GetStringValue();
                str = str.Replace("OP ", "");
                std::istringstream iss(str);
                GUID guid;
                iss >> guid;
                inputObject->SetGUID(guid);
            }
            else if (UIInputColor *inputColor = DCAST<UIInputColor *>(widget))
            {
                inputColor->SetColor(metaAttr.Get<Color>());
            }
            else if (UIComboBox *comboBox = widget->GetComponent<UIComboBox>())
            {
                if (comboBox->GetMultiCheck())
                {
                    comboBox->SetSelectionForFlag(metaAttr.Get<int>());
                }
                else
                {
                    comboBox->SetSelectionByValue(metaAttr.Get<int>());
                }
            }
            else if (UIInputNumber *inputNumber =
                         widget->GetComponent<UIInputNumber>())
            {
                inputNumber->SetValue(SCAST<float>(metaAttr.Get<double>()));
            }
            else if (UISlider *inputSlider = widget->GetComponent<UISlider>())
            {
                inputSlider->SetValue(SCAST<float>(metaAttr.Get<double>()));
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

bool SerializableInspectorWidget::NeedsToRecreateWidget(
    const ReflectVariable &previousVariable,
    const ReflectVariable &currentVariable) const
{
    const ReflectVariableHints &prvh = previousVariable.GetHints();
    const ReflectVariableHints &crvh = currentVariable.GetHints();

    bool needsToRemoveWidget = false;
    needsToRemoveWidget |= (prvh.GetIsEnum() != crvh.GetIsEnum());
    needsToRemoveWidget |= (prvh.GetIsShown() != crvh.GetIsShown());
    needsToRemoveWidget |= (prvh.GetIsButton() != crvh.GetIsButton());
    needsToRemoveWidget |= (prvh.GetIsSlider() != crvh.GetIsSlider());
    needsToRemoveWidget |= (prvh.GetIsEnumFlags() != crvh.GetIsEnumFlags());
    needsToRemoveWidget |= (previousVariable.GetVariant().GetType() !=
                            currentVariable.GetVariant().GetType());
    return needsToRemoveWidget;
}

String GetStringValueFromWidget(GameObject *widget)
{
    String valueStr = "";
    if (UIInputVector *inputVec = DCAST<UIInputVector *>(widget))
    {
        switch (inputVec->GetSize())
        {
            case 2: valueStr = String::ToString(inputVec->GetVector2()); break;
            case 3: valueStr = String::ToString(inputVec->GetVector3()); break;
            default: valueStr = String::ToString(inputVec->GetVector4()); break;
        }
    }
    else if (UIInputFile *inputFile = DCAST<UIInputFile *>(widget))
    {
        if (inputFile->GetShowPreview())
        {
            GUID guid = MetaFilesManager::GetGUID(inputFile->GetPath());
            valueStr = String::ToString(guid);
        }
        else
        {
            valueStr = inputFile->GetPath().GetAbsolute();
        }
    }
    else if (UIComboBox *comboBox = widget->GetComponent<UIComboBox>())
    {
        valueStr = String::ToString(comboBox->GetMultiCheck()
                                        ? comboBox->GetSelectedValuesForFlag()
                                        : comboBox->GetSelectedValue());
    }
    else if (UIInputObject *inputObject = DCAST<UIInputObject *>(widget))
    {
        valueStr = String::ToString(inputObject->GetObjectPtr());
    }
    else if (UIInputColor *inputColor = DCAST<UIInputColor *>(widget))
    {
        valueStr = String::ToString(inputColor->GetColor());
    }
    else if (UICheckBox *inputCheckBox = widget->GetComponent<UICheckBox>())
    {
        valueStr = inputCheckBox->IsChecked() ? "True" : "False";
    }
    else if (UISlider *slider = widget->GetComponent<UISlider>())
    {
        valueStr = String::ToString(
            slider->GetValue(),
            SCAST<int>(slider->GetInputNumber()->GetDecimalPlaces()));
    }
    else if (UIInputNumber *inputNumber = widget->GetComponent<UIInputNumber>())
    {
        valueStr =
            String::ToString(inputNumber->GetValue(),
                             SCAST<int>(inputNumber->GetDecimalPlaces()));
    }
    else if (UIInputText *inputText = widget->GetComponent<UIInputText>())
    {
        valueStr = inputText->GetText()->GetContent();
    }
    return valueStr;
}

MetaNode SerializableInspectorWidget::GetMetaFromWidget(
    GameObject *widget) const
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

MetaNode SerializableInspectorWidget::GetMetaFromReflectWidgets() const
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
    &SerializableInspectorWidget::GetWidgetToReflectedVar() const
{
    return m_reflectWidgetToReflectVar;
}

const Map<String, GameObject *>
    &SerializableInspectorWidget::GetVarNameToWidget() const
{
    return m_varNameToReflectWidget;
}

const Array<GameObject *> &SerializableInspectorWidget::GetWidgets() const
{
    return m_reflectWidgets;
}

Serializable *SerializableInspectorWidget::GetSerializable() const
{
    return p_serializable;
}

void SerializableInspectorWidget::OnReflectableSet()
{
    EventListener<IEventsValueChanged>::SetReceiveEvents(false);

    ReflectStruct reflectStruct = GetReflectableReflectStruct();
    UpdateReflectWidgetsFromReflection(reflectStruct, this);

    UpdateFromReference();

    EventListener<IEventsValueChanged>::SetReceiveEvents(true);
}

ReflectStruct SerializableInspectorWidget::GetReflectableReflectStruct() const
{
    ReflectStruct reflectStruct;
    if (GetSerializable())
    {
        reflectStruct = GetSerializable()->GetReflectStruct();
    }
    return reflectStruct;
}

void SerializableInspectorWidget::OnValueChanged(
    EventEmitter<IEventsValueChanged> *object)
{
    InspectorWidget::OnValueChanged(object);

    if (GetSerializable())
    {
        MetaNode meta = GetMetaFromReflectWidgets();
        GetSerializable()->ImportMeta(meta);
    }
}
