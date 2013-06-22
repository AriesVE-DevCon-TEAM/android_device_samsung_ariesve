package com.cyanogenmod.settings.device;

import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;

public class DeviceSettings extends PreferenceActivity  {

    public static final String KEY_GSENSOR = "gsensor";
    public static final String KEY_BACKLIGHT = "backlight";
    public static final String KEY_BACKLIGHT_TIMEOUT = "backlight_timeout";
    public static final String KEY_TOUCHLIGHT_STATUS = "touchlight_status";
    public static final String KEY_FAST_CHARGE = "force_fast_charge";
    public static final String KEY_VIBRATION = "vibration";
    public static final String KEY_PKFMANAGER = "pkfmanager";

    private ListPreference mBacklightTimeout;
    private CheckBoxPreference mTouchLightStatus;
    private Preference mBacklight;
    private Preference mGSensor;
    private CheckBoxPreference mFastCharge;
    private VibrationPreference mVibration;
    private Preference mPkfManager;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.main);

        mBacklightTimeout = (ListPreference) findPreference(KEY_BACKLIGHT_TIMEOUT);
        mBacklightTimeout.setEnabled(TouchKeyBacklightTimeout.isSupported());
        mBacklightTimeout.setOnPreferenceChangeListener(new TouchKeyBacklightTimeout());

        mTouchLightStatus = (CheckBoxPreference) findPreference(KEY_TOUCHLIGHT_STATUS);
        mTouchLightStatus.setEnabled(TouchLightStatus.isSupported());
        mTouchLightStatus.setOnPreferenceChangeListener(new TouchLightStatus());

        mBacklight = (Preference) findPreference(KEY_BACKLIGHT);
        mBacklight.setOnPreferenceClickListener(new TouchKeyBacklight(this));

        mGSensor = (Preference) findPreference(KEY_GSENSOR);
        mGSensor.setEnabled(GSensor.isSupported());

        mFastCharge = (CheckBoxPreference) findPreference(KEY_FAST_CHARGE);
        mFastCharge.setEnabled(FastCharge.isSupported());
        mFastCharge.setOnPreferenceChangeListener(new FastCharge());

        mVibration = (VibrationPreference) findPreference(KEY_VIBRATION);
        mVibration.setEnabled(VibrationPreference.isSupported());

        mPkfManager = (Preference) findPreference(KEY_PKFMANAGER);
        mPkfManager.setOnPreferenceClickListener(new PkfManager(this));
    }
}
