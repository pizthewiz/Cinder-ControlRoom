//
//  Label.cpp
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014-2015 Chorded Constructions. All rights reserved.
//

#include "Label.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;

LabelRef Label::create(const Rectf& frame, const std::string& text) {
    return LabelRef(new Label(frame, text));
}

Label::Label(const Rectf& frame, const std::string& text) : View(frame), mText(text), mAlignment(TextAlignment::Left), mEnabled(true) {
    mFont = gl::TextureFont::create(Font("Menlo", 12.0f * 2.0f), gl::TextureFont::Format().enableMipmapping());
}

Label::~Label() {
}

#pragma mark -

void Label::draw() {
    gl::pushModelView(); {
        gl::translate(getFrame().getUpperLeft());

        gl::enableAlphaBlending(); {
            gl::color(Color::gray(74.0f/255.0f));

            ci::Vec2f textSize = mFont->measureString(mText) * 0.5f;
            Rectf availableFrame = getBounds();
            float x = availableFrame.getX1();
            if (mAlignment == TextAlignment::Center) {
                x = availableFrame.getX1() + math<float>::max((availableFrame.getWidth() - textSize.x) * 0.5f, 0.0f);
            } else if (mAlignment == TextAlignment::Right) {
                x = math<float>::max(availableFrame.getX2() - textSize.x, 0.0f);
            }
            float y = availableFrame.getY1() + math<float>::max((availableFrame.getHeight() - textSize.y) * 0.5f, 0.0f) + textSize.y - 4.0f /* magic number */;
            ci::Vec2f baseline = ci::Vec2f(x, y);
            mFont->drawString(mText, baseline, gl::TextureFont::DrawOptions().scale(0.5f).pixelSnap(false));
        } gl::disableAlphaBlending();
    } gl::popModelView();
}

}}
