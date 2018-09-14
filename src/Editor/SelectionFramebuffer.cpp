#include "BangEditor/SelectionFramebuffer.h"

#include "Bang/GL.h"
#include "Bang/Paths.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/GEngine.h"
#include "Bang/Vector3.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    ShaderProgram *selectionProgram =
        ShaderProgramFactory::Get(
                ShaderProgramFactory::GetDefaultVertexShaderPath(),
                EPATH("Shaders/SelectionBuffer.frag") );

    p_selectionMaterial = Resources::Create<Material>();
    p_selectionMaterial.Get()->SetShaderProgram(selectionProgram);

    Bind();
    CreateAttachmentTex2D(AttColor, GL::ColorFormat::RGBA8);
    CreateAttachmentTex2D(GL::Attachment::DEPTH, GL::ColorFormat::DEPTH16);
    UnBind();

    p_colorTexture.Set(GetAttachmentTex2D(AttColor));
}

SelectionFramebuffer::~SelectionFramebuffer()
{
}

void SelectionFramebuffer::SetNextRenderSelectable(GameObject *go)
{
    p_nextRenderSelectable = go;
}

void SelectionFramebuffer::PrepareNewFrameForRender(const GameObject *go)
{
    IdType id = 1;
    m_gameObject_To_Id.Clear();
    m_id_To_GameObject.Clear();

    Array<GameObject*> gameObjects = go->GetChildrenRecursively();
    for (GameObject *go : gameObjects)
    {
        m_gameObject_To_Id[go] = id;
        m_id_To_GameObject[id] = go;

        go->EventEmitter<IEventsDestroy>::RegisterListener(this);
        ++id;
    }
}

void SelectionFramebuffer::RenderForSelectionBuffer(Scene *scene)
{
    GEngine *ge = GEngine::GetInstance();

    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Push(GL::Pushable::BLEND_STATES);
    GL::Push(GL::Pushable::DEPTH_STATES);

    ge->SetRenderRoutine([this](Renderer *rend)
    {
        RenderForSelectionBuffer(rend);
    });

    GL::Disable(GL::Enablable::BLEND);

    Bind();
    SetAllDrawBuffers();

    GL::ClearColorStencilDepthBuffers();
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEQUAL);
    ge->RenderWithPass(scene, RenderPass::SCENE);
    ge->RenderWithPass(scene, RenderPass::SCENE_TRANSPARENT);
    GL::ClearStencilDepthBuffers();
    GL::SetDepthFunc(GL::Function::LEQUAL);
    ge->RenderWithPass(scene, RenderPass::CANVAS);

    if (m_drawOverlay)
    {
        GL::ClearDepthBuffer();
        GL::SetDepthMask(false);
        ge->RenderWithPass(scene, RenderPass::OVERLAY);
    }

    UnBind();

    ge->SetRenderRoutine(nullptr);
    GL::Pop(GL::Pushable::DEPTH_STATES);
    GL::Pop(GL::Pushable::BLEND_STATES);
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
}

void SelectionFramebuffer::SetDrawOverlay(bool drawOverlay)
{
    m_drawOverlay = drawOverlay;
}

void SelectionFramebuffer::RenderForSelectionBuffer(Renderer *rend)
{
    ASSERT(GL::IsBound(this));
    if (!rend->GetActiveMaterial())
    {
        return;
    }

    GameObject *go = p_nextRenderSelectable ? p_nextRenderSelectable :
                                              rend->GetGameObject();

    RH<ShaderProgram> prevSP;
    prevSP.Set( rend->GetActiveMaterial()->GetShaderProgram() );

    rend->GetActiveMaterial()->EventEmitter<IEventsResource>::SetEmitEvents(false);

    ShaderProgram *selSP = p_selectionMaterial.Get()->GetShaderProgram();
    rend->GetActiveMaterial()->SetShaderProgram(selSP);

    rend->Bind();
    selSP->SetColor("selectionColor", GetSelectionColor(go));
    rend->OnRender();
    rend->UnBind();

    rend->GetActiveMaterial()->SetShaderProgram(prevSP.Get());

    rend->GetActiveMaterial()->EventEmitter<IEventsResource>::SetEmitEvents(true);

    p_nextRenderSelectable = nullptr;
}

GameObject *SelectionFramebuffer::
GetGameObjectInViewportPoint(const Vector2i &vpPoint)
{
    if (vpPoint.x < 0 || vpPoint.y < 0 ||
        vpPoint.x >= GetWidth() || vpPoint.y >= GetHeight())
    {
        return nullptr;
    }

    Color colorUnderMouse = ReadColor(vpPoint.x, vpPoint.y, AttColor);
    IdType id = MapColorToId(colorUnderMouse);

    if (colorUnderMouse != Color::Zero && m_id_To_GameObject.ContainsKey(id))
    {
        return m_id_To_GameObject[id];
    }
    return nullptr;
}

void SelectionFramebuffer::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GameObject *go = DCAST<GameObject*>(object);
    if (go)
    {
        if (m_gameObject_To_Id.ContainsKey(go))
        {
            IdType id = m_gameObject_To_Id.Get(go);
            m_gameObject_To_Id.Remove(go);
            m_id_To_GameObject.Remove(id);
        }
    }
}

Color SelectionFramebuffer::GetSelectionColor(GameObject *go) const
{
    return MapIdToColor(m_gameObject_To_Id[go]);
}

Color SelectionFramebuffer::MapIdToColor(IdType id)
{
    constexpr IdType C = 255;
    Color color (double(   id                % C),
                 double(  (id / C)           % C),
                 double( ((id / C) / C)      % C),
                 double((((id / C) / C) / C) % C)
                );
   return color / double(C);
}

typename SelectionFramebuffer::IdType
SelectionFramebuffer::MapColorToId(const Color &color)
{
    constexpr IdType C = 255;
    return   IdType(color.r * C)
           + IdType(color.g * C * C)
           + IdType(color.b * C * C * C)
           + IdType(color.a * C * C * C * C);
}

RH<Texture2D> SelectionFramebuffer::GetColorTexture() const
{
    return p_colorTexture;
}
