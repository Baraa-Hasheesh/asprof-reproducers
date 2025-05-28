package multiwall;

public class MultipleProfilers {

    public static void sleep() {
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Thread thread1 = new Thread(MultipleProfilers::sleep);
        Thread thread2 = new Thread(MultipleProfilers::sleep);
        thread1.setName("Thread 1");
        thread2.setName("Thread 2");

        thread1.start();
        thread2.start();

        thread2.join();
        thread1.join();
    }
}
