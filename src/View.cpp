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

#pragma mark - HIERARCHY

void View::addSubview(const ViewRef& view) {
    // prevent cycle
    if (isDescendantOfView(view)) {
        // TODO - throw?
        return;
    }

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
    // 💀 - nullptr is used to represent window's content view, i.e. the root view
    if (!view) {
        return true;
    }

    bool status = false;
    ViewRef v = getSuperview();
    while (v) {
        if (v != view) {
            v = v->getSuperview();
            continue;
        }

        status = true;
        break;
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

            view->draw();
        }
    } gl::popModelView();
}

#pragma mark -

void View::connectEventListeners() {
    app::App* app = app::App::get();
    mConnectionMouseDown = app->getWindow()->getSignalMouseDown().connect([&](MouseEvent event) {
        mTrackingView = nullptr;

        Vec2i point = convertPointFromView(event.getPos(), nullptr);
        mTrackingView = hitTestPoint(point);
        if (!mTrackingView) {
            return;
        }

        mTrackingView->mouseDown(event);
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
    mConnectionMouseDrag = app->getWindow()->getSignalMouseDrag().connect([&](MouseEvent event) {
        if (!mTrackingView) {
            return;
        }

        mTrackingView->mouseDrag(event);
        event.setHandled();
    });
    mConnectionMouseMove = app->getWindow()->getSignalMouseMove().connect([&](MouseEvent event) {
        if (!mTrackingView) {
            return;
        }

        mTrackingView->mouseMove(event);
        event.setHandled();
    });
}

void View::disconnectEventListeners() {
    mConnectionMouseDown.disconnect();
    mConnectionMouseDrag.disconnect();
    mConnectionMouseUp.disconnect();
    mConnectionMouseMove.disconnect();
}

#pragma mark - COORDINATE CONVERSION

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

#pragma mark - PRIVATE

void View::removeSubview(const ViewRef& view) {
    if (!view) {
        return;
    }

    mSubviews.erase(std::find(mSubviews.begin(), mSubviews.end(), view));
    view->setSuperview(nullptr);
}

ViewRef View::hitTestPoint(const ci::Vec2i& point) {
    if (!isPointInsideBounds(point)) {
        return nullptr;
    }

    ViewRef view = nullptr;
    for (const ViewRef& v : mSubviews) {
        if (v->isHidden()) {
            continue;
        }

        Vec2i p = v->convertPointFromView(point, shared_from_this());
        if (!v->isPointInsideBounds(p)) {
            continue;
        }

        view = v;
        break;
    }

    if (!view) {
        view = shared_from_this();
    } else {
        Vec2i p = view->convertPointFromView(point, shared_from_this());
        view = view->hitTestPoint(p);
    }

    return view;
}

bool View::isPointInsideBounds(const ci::Vec2i& point) {
    return getBounds().contains(point);
}

}}
