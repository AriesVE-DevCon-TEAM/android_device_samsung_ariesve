
package com.cyanogenmod.settings.device;

import android.os.SystemProperties;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;

import com.cyanogenmod.settings.device.R;
import com.cyanogenmod.settings.device.util.CMDProcessor;
import com.cyanogenmod.settings.device.util.CMDProcessor.CommandResult;

public class LowRamChanger implements OnPreferenceChangeListener {

    public static boolean isSupported() {
	String versionString = SystemProperties.get("ro.build.version.release");
	if (versionString != null && !versionString.isEmpty()) {
		int index = versionString.indexOf(".");
		float version = Float.parseFloat((index > 0 && versionString.length() >= index + 2) ? versionString.substring(0, index + 2) : versionString);
		return version >= 4.4;
	} else {
		return false;
	}
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        CMDProcessor.getMount("rw");
        new CMDProcessor().su.runWaitFor("busybox sed -i 's|ro.config.low_ram=.*|"
                + "ro.config.low_ram" + "=" + (((Boolean)newValue) ? "true" : "false") + "|' " + "/system/build.prop"); 
        CMDProcessor.getMount("ro");
        return true;
    }

}

