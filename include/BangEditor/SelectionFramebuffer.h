#ifndef SELECTIONFRAMEBUFFER_H
#define SELECTIONFRAMEBUFFER_H

#include "Bang/UMap.h"
#include "Bang/Framebuffer.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Renderer;
FORWARD class Material;
FORWARD class GameObject;
FORWARD class ShaderProgram;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

class SelectionFramebuffer : public Framebuffer,
                             public EventListener<IEventsDestroy>
{
public:
    static const GL::Attachment AttColor = GL::Attachment::COLOR0;

    SelectionFramebuffer(int width, int height);
    virtual ~SelectionFramebuffer();

    void PrepareNewFrameForRender(const GameObject *go);
    void SetNextRenderSelectable(GameObject *go);
    void RenderForSelectionBuffer(Scene *scene);

    RH<Texture2D> GetColorTexture() const;
    GameObject *GetGameObjectInViewportPoint(const Vector2i &vpPoint);

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
private:
    using IdType = unsigned long long;

    RH<Texture2D> p_colorTexture;
    RH<Material> p_selectionMaterial;

    GameObject *p_nextRenderSelectable = nullptr;
    mutable UMap<GameObject*, IdType> m_gameObject_To_Id;
    mutable UMap<IdType, GameObject*> m_id_To_GameObject;

    void RenderForSelectionBuffer(Renderer *renderer);

    static Color MapIdToColor(IdType id);
    static IdType MapColorToId(const Color &color);

    Color GetSelectionColor(GameObject *go) const;

    friend class Gizmos;
    friend class GEngine;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTIONFRAMEBUFFER_H
