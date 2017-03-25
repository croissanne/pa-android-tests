package gundersanne.portaudioexample;

import android.content.Context;
import android.media.AudioManager;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
                    @Override
                    public void run() {
                        AudioManager audio = (AudioManager) getSystemService(Context.AUDIO_SERVICE);


                        String sr = audio.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
                        String buffers = audio.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
                        Log.d("portaudioexample", sr + " " + buffers);
                        playSine(Integer.parseInt(sr), 240);
                    }
                }).start();
            }
        });
    }
    public native void playSine(int sampleRate, int bufferSize);
}
