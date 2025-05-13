public class Preload {

    public static void threadStart() throws InterruptedException {
        Thread thread = new Thread(() -> {
            for (int i = 0; i < 10000; i++) {
                System.out.println("Thread is running");
            }
        });
        thread.setName("JavaPreloadThread");
        thread.start();
        thread.join();
    }
}
