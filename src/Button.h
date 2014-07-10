//
//  Button.h
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014 Chorded Constructions. All rights reserved.
//

#pragma once

#include "Control.h"
#include "cinder/gl/TextureFont.h"

namespace Cinder { namespace ControlRoom {

typedef std::shared_ptr<class Button> ButtonRef;

enum class ButtonType {MomentaryPushIn};

class Button : public Control {
public:
    static ButtonRef create(const Rectf& frame, const ButtonType& type, const std::string& title);
    ~Button();

    ButtonRef getPtr() { return std::static_pointer_cast<Button>(shared_from_this()); }

    inline void setType(const ButtonType& type) { mType = type; }
    inline ButtonType getType() const { return mType; }

    inline void setTitle(const std::string& title) { mTitle = title; }
    inline std::string getTitle() const { return mTitle; }

    void draw();

    void mouseDown(MouseEvent event);
    void mouseDrag(MouseEvent event);
    void mouseUp(MouseEvent event);

private:
    Button(const Rectf& frame, const ButtonType& type, const std::string& title);

    ButtonType mType;
    std::string mTitle;

    gl::TextureFontRef mFont;
};

}}
