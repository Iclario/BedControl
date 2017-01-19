package fr.iclario.bedcontrol;

import android.widget.SeekBar;

import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;

public class OnSeekBarPowerChangeListener implements SeekBar.OnSeekBarChangeListener
{
	@Override
	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)
	{
		MainActivity.power = progress;
		// Instantiate the RequestQueue.
		RequestQueue queue = MainActivity.queue;

		// Request a string response from the provided URL.
		StringRequest stringRequest = new StringRequestPower(MainActivity.getPowerString(), MainActivity.responseTextView);

		// Add the request to the RequestQueue.
		queue.add(stringRequest);
		MainActivity.textViewPowerValue.setText(progress + "/" + MainActivity.seekBarPower.getMax());
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar)
	{
	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar)
	{
	}
}