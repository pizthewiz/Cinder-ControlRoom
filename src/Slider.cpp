//
//  Slider.cpp
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 01 Jul 2014.
//  Copyright 2014 Chorded Constructions. All rights reserved.
//

#include "Slider.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;

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
    // background
    gl::color(Color::gray(216.0f/255.0f));
    Rectf bounds = getBounds();
    gl::drawSolidRect(bounds);

    // fill
    float width = bounds.getWidth() * (mValue - mMinimum) / mMaximum;
    Rectf fillFrame = Rectf(Vec2f(0.0f, 0.0f), Vec2f(width, bounds.getHeight()));
    gl::color(Color::gray(170.0f/255.0f));
    gl::drawSolidRect(fillFrame);
}

#pragma mark -

void Slider::mouseDown(MouseEvent event) {
    if (!isEnabled()) {
        return;
    }

    mTracking = true;

    Vec2i point = convertPointFromView(event.getPos(), nullptr);
    float value = (float)point.x / mFrame.getWidth() * (mMaximum - mMinimum);
    setValue(value);

    if (mControlEventHandlerMap.find(ControlEvent::Down) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::Down](getPtr());
    }

    if (mContinuous && mControlEventHandlerMap.find(ControlEvent::ValueChanged) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::ValueChanged](getPtr());
    }
}

void Slider::mouseDrag(MouseEvent event) {
    if (!isEnabled()) {
        return;
    }

    Vec2i point = convertPointFromView(event.getPos(), nullptr);
    float value = (float)point.x / mFrame.getWidth() * (mMaximum - mMinimum);
    setValue(value);

    if (mContinuous && mControlEventHandlerMap.find(ControlEvent::ValueChanged) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::ValueChanged](getPtr());
    }
}

void Slider::mouseUp(MouseEvent event) {
    if (!isEnabled()) {
        return;
    }

    Vec2i point = convertPointFromView(event.getPos(), nullptr);
    ControlEvent e = getBounds().contains(point) ? ControlEvent::UpInside : ControlEvent::UpOutside;
    if (mControlEventHandlerMap.find(e) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[e](getPtr());
    }

    if (!mContinuous && mControlEventHandlerMap.find(ControlEvent::ValueChanged) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::ValueChanged](getPtr());
    }

    mTracking = false;
}

}}
