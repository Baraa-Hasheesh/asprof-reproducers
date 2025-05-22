package javadoubleprofilerinfiniteloop;

import one.profiler.AsyncProfiler;

import java.io.IOException;

public class DoubleProfiler {

    public static native void method();

    public static void main(String[] args) throws InterruptedException, IOException {
        AsyncProfiler profiler = AsyncProfiler.getInstance();
        System.out.println(profiler.execute("start,event=cpu,nativemem,file=java1.jfr"));
        Thread.sleep(15000);
        System.out.println(profiler.execute("stop"));
        System.out.println(profiler.execute("start,event=cpu,nativemem,file=java2.jfr"));
        System.loadLibrary("DoubleProfiler");
        method();
    }
}
