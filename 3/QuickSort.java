import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.ForkJoinTask;
import java.util.concurrent.RecursiveTask;

public class QuickSort extends RecursiveTask<Void>{

    private int[] array;
    private int left;
    private int right;

    public QuickSort(int[] array, int left, int right) {
        this.array = array;
        this.left = left;
        this.right = right;
    }

    @Override
    protected Void compute() {
        if(left>=right)
            return null;
        int pivot = partition(array, left, right);
        QuickSort task1 = null;
        QuickSort task2 = null;
        task1 = new QuickSort(array, left, pivot-1);
        task2 = new QuickSort(array, pivot+1, right);
        task1.fork();
        task2.fork();
        //join the result
        task1.join();
        task2.join();
        return null;
    }

    public static int partition(int[] a, int left, int right) {
        int pivot = a[left];
        int first = left, last = right;
        while (first < last) {
            while (first < last && a[last] >= pivot) {
                last--;
            }
            a[first] = a[last];
            while (first < last && a[first] <= pivot) {
                first++;
            }
            a[last] = a[first];
        }
        a[first] = pivot;
        return first;
    }

    public static void main(String[] args) {
        int[] a = {4,2,1,4,7,5,3,8,2,7,1,78,89,6,5,4,8,5};
        ForkJoinPool forkJoinPool = new ForkJoinPool();
        QuickSort task = new QuickSort(a, 0, a.length-1);
        forkJoinPool.invoke(task);
        //print out the result
        System.out.println("The sorted array: ");
        for(int i=0;i<a.length;++i){
            System.out.printf("%d\t",a[i]);
        }
        System.out.print("\n");
    }
}