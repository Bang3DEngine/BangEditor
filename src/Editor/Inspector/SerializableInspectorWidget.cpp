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

    MetaNode reflectableMeta = GetSerializable()->GetMeta();
    UpdateWidgetsContentFromMeta(reflectableMeta);
}

void SerializableInspectorWidget::SetSerializable(Serializable *serializable)
{
    if (serializable != GetSerializable())
    {
        p_serializable = serializable;

        SetTitle(GetSerializable()->GetClassName());

        EventListener<IEventsValueChanged>::SetReceiveEvents(false);
        OnReflectableSet();
        EventListener<IEventsValueChanged>::SetReceiveEvents(true);
    }
}

void SerializableInspectorWidget::UpdateReflectWidgetsFromReflection(
    const ReflectStruct &reflectStruct,
    InspectorWidget *inspectorWidget)
{
    if (!reflectStruct.EqualsWithoutValue(m_previousReflectStruct))
    {
        // Clear old reflect widgets (if any)
        for (GameObject *widget : GetWidgets())
        {
            inspectorWidget->RemoveWidget(widget);
        }

        // Clear structures
        m_reflectWidgets.Clear();
        m_reflectWidgetToReflectVar.Clear();
        m_varNameToReflectWidget.Clear();

        int totalLabelsWidth = GetLabelsWidth();

        // Add widgets to structures
        for (const ReflectVariable &reflVar : reflectStruct.GetVariables())
        {
            if (!reflVar.GetHints().GetIsShown())
            {
                continue;
            }

            GameObject *widgetToAdd = nullptr;
            String widgetName = reflVar.GetName();
            Variant::Type variantType = reflVar.GetVariant().GetType();
            if (variantType == Variant::Type::FLOAT ||
                variantType == Variant::Type::DOUBLE ||
                variantType == Variant::Type::INT ||
                variantType == Variant::Type::UINT)
            {
                if (reflVar.GetHints().GetIsEnum() ||
                    reflVar.GetHints().GetIsEnumFlags())
                {
                    UIComboBox *enumInput =
                        GameObjectFactory::CreateUIComboBox();
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
                    widgetToAdd = enumInput->GetGameObject();
                }
                else
                {
                    UIInputNumber *inputNumber = nullptr;
                    if (reflVar.GetHints().GetIsSlider())
                    {
                        UISlider *slider = GameObjectFactory::CreateUISlider();

                        slider->SetMinMaxValues(
                            reflVar.GetHints().GetMinValue().x,
                            reflVar.GetHints().GetMaxValue().x);
                        slider->SetValue(reflVar.GetInitValue().GetFloat());

                        inputNumber = slider->GetInputNumber();

                        slider->EventEmitter<IEventsValueChanged>::
                            RegisterListener(inspectorWidget);
                        widgetToAdd = slider->GetGameObject();
                    }
                    else
                    {
                        inputNumber = GameObjectFactory::CreateUIInputNumber();

                        inputNumber->SetMinMaxValues(
                            reflVar.GetHints().GetMinValue().x,
                            reflVar.GetHints().GetMaxValue().x);
                        inputNumber->SetValue(
                            reflVar.GetInitValue().GetFloat());

                        inputNumber->EventEmitter<IEventsValueChanged>::
                            RegisterListener(inspectorWidget);
                        widgetToAdd = inputNumber->GetGameObject();
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
                        GameObjectFactory::CreateUIButton(reflVar.GetName());
                    button->AddClickedCallback([reflVar]() {
                        reflVar.GetSetter()(Variant::From(true));
                    });
                    button->SetBlocked(reflVar.GetHints().GetIsBlocked());
                    widgetToAdd = button->GetGameObject();
                }
                else
                {
                    UICheckBox *checkBox =
                        GameObjectFactory::CreateUICheckBox();
                    checkBox->SetChecked(reflVar.GetInitValue().GetBool());
                    checkBox
                        ->EventEmitter<IEventsValueChanged>::RegisterListener(
                            inspectorWidget);
                    widgetToAdd = checkBox->GetGameObject();
                }
            }
            else if (variantType == Variant::Type::GUID)
            {
                UIInputFile *inputFile = new UIInputFile();
                inputFile->SetShowPreview(true);
                inputFile->SetPath(Path::Empty());
                inputFile->SetZoomable(reflVar.GetHints().GetZoomablePreview());
                inputFile->SetExtensions(reflVar.GetHints().GetExtensions());
                inputFile->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widgetToAdd = inputFile;
            }
            else if (variantType == Variant::Type::OBJECT_PTR)
            {
                UIInputObject *inputObject = new UIInputObject();
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

                widgetToAdd = inputObject;
            }
            else if (variantType == Variant::Type::PATH)
            {
                UIInputFile *inputFile = new UIInputFile();
                inputFile->SetShowPreview(false);
                inputFile->SetPath(Path::Empty());
                inputFile->SetExtensions(reflVar.GetHints().GetExtensions());
                inputFile->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                inputFile->GetInputText()->SetBlocked(
                    reflVar.GetHints().GetIsBlocked());
                widgetToAdd = inputFile;
            }
            else if (variantType == Variant::Type::STRING)
            {
                UIInputText *inputText = GameObjectFactory::CreateUIInputText();
                inputText->GetText()->SetContent(
                    reflVar.GetInitValue().GetString());
                inputText->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                inputText->SetBlocked(reflVar.GetHints().GetIsBlocked());
                widgetToAdd = inputText->GetGameObject();
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

                UIInputVector *inputVec = new UIInputVector();
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
                widgetToAdd = inputVec;
            }
            else if (variantType == Variant::Type::COLOR)
            {
                UIInputColor *inputColor = new UIInputColor();
                inputColor->SetColor(reflVar.GetInitValue().GetColor());
                inputColor->EventEmitter<IEventsValueChanged>::RegisterListener(
                    inspectorWidget);
                widgetToAdd = inputColor;
            }

            if (widgetToAdd)
            {
                m_varNameToReflectWidget.Add(reflVar.GetName(), widgetToAdd);
                m_reflectWidgetToReflectVar.Add(widgetToAdd, reflVar);
                m_reflectWidgets.PushBack(widgetToAdd);
            }
        }

        // Add reflect widgets
        for (GameObject *widget : GetWidgets())
        {
            String name = GetWidgetToReflectedVar().Get(widget).GetName();
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
                comboBox->SetSelectionByValue(metaAttr.Get<int>());
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
    else if (UIInputFile *inputFile = DCAST<UIInputFile *>(widget))
    {
        if (inputFile->GetShowPreview())
        {
            GUID guid = MetaFilesManager::GetGUID(inputFile->GetPath());
            value = String::ToString(guid);
        }
        else
        {
            value = inputFile->GetPath().GetAbsolute();
        }
    }
    else if (UIComboBox *comboBox = widget->GetComponent<UIComboBox>())
    {
        value = String::ToString(comboBox->GetSelectedValue());
    }
    else if (UIInputObject *inputObject = DCAST<UIInputObject *>(widget))
    {
        value = String::ToString(inputObject->GetObjectPtr());
    }
    else if (UIInputColor *inputColor = DCAST<UIInputColor *>(widget))
    {
        value = String::ToString(inputColor->GetColor());
    }
    else if (UICheckBox *inputCheckBox = widget->GetComponent<UICheckBox>())
    {
        value = inputCheckBox->IsChecked() ? "True" : "False";
    }
    else if (UISlider *slider = widget->GetComponent<UISlider>())
    {
        value = String::ToString(
            slider->GetValue(),
            SCAST<int>(slider->GetInputNumber()->GetDecimalPlaces()));
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

    MetaNode meta = GetMetaFromReflectWidgets();
    GetSerializable()->ImportMeta(meta);
}
