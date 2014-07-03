//
//  Slider.h
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 01 Jul 2014.
//  Copyright 2014 Chorded Constructions. All rights reserved.
//

#pragma once

#include "Control.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;

typedef std::shared_ptr<class Slider> SliderRef;

class Slider : public Control {
public:
    static SliderRef create(const Rectf& frame, float value, float minimum, float maximum);

    inline void setValue(float value);
    inline float getValue() const {
        return mValue;
    }
    inline void setMinimumValue(float value) {
        mMinimum = value;
    }
    inline float getMinimumValue() const {
        return mMinimum;
    }
    inline void setMaximumValue(float value) {
        mMaximum = value;
    }
    inline float getMaximumValue() const {
        return mMaximum;
    }

    inline void setContinuous(bool continuous) {
        mContinuous = continuous;
    }
    inline bool isContinuous() const {
        return mContinuous;
    }

    void draw();

    void mouseDown(MouseEvent event);
    void mouseDrag(MouseEvent event);
    void mouseUp(MouseEvent event);

private:
    Slider(const Rectf& frame, float value, float minimum, float maximum);

    float mValue;
    float mMinimum;
    float mMaximum;

    bool mContinuous;
    bool mTracking;
};

}}
