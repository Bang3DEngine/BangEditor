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
    CreateAttachmentTex2D(GL::Attachment::DEPTH, GL::ColorFormat::DEPTH24);
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

    List<GameObject*> gameObjects = go->GetChildrenRecursively();
    for (GameObject *go : gameObjects)
    {
        m_gameObject_To_Id[go] = id;
        m_id_To_GameObject[id] = go;

        if (Input::GetKeyDown(Key::X))
        {
            Debug_Log(go << ": " << MapIdToColor(id));
        }

        go->EventEmitter<IEventsDestroy>::RegisterListener(this);

        ++id;
    }
    if (Input::GetKeyDown(Key::X)) { Debug_Log("============"); }
}

void SelectionFramebuffer::RenderForSelectionBuffer(Scene *scene)
{
    GEngine *ge = GEngine::GetInstance();

    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Push(GL::Pushable::BLEND_STATES);
    GL::Push(GL::Pushable::DEPTH_STATES);

    // Selection rendering
    List<Renderer*> sceneRends;
    List<GameObject*> gos = scene->GetChildrenRecursively();
    for (GameObject *go : gos)
    {
        if (go->IsActive() && !go->HasComponent<NotSelectableInEditor>())
        {
            List<Renderer*> goRends = go->GetComponents<Renderer>();
            for (Renderer *rend : goRends)
            {
                if (rend->IsActive() && rend->IsVisible())
                {
                    sceneRends.PushBack(rend);
                }
            }
        }
    }

    auto RenderScenePassForSFB = [&sceneRends, this, ge](RenderPass renderPass)
    {
        for (Renderer *rend : sceneRends)
        {
            if (Material *mat = rend->GetActiveMaterial())
            {
                if (mat->GetRenderPass() == renderPass)
                {
                    RenderForSelectionBuffer(rend);
                }
            }
        }
    };

    Bind();
    SetAllDrawBuffers();
    GL::Disable(GL::Enablable::BLEND);

    RenderScenePassForSFB(RenderPass::SCENE);
    RenderScenePassForSFB(RenderPass::SCENE_TRANSPARENT);
    GL::ClearStencilDepthBuffers();
    GL::SetDepthFunc(GL::Function::LEQUAL);
    RenderScenePassForSFB(RenderPass::CANVAS);
    GL::ClearDepthBuffer();
    // RenderScenePassForSFB(RenderPass::OVERLAY);

    UnBind();

    GL::Pop(GL::Pushable::DEPTH_STATES);
    GL::Pop(GL::Pushable::BLEND_STATES);
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
}

void SelectionFramebuffer::RenderForSelectionBuffer(Renderer *rend)
{
    ASSERT(GL::IsBound(this));
    if (!rend->GetActiveMaterial()) { return; }

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

    if (Input::GetKeyDown(Key::X))
    {
        Texture2D *tex = GetAttachmentTex2D(AttColor);
        Imageb img;
        img = tex->ToImage<Byte>();
        for (int y = 0; y < img.GetHeight(); ++y)
        {
            for (int x = 0; x < img.GetWidth(); ++x)
            {
                if (rand()%10000 == 0) { Debug_Log(img.GetPixel(x,y)); };
                img.SetPixel(x, y, img.GetPixel(x,y).WithAlpha(1.0f));
            }
        }
        // img.Create(tex->GetWidth(), tex->GetHeight());
        // for (int y = 0; y < img.GetHeight(); ++y)
        // {
        //     for (int x = 0; x < img.GetWidth(); ++x)
        //     {
        //         img.SetPixel(x, y, ReadColor(x,y,AttColor));
        //     }
        // }
        img.Export(Paths::GetExecutableDir().Append("test.png"));
    }

    // Debug_Log(vpPoint << ", " << colorUnderMouse << ", " << id);
    if (colorUnderMouse != Color::Zero && m_id_To_GameObject.ContainsKey(id))
    {
        // Debug_Log(GetAttachmentTex2D(AttColor)->GetSize());
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
    constexpr IdType C = 5;
    Color color (double(   id                % C),
                 double(  (id / C)           % C),
                 double( ((id / C) / C)      % C),
                 1.0f // double((((id / C) / C) / C) % C)
                );
   return color / double(C);
}

typename SelectionFramebuffer::IdType
SelectionFramebuffer::MapColorToId(const Color &color)
{
    constexpr IdType C = 5;
    return   IdType(color.r * C)
           + IdType(color.g * C * C)
           + IdType(color.b * C * C * C)
           ; // + IdType(color.a * C * C * C * C);
}

RH<Texture2D> SelectionFramebuffer::GetColorTexture() const
{
    return p_colorTexture;
}
