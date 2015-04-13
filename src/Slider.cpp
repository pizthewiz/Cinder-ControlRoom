//
//  Slider.cpp
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 01 Jul 2014.
//  Copyright 2014-2015 Chorded Constructions. All rights reserved.
//

#include "Slider.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;
using namespace ci::app;

SliderRef Slider::create(const Rectf& frame, float value, float minimum, float maximum) {
    return SliderRef(new Slider(frame, value, minimum, maximum));
}

Slider::Slider(const Rectf& frame, float value, float minimum, float maximum) : Control(frame), mValue(value), mMinimum(minimum), mMaximum(maximum), mContinuous(true), mTracking(false) {
}

Slider::~Slider() {
}

#pragma mark -

inline void Slider::setValue(float value) {
    value = math<float>::clamp(value, mMinimum, mMaximum);
    if (value == mValue) {
        return;
    }

    mValue = value;
}

#pragma mark -

void Slider::draw() {
    gl::pushModelView(); {
        gl::translate(getFrame().getUpperLeft());

        // background
        gl::color(Color::gray(216.0f/255.0f));
        Rectf bounds = getBounds();
        gl::drawSolidRect(bounds);

        // fill
        float width = bounds.getWidth() * (mValue - mMinimum) / (mMaximum - mMinimum);
        Rectf fillFrame = Rectf(vec2(0.0f, 0.0f), vec2(width, bounds.getHeight()));
        gl::color(Color::gray(170.0f/255.0f));
        gl::drawSolidRect(fillFrame);
    } gl::popModelView();
}

#pragma mark -

void Slider::mouseDown(const MouseEvent& event) {
    if (!isEnabled()) {
        return;
    }

    mTracking = true;

    ivec2 point = convertPointFromView(event.getPos(), nullptr);
    float value = mMinimum + (float)point.x / mFrame.getWidth() * (mMaximum - mMinimum);
    setValue(value);

    if (mControlEventHandlerMap.find(ControlEvent::Down) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::Down](getPtr());
    }

    if (mContinuous && mControlEventHandlerMap.find(ControlEvent::ValueChanged) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::ValueChanged](getPtr());
    }
}

void Slider::mouseUp(const MouseEvent& event) {
    if (!isEnabled()) {
        return;
    }

    ivec2 point = convertPointFromView(event.getPos(), nullptr);
    ControlEvent e = getBounds().contains(point) ? ControlEvent::UpInside : ControlEvent::UpOutside;
    if (mControlEventHandlerMap.find(e) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[e](getPtr());
    }

    if (!mContinuous && mControlEventHandlerMap.find(ControlEvent::ValueChanged) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::ValueChanged](getPtr());
    }

    mTracking = false;
}

void Slider::mouseDrag(const MouseEvent& event) {
    if (!isEnabled()) {
        return;
    }

    ivec2 point = convertPointFromView(event.getPos(), nullptr);
    float value = mMinimum + (float)point.x / mFrame.getWidth() * (mMaximum - mMinimum);
    setValue(value);

    if (mContinuous && mControlEventHandlerMap.find(ControlEvent::ValueChanged) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::ValueChanged](getPtr());
    }
}

}}
