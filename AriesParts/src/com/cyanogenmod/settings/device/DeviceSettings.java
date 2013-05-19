package com.cyanogenmod.settings.device;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;

public class DeviceSettings extends PreferenceActivity  {

    public static final String KEY_GSENSOR = "gsensor";
    public static final String KEY_BACKLIGHT = "backlight";
    public static final String KEY_FAST_CHARGE = "force_fast_charge";
    public static final String KEY_VIBRATION = "vibration";

    private Preference mBacklight;
    private Preference mGSensor;
    private CheckBoxPreference mFastCharge;
    private VibrationPreference mVibration;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.main);

        mBacklight = (Preference) findPreference(KEY_BACKLIGHT);
        mBacklight.setOnPreferenceClickListener(new TouchKeyBacklight(this));

	mGSensor = (Preference) findPreference(KEY_GSENSOR);
        mGSensor.setEnabled(GSensor.isSupported());

        mFastCharge = (CheckBoxPreference) findPreference(KEY_FAST_CHARGE);
        mFastCharge.setEnabled(FastCharge.isSupported());
        mFastCharge.setOnPreferenceChangeListener(new FastCharge());

        mVibration = (VibrationPreference) findPreference(KEY_VIBRATION);
        mVibration.setEnabled(VibrationPreference.isSupported());

    }
}
