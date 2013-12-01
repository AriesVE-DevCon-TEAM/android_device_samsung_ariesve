
package com.cyanogenmod.settings.device;

import android.app.AlertDialog;
import android.app.Activity;
import android.app.Dialog;
import android.app.Fragment;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.os.SystemProperties;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceManager;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.Button;

import com.cyanogenmod.settings.device.R;
import com.cyanogenmod.settings.device.util.CMDProcessor;
import com.cyanogenmod.settings.device.util.CMDProcessor.CommandResult;

public class LowRamChanger extends Activity {
    
	final Context context = this;
	private Button lowrambutton;

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

	public void onCreate(Bundle savedInstanceState) {
		 
		super.onCreate(savedInstanceState);
		setContentView(R.layout.lowrambutton);
 
		String lowramvalue = SystemProperties.get("ro.config.low_ram");
		TextView tv = (TextView) findViewById(R.id.lowramviewvalue);
		if (lowramvalue.equals("true")){
			tv.setText("LOWRAM is currently ON");
		}else tv.setText("LOWRAM is currently OFF");
		
		lowrambutton = (Button) findViewById(R.id.buttonSwitch);
 
		// add button listener
		lowrambutton.setOnClickListener(new OnClickListener() {
	
	@Override
	public void onClick(View view) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setMessage("If you want to switch the LowRam state press Reboot otherwise Cancel")
               .setPositiveButton("Reboot", new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
    			   boolean newValue = Boolean.parseBoolean(SystemProperties.get("ro.config.low_ram"));
            		   CMDProcessor.getMount("rw");
        		   new CMDProcessor().su.runWaitFor("busybox sed -i 's|ro.config.low_ram=.*|"
                + "ro.config.low_ram" + "=" + (newValue ? "false" : "true") + "|' " + "/system/build.prop"); 
        		   CMDProcessor.getMount("ro");
			   
			   Toast.makeText(getApplicationContext(), 
                               "Phone reboot in 5s waiting...", Toast.LENGTH_LONG).show();

                           final Handler handler = new Handler();
                           handler.postDelayed(new Runnable() {
                           @Override
                            public void run() {
                         	PowerManager pm = (PowerManager) context
                                 .getSystemService(Context.POWER_SERVICE);
                         	pm.reboot("Reboot device");
              		    }
          		   },5000);
                   }
               })
               .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                       dialog.cancel();
                   }
               });
        
		// create alert dialog
		AlertDialog alertDialog = builder.create();

		// show it
		alertDialog.show();
    		}
        });
      }
}
