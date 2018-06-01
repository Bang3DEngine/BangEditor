#ifndef IEVENTSTABHEADER_H
#define IEVENTSTABHEADER_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabHeader;

class IEventsTabHeader
{
    IEVENTS_NS(IEventsTabHeader);

public:
    virtual void OnTabHeaderClicked(UITabHeader *header) = 0;
};

NAMESPACE_BANG_EDITOR_END

#endif // IEVENTSTABHEADER_H
