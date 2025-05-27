package doubleclock;

import one.profiler.AsyncProfiler;

import java.io.IOException;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.concurrent.Semaphore;

public class DoubleProfiler {

    static final Semaphore semaphore = new Semaphore(1);

    public static void lockHeavy() {
        try {
            semaphore.acquire();
            for (int i = 0; i < 100_000_000; i++) {
                System.out.print("");
            }
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            semaphore.release();
        }
    }

    public static synchronized void lockHeavySync() {
        for (int i = 0; i < 100_000_000; i++) {
            System.out.print("");
        }
    }

    public static void main(String[] args) throws InterruptedException, IOException {
        String outputFile = System.getenv("JAVA_PROFILER_FILE_NAME");
        String profiler = System.getenv("JAVA_PROFILER");
        String lockMode = System.getenv("LOCK_MODE");

        AsyncProfiler asyncProfiler = null;

        if ("api".equalsIgnoreCase(profiler)) {
            asyncProfiler = AsyncProfiler.getInstance();
            asyncProfiler.execute("start,event=lock,file=" + outputFile);
        }

        Thread.sleep(2000);

        // Stop profiler to break callback chain
        if (asyncProfiler != null) {
            Thread.sleep(3000);
            asyncProfiler.execute("stop");
        }

        List<Thread> collection = IntStream.range(0, 10)
                .mapToObj(ignore -> (Runnable) () -> {
                    if ("sync".equalsIgnoreCase(lockMode)) {
                        lockHeavySync();
                    } else {
                        lockHeavy();
                    }
                })
                .map(Thread::new)
                .collect(Collectors.toList());

        for (int i = 0; i < 10; i++) {
            collection.get(i).start();
        }

        for (int i = 0; i < 10; i++) {
            collection.get(i).join();
        }
    }
}
