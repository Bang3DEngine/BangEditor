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

    p_sourceInputFile = GameObject::Create<UIInputFileWithPreview>();
    p_sourceInputFile->SetExtensions(Extensions::GetSourceFileExtensions());
    p_sourceInputFile->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_sourceInputFile->SetZoomable(false);
    AddWidget("Source", p_sourceInputFile);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);

    SetLabelsWidth(80);
}

void CIWBehaviourContainer::UpdateInitializationMetaFromWidgets()
{
    MetaNode initializationMeta =
        GetReflectWidgetsManager()->GetMetaFromWidgets();
    GetBehaviourContainer()->SetInitializationMeta(initializationMeta);
}

void CIWBehaviourContainer::OnComponentSet()
{
    MetaNode initMeta = GetBehaviourContainer()->GetInitializationMeta();

    ComponentInspectorWidget::OnComponentSet();
    GetReflectWidgetsManager()->UpdateWidgetsContentFromMeta(initMeta);
}

ReflectStruct CIWBehaviourContainer::GetComponentReflectStruct() const
{
    Path srcPath = GetBehaviourContainer()->GetSourceFilepath();
    if (srcPath.IsFile())
    {
        Path headerPath = srcPath.WithExtension("h");
        Time timeHeaderChanged =
            EditorFileTracker::GetInstance()->GetModificationTime(headerPath);
        if (timeHeaderChanged > m_prevTimeHeaderChanged)
        {
            Array<ReflectStruct> reflStructs =
                BangPreprocessor::GetReflectStructs(headerPath);
            if (reflStructs.Size() >= 1)
            {
                m_behaviourReflectStruct = reflStructs.Front();
            }
            else
            {
                m_behaviourReflectStruct = ReflectStruct();
            }
            m_prevTimeHeaderChanged = timeHeaderChanged;
        }
    }
    return m_behaviourReflectStruct;
}

void CIWBehaviourContainer::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    Path srcPath = GetBehaviourContainer()->GetSourceFilepath();
    if (srcPath.IsFile())
    {
        SetTitle(srcPath.GetName());
        p_sourceInputFile->SetPath(srcPath);
    }
    UpdateInitializationMetaFromWidgets();
}

void CIWBehaviourContainer::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    GetBehaviourContainer()->SetSourceFilepath(p_sourceInputFile->GetPath());
    UpdateInitializationMetaFromWidgets();
}

BehaviourContainer *CIWBehaviourContainer::GetBehaviourContainer() const
{
    return SCAST<BehaviourContainer *>(GetComponent());
}
