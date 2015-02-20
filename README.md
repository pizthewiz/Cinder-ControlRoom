# Cinder-ControlRoom
`Cinder-ControlRoom` is CinderBlock to assemble and manage interfaces and controls.

- Drawing
- Subview Management
- Mouse Event Handling
- Reusable Controls

*NOTE* - `Cinder-ControlRoom` is to be used with the heir apparent [glNext](https://github.com/cinder/Cinder/tree/glNext) Cinder branch.

### USAGE
```C++
// create root view
Rectf frame = Rectf(ivec2(0, 0), ivec2(300, getWindowHeight()));
ViewRef inspectorView = View::create(frame);

// connect mouse event listeners, should only be done on the root view
inspectorView->connectEventListeners();

float value = 0.5f;
float min = 0.0f;
float max = 1.0f;

// add a label
std::string stringValue = str(boost::format("%.3f") % value);
LabelRef label = Label::create(Rectf(xAlt, y, xAlt + controlWidthAlt, y + height), stringValue);
inspectorView->addSubview(label);

// add a slider
Rectf sliderFrame = Rectf(10.0f, 10.0f, frame.getWidth - 10.0f, 38.0f);
SliderRef slider = Slider::create(sliderFrame, value, min, max);
inspectorView->addSubview(slider);
// connect value changed event handler
slider->connectControlEventHandler(ControlEvent::ValueChanged, [&, label](const ControlRef& control) {
  SliderRef s = std::static_pointer_cast<Slider>(control);
  float value = s->getValue();

  // update value label
  std::string stringValue = str(boost::format("%.3f") % value);
  label->setText(stringValue);
});
```

### GREETZ
- Apple's excellent [AppKit](https://developer.apple.com/library/mac/documentation/cocoa/reference/applicationkit/objc_classic/_index.html) and [UIKit](https://developer.apple.com/library/ios/documentation/uikit/reference/uikit_framework/_index.html)
- Reza Ali's [ciUI](https://github.com/rezaali/ciUI)
