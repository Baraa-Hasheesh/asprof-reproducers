package doublectimer;

public class DoubleProfiler {

    public static void main(String[] args) throws InterruptedException {
        Thread thread = new Thread(() -> {
            for (int i = 0; i < 1_000_000_000; i++) {
                System.out.print("");
            }
        });
        thread.start();
        thread.join();
    }
}
