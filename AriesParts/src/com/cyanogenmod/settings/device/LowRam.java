/*
 * Copyright (C) 2014 The CyanogenMod Project
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

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.PowerManager;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.Toast;

import com.cyanogenmod.settings.device.R;

public class LowRam implements OnPreferenceChangeListener {

	// Logging tag
	private static final String TAG = "DeviceSettings_LowRam";

	// Keys of used system properties
	private static final String SYSTEM_VERSION_PROP_KEY = "ro.build.version.release";
	private static final String LOWRAM_PROP_KEY = "ro.config.low_ram";

	// LowRam property is supported starting from Android 4.4 (KitKat)
	private static final float MIN_VERSION_SUPPORTED = 4.4f;

	// Reboot timeout in seconds
	private static final int REBOOT_TIMEOUT = 5;

	/**
	 * Checks if the feature is supported
	 * @return True if the feature is supported, otherwise false
	 */
	public static boolean isSupported() {
		// Get the system version
		String versionString = BuildProp.get(SYSTEM_VERSION_PROP_KEY);

		// Return true if the system is Android 4.4 (KitKat) or above
		if (versionString != null && !versionString.isEmpty()) {
			int index = versionString.indexOf(".");
			float version = Float.parseFloat((index > 0 && versionString.length() >= index + 2) ?
					versionString.substring(0, index + 2) : versionString);
			return version >= MIN_VERSION_SUPPORTED;
		} else {
			return false;
		}
	}

	/**
	 * Gets the current value of LowRam property inside system properties
	 * @return The LowRam value currently set
	 */
	public static boolean getPropertyValue() {
		return BuildProp.getBoolean(LOWRAM_PROP_KEY);
	}

	/**
	 * Stores the value of LowRam property inside system properties
	 * @param value The value to store
	 * @return True if the store succeeded, otherwise false
	 */
	private static boolean storePropertyValue(Boolean value) {
		return BuildProp.set(LOWRAM_PROP_KEY, value);
	}

	/**
	 * Shows the confirm dialog to reboot the device
	 * @param context The context of this Preference
	 */
	private void showRebootDialog(final Context context) {
		// Create and show the reboot confirm dialog
		AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(context);
		dialogBuilder.setTitle(R.string.low_ram_title)
			.setMessage(R.string.low_ram_reboot_alert)
			.setPositiveButton(R.string.low_ram_reboot_yes, new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int id) {
					// Show wait message for reboot in progress
					Toast.makeText(context,
						String.format(context.getString(R.string.low_ram_reboot_wait), REBOOT_TIMEOUT),
						Toast.LENGTH_LONG).show();
					final Handler handler = new Handler();
					handler.postDelayed(new Runnable() {
						@Override
						public void run() {
							PowerManager pm = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
							pm.reboot(null);
						}
					}, REBOOT_TIMEOUT * 1000);
				}
			})
			.setNegativeButton(R.string.low_ram_reboot_no, new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int id) {
					dialog.cancel();
				}
			})
			.create()
			.show();
	}
	
	/**
	 * Shows the error dialog for an update failure
	 * @param context The context of this Preference
	 */
	private void showErrorDialog(final Context context) {
		AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(context);
		dialogBuilder.setTitle(R.string.low_ram_title)
			.setMessage(R.string.low_ram_error)
			.create()
			.show();
	}

	@Override
	public boolean onPreferenceChange(Preference preference, Object value) {
		// Get the preference context
		final Context context = preference.getContext();

		// Store the new value, if it fails abort Preference update
		if (storePropertyValue((Boolean)value)) {
			showRebootDialog(context);
			return true;
		} else {
			showErrorDialog(context);
			return false;
		}
	}

}
