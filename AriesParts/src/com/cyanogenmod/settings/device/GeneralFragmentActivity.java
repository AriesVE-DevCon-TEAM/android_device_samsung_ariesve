/*
 * Copyright (C) 2013 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.cyanogenmod.settings.device;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.preference.PreferenceScreen;
import android.util.Log;
import android.view.Gravity;
import android.widget.Toast;

import com.cyanogenmod.settings.device.R;

public class GeneralFragmentActivity extends PreferenceFragment {

    private static final String TAG = "DeviceSettings_General";
 
    private Preference mGSensor;
    private CheckBoxPreference mFastCharge;
    private VibrationPreference mVibration;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        addPreferencesFromResource(R.xml.general_preferences);

        PreferenceScreen prefSet = getPreferenceScreen();

        mGSensor = (Preference) findPreference(DeviceSettings.KEY_GSENSOR);
        mGSensor.setEnabled(GSensor.isSupported());

        mFastCharge = (CheckBoxPreference) findPreference(DeviceSettings.KEY_FAST_CHARGE);
        mFastCharge.setEnabled(FastCharge.isSupported());
        mFastCharge.setOnPreferenceChangeListener(new FastCharge());

        mVibration = (VibrationPreference) findPreference(DeviceSettings.KEY_VIBRATION);
        mVibration.setEnabled(VibrationPreference.isSupported());
    }

    public static void restore(Context context) {
        FastCharge.restore(context);
        VibrationPreference.restore(context);
    }
}
