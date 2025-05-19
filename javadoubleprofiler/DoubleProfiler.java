package javadoubleprofiler;

public class DoubleProfiler {

    public static native void method();

    public static void main(String[] args) throws InterruptedException {
        Thread.sleep(10000);
        System.loadLibrary("DoubleProfiler");
        method();
    }
}
