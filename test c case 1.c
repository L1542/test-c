#include <stdio.h>

int main()
{
    char answer;
    float money,bath;
    printf("'D' for US Dollar\n");
    printf("'Y' for Japanese Yen\n");
    printf("'K' for Korean Won\n");
    printf("Please input choice:");
    answer = getchar();
    printf("Enter amount in That Baht:");
    scanf("%f",&money);
    switch(answer){
        case 'D':
        case 'd':
        bath = money/33.46 ;
        printf("%.2f bath = %.2f us Dollar",money,bath);break;
        case 'Y':
        case 'y':
        bath = money/0.29 ;
        printf("%.2f bath = %.2f Japanese Yen",money,bath);break;
        case 'K':
        case 'k':
        bath = money/35.28;
        printf("%.2f bath = %.2f Korean Won",money,bath);break;
    }
    
    
    
}
