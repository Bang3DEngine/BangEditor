#ifndef QTPROJECTMANAGER_H
#define QTPROJECTMANAGER_H

#include "Bang/Path.h"
#include "Bang/String.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class QtProjectManager
{
public:
    static Path GetQtProjectFilepath();

    static void CreateQtProjectFile();
    static void OpenBehaviourInQtCreator(const Path &behaviourFilepath);

private:
    QtProjectManager();

    static Path GetQtProjectDir();
    static bool IsQtCreatorOpenedCurrentProject();
};

NAMESPACE_BANG_EDITOR_END

#endif // QTPROJECTMANAGER_H
