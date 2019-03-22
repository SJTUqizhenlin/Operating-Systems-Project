import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.ForkJoinTask;
import java.util.concurrent.RecursiveTask;
public class MergeSort extends RecursiveTask<Void>{
    //不需要返回值的task继承RecursiveAction更好
    private int[] array;
    private int left;
    private int right;


    public MergeSort(int[] array, int left, int right) {
        this.array = array;
        this.left = left;
        this.right = right;
    }


    @Override
    protected Void compute() {
        // if right==left+1, it's said that no need to divide the array
        boolean isNeedSplit = right - left > 1;
        if (isNeedSplit) {
            int mid = (left + right)/2;
            // sort two subarrays
            MergeSort mergeSortTask1 = new MergeSort(array, left, mid);
            MergeSort mergeSortTask2 = new MergeSort(array, mid+1, right);
            // run
            mergeSortTask1.fork();
            mergeSortTask2.fork();
            // join the result
            mergeSortTask1.join();
            mergeSortTask2.join();
            // combine the result
            merge(array, left, mid, right);
        }else {
            int mid = (left+right)/2;
            merge(array, left, mid, right);
        }
        return null;
    }

    public static void merge(int a[], int left, int mid, int right){
        int len = right - left + 1;
        int temp[] = new int[len];
        int i = left;
        int j = mid + 1;
        int index = 0;
        while(i<=mid && j <= right) {
            if(a[i]<=a[j]){
                temp[index++] = a[i++];
            }
            else{
                temp[index++] = a[j++];
            }
        }
        while (i <= mid) {
            temp[index++] = a[i++];
        }
        while (j<=right) {
            temp[index++] = a[j++];
        }
        for (int k = 0; k<temp.length; k++) {
            a[left++] = temp[k];
        }
    }

    public static void main(String[] args) {
        // the array given
        int[] a = {4,2,1,4,7,5,3,8,2,7,1,78,89,6,5,4,8,5};
        ForkJoinPool forkJoinPool = new ForkJoinPool();
        MergeSort task = new MergeSort(a, 0, a.length-1);

        // running the function to fork
        forkJoinPool.invoke(task);

        //print out the result
        System.out.println("The sorted array: ");
        for(int i=0;i<a.length;++i){
            System.out.printf("%d\t",a[i]);
        }
        System.out.print("\n");
    }
}