# Integration 1 — Gestion Employés et Gestion Salles

## Objective
Integrate the Gestion Salles module into the existing Gestion Employés application to provide navigation between both interfaces within the same Qt application.

## Changes Made
- Integrated the Gestion Salles interface into the existing application.
- Connected the **Gestion Salles** navigation button to the corresponding interface.
- Preserved the existing Gestion Employés interface and its navigation.
- Used `QStackedWidget` to switch between interfaces without opening a separate application window.
- Adapted the existing Qt project files to support the integration.

## Files Modified
- `integration.cpp`
- `integration.h`
- `integration.ui`
- `main.cpp`
- `hacknova.pro`

## Expected Result
Users can navigate between Gestion Employés and Gestion Salles from the same application while retaining the existing login interface.

## Branch
`integration-1`

## Future Improvements
- Test all navigation paths and interface actions.
- Resolve any integration or compilation issues.
- Ensure compatibility between both modules and prepare the integrated version for merging.
