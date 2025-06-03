#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"        // 화면 지우기 등 유틸리티 함수
#include "user.h"         // 사용자 로그인 및 정보 관련
#include "problem.h"      // 문제 관련 함수
#include "statistics.h"   // 통계 관련 함수
#include "menu.h"         // 관리자/학생 메뉴 함수

// 프로그램 시작점
int main() {
    int loginStatus = LOGIN_NONE; // 로그인 상태를 저장 (로그인 전 상태로 초기화)
    UserInfo user;                // 사용자 정보를 저장할 구조체

    clear_screen(); // 화면을 깨끗이 지움 (유틸리티 함수)

    // 로그인 성공할 때까지 반복
    while (loginStatus == LOGIN_NONE) {
        loginStatus = login(&user); // 로그인 시도 -> 결과는 loginStatus에 저장
    }

    // 로그인 상태에 따라 적절한 메뉴 호출
    if (loginStatus == LOGIN_STUDENT) {
        studentMenu(&user); // 학생 메뉴
    }
    else if (loginStatus == LOGIN_ADMIN) {
        adminMenu(&user); // 관리자 메뉴
    }
    else {
        // 로그인 중 오류가 발생한 경우
        printf("Something went Wrong :(\n");
        return 1; // 비정상 종료
    }

    return 0; // 정상 종료
}
