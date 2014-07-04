//
//  View.h
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014 Chorded Constructions. All rights reserved.
//

#pragma once

#include "cinder/app/App.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;
using namespace ci::app;

typedef std::shared_ptr<class View> ViewRef;

class View : public std::enable_shared_from_this<View> {
public:
    virtual ~View();

    ViewRef getPtr() { return shared_from_this(); }

    inline void setFrame(const Rectf& frame) {
        mFrame = frame;
    }
    inline Rectf getFrame() const {
        return mFrame;
    }
    inline Rectf getBounds() const {
        return Rectf(Vec2f(0.0f, 0.0f), mFrame.getSize());
    }

    inline ViewRef getSuperview() const {
        return mSuperview;
    }
    inline std::vector<ViewRef> getSubviews() const {
        return mSubviews;
    }
    inline void addSubview(const ViewRef& view) {
        mSubviews.push_back(view);
        view->setSuperview(shared_from_this());
    }
    inline void removeFromSuperview() {
        if (!mSuperview) {
            return;
        }
        mSuperview->removeSubview(shared_from_this());
    }
    bool isDescendantOfView(const ViewRef& view);

    inline void setBackgroundColor(const Color& color) {
        mBackgroundColor = color;
    }
    inline Color getBackgroundColor() const {
        return mBackgroundColor;
    }

    inline void setHidden(bool hidden) {
        mHidden = hidden;
    }
    inline bool isHidden() const {
        return mHidden;
    }

    virtual void draw();

    virtual void mouseDown(MouseEvent event) {}
    virtual void mouseDrag(MouseEvent event) {}
    virtual void mouseUp(MouseEvent event) {}

    Vec2i convertPointFromView(const Vec2f& point, const ViewRef& view);
    Vec2i convertPointToView(const Vec2i& point, const ViewRef& view);

protected:
    View(const Rectf& frame);

    inline void setSuperview(const ViewRef& view) {
        mSuperview = view;
    }
    inline void removeSubview(const ViewRef& view) {
        if (!view) {
            return;
        }
        mSubviews.erase(std::find(mSubviews.begin(), mSubviews.end(), view));
        view->setSuperview(nullptr);
    }

    Rectf mFrame;

    ViewRef mSuperview;
    std::vector<ViewRef> mSubviews;

    Color mBackgroundColor;
    bool mHidden;
};

}}
