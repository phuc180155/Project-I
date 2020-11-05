#include <bits/stdc++.h>

using namespace std;

/* ---------------Weighted Median ------------ */
typedef pair<float, float> a_w;
vector<a_w> a, tmp(1000);
int n;
float sum=0;

int ModifiedPartition(vector<a_w> &vec, int left, int right, float x) {
    int q;
    for (q = left; q<=right;q++)
        if (vec[q].first == x) {
            break;
        }
    swap(vec[q],vec[right]);
    int i = left-1, j = right;
    float pivot = vec[right].first;
    while(true) {
        i++;j--;
        while(i<=right && vec[i].first < pivot) i++;
        while(j>=left && vec[j].first > pivot) j--;
        if (i < j)
            swap(vec[i],vec[j]);
        else {
            swap(vec[i],vec[right]);
            break;
        }
    }
    return i;
}

a_w findMedian(vector<a_w> &a, int left, int right) {
    for (int i = left; i<=right;i++)
        tmp[i] = a[i];
    sort(tmp.begin()+left,tmp.begin()+right+1);
    return tmp[(left+right)/2];
}

int select_order_statistic_return_index(vector<a_w> &vec, int left, int right, int i) {
    if (left == right)
        return left;
    if (left < right) {
        int n = right-left+1;
            /* Step 1: Divide ceiling(n/5) group and find median of n/5  groups */
        int numGroup = n/5, j;
        vector<a_w> median;
        median.push_back(a_w(0,0));
        for (j=1; j<=numGroup;j++) {
            int l = 5*(j-1)+left, r = 5*j+left-1;
            median.push_back(findMedian(vec,l,r));
        }
        if (n%5) {
            int l = 5*(j-1)+left, r = right;
            median.push_back(findMedian(vec,l,r));
        }
            /* Step 2: Find median of the median array:*/
        int medSize = median.size()-1;
        int x = select_order_statistic_return_index(median,1,medSize,(medSize+1)/2);

            /* Step 3: Choose pivot x and partition around x*/
        int pivot = ModifiedPartition(vec,left,right,median[x].first);

            /* Step 4: Recursive around left-side pivot or high-side pivor or return result*/

        int num = pivot-left+1;
        if (num == i)
            return pivot;
        else if (num < i)
            return select_order_statistic_return_index(vec,pivot+1,right,i-num);
        else if (num > i)
            return select_order_statistic_return_index(vec,left,pivot-1,i);

    }
    return -1;
}

a_w weighted_median(vector<a_w> &vec, int left, int right) {
    if (left == right)
        return vec[left];

    if (right - left == 1) {
        if (vec[left].second > vec[right].second)
            return vec[left];
        else
            return vec[right];
    }

    else if (right - left >= 1) {
            /* Step 1: Find the median of (a,left,right) and partition around this median: */
        int median_index = select_order_statistic_return_index(vec,1,vec.size()-1,(left+right)/2);

            /* Step 2: Calculate weight of all elements < median and weight of all elements > median */
        float wLeft = 0, wRight = 0;
        for (int i = left; i<median_index;i++)
            wLeft += vec[i].second;
        for (int i = median_index+1;i<=right;i++)
            wRight += vec[i].second;

            /* Step 3: Find the solution*/
        if (wLeft < sum/2 && wRight <= sum/2)
            return vec[median_index];
        else {
            if (wLeft > wRight) {
                vec[median_index].second += wRight;          // Consider right-side and median as a number (of course > left-side) and its weight = sum of those.
                return weighted_median(vec,left,median_index);
            }
            else {
                vec[median_index].second += wLeft;
                return weighted_median(vec,median_index,right);
            }
        }
    }
    return a_w(-1,-1);
}


int main() {
    freopen("D:\\Untitled\\input.inp","r",stdin);
    a.push_back(a_w(0,0));
    cin >> n;
    float height, weight;
    for (int i = 1; i<=n; i++) {
        cin >> height >> weight;
        sum += weight;
        a.push_back(a_w(height,weight));
    }
    a_w x = weighted_median(a,1,a.size()-1);
    freopen("D:\\Untitled\\output.inp","w",stdout);
    cout << x.first;
    return 0;
}
