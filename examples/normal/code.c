#include <stdio.h>

int main() {
    int a, b;
    scanf("%d %d",&a,&b);
    printf("%d",a+b);
    return 0;
}

// #include <stdio.h>  
  
// int main() {  
//     int *ptr; // 声明一个整数指针，但没有初始化  
//     *ptr = 2; // 试图给ptr指向的内存地址赋值，但ptr并未指向有效的内存区域  
//     printf("Value: %d\n", *ptr); // 试图打印ptr指向的值，但同样会因为ptr无效而失败  
//     return 0;  
// }