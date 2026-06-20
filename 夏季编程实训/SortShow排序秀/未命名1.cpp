#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;


void bubbleSort_quick(int arr[], int len)
{
	bool flag = true;//true 代表交换过  false代表还没交换
	//外层循环 len-1次
	for (int i = 0; i < len - 1 && flag==true; i++)
	{
		flag = false;//每轮初始化状态为真
		//内层循环len-i-1
		for (int j = 0; j < len - i - 1; j++)
		{
			//相邻元素，前者大于后者，进行交换
			if (arr[j]>arr[j+1])
			{
				flag = true;//发生交换  状态改为真
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void SelectionSort_quick(vector<int> &v)
{
    int len = v.size();
    for (int left = 0, right = len - 1; left < right; left++, right--)
    {
        int min = left;     // 记录最小值
        int max = right;    // 记录最大值
        for (int index = left; index <= right; index++)
        {
            if (v[index] < v[min])
                min = index;
            if (v[index] > v[max])
                max = index;
        }
        // 最小值交换
        swap(v[min], v[left]);
        // 此处是先排最小值的位置，所以得考虑最大值在最小位置的情况
        if (left == max)
        {
            max = min;
        }
        swap(v[max], v[right]);
    }
}
template<typename T>

void Selection_quick(T *arr, int n)
{
    assert(arr);
    int left = 0;
    int right = n - 1;
    while (left < right)
    {
        int min = left;
        int max = right;
        for (int i = left; i <= right; i++)
        {
            if (arr[i] < arr[min])
                min = i;
            if (arr[i] > arr[max])
                max = i;
        }
        // 考虑修正的情况，最大值在最小位置，最小值在最大位置
        swap(arr[max], arr[right]);
        if (min == right)
            min = max;
        swap(arr[min], arr[left]);
        left++;
        right--;
    }
}

