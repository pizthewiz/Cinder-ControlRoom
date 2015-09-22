//
//  View.cpp
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014-2015 Chorded Constructions. All rights reserved.
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

void View::setWindow(const ci::app::WindowRef window) {
    mWindow = window;

    for (auto subview : mSubviews) {
        subview->setWindow(window);
    }
}

void View::addSubview(const ViewRef& view) {
    // prevent cycle
    if (isDescendantOfView(view)) {
        // TODO - throw?
        return;
    }

    mSubviews.push_back(view);
    view->setSuperview(shared_from_this());
    view->setWindow(mWindow);
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

      ci::Vec2f point = convertPointFromView(event.getPos(), nullptr);
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
    mConnectionMouseWheel = app->getWindow()->getSignalMouseWheel().connect([&](MouseEvent event) {
        if (!mTrackingOverView) {
            return;
        }

        mTrackingOverView->mouseWheel(event);
        event.setHandled();
    });
    mConnectionMouseMove = app->getWindow()->getSignalMouseMove().connect([&](MouseEvent event) {
        mTrackingOverView = nullptr;

      ci::Vec2f point = convertPointFromView(event.getPos(), nullptr);
        mTrackingOverView = hitTestPoint(point);
        if (!mTrackingOverView) {
            return;
        }

        mTrackingOverView->mouseMove(event);
        event.setHandled();
    });
    mConnectionMouseDrag = app->getWindow()->getSignalMouseDrag().connect([&](MouseEvent event) {
        if (!mTrackingView) {
            return;
        }

        mTrackingView->mouseDrag(event);
        event.setHandled();
    });
}

void View::disconnectEventListeners() {
    mConnectionMouseDown.disconnect();
    mConnectionMouseUp.disconnect();
    mConnectionMouseWheel.disconnect();
    mConnectionMouseMove.disconnect();
    mConnectionMouseDrag.disconnect();
}

#pragma mark - COORDINATE CONVERSION

  ci::Vec2f View::convertPointFromView(const ci::Vec2f& point, const ViewRef& view) {
    // view to local
    ci::Vec2f p = point;
    if (isDescendantOfView(view)) {
        ViewRef v = shared_from_this();
        while (v && v != view) {
            p -= v->getFrame().getUpperLeft();
            v = v->getSuperview();
        }
    } else {
        ViewRef v = view;
        while (v && v != shared_from_this()) {
            p += v->getFrame().getUpperLeft();
            v = v->getSuperview();
        }
    }
    return p;
}

ci::Vec2f View::convertPointToView(const ci::Vec2f& point, const ViewRef& view) {
    // local to view
    return view->convertPointFromView(point, shared_from_this());
}

#pragma mark - PRIVATE

void View::removeSubview(const ViewRef& view) {
    if (!view) {
        return;
    }

    mSubviews.erase(std::find(mSubviews.begin(), mSubviews.end(), view));
    view->setSuperview(nullptr);
    view->setWindow(nullptr);
}

ViewRef View::hitTestPoint(const ci::Vec2f& point) {
    if (!isPointInsideBounds(point)) {
        return nullptr;
    }

    ViewRef view = nullptr;
    for (const ViewRef& v : mSubviews) {
        if (v->isHidden()) {
            continue;
        }

        ci::Vec2f p = v->convertPointFromView(point, shared_from_this());
        if (!v->isPointInsideBounds(p)) {
            continue;
        }

        view = v;
        break;
    }

    if (!view) {
        view = shared_from_this();
    } else {
        ci::Vec2f p = view->convertPointFromView(point, shared_from_this());
        view = view->hitTestPoint(p);
    }

    return view;
}

bool View::isPointInsideBounds(const ci::Vec2f& point) {
    return getBounds().contains(point);
}

}}
