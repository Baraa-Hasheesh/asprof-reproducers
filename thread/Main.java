package thread;

public class Main {
    
    public static native void startBusyThread();

    static {
        System.loadLibrary("thread_Main");
    }

    public static void main(String[] args) throws InterruptedException {
        Thread myJavaThread = new Thread(() -> {
            for (int i = 0; i < 1000000000; i++) {
                for (int j = 0; j < 1; j++) {
                        int k = i + j;
                }
            }
            for (int i = 0; i < 1000000000; i++) {
                for (int j = 0; j < 1; j++) {
                        int k = i + j;
                }
            }
            for (int i = 0; i < 1000000000; i++) {
                for (int j = 0; j < 1; j++) {
                        int k = i + j;
                }
            }
            for (int i = 0; i < 1000000000; i++) {
                for (int j = 0; j < 1; j++) {
                        int k = i + j;
                }
            }
        });
        myJavaThread.setName("myJavaThread");

        startBusyThread();
        myJavaThread.start();
        myJavaThread.join();
    }
}
