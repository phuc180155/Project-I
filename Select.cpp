#include <bits/stdc++.h>
using namespace std;

float a[] = {0,1.64,1.63,1.66,1.65,1.62,1.61,1.67,1.7,1.69,1.68,1.71};
float w[] = {0,4,3,6,5,2,1,7,10,9,8,11};
float median[100], tmp[100];

int n = 11;

void display() {
    for (int i = 1; i<= n;i++)
        cout << a[i] << " ";
    cout << endl;
}

int Partition(float a[], int left, int right) {
    int i = left-1, j = right;
    float pivot = a[right];
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

int Random_Partition(float a[], int left, int right) {
    int q = left+rand()%(right-left+1);
    swap(a[right],a[q]);
    return Partition(a,left,right);
}

int Modified_Partition(float a[], int left, int right, float x) {
    int q;
    for (q = left; q<= right; q++)
        if (a[q] == x)
            break;
    if (q <= right) {
        swap(a[q],a[right]);
        return Partition(a,left,right);
    }
    return 0;
}

/* -----------------RANDOMIZED_SELECT----------------------*/
float randomized_select_order_statistic(float a[], int left, int right, int i) {
    if (left == right)
        return a[left];
    if (left < right) {
            mycount++;
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

/* -----------------SELECT----------------------*/
float findMedian(float a[], int left, int right) {
    for (int i = left; i<=right;i++)
        tmp[i] = a[i];
    sort(tmp+left,tmp+right+1);
    return tmp[(left+right)/2];
}

float select_order_statistic(float a[], int left, int right, int i) {
    if (left == right)
        return a[left];
    if (left < right) {
        int len = right-left+1, numGroup = len/5, j, medsize = 0;
        for (j = 1; j <= numGroup; j++)
            median[++medsize] = findMedian(a,left+5*(j-1),left+5*j-1);
        if (len != 5*numGroup)
            median[++medsize] = findMedian(a,left+5*(j-1),right);
        float X = select_order_statistic(median,1,medsize,(1+medsize)/2);
        int pivot = Modified_Partition(a,left,right,X);
        int leftPart = pivot - left + 1;
        if (leftPart == i)
            return a[pivot];
        else if (leftPart > i)
            return select_order_statistic(a,left,pivot-1,i);
        else
            return select_order_statistic(a,pivot+1,right,i-leftPart);
    }
    return -1;
}


int main() {
    display();
    float median = randomized_select_order_statistic(a,1,n,(n+1)/2);
    cout << median << endl;
    median = select_order_statistic(a,1,n,(n+1)/2);
    cout << median << endl;
    return 0;
}
