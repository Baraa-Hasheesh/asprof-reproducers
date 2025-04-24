package unpatched;

import java.io.FileInputStream;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ForkJoinPool;
import java.util.stream.IntStream;

public class Main {

    private static final ForkJoinPool THREAD_POOL = new ForkJoinPool(10);

    public static native void loadLibUnpatched();

    public static native void execute(String libPath);

    private static Void executeFlow() {
        try {
            // Create a temp library file
            Path libraryPath = Files.createTempFile("temp", "my_lib.so");
            libraryPath.toFile().deleteOnExit();

            InputStream in = new FileInputStream("libMy_lib.so");
            assert in != null;
            Files.copy(in, libraryPath, StandardCopyOption.REPLACE_EXISTING);

            execute(libraryPath.toString());
            libraryPath.toFile().delete();
        } catch (Exception exception) {
            throw new RuntimeException(exception);
        }
        return null;
    }

    public static void execute() {
        for (int i = 0; i < 1000; i++) {
            List<CompletableFuture<Void>> futures = new ArrayList<>();
            IntStream.range(0, 100).forEach(ignore -> futures.add(CompletableFuture.supplyAsync(Main::executeFlow, THREAD_POOL)));
            futures.forEach(CompletableFuture::join);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        System.loadLibrary("Unpatched_Main");
        loadLibUnpatched();

        Thread thread = new Thread(() -> {
            try {
                Thread.sleep(1000);
            } catch (Exception e) {}
            System.loadLibrary("My_lib");
        });
        thread.start();
        execute();
        thread.join();
    }
}
