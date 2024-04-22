#include <stdio.h>

struct score {

    int math;  //구조체 score 의 멤버 math 선언
    int english; //구조체 score 의 멤버 english 선언

};

struct score getScore()    
{   
    struct score s; //구조체 score을 s 변수로 선언

    

    s.math = 87;   //structure s의 math 멤버에 87 대입
    s.english = 76; // structure s의 english 멤버에 87 대입
    

    return s ;    // 구조체 변수 반환
}

int main()
{
   struct score s1; 

   s1 = getScore(); //구조체의 반환 값이 s1에 들어감
   
   printf("math score : %d\n", s1.math); //87
   printf("english score : %d\n", s1.english); //76

    return 0;
}
출처: https://abcdefgh123123.tistory.com/112 [꾸준함의 기록:티스토리]