package doublealloc;

import one.profiler.AsyncProfiler;

import java.io.IOException;

public class MultipleProfilers {

    public static void main(String[] args) throws InterruptedException, IOException {
        String timeToStart = System.getenv("API_DELAY_START");
        String runDuration = System.getenv("API_RUN_DURATION");
        String path =  System.getenv("PWD");
        if (timeToStart != null && !timeToStart.isBlank()) {
            Thread thread = new Thread(() -> {
                try {
                    System.out.println("Starting API profiler in " + timeToStart + " seconds");
                    Thread.sleep(Integer.parseInt(timeToStart) * 1000L);
                    AsyncProfiler asyncProfiler = AsyncProfiler.getInstance(path + "/libasyncProfiler.so");
                    asyncProfiler.execute("start,alloc=0,file=output_api.jfr");
                    Thread.sleep(Integer.parseInt(runDuration) * 1000L);
                    asyncProfiler.execute("stop");
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            });
            thread.start();
        }

        Thread.sleep(5000);
        Integer[] array = new Integer[1000000];
    }
}
