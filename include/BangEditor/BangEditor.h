#define NAMESPACE_EDITOR_NAME        BangEditor
#define USING_NAMESPACE_BANG_EDITOR  using namespace NAMESPACE_EDITOR_NAME;
#define NAMESPACE_BANG_EDITOR_BEGIN  namespace NAMESPACE_EDITOR_NAME {
#define NAMESPACE_BANG_EDITOR_END    }

#define GAMEOBJECT_EDITOR(Class) \
    GAMEOBJECT(Class) \
    friend class Bang::ObjectManager;