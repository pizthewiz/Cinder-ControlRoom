
# VIEW
- implement point conversions
- some sort of canonical size from metrics -sizeToFit, -sizeThatFits:, intrinsicContentSize

# CONTENT VIEW
- route key events to first responder
- tracking areas instead of just mouseMoved

# SCROLLVIEW
- derrived class, proportional scrollbars

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

# GENERAL
- migrate texture font to a singleton, share resources
- support the disabled state across the board
- come up with some sort of theme handling
- support high density with a content scale other than 2, check `getWindowContentScale()`
