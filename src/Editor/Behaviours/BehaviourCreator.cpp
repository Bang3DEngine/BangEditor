#include "BangEditor/BehaviourCreator.h"

#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/Extensions.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/QtProjectManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Path BehaviourCreator::GetNewBehaviourHeaderFilepath(const Path &dirPath,
                                                     const String &behaviourName)
{
    return dirPath.Append(behaviourName)
             .AppendExtension(Extensions::GetDefaultBehaviourHeaderExtension());
}

Path BehaviourCreator::GetNewBehaviourSourceFilepath(const Path &dirPath,
                                                     const String &behaviourName)
{
    return dirPath.Append(behaviourName)
             .AppendExtension(Extensions::GetDefaultBehaviourSourceExtension());
}

bool BehaviourCreator::CanCreateNewBehaviour(const Path &dirPath,
                                             const String &behaviourName)
{
    if (!dirPath.IsDir()) { return false;  }

    Path headerPath = BehaviourCreator::GetNewBehaviourHeaderFilepath(dirPath,
                                                               behaviourName);
    Path sourcePath = BehaviourCreator::GetNewBehaviourSourceFilepath(dirPath,
                                                               behaviourName);

    if (headerPath.IsFile()) { return false; }
    if (sourcePath.IsFile()) { return false; }

    return true;
}

void BehaviourCreator::CreateNewBehaviour(const Path &dirPath,
                                          const String &behaviourName,
                                          Path *behaviourHeaderPath,
                                          Path *behaviourSourcePath)
{
    ASSERT(behaviourHeaderPath);
    ASSERT(behaviourSourcePath);
    ASSERT(CanCreateNewBehaviour(dirPath, behaviourName));

    Path headerPath = BehaviourCreator::GetNewBehaviourHeaderFilepath(dirPath,
                                                               behaviourName);
    Path sourcePath = BehaviourCreator::GetNewBehaviourSourceFilepath(dirPath,
                                                               behaviourName);

    File::Write(headerPath, BehaviourCreator::GetNewBehaviourHeaderCode(behaviourName));
    File::Write(sourcePath, BehaviourCreator::GetNewBehaviourSourceCode(behaviourName));

    QtProjectManager::CreateQtProjectFile();

    *behaviourHeaderPath = headerPath;
    *behaviourSourcePath = sourcePath;
}

String BehaviourCreator::GetNewBehaviourHeaderCode(const String &behaviourName)
{
    Path templatePath = EditorPaths::GetEditorAssetsDir().
                                     Append("Templates").
                                     Append("BehaviourTemplates").
                                     Append("Header").
                                     AppendExtension("template");

    String code = File::GetContents(templatePath);
    code = code.Replace("BEHAVIOUR_NAME", behaviourName);

    return code;
}

String BehaviourCreator::GetNewBehaviourSourceCode(const String &behaviourName)
{
    Path templatePath = EditorPaths::GetEditorAssetsDir().
                                     Append("Templates").
                                     Append("BehaviourTemplates").
                                     Append("Source").
                                     AppendExtension("template");

    String code = File::GetContents(templatePath);
    code = code.Replace("BEHAVIOUR_NAME", behaviourName);

    return code;
}
