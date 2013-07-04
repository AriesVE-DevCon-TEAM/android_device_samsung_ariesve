package com.cyanogenmod.settings.device;

import java.util.List;

import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.net.Uri;
import android.content.Context;
import android.content.Intent;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceClickListener;
import android.preference.PreferenceActivity;

public class TouchKeyBacklight implements OnPreferenceClickListener {

    private Context mContext;

    public static final String PACKAGE_BLN = "neldar.bln.control.free";
    public static final String PACKAGE_BLN_PRO = "neldar.bln.control.pro";

    public boolean isPackageExists(String targetPackage){
        List<ApplicationInfo> packages;
        PackageManager pm;
            pm = mContext.getPackageManager();        
            packages = pm.getInstalledApplications(0);
            for (ApplicationInfo packageInfo : packages) {
        if(packageInfo.packageName.equals(targetPackage)) return true;
        }        
        return false;
    }

    @SuppressLint("NewApi")
    public void launchPackage(String targetPackage){
        Intent i = new Intent();
        PackageManager manager = mContext.getPackageManager();
        i = manager.getLaunchIntentForPackage(targetPackage);
        i.addCategory(Intent.CATEGORY_LAUNCHER);
        mContext.startActivity(i);
    }

    public TouchKeyBacklight(Context context) {
        mContext = context;
    }

    @Override
    public boolean onPreferenceClick(Preference preference) {
        if (isPackageExists(PACKAGE_BLN_PRO)) {
            launchPackage(PACKAGE_BLN_PRO);
        } else if (isPackageExists(PACKAGE_BLN)) {
            launchPackage(PACKAGE_BLN);
        } else {
            Intent intent = new Intent(Intent.ACTION_VIEW); 
            intent.setData(Uri.parse("market://details?id="+PACKAGE_BLN)); 
            mContext.startActivity(intent);
        }
        return true;
    }

}
