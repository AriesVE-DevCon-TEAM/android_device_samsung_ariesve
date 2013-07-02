package com.cyanogenmod.settings.device;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Vibrator;
import android.preference.DialogPreference;
import android.preference.PreferenceManager;
import android.util.AttributeSet;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

public class VibrationPreference extends DialogPreference implements OnClickListener {

    private static final int SEEKBAR_ID = R.id.vibration_seekbar;

    private static final int VALUE_DISPLAY_ID = R.id.vibration_value;

    private static final String FILE_PATH = "/sys/class/vibetonz/immDuty/pwmvalue_intensity";
	
    private VibrationSeekBar mSeekBar = new VibrationSeekBar();

    private static final int MAX_VALUE = 127;

    // Track instances to know when to restore original value
    // (when the orientation changes, a new dialog is created before the old one is destroyed)
    private static int sInstances = 0;

    public VibrationPreference(Context context, AttributeSet attrs) {
        super(context, attrs);

        setDialogLayoutResource(R.layout.preference_dialog_vibration);
    }

    @Override
    protected void onBindDialogView(View view) {
        super.onBindDialogView(view);

        sInstances++;

        SeekBar seekBar = (SeekBar) view.findViewById(SEEKBAR_ID);
        TextView valueDisplay = (TextView) view.findViewById(VALUE_DISPLAY_ID);
        mSeekBar = new VibrationSeekBar(seekBar, valueDisplay, FILE_PATH);

        SetupButtonClickListener(view);
    }

    private void SetupButtonClickListener(View view) {
        Button mTestButton = (Button)view.findViewById(R.id.vibration_test);
        mTestButton.setOnClickListener(this);

        Button mResetButton = (Button)view.findViewById(R.id.vibration_reset);
        mResetButton.setOnClickListener(this);
    }

    @Override
    protected void onDialogClosed(boolean positiveResult) {
        super.onDialogClosed(positiveResult);

        sInstances--;

        if (positiveResult) {
            mSeekBar.save();
        } else if (sInstances == 0) {
            mSeekBar.reset();
        }
    }

    public static void restore(Context context) {
        if (!isSupported()) {
            return;
        }

        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
        int value = sharedPrefs.getInt(FILE_PATH, 121);
        Utils.writeValue(FILE_PATH, String.valueOf(value));
    }

    public static boolean isSupported() {
        boolean supported = true;
        if (!Utils.fileExists(FILE_PATH)) {
            supported = false;
        }

        return supported;
    }

    class VibrationSeekBar implements SeekBar.OnSeekBarChangeListener {

        protected String mFilePath;
        protected int mOriginal;
        protected SeekBar mSeekBar;
        protected TextView mValueDisplay;

        public VibrationSeekBar(SeekBar seekBar, TextView valueDisplay, String filePath) {
            mSeekBar = seekBar;
            mValueDisplay = valueDisplay;
            mFilePath = filePath;

            // Read original value
            SharedPreferences sharedPreferences = getSharedPreferences();
            mOriginal = sharedPreferences.getInt(mFilePath, 121);

            seekBar.setMax(MAX_VALUE);
            reset();
            seekBar.setOnSeekBarChangeListener(this);
        }

        // For inheriting class
        protected VibrationSeekBar() {
        }

        public void reset() {
            mSeekBar.setProgress(mOriginal);
            updateValue(mOriginal);
        }

        public void save() {
            Editor editor = getEditor();
            editor.putInt(mFilePath, mSeekBar.getProgress());
            editor.commit();
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int progress,
                boolean fromUser) {
            Utils.writeValue(mFilePath, String.valueOf(progress));
            updateValue(progress);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
            // Do nothing
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            // Do nothing
        }

        protected void updateValue(int progress) {
            mValueDisplay.setText(String.valueOf(strengthToPercent(progress)) + "%");
        }

        public void resetDefault() {
            mSeekBar.setProgress(121);
            updateValue(121);
            Utils.writeValue(FILE_PATH, String.valueOf(121));
        }

        public int strengthToPercent(int strength) {
		double percent = ((double)strength * 100) / 127;

		if (percent > 100)
			percent = 100;
		else if (percent < 0)
			percent = 0;
		return (int) percent;
		}
    }

    public void onClick(View v) {
        switch(v.getId()) {
            case R.id.vibration_test:
                testVibration();
                break;
            case R.id.vibration_reset:
                mSeekBar.resetDefault();
                break;
        }
    }

    public void testVibration() {
        Vibrator vib = (Vibrator) this.getContext().getSystemService(Context.VIBRATOR_SERVICE);
        vib.vibrate(1000);
    }
}
