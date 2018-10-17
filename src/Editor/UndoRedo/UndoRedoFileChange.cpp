#include "BangEditor/UndoRedoFileChange.h"

#include <fstream>
#include <string>

#include "Bang/Assert.h"
#include "Bang/File.h"
#include "Bang/String.h"

using namespace Bang;
using namespace BangEditor;

UndoRedoFileChange::UndoRedoFileChange(const Path &path)
{
    ASSERT(path.IsFile());

    m_path = path;
}

UndoRedoFileChange::~UndoRedoFileChange()
{
    if (m_previousContents)
    {
        delete[] m_previousContents;
    }

    if (m_newContents)
    {
        delete[] m_newContents;
    }
}

void UndoRedoFileChange::ReadBefore()
{
    Read(&m_previousContents, &m_previousContentsSize);
}

void UndoRedoFileChange::ReadAfter()
{
    Read(&m_newContents, &m_newContentsSize);
}

void UndoRedoFileChange::Undo()
{
    ASSERT(m_previousContents);
    File::Write(m_path, m_previousContents, m_previousContentsSize);
}

void UndoRedoFileChange::Redo()
{
    ASSERT(m_newContents);
    File::Write(m_path, m_newContents, m_newContentsSize);
}

void UndoRedoFileChange::Read(Byte **bytePointer, std::size_t *sizePointer)
{
    std::ifstream ifs(m_path.GetAbsolute().ToCString(),
                      std::ios::binary | std::ios::ate);
    *sizePointer = ifs.tellg();

    *bytePointer = new Byte[*sizePointer];
    ifs.seekg(0, std::ios::beg);
    ifs.read(RCAST<char *>(*bytePointer), *sizePointer);

    ifs.close();
}
