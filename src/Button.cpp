//
//  Button.cpp
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014-2015 Chorded Constructions. All rights reserved.
//

#include "Button.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;
using namespace ci::app;

ButtonRef Button::create(const Rectf& frame, const ButtonType type, const std::string& title) {
    return ButtonRef(new Button(frame, type, title));
}

Button::Button(const Rectf& frame, const ButtonType type, const std::string& title) : Control(frame), mType(type), mTitle(title) {
    mFont = gl::TextureFont::create(Font("Menlo", 12.0f * 2.0f), gl::TextureFont::Format().enableMipmapping());
}

Button::~Button() {
}

#pragma mark -

void Button::draw() {
    gl::pushModelView(); {
        gl::translate(getFrame().getUpperLeft());

        // background
        gl::color(isHighlighted() ? Color::gray(170.0f/255.0f) : Color::gray(216.0f/255.0f));
        Rectf bounds = getBounds();
        gl::drawSolidRect(bounds);

        // label
        gl::enableAlphaBlending(); {
            gl::color(isHighlighted() ? Color::gray(230.0f/255.0f) : Color::gray(74.0f/255.0f));

          ci::Vec2f titleSize = mFont->measureString(mTitle) * 0.5f;
          Rectf availableFrame = bounds.inflated(ci::Vec2f(-5.0f, 0.0f));
            float x = availableFrame.getX1() + math<float>::max((availableFrame.getWidth() - titleSize.x) * 0.5f, 0.0f);
            float y = availableFrame.getY1() + math<float>::max((availableFrame.getHeight() - titleSize.y) * 0.5f, 0.0f) + titleSize.y - 4.0f /* magic number */;
            ci::Vec2f baseline = ci::Vec2f(x, y);
            mFont->drawString(mTitle, baseline, gl::TextureFont::DrawOptions().scale(0.5f).pixelSnap(false));
        } gl::disableAlphaBlending();
    } gl::popModelView();
}

#pragma mark -

void Button::mouseDown(const MouseEvent& event) {
    if (!isEnabled()) {
        return;
    }

    mState = ControlState::Highlighted;

    if (mControlEventHandlerMap.find(ControlEvent::Down) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[ControlEvent::Down](getPtr());
    }
}

void Button::mouseUp(const MouseEvent& event) {
    if (!isEnabled()) {
        return;
    }

  ci::Vec2f point = convertPointFromView(event.getPos(), nullptr);
    ControlEvent e = getBounds().contains(point) ? ControlEvent::UpInside : ControlEvent::UpOutside;
    if (mControlEventHandlerMap.find(e) != mControlEventHandlerMap.end()) {
        mControlEventHandlerMap[e](getPtr());
    }

    mState = ControlState::Normal;
}

void Button::mouseDrag(const MouseEvent& event) {
    if (!isEnabled()) {
        return;
    }

  ci::Vec2f point = convertPointFromView(event.getPos(), nullptr);
    if (getBounds().contains(point) != isHighlighted()) {
        mState = mState == ControlState::Normal ? ControlState::Highlighted : ControlState::Normal;
    }
}

}}
