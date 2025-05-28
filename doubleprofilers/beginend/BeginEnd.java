package beginend;

import one.profiler.AsyncProfiler;

import java.io.IOException;

public class BeginEnd {

    public static native void start0();

    public static native void start1();

    public static native void stop();

    static {
        System.loadLibrary("BeginEnd");
    }

    public static void main(String[] args) throws InterruptedException, IOException {
        boolean apiProfiler = Boolean.parseBoolean(System.getenv("API_PROFILER"));
        AsyncProfiler asyncProfiler;

        if (apiProfiler) {
            asyncProfiler = AsyncProfiler.getInstance();
            asyncProfiler.execute("start,alloc=0,begin=Java_beginend_BeginEnd_start0,end=Java_beginend_BeginEnd_stop");
        }

        Thread.sleep(6000);
        System.out.println("start0");
        start0();
        Thread.sleep(1000);
        System.out.println("start1");
        start1();
        Thread.sleep(1000);
        System.out.println("stop");
        stop();

        Integer[] array = new Integer[1000000];
    }
}
