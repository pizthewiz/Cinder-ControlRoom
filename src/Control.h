//
//  Control.h
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 01 Jul 2014.
//  Copyright 2014 Chorded Constructions. All rights reserved.
//

#pragma once

#include "View.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;

typedef std::shared_ptr<class Control> ControlRef;

enum class ControlState {Normal, Highlighted, Disabled};
enum class ControlEvent {Down, UpInside, UpOutside, ValueChanged};

class Control : public View {
public:
    ~Control() {}

    inline void setEnabled(bool enabled) {
        mState = enabled ? ControlState::Normal : ControlState::Disabled;
    }
    inline bool isEnabled() const {
        return mState != ControlState::Disabled;
    }

    inline ControlState getState() const {
        return mState;
    }

    inline void setHighlighted(bool highlighted) {
        if (!isEnabled()) {
            return;
        }
        mState = highlighted ? ControlState::Highlighted : ControlState::Normal;
    }
    inline bool isHighlighted() const {
        return mState == ControlState::Highlighted;
    }

    template<typename T, typename Y>
    inline void connectControlEventHandler(const ControlEvent& event, T handler, Y* obj) {
        connectControlEventHandler(event, std::bind(handler, obj));
    }
    void connectControlEventHandler(const ControlEvent& event, const std::function<void (void)>& handler) {
        mControlEventHandlerMap[event] = handler;
    }

    virtual void draw() = 0;

protected:
    Control(const Rectf& frame) : View(frame), mState(ControlState::Normal) {}

    ControlState mState;

    std::map<ControlEvent, std::function<void (void)>> mControlEventHandlerMap;
};

}}
