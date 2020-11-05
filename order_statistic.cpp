#include <bits/stdc++.h>
#include <cstdlib>
using namespace std;

int i, b[] = {0,9,8,7,6,5,10,2,3,4,1,11,13,12,14,16,18,20,0,21,22,23,24,25,26,27,28,29,30,31,33,32,36,35,51,57,58,59,60,62,64,65,63,61,66,68,67,69,72,78,76};
int len = sizeof(b)/sizeof(int);

int a[] = {0,9,8,4,5,3,1,2,6,7};
float w[] = {0,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.2};

typedef pair<int,int> a_w;

int Partition(int a[], int left, int right) {
    int i = left-1, j = right,pivot = a[right];
    while(true) {
        i++;j--;
        while(i<=right && a[i] < pivot) i++;
        while(j>=left && a[j] > pivot) j--;
        if (i < j)
            swap(a[i],a[j]);
        else {
            swap(a[i],a[right]);
            break;
        }
    }
    return i;

}

int Random_Partition(int a[], int left, int right) {
    int q = left+rand()%(right-left+1);
    swap(a[right],a[q]);
    return Partition(a,left,right);
}

int ModifiedPartition(int a[], int left, int right, int x) {
    int q = left;
    for (q; q<=right;q++)
        if (a[q] == x) {
            break;
        }
    swap(a[q],a[right]);
    return Partition(a,left,right);
}

void quicksort(int a[], int left, int right) {
    if (left < right) {
        int pivot = Partition(a,left,right);
        cout << pivot << endl;
        quicksort(a,left,pivot-1);
        quicksort(a,pivot+1,right);
    }
}

void Random_Quicksort(int a[], int left, int right) {
    if (left < right) {
        int pivot = Random_Partition(a,left,right);
        Random_Quicksort(a,left,pivot-1);
        Random_Quicksort(a,pivot+1,right);
    }
    //display();
}

void display() {
    for (int i = 1; i<sizeof(a)/sizeof(int);i++)
        cout << a[i] << " ";
    cout << endl;
}

/* ---------------------------------*/
int randomized_select_order_statistic(int a[], int left, int right, int i) {
    if (left == right)
        return a[left];
    if (left < right) {
        int mid = Random_Partition(a,left,right);
        int num = mid-left+1;
        if (num == i)
            return a[mid];
        else if (num < i)
            return randomized_select_order_statistic(a,mid+1,right,i-num);
        else if (num > i)
            return randomized_select_order_statistic(a,left,mid-1,i);
    }
    return -1;
}

int findMedian(int a[], int left, int right) {
    int b[100];
    for (int i = left; i<=right;i++)
        b[i] = a[i];
    sort(b+left,b+right+1);
    return b[(left+right)/2];

}

int select_order_statistic(int a[], int left, int right, int i) {
    if (left == right)
        return a[left];
    if (left < right) {
        int n = right-left+1;
            // Step 1: Divide n/5 group, sort and find median of n/5 (+1) groups
        int numGroup = n/5, j = 1;
        int median[100], medSize = 0;
        for (j; j<=numGroup;j++) {
            int l = 5*(j-1)+1, r = 5*j;
            median[++medSize] = findMedian(a,l,r);           // Not sort a
        }
        if (n%5) {
            int l = 5*(j-1)+1, r = n;
            median[++medSize] = findMedian(a,l,r);           // Not sort a
        }
            // Step 2: Find median of the median array:
        int x = select_order_statistic(median,1,medSize,(medSize+1)/2);
            // Step 3: Choose pivot x and partition around x
        int pivot = ModifiedPartition(a,left,right,x);
            // Step 4: Recursive around left-side pivot or high-side pivor or return result
        int num = pivot-left+1;
        if (num == i)
            return a[pivot];
        else if (num < i)
            return select_order_statistic(a,pivot+1,right,i-num);
        else if (num > i)
            return select_order_statistic(a,left,pivot-1,i);

    }
    return -1;

}


/* ---------------Weighted Median ------------ */
int select_order_statistic_return_index(a_w a[], int left, int right, int i) {
    if (left == right)
        return left;
    if (left < right) {
        int n = right-left+1;
            // Step 1: Divide n/5 group, sort and find median of n/5 (+1) groups
        int numGroup = n/5, j = 1;
        a_w median[100], medSize = 0;
        for (j; j<=numGroup;j++) {
            int l = 5*(j-1)+1, r = 5*j;
            median[++medSize] = findMedian(a,left,right);           // Not sort a
        }
        if (n%5) {
            int l = 5*(j-1)+1, r = n;
            median[++medSize] = findMedian(a,left,right);           // Not sort a
        }
            // Step 2: Find median of the median array:
        int x = select_order_statistic_return_index(median,1,medSize,(medSize+1)/2);
            // Step 3: Choose pivot x and partition around x
        int pivot = ModifiedPartition(a,left,right,median[x]);
            // Step 4: Recursive around left-side pivot or high-side pivor or return result
        int num = pivot-left+1;
        if (num == i)
            return pivot;
        else if (num < i)
            return select_order_statistic_return_index(a,pivot+1,right,i-num);
        else if (num > i)
            return select_order_statistic_return_index(a,left,pivot-1,i);

    }
    return -1;

}

int weighted_median(int a[], int left, int right) {
    if (left == right)
        return a[left];
    if (left < right) {
            // Step 1: Find the median of (a,left,right) and partition around this median:
        int median_index = select_order_statistic_return_index(a,left,right,(left+right)/2);
        float wLeft = 0, wRight = 0;
            // Step 2: Calculate weight of all elements < median and weight of all elements > median
        for (int i = left; i<median_index;i++)
            wLeft += w[i];
        for (int i = median_index+1;i<=right;i++)
            wRight += w[i];
            // Step 3: Find the solution
        if (wLeft < 1/2 && wRight <= 1/2)
            return a[median_index];
        else {
            if (wLeft > wRight) {
                w[median_index] += wRight;          // Consider right-side and median as a number (of course > left-side) and its weight = sum of those.
                return weighted_median(a,left,median_index);
            }
            else {
                w[median_index] += wLeft;
                return weighted_median(a,median_index,right);
            }
        }
    }
}

int main()
{
    //quicksort(1,len-1);
    //Random_Quicksort(a,1,len-1);
    display();
/*
    auto t1 = std::chrono::high_resolution_clock::now();
    int x = select_order_statistic(a,1,sizeof(a)/sizeof(int)-1,20);
    cout << "NOT RANDOM: " << y;
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "select took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " milliseconds\n";

    auto t3 = std::chrono::high_resolution_clock::now();
    int x = randomized_select_order_statistic(a,1,sizeof(a)/sizeof(int)-1,20);
    cout << "RANDOM: " << x << endl;
    auto t4 = std::chrono::high_resolution_clock::now();
    std::cout << "random select took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count()
              << " milliseconds\n";

    cout << endl;
*/
qweqweqwe


    return 0;
}
