#ifndef __GAME_INPUT_DEFINITION_H__
#define __GAME_INPUT_DEFINITION_H__

#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/gmap.h>

struct InputDef : public RefObjectImpl<IRefObject> {
    typedef GameBasMap<wxString, wxInt32> TInputMap;

public:
    TInputMap m_inputMap;

public:
    InputDef()
    {
    }
};

#endif // __GAME_INPUT_DEFINITION_H__