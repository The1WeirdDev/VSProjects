
package com.VoxelGame_Android_Client;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import java.lang.*;

public class VoxelGame_Android_Client extends Activity
{
    /** Called when the activity is first created. */
    static{
        System.loadLibrary("native-lib");
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its text to "Hello world" */
        TextView  tv = new TextView(this);
        tv.setText("Hello World!");
        setContentView(tv);
        Game thread = new Game(this);
        thread.start();
    }

    public native void Method();
}
