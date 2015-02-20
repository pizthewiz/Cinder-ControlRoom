
# VIEW
- some sort of canonical size from metrics -sizeToFit, -sizeThatFits:, intrinsicContentSize

# CONTENT VIEW
- route key events to first responder
- tracking areas instead of just mouseMove

# SCROLLVIEW
- derived class, proportional scrollbars

# LABEL
- expose different sizes via enum
- truncate text with elipsis
- calculate baseline on title/frame change and cache

# BUTTON
- clip text
- calculate baseline on title/frame change and cache

# SLIDER
- shift and option accelerators
- variant that takes a pointer to a float value
- double click to revert to default

# GENERAL
- draw with Cinder-NanoVG instead of gl primitives
- migrate texture font to a singleton, share resources
- support the disabled state across the board
- come up with some sort of theme handling
- support high density with a content scale other than 2, check `getWindowContentScale()`
- inline documentation
