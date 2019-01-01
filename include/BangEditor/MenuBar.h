#ifndef MENUBAR_H
#define MENUBAR_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ShortcutManager.h"

namespace Bang
{
class UICanvas;
class UIFocusable;
class UIHorizontalLayout;
}

using namespace Bang;
namespace BangEditor
{
class MenuItem;
class Shortcut;

class MenuBar : public GameObject
{
    GAMEOBJECT_EDITOR(MenuBar);

public:
    MenuBar();

    void Update() override;

    MenuItem *AddItem();
    MenuItem *GetItem(int i);

    static void CreateGameObjectCreateMenuInto(MenuItem *rootItem);
    static void CreateGameObjectMiscMenuInto(
        MenuItem *rootItem,
        MenuItem **alignGameObjectWithViewItemOut = nullptr,
        MenuItem **alignViewWithGameObjectItemOut = nullptr);
    static void CreateComponentsMenuInto(MenuItem *rootItem);
    static void CreateAssetsMenuInto(MenuItem *rootItem);

    static constexpr int GetFixedHeight()
    {
        return 18;
    }

protected:
    virtual ~MenuBar() override;

private:
    MenuItem *m_fileItem = nullptr;
    MenuItem *m_editItem = nullptr;
    MenuItem *m_undoItem = nullptr;
    MenuItem *m_redoItem = nullptr;
    MenuItem *m_assetsItem = nullptr;
    MenuItem *m_componentsItem = nullptr;
    MenuItem *m_gameObjectsItem = nullptr;
    MenuItem *m_projectSettingsItem = nullptr;
    MenuItem *m_alignGameObjectWithViewItem = nullptr;
    MenuItem *m_alignViewWithGameObjectItem = nullptr;

    Array<MenuItem *> m_items;
    UIFocusable *p_focusable = nullptr;
    MenuItem *p_currentTopItemBeingShown = nullptr;
    UIHorizontalLayout *m_horizontalLayout = nullptr;

    void RegisterShortcut(const Shortcut &shortcut);

    static void OnShortcutPressed(const Shortcut &shortcut);

    static void OnNewProject(MenuItem *item);
    static void OnNewScene(MenuItem *item);
    static void OnSaveScene(MenuItem *item);
    static void OnSaveSceneAs(MenuItem *item);
    static void OnOpenScene(MenuItem *item);
    static void OnBuild(MenuItem *item);
    static void OnBuildAndRun(MenuItem *item);

    static void OnUndo(MenuItem *item);
    static void OnRedo(MenuItem *item);
    static void OnEditorSettings(MenuItem *item);
    static void OnRenderSettings(MenuItem *item);
    static void OnPhysicsSettings(MenuItem *item);

    static void OnCreateMaterial(MenuItem *item);
    static void OnCreateBehaviour(MenuItem *item);
    static void OnCreateShaderProgram(MenuItem *item);
    static void OnCreateTextureCubeMap(MenuItem *item);
    static void OnCreatePhysicsMaterial(MenuItem *item);
    static void OnCreateAnimatorLayerMask(MenuItem *item);
    static void OnCreateAnimatorStateMachine(MenuItem *item);

    static void OnAddAnimator(MenuItem *item);
    static void OnAddAudioListener(MenuItem *item);
    static void OnAddAudioSource(MenuItem *item);
    static void OnAddNewBehaviour(MenuItem *item);
    static void OnAddExistingBehaviour(MenuItem *item);
    static void OnAddEmptyBehaviour(MenuItem *item);
    static void OnAddCamera(MenuItem *item);
    static void OnAddBoxCollider(MenuItem *item);
    static void OnAddCapsuleCollider(MenuItem *item);
    static void OnAddSphereCollider(MenuItem *item);
    static void OnAddMeshCollider(MenuItem *item);
    static void OnAddPointLight(MenuItem *item);
    static void OnAddDirectionalLight(MenuItem *item);
    static void OnAddLineRenderer(MenuItem *item);
    static void OnAddMeshRenderer(MenuItem *item);
    static void OnAddDecalRenderer(MenuItem *item);
    static void OnAddVolumeRenderer(MenuItem *item);
    static void OnAddSkinnedMeshRenderer(MenuItem *item);
    static void OnAddWaterRenderer(MenuItem *item);
    static void OnAddParticleSystem(MenuItem *item);
    static void OnAddReflectionProbe(MenuItem *item);
    static void OnAddNavigationMesh(MenuItem *item);
    static void OnAddTransform(MenuItem *item);
    static void OnAddRectTransform(MenuItem *item);
    static void OnAddPostProcessEffect(MenuItem *item);
    static void OnAddRope(MenuItem *item);
    static void OnAddCloth(MenuItem *item);
    static void OnAddSSAO(MenuItem *item);
    static void OnAddBloom(MenuItem *item);
    static void OnAddFXAA(MenuItem *item);
    static void OnAddToneMapping(MenuItem *item);
    static void OnAddDepthOfField(MenuItem *item);
    static void OnAddRigidBody(MenuItem *item);
    static void OnAddUIAutoFocuser(MenuItem *item);
    static void OnAddUIButton(MenuItem *item);
    static void OnAddUICanvas(MenuItem *item);
    static void OnAddUICheckBox(MenuItem *item);
    static void OnAddUIComboBox(MenuItem *item);
    static void OnAddUIFocusable(MenuItem *item);
    static void OnAddUIHorizontalLayout(MenuItem *item);
    static void OnAddUIImageRenderer(MenuItem *item);
    static void OnAddUIInputText(MenuItem *item);
    static void OnAddUILabel(MenuItem *item);
    static void OnAddUILayoutElement(MenuItem *item);
    static void OnAddUILayoutIgnorer(MenuItem *item);
    static void OnAddUIMask(MenuItem *item);
    static void OnAddUIRectMask(MenuItem *item);
    static void OnAddUISlider(MenuItem *item);
    static void OnAddUIScrollPanel(MenuItem *item);
    static void OnAddUITextRenderer(MenuItem *item);
    static void OnAddUIVerticalLayout(MenuItem *item);

    static void OnCreateEmpty(MenuItem *item);
    static void OnCreateCone(MenuItem *item);
    static void OnCreateCube(MenuItem *item);
    static void OnCreateCapsule(MenuItem *item);
    static void OnCreateCylinder(MenuItem *item);
    static void OnCreatePlane(MenuItem *item);
    static void OnCreateSphere(MenuItem *item);
    static void OnCreateCamera(MenuItem *item);
    static void OnCreateParticleSystemGO(MenuItem *item);
    static void OnCreateDecalGO(MenuItem *item);
    static void OnCreateDirectionalLightGO(MenuItem *item);
    static void OnCreatePointLightGO(MenuItem *item);
    static UICanvas *OnCreateUICanvasGO(MenuItem *item);
    static void OnCreateUIEmptyGO(MenuItem *item);
    static void OnCreateUIImageGO(MenuItem *item);
    static void OnCreateUITextGO(MenuItem *item);
    static void OnCreateReflectionProbeGO(MenuItem *item);
    static void OnCreateNavigationMeshGO(MenuItem *item);
    static void OnAlignGameObjectWithView(MenuItem *item);
    static void OnAlignViewWithGameObject(MenuItem *item);
    static void OnEndCreateGameObjectFromMenuBar(GameObject *primitive);
    static void OnEndCreateUIGameObjectFromMenuBar(GameObject *uiGo);

    static void OpenProject(const Path &projectFileFilepath);

    static Path CreateNewBehaviour();
    static MenuBar *GetInstance();

    friend class ProjectManager;
    friend class SceneOpenerSaver;
};
}

#endif  // MENUBAR_H
