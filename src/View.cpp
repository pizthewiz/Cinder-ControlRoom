//
//  View.cpp
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014 Chorded Constructions. All rights reserved.
//

#include "View.h"


namespace Cinder { namespace ControlRoom {

using namespace ci;
using namespace ci::app;

ViewRef View::create(const ci::Rectf& frame) {
    return ViewRef(new View(frame));
}

View::View(const Rectf& frame) : mFrame(frame), mBackgroundColor(ColorAf::zero()), mHidden(false) {
}

View::~View() {
    disconnectEventListeners();
    removeFromSuperview();
}

#pragma mark -

void View::addSubview(const ViewRef& view) {
    mSubviews.push_back(view);
    view->setSuperview(shared_from_this());
}

void View::removeFromSuperview() {
    if (!mSuperview) {
        return;
    }
    mSuperview->removeSubview(shared_from_this());
}

bool View::isDescendantOfView(const ViewRef& view) {
    // 💀 - view of nullptr is used to represent window content view
    if (!view) {
        return true;
    }

    bool status = false;
    ViewRef v = getSuperview();
    while (v) {
        if (v == view) {
            status = true;
            break;
        }
        v = v->getSuperview();
    }
    return status;
}

#pragma mark -

void View::draw() {
    gl::pushModelView(); {
        gl::translate(mFrame.getUpperLeft());

        // draw background
        if (mBackgroundColor != ColorAf::zero()) {
            gl::color(mBackgroundColor);
            gl::drawSolidRect(getBounds());
        }

        // draw subviews
        for (const ViewRef& view : mSubviews) {
            if (view->isHidden()) {
                continue;
            }

            // TODO - move this into Control.cpp
            gl::pushModelView(); {
                gl::translate(view->getFrame().getUpperLeft());

                view->draw();
            } gl::popModelView();
        }
    } gl::popModelView();
}

#pragma mark -

Vec2i View::convertPointFromView(const Vec2f& point, const ViewRef& view) {
    // view to local
    Vec2f p = point;
    if (isDescendantOfView(view)) {
        p -= mFrame.getUpperLeft();
        if (mSuperview && mSuperview != view) {
            p = mSuperview->convertPointFromView(p, view);
        }
    } else {
        // TODO - implement
    }
    return p;
}

Vec2i View::convertPointToView(const Vec2i& point, const ViewRef& view) {
    // local to view
    Vec2i p = point;
    if (isDescendantOfView(view)) {
        // TODO - implement
    } else {
        // TODO - implement
    }
    return p;
}

#pragma mark -

void View::connectEventListeners() {
    app::App* app = app::App::get();
    mConnectionMouseDown = app->getWindow()->getSignalMouseDown().connect([&](MouseEvent event) {
        mTrackingView = nullptr;

        Vec2i point = convertPointFromView(event.getPos(), nullptr);
        if (!getBounds().contains(point)) {
            return;
        }

        for (const ViewRef& view : mSubviews) {
            if (!view->getFrame().contains(point) || view->isHidden()) {
                continue;
            }

            // TODO - reject on disabled controls

            mTrackingView = view;
            break;
        }

        if (mTrackingView) {
            mTrackingView->mouseDown(event);
        }
        event.setHandled();
    });
    mConnectionMouseDrag = app->getWindow()->getSignalMouseDrag().connect([&](MouseEvent event) {
        if (!mTrackingView) {
            return;
        }

        mTrackingView->mouseDrag(event);
        event.setHandled();
    });
    mConnectionMouseUp = app->getWindow()->getSignalMouseUp().connect([&](MouseEvent event) {
        if (!mTrackingView) {
            return;
        }

        mTrackingView->mouseUp(event);
        event.setHandled();
        
        mTrackingView = nullptr;
    });
}

void View::disconnectEventListeners() {
    mConnectionMouseDown.disconnect();
    mConnectionMouseDrag.disconnect();
    mConnectionMouseUp.disconnect();
}

#pragma mark - PRIVATE

void View::removeSubview(const ViewRef& view) {
    if (!view) {
        return;
    }
    mSubviews.erase(std::find(mSubviews.begin(), mSubviews.end(), view));
    view->setSuperview(nullptr);
}

}}
