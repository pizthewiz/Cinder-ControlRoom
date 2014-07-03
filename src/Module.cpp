//
//  Module.cpp
//  Cinder-ControlRoom
//
//  Created by Jean-Pierre Mouilleseaux on 02 Jul 2014.
//  Copyright 2014 Chorded Constructions. All rights reserved.
//

#include "Module.h"

namespace Cinder { namespace ControlRoom {

using namespace ci;
using namespace ci::app;

ModuleRef Module::create(const Rectf& frame) {
    return ModuleRef(new Module(frame));
}

Module::Module(const Rectf& frame) : View(frame) {
    app::App* app = app::App::get();
    mConnectionMouseDown = app->getWindow()->getSignalMouseDown().connect([&](MouseEvent event) {
        Vec2i point = convertPointFromView(event.getPos(), nullptr);

        mTrackingView = nullptr;
        for (const ViewRef& view : mSubviews) {
            if (!view->getFrame().contains(point)) {
                continue;
            }
            mTrackingView = view;
            break;
        }

        if (mTrackingView) {
            mTrackingView->mouseDown(event);
            event.setHandled();
        }
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

Module::~Module() {
    mConnectionMouseDown.disconnect();
    mConnectionMouseDrag.disconnect();
    mConnectionMouseUp.disconnect();
}

}}
