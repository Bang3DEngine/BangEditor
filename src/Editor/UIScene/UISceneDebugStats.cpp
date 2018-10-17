#include "BangEditor/UISceneDebugStats.h"

#include "Bang/Alignment.h"
#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Object.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UITextRenderer.h"

using namespace Bang;
using namespace BangEditor;

UISceneDebugStats::UISceneDebugStats()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors(Vector2::One);
    rt->SetMargins(Vector2i(-80, -30), Vector2i::Zero);

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White.WithAlpha(0.65f));

    GameObject *debugStatsTextGo = GameObjectFactory::CreateUIGameObject();
    debugStatsTextGo->GetRectTransform()->SetMargins(8, 8, 0, 0);

    constexpr int MeanFPSSamples = 10;
    m_editorRenderFPSChrono.SetMeanSamples(MeanFPSSamples);

    p_debugStatsText = debugStatsTextGo->AddComponent<UITextRenderer>();
    p_debugStatsText->SetHorizontalAlign(HorizontalAlignment::LEFT);
    p_debugStatsText->SetVerticalAlign(VerticalAlignment::TOP);
    p_debugStatsText->SetTextColor(Color::Black);
    p_debugStatsText->SetWrapping(true);
    p_debugStatsText->SetTextSize(11);

    debugStatsTextGo->SetParent(this);
}

UISceneDebugStats::~UISceneDebugStats()
{
}

void UISceneDebugStats::Update()
{
    GameObject::Update();

    m_editorRenderFPSChrono.MarkEnd();
    m_editorRenderFPSChrono.MarkBegin();

    String fpsText = "";
    fpsText += "FPS: ";
    fpsText += String::ToString(m_editorRenderFPSChrono.GetMeanFPS(), 2);

    p_debugStatsText->SetContent(fpsText);

    // std::cerr << fpsText << std::endl;
}

void UISceneDebugStats::OnEnabled(Object *object)
{
    Object::OnEnabled(object);
    m_editorRenderFPSChrono.MarkBegin();
}
