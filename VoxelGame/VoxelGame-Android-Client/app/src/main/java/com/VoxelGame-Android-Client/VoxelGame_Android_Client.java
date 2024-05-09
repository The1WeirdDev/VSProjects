
package com.VoxelGame_Android_Client;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import java.lang.String;

public class VoxelGame_Android_Client extends Activity
{
    public native String Method();
     static {
    System.loadLibrary("native-lib");
    }
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its text to "Hello world" */
        TextView  tv = new TextView(this);
        tv.setText(Method());
        setContentView(tv);
    }
}
