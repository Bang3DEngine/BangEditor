#ifndef EXPLORER_H
#define EXPLORER_H

#include "Bang/Path.h"

#include "BangEditor/EditorUITab.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ExplorerEntry;

class Explorer : public EditorUITab
{
public:
    Explorer();
    virtual ~Explorer();

    void SetCurrentFilepath(const Path &path);
    const Path &GetCurrentPath() const;

    void Clear();

    static Explorer *GetInstance();

private:
    Path m_currentPath = Path::Empty;
    List<ExplorerEntry*> p_entries;

    UILabel *p_directionLabel = nullptr;
    GameObject *p_entriesContainer = nullptr;

    void AddEntry(const Path &entryPath);
};

NAMESPACE_BANG_EDITOR_END


#endif // EXPLORER_H
