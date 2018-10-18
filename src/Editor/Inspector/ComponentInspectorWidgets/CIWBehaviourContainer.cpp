#include "BangEditor/CIWBehaviourContainer.h"

#include <functional>
#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangPreprocessor.h"
#include "Bang/BehaviourContainer.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Map.h"
#include "Bang/Map.tcc"
#include "Bang/MetaNode.h"
#include "Bang/Path.h"
#include "Bang/ReflectStruct.h"
#include "Bang/ReflectVariable.h"
#include "Bang/UIButton.h"
#include "Bang/UILabel.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/CIWBehaviour.h"
#include "BangEditor/EditorFileTracker.h"
#include "BangEditor/ReflectWidgetsManager.h"
#include "BangEditor/UIInputFileWithPreview.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

CIWBehaviourContainer::CIWBehaviourContainer()
{
}

CIWBehaviourContainer::~CIWBehaviourContainer()
{
}

void CIWBehaviourContainer::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWBehaviourContainer");
    SetTitle("Behaviour");

    p_sourceInputFile = new UIInputFileWithPreview();
    p_sourceInputFile->SetExtensions(Extensions::GetSourceFileExtensions());
    p_sourceInputFile->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_sourceInputFile->SetZoomable(false);

    p_resetValuesButton = GameObjectFactory::CreateUIButton("Reset values");
    p_resetValuesButton->AddClickedCallback([this]() {
        GetBehaviourContainer()->ResetInitializationModificationsMetaNode();
    });

    AddWidget("Source", p_sourceInputFile);
    AddWidget(p_resetValuesButton->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);

    SetLabelsWidth(80);
}

void CIWBehaviourContainer::UpdateModifiedInitializationMetaFromWidget(
    GameObject *widget)
{
    MetaNode widgetMeta = GetReflectWidgetsManager()->GetMetaFromWidget(widget);
    GetBehaviourContainer()->GetIntializationModificationMetaPtr()->Import(
        widgetMeta.ToString());
}

void CIWBehaviourContainer::OnComponentSet()
{
    MetaNode modifiedInitMeta =
        GetBehaviourContainer()->GetInitializationModificationsMeta();

    ComponentInspectorWidget::OnComponentSet();
    GetReflectWidgetsManager()->UpdateWidgetsContentFromMeta(modifiedInitMeta);
}

ReflectStruct CIWBehaviourContainer::GetComponentReflectStruct() const
{
    return GetBehaviourContainer()->GetBehaviourReflectStruct();
}

void CIWBehaviourContainer::UpdateFromReference()
{
    Path srcPath = GetBehaviourContainer()->GetSourceFilepath();
    if (srcPath.IsFile())
    {
        SetTitle(srcPath.GetName());
        p_sourceInputFile->SetPath(srcPath);
    }
    GetBehaviourContainer()->UpdateInformationFromHeaderIfNeeded();

    ComponentInspectorWidget::UpdateFromReference();

    MetaNode initMeta = GetBehaviourContainer()->GetInitializationMeta();
    GetReflectWidgetsManager()->UpdateWidgetsContentFromMeta(initMeta);

    MetaNode initializationModificationMeta =
        GetBehaviourContainer()->GetInitializationModificationsMeta();
    for (const auto &pair : GetWidgetToLabel())
    {
        if (UILabel *label = pair.second)
        {
            String labelStr = label->GetText()->GetContent();
            bool isModified = initializationModificationMeta.Contains(labelStr);
            Color labelColor = Color::Black;
            if (initMeta.Contains(labelStr))
            {
                labelColor = (isModified ? Color::Black : Color::Blue);
            }
            label->GetText()->SetTextColor(labelColor);
        }
    }
}

void CIWBehaviourContainer::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    GetBehaviourContainer()->SetSourceFilepath(p_sourceInputFile->GetPath());
    GameObject *go = DCAST<GameObject *>(object);
    if (!go)
    {
        if (Component *comp = DCAST<Component *>(object))
        {
            go = comp->GetGameObject();
        }
    }

    if (go)
    {
        UpdateModifiedInitializationMetaFromWidget(go);
    }
}

BehaviourContainer *CIWBehaviourContainer::GetBehaviourContainer() const
{
    return SCAST<BehaviourContainer *>(GetComponent());
}
