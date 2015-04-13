//
//  Button.h
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014-2015 Chorded Constructions. All rights reserved.
//

#pragma once

#include "Control.h"

namespace Cinder { namespace ControlRoom {

typedef std::shared_ptr<class Button> ButtonRef;

class Button : public Control {
public:
    enum class ButtonType { MomentaryPushIn };

    static ButtonRef create(const ci::Rectf& frame, const ButtonType type, const std::string& title);
    ~Button();

    ButtonRef getPtr() { return std::static_pointer_cast<Button>(shared_from_this()); }

    inline void setType(const ButtonType type) { mType = type; }
    inline ButtonType getType() const { return mType; }

    inline void setTitle(const std::string& title) { mTitle = title; }
    inline std::string getTitle() const { return mTitle; }

    void draw();

    void mouseDown(const ci::app::MouseEvent& event);
    void mouseUp(const ci::app::MouseEvent& event);
    void mouseDrag(const ci::app::MouseEvent& event);

private:
    Button(const ci::Rectf& frame, const ButtonType type, const std::string& title);

    ButtonType mType;
    std::string mTitle;

    ci::gl::TextureFontRef mFont;
};

}}
