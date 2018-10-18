#include <Serial.h>

int main()
{
    Serial::init(38400);
    printf("Hello World!\n");

    char c;
    while(true){
        if(Serial::available()){
            scanf("%c", &c);
            printf("%c", c);
        }
    }

    return 0;
}