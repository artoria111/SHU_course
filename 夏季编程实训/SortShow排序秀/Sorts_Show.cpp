// Sorts_Show.cpp	具体的排序算法成员函数定义（相应的函数声明在文件SortShow.h中）
#include "SortShow.h"
#include <cassert>

int getch(void){
	char x=getchar();
	return (int)x;
}

int kbhit(void){
	char x=getchar();
	if((int)x) return 1;
	return 0;
}

void Bubble_Sort(int *array, int length)
{
	ShowText(col1-4, top-1, 0, 15, "︻");
	ShowText(col1-4, top+length, 0, 15, "︼");
	for(int i=1; i<length; i++)
	{
		for(int j=0; j<length-i; j++)
		{
			if(array[j] > array[j+1])		// 有元素需要交换时
				SWAP(array, j, j+1);
			else							// 无元素需要交换时，展示曾比较过
				ShowBars(col1, top+j, array[j], col1, top+j+1, array[j+1]);
		}
		ShowText(col1-4, top+length-i+1, 0, 15, "  ");
		ShowText(col1-4, top+length-i, 0, 15, "︼");
	}
	ShowText(col1-4, top+1, 0, 15, "  ");
	ShowText(col1-4, top-1, 0, 15, "  ");
}


void bubbleSort_quick(int *arr, int len)
{
	ShowText(col1-4, top-1, 0, 15, "︻");
	ShowText(col1-4, top+len, 0, 15, "︼");
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
				SWAP(arr, j, j+1);
			}
			else{
				ShowBars(col1, top+j, arr[j], col1, top+j+1, arr[j+1]);
			} 
		}
		ShowText(col1-4, top+len-i, 0, 15, "  ");
		ShowText(col1-4, top+len-i-1, 0, 15, "︼");
	}
	ShowText(col1-4, top+1, 0, 15, "  ");
	ShowText(col1-4, top-1, 0, 15, "  ");
}

void Select_Sort(int *array, int length)	// 选择排序
{
	int k;
	
	ShowText(col1-4, top+length, 0, 15, "︼ ");
	for(int i=0; i<length-1; i++)
	{
		k = i;
		ShowText(col1-4, top+i-1, 0, 15, "︻");
		ShowText(col1-4, top+i, 0, 15, "min");
		for(int j=i+1; j<length; j++)
		{
			ShowBars(col1, top+j, array[j], col1, top+k, array[k]);
			if(array[j] < array[k])
			{
				ShowText(col1-4, top+k, 0, 15, "   ");
				k = j;
				ShowText(col1-4, top+k, 0, 15, "min");
			}
		}
		if(k!=i)
			SWAP(array, k, i);
		ShowText(col1-4, top+k, 0, 15, "   ");
		ShowText(col1-4, top+i-1, 0, 15, "   ");
	}
	ShowText(col1-4, top+length, 0, 15, "   ");
}

void Selectsort_quick(int *arr, int len)
{
	
    assert(arr);
    int left = 0;
    int right = len - 1;
    ShowText(col1-4, top, 0, 15, "min");
    ShowText(col1-4, top+len-1, 0, 15, "max");
    while (left < right)
    {
    	int min = left;
        int max = right;
        ShowText(col1-4, top+min, 0, 15, "min");
        ShowText(col1-4, top+max, 0, 15, "max");
    	ShowText(col1-4, top+right+1, 0, 15, "︼ ");
    	ShowText(col1-4, top+left-1, 0, 15, "︻");
        
        for (int i = left; i <= right; i++)
        {
            if (arr[i] < arr[min])
            {
            	ShowText(col1-4, top+min, 0, 15, "   ");
                min = i;
                
            }
            if (arr[i] > arr[max])
            {
            	ShowText(col1-4, top+max, 0, 15, "   ");
                max = i;
                
            }
        }
        ShowText(col1-4, top+min, 0, 15, "min");
        ShowText(col1-4, top+max, 0, 15, "max");
        SWAP(arr, max, right);
        if (min == right)
            {
            ShowText(col1-4, top+min, 0, 15, "   ");
			min = max;
			ShowText(col1-4, top+min, 0, 15, "equ");
			}
        SWAP(arr, min, left);
        ShowText(col1-4, top+right+1, 0, 15, "  ");
    	ShowText(col1-4, top+left-1, 0, 15, " ");
    	ShowText(col1-4, top+min, 0, 15, "   ");
        ShowText(col1-4, top+max, 0, 15, "   ");
        left++;
        right--;
        
    }

}

