#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    int a;
    scanf("%d", &a);
    if(a<3){
    printf("%d",a*2);}
    else{
        printf("0");
    }
    return 0;
}