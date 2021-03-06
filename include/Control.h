//
//  Control.h
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 01 Jul 2014.
//  Copyright 2014-2015 Chorded Constructions. All rights reserved.
//

#pragma once

#include "View.h"

namespace Cinder { namespace ControlRoom {

typedef std::shared_ptr<class Control> ControlRef;

class Control : public View {
public:
    enum class ControlState { Normal, Highlighted, Disabled };
    enum class ControlEvent { Down, UpInside, UpOutside, ValueChanged };

    virtual ~Control() {}

    inline void setEnabled(bool enabled) { mState = enabled ? ControlState::Normal : ControlState::Disabled; }
    inline bool isEnabled() const { return mState != ControlState::Disabled; }

    inline ControlState getState() const { return mState; }

    inline void setHighlighted(bool highlighted) { mState = !isEnabled() ? ControlState::Disabled : (highlighted ? ControlState::Highlighted : ControlState::Normal); }
    inline bool isHighlighted() const { return mState == ControlState::Highlighted; }

    template<typename T, typename Y>
    inline void connectControlEventHandler(const ControlEvent event, T handler, Y* obj) {
        connectControlEventHandler(event, std::bind(handler, obj));
    }
    void connectControlEventHandler(const ControlEvent event, const std::function<void (const ControlRef&)>& handler) {
        mControlEventHandlerMap[event] = handler;
    }

    virtual void draw() = 0;

protected:
    Control(const ci::Rectf& frame) : View(frame), mState(ControlState::Normal) {}

    ControlState mState;

    std::map<ControlEvent, std::function<void (const ControlRef&)>> mControlEventHandlerMap;
};

}}
