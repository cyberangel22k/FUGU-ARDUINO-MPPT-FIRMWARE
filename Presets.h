#ifndef PRESETS_H
#define PRESETS_H

// Tell the compiler these variables are declared in the main .ino file
extern float
voltageBatteryMax,
voltageBatteryFloat,
voltageBatteryMin,
voltageLVD,
voltageLVR;

// Enum definition for the battery types to make the code readable
enum BatteryType { TYPE_LIFEPO4, TYPE_LI_ION_3S, TYPE_AGM_SEALED, TYPE_FLOODED, TYPE_CUSTOM };

// Function to calculate and apply the preset parameters
inline void applyBatteryPreset(int type, int systemVoltage) {
    // Determine the cell multiplier based on nominal system voltage
    float multiplier = 1.0;
    if (systemVoltage == 24) multiplier = 2.0;
    if (systemVoltage == 48) multiplier = 4.0;

    switch (type) {
        case TYPE_LIFEPO4: // Standard 4S LiFePO4 (12.8V Nominal)
            voltageBatteryMax   = 14.4 * multiplier;
            voltageBatteryFloat = 13.5 * multiplier;
            voltageBatteryMin   = 11.0 * multiplier;
            voltageLVD          = 11.5 * multiplier;
            voltageLVR          = 13.0 * multiplier;
            break;

        case TYPE_LI_ION_3S: // Standard 3S NMC Lithium-Ion (11.1V Nominal)
            voltageBatteryMax   = 12.6 * multiplier;
            voltageBatteryFloat = 12.3 * multiplier;
            voltageBatteryMin   = 9.0  * multiplier;
            voltageLVD          = 9.6  * multiplier;
            voltageLVR          = 11.5 * multiplier;
            break;

        case TYPE_AGM_SEALED: // Sealed Lead Acid / AGM
            voltageBatteryMax   = 14.4 * multiplier;
            voltageBatteryFloat = 13.8 * multiplier;
            voltageBatteryMin   = 10.5 * multiplier;
            voltageLVD          = 11.5 * multiplier;
            voltageLVR          = 12.6 * multiplier;
            break;

        case TYPE_FLOODED: // Wet Cell / Flooded Lead Acid
            voltageBatteryMax   = 14.6 * multiplier;
            voltageBatteryFloat = 13.5 * multiplier;
            voltageBatteryMin   = 10.5 * multiplier;
            voltageLVD          = 11.5 * multiplier;
            voltageLVR          = 12.6 * multiplier;
            break;

        case TYPE_CUSTOM:
            // Do nothing; leaves the current flash-saved settings intact 
            // so the user can manually adjust them via the Settings Menu.
            break;
    }
}
#endif
