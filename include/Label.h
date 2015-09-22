//
//  Label.h
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014-2015 Chorded Constructions. All rights reserved.
//

#pragma once

#include "View.h"
#include "cinder/gl/TextureFont.h"

namespace Cinder { namespace ControlRoom {

typedef std::shared_ptr<class Label> LabelRef;

class Label : public View {
public:
    enum class TextAlignment { Left, Center, Right };

    static LabelRef create(const ci::Rectf& frame, const std::string& text);
    ~Label();

    LabelRef getPtr() { return std::static_pointer_cast<Label>(shared_from_this()); }

    inline void setText(const std::string& text) { mText = text; }
    inline std::string getText() const { return mText; }

    inline void setTextAlignment(const TextAlignment alignment) { mAlignment = alignment; }
    inline TextAlignment getTextAlignment() const { return mAlignment; }

    inline void setEnabled(bool enabled) { mEnabled = enabled; }
    inline bool isEnabled() const { return mEnabled; }

    void draw() override;

protected:
    Label(const ci::Rectf& frame, const std::string& text);

    std::string mText;
    TextAlignment mAlignment;
    bool mEnabled;

    ci::gl::TextureFontRef mFont;
};

}}
