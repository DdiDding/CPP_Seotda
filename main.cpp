#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <string>

#define SHUFFLE 100
#define MAP_WIDTH 55
#define MAP_HEIGHT 20
#define MAP_X 0
#define MAP_Y 0

#define l_com 15
#define r_com 79
#define y_com 2


#define BLUE 3
#define WHITE 7
#define RED 4
#define GRAY 8
#define YELLOW 14
#define GREAN 2
#define SKY 11
#define PURPLE 5

void Title();
void move_line_num();
void Gotoxy(int x, int y);
int kb(int kb_result);
void Draw_map(int scene);
void Draw_deck();//덱을 그려줌
void Draw_race_money(int * money, int * r_money);//판돈의 단위에 점을 찍어서 출력해줌
void Draw_image(int * deck, int turn, int player); //패를 그려줌
void Draw_money(int * players_money);//소지금을 그려줌
void Interface(int race_money);
void Reset_text();
int Move_betting(int status);//플레이어 배팅 선택
void Select_betting(struct play_status *pp,int * players_money,int * t_money , int * r_money);

int Deck();//카드 뭉치에서 한가지 반환 해줌 포인터로 해야 할 듯
int Tier(int fir, int sec); // 가진 패를 조합해 등급을 반환
void Show_tier(int player, int * tier);
void Swap(int * plate);//카드를 섞음
void Com_select_text(int select);
void Win(int winner); //승자 텍스트로 출력

int player_num = 5;//플레이어 수
int call_people = player_num - 1; //콜가능한 인원수
int call_cnt = 0; //콜 카운트
int die_cnt = 0; //다이 카운트
int turn = 0; //현재 진행 턴
int last_turn = player_num; //콜만하기 전 마지막 차례

bool first_turn = true;

struct play_status {
	bool first; //선빵인가
	bool call; //콜을 외쳤는가
	bool die;
};

int main(){

	//Title();
	//system("cls");
	

	/* 카드들 저장함
	1,2 - 사용자
	3,4 - com1
	5,6 - com2
	7,8 - com3
	9,10 - com4 */
	int * deck;
	deck = (int *)calloc(sizeof(int), 20);
	//플레이어들(컴퓨터 포함)의 결과 티어 값 저장 할 곳
	int * players;
	players = (int *)calloc(sizeof(int), player_num);
	
	//플레이어들의 소지금 저장할 곳
	int * players_money;
	players_money = (int *)calloc(sizeof(int), player_num);
	for (int i = 0; i < 5; i++) {
		players_money[i] = 500000;
	}

	//몇번째 카드를 까는가
	int open_turn = 1; 
	
	//플레이어가 선택한 명령
	int player_select = 0; 
	//판돈!
	int total_money;
	int * p_total_money = &total_money;
	//콜비용!
	int race_money;
	int * p_race_money = &race_money;

	struct play_status p[5] = { 0 }; //선택 상태 구조체배열
	struct play_status *pp;
	pp = p;

	//게임시작!!!
	while (1) {

		total_money = 1500;
		race_money = 0;
		for (int i = 0; i < player_num; i++) {
			pp[i] = { false,false,false };
		}
		call_people = player_num - 1;
		call_cnt = 0;
		first_turn = true;



		system("cls");
		Draw_map(2);//게임 외곽선 그리기
		Draw_deck();//카드 틀 그리기
		Draw_race_money(p_total_money, p_race_money); //판돈 그리기 //기본 1500원
		Draw_money(players_money);//소지금 그리기


		Swap(deck); //카드 섞기
		for (int i = 0; i < player_num; i++) {
			players[i] = Tier(deck[2 * i], deck[(2 * i) + 1]);
		}

		Draw_image(deck, open_turn, 0); //플레이어 카드패 한장 확인
		Gotoxy(5, 33);
		printf("첫번째 패를 확인합니다.");

		Sleep(1000);
		//TODO 배팅 함수 만들기----------------------------------------------------------------------------------

		Reset_text();
		Gotoxy(5, 33);

		int next = 0; // 1이면 다음 패까기 , 0이면 기권승

		while (1) {

			Select_betting(pp, players_money, p_total_money, p_race_money);
			Reset_text();

			Draw_race_money(p_total_money, p_race_money); //판돈 그리기 //기본 1500원
			Draw_money(players_money);//소지금 그리기
			if (die_cnt == player_num - 1) {
				next = 0;
				break;
			}
			else if (call_cnt == call_people) {
				next = 1;
				break; //모든 사람이 콜 하였는지 확인 함!

			}

			//콜만하는 기능 보류
			if (turn + die_cnt > turn + player_num * 2) {
				break;
				for (int i = 0; i < player_num; i++) {
					(pp + i)->call = true;
				}
			}

			Sleep(1000);
		}

		//두번째 패를 까는 부분
		if (next == 1) {

			//초기화 부분
			call_cnt = 0;

			//콜 스텍 초기화
			for (int i = 0; i < player_num; i++) {
				(pp + i)->call = false;
			}

			first_turn = true;

			Gotoxy(5, 33);
			Reset_text();
			Gotoxy(5, 33);
			printf("두번째 패를 확인합니다.");
			Draw_image(deck, 2, 0); //플레이어 카드패 한장 확인
			Sleep(1000);
			Reset_text();
			while (1) {

				Select_betting(pp, players_money, p_total_money, p_race_money);
				Reset_text();
				Sleep(1000);
				Draw_race_money(p_total_money, p_race_money); //판돈 그리기 //기본 1500원
				Draw_money(players_money);//소지금 그리기
				if (die_cnt == player_num - 1) {

					next = 0;
					break;
				}
				else if (call_cnt == call_people) {
					next = 1;
					break; //모든 사람이 콜 하였는지 확인 함!
				}

				//콜만하는 기능 보류
				if (turn + die_cnt > turn + player_num * 2) {
					break;
					for (int i = 0; i < player_num; i++) {
						(pp + i)->call = true;
					}
				}
			}

			if (next == 0) {

				for (int i = 0; i < player_num; i++) {
					if ((pp + i)->die == false) {
						players_money[i] += p_total_money[0];
						Draw_money(players_money);
						Win(i);
						break;
					}
				}
			}
			else {

				int result = 200;
				int winner = 0;

				for (int i = 0; i < player_num; i++) {
					if ((pp + i)->die == false) {

						if (result > players[i]) {
							result = players[i];
							winner = i; // 승자 번호 인덱스 0부터 시작임~
						}

						Draw_image(deck, 1, i); //플레이어 카드패 한장 확인
						Draw_image(deck, 2, i); //플레이어 카드패 한장 확인
						Show_tier(i, players); //플레이어 티어 확인
					}
				}

				players_money[winner] += p_total_money[0];
				Draw_money(players_money);

				p_total_money[0] = 1500;
				Win(winner);

			}
		}
		//기권 승
		else {

			for (int i = 0; i < player_num; i++) {
				if ((pp + i)->die == false) {
					players_money[i] += p_total_money[0];
					Draw_money(players_money);
					Win(i);
					break;
				}
			}
		}

		Sleep(4000);
	}
	
	//TODO 배팅 함수 만들기 끝----------------------------------------------------------------------------------


	Gotoxy(0, 50);
	free(deck);

	return 0;
}

void Color(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

void Gotoxy(int x, int y) {
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int kb(int kb_result) { //1,2,3,4반환시 계속 입력 5반환시 게임시작 6반환시 게임종료

	int key;

	key = _getch(); //키보드의 아스키값은 224 후에 다음 x 로 확장문자임

	if (key == 224) {

		key = _getch();

		switch (key) {
		case 72:
			//printf("위방향키\n");
			kb_result = 1;
			return 1;
		case 75:
			//printf("왼쪽방향키\n");
			kb_result = 3;
			return 3;
		case 77:
			//printf("오른쪽방향키\n");
			kb_result = 4;
			return 4;
		case 80:
			//printf("아래방향키\n");
			kb_result = 2;
			return 2;
		}
		return 0;
	}
	else if (key == 13) { //엔터 입력시

		if (kb_result == 1) {

			return 5;
		}
		else if (kb_result == 2) {

			return 6;
		}
	}
	else return kb_result;
}

void Title() {

	system("cls");

	Draw_map(1);

	Gotoxy(MAP_WIDTH / 2 - 10, 5);
	printf("〓〓〓〓〓〓〓〓〓〓〓〓");

	Gotoxy(27, 7);
	printf("섯다");

	Gotoxy(MAP_WIDTH / 2 - 10, 9);
	printf("〓〓〓〓〓〓〓〓〓〓〓〓");

	move_line_num();

	Color(WHITE);
	Gotoxy(MAP_WIDTH / 2 - 3, 14);
	printf("게임 시작");

	Color(GRAY);
	Gotoxy(MAP_WIDTH / 2 - 3, 17);
	printf("게임 종료");


	int kb_result = 1;
	int kb_sub = 1; // 2인 - 2 3인 -3 4인 -4 5인 -5

	while (kb_result == 1 || kb_result == 2 || kb_result == 3 || kb_result == 4) {
		kb_result = kb(kb_result);

		if (kb_result == 1) {
			Color(WHITE);
			Gotoxy(MAP_WIDTH / 2 - 3, 14);
			printf("게임 시작");

			Color(GRAY);
			Gotoxy(MAP_WIDTH / 2 - 3, 17);
			printf("게임 종료");

		}
		else if (kb_result == 2) {
			Color(GRAY);
			Gotoxy(MAP_WIDTH / 2 - 3, 14);
			printf("게임 시작");

			Color(WHITE);
			Gotoxy(MAP_WIDTH / 2 - 3, 17);
			printf("게임 종료");

		}
		else if (kb_result == 3) {
			if (player_num == 3 || player_num == 4 || player_num == 5) {
				player_num--;
				move_line_num();
			}
		}
		else if (kb_result == 4) {
			if (player_num == 2 || player_num == 3 || player_num == 4) {
				player_num++;
				move_line_num();
			}
		}

	}

	if (kb_result == 5) { //r게임 시작
		//game_start = true;
	}
	else if (kb_result == 6) { //종료
		//game_start = false;
	}


	
}

void Draw_map(int scene) { //맵 테두리 그리는 함수  보니까 순서도 중요했다.. 바닥을 마지막에 깔아야함 1이면 타이틀, 2이면 인게임 외곽선
	
	if (scene == 1) {
		int i;

		Gotoxy(MAP_X, MAP_Y);

		printf("┌");
		for (i = MAP_X + 1;i < MAP_WIDTH - 1;i++) {
			printf("━");
		}
		printf("┐");


		for (i = MAP_Y + 1;i < MAP_HEIGHT;i++) {
			Gotoxy(MAP_X, i);
			printf("│");
			Gotoxy(MAP_WIDTH - 1, i);
			printf("│");

		}

		Gotoxy(MAP_X, MAP_HEIGHT);

		printf("└");
		for (i = MAP_X + 1;i < MAP_WIDTH - 1;i++) {
			printf("━");
		}
		printf("┘ ");
	}
	else {
		printf("┏");
		for (int i = 0; i < 114; i++) {
			printf("━");
		}
		printf("┓");

		for (int i = 1; i < 36; i++) {
			Gotoxy(0, i);
			printf("┃");
			Gotoxy(115, i);
			printf("┃");
		}

		Gotoxy(0, 31);
		printf("┠");
		for (int i = 0; i < 114; i++) {
			printf("━");
		}
		printf("┫");

		Gotoxy(0, 36);
		printf("┗");
		for (int i = 0; i < 114; i++) {
			printf("━");
		}
		printf("┛");
	}
}

void move_line_num() {

	Gotoxy(MAP_WIDTH / 2 - 10, 12);
	printf("                              ");

	if (player_num == 2) {
		Gotoxy(MAP_WIDTH / 2 - 10, 12);
		Color(GRAY);
		printf("◀");
		printf("    ");
		Color(WHITE);
		printf("    2 인   ");
		Color(GRAY);
		printf("3인 ");
		Color(YELLOW);
		printf("▶");
	}
	else if (player_num == 3) {
		Gotoxy(MAP_WIDTH / 2 - 10, 12);
		Color(YELLOW);
		printf("◀");
		Color(GRAY);
		printf(" 2인");
		Color(WHITE);
		printf("    3 인   ");
		Color(GRAY);
		printf("4인 ");
		Color(YELLOW);
		printf("▶");
	}
	else if (player_num == 4) {
		Gotoxy(MAP_WIDTH / 2 - 10, 12);
		Color(YELLOW);
		printf("◀");
		Color(GRAY);
		printf(" 3인");
		Color(WHITE);
		printf("    4 인   ");
		Color(GRAY);
		printf("5인 ");
		Color(YELLOW);
		printf("▶");
	}
	else if (player_num == 5) {
		Gotoxy(MAP_WIDTH / 2 - 10, 12);
		Color(YELLOW);
		printf("◀");
		Color(GRAY);
		printf(" 4인");
		Color(WHITE);
		printf("    5 인   ");
		Color(GRAY);
		printf("    ");
		printf("▶");
	}
}

void Swap(int * deck) {
	
	srand((unsigned int)time(NULL));

	int a, b, temp;

	//받은 영역에 숫자 집어넣음
	for (int i = 0; i < 20; i++) {
		deck[i] = i + 1;
	}

	//랜덤 섞기
	for (int i = 0; i < SHUFFLE; i++) {

		do {
			a = rand() % 20;
			b = rand() % 20;
		} while (a == b);

		temp = deck[a];
		deck[a] = deck[b];
		deck[b] = temp;
	}
}

int Tier(int fir, int sec) {


	if (fir==3 && sec==8 || fir==8 && sec==3) { //삼팔광땡 - 1
		return 1;
	}
	else if (fir == 4 && sec == 7 || fir == 4 && sec == 7) { //암행어사 - 2
		return 2;
	}
	else if (fir == 1 && sec == 8 || fir == 8 && sec == 1) { //일팔광땡 - 3
		return 3;
	}
	else if (fir == 1 && sec == 3 || fir == 3 && sec == 1) { //일삼광땡 - 4
		return 4;
	}
	else if (fir == 10 && sec == 20 || fir == 20 && sec == 10) { //장땡 - 5
		return 5;
	}
	else if (fir == 7 && sec == 4 || fir == 4 && sec == 7) { //암행어사 - 6
		return 4;
	}
	else if (fir == 19 && sec == 9 || fir == 9 && sec == 19) { //구땡 - 7
		return 7;
	}
	else if (fir == 18 && sec == 8 || fir == 8 && sec == 18) { //팔땡 - 8
		return 8;
	}
	else if (fir == 17 && sec == 7 || fir == 7 && sec == 17) { //칠땡 - 9
		return 9;
	}
	else if (fir == 16 && sec == 6 || fir == 6 && sec == 16) { //육땡 - 10
		return 10;
	}
	else if (fir == 15 && sec == 5 || fir == 5 && sec == 15) { //5땡 - 11
		return 11;
	}
	else if (fir == 14 && sec == 4|| fir == 4 && sec == 14) { //4땡 - 12
		return 12;
	}
	else if (fir == 13 && sec == 3 || fir == 3 && sec == 13) { //3땡 - 13
		return 13;
	}
	else if (fir == 12 && sec == 2 || fir == 2 && sec == 12) { //2땡 - 14
		return 14;
	}
	else if (fir == 11 && sec == 1 || fir == 1 && sec == 11) { //1땡 - 15
		return 15;
	}
	else if (fir == 1 && sec == 2 || fir == 1 && sec == 12 || fir == 11 && sec == 2 || fir == 11 && sec == 12) { //알리 - 16
		return 16;
	}
	else if (fir == 1 && sec == 4 || fir == 1 && sec == 14 || fir == 11 && sec == 4 || fir == 11 && sec == 14) { //독사 - 17
		return 17;
	}
	else if (fir == 1 && sec == 9 || fir == 1 && sec == 19 || fir == 11 && sec == 9 || fir == 11 && sec == 19) { //구삥 - 18
		return 18;
	}
	else if (fir == 1 && sec == 10 || fir == 1 && sec == 20 || fir == 11 && sec == 10 || fir == 11 && sec == 20) { //장삥 - 19
		return 19;
	}
	else if (fir == 10 && sec == 4 || fir == 10 && sec == 14 || fir == 20 && sec == 4 || fir == 20 && sec == 14) { //장사 - 20
		return 20;
	}
	else if (fir == 6 && sec == 4 || fir == 16 && sec == 14 || fir == 16 && sec == 4 || fir == 16 && sec == 14) { //세륙 - 21
		return 21;
	}
	else if (fir == 4 && sec == 9 || fir == 9 && sec == 4) { //멍텅구리 사구 - 100
		return 100;
	}
	else if (fir == 4 && sec == 19 || fir == 14 && sec == 9 || fir == 14 && sec == 19) { //사구 - 101
		return 101;
	}
	else { // 끗 부분
		int temp = (fir + sec) % 10;

		if (temp == 9) return 22; 
		if (temp == 8) return 23;
		if (temp == 7) return 24;
		if (temp == 6) return 25;
		if (temp == 5) return 26;
		if (temp == 4) return 27;
		if (temp == 3) return 28;
		if (temp == 2) return 29;
		if (temp == 1) return 30;
		if (temp == 0) return 31;

	}
}

void Draw_deck() {

	//사용자 패 출력
	{
		int x = 47;
		int y = 23;

		Gotoxy(x, y);

		printf("┌─────────┐┌─────────┐");
		Gotoxy(x, y + 1);
		printf("│         ││         │");
		Gotoxy(x, y + 2);
		printf("│         ││         │");
		Gotoxy(x, y + 3);
		printf("│         ││         │");
		Gotoxy(x, y + 4);
		printf("│         ││         │");
		Gotoxy(x, y + 5);
		printf("│         ││         │");
		Gotoxy(x, y + 6);
		printf("│         ││         │");
		Gotoxy(x, y + 7);
		printf("└─────────┘└─────────┘");

		Gotoxy(x - 9, y + 1);
		printf("사용자%d", 1);
		Gotoxy(x - 8, y + 3);
		printf("소지금");
	}
	//컴퓨터 패 출력
	{
		Gotoxy(l_com, y_com + 9);
		printf("┌─────────┐┌─────────┐");
		Gotoxy(l_com, y_com + 1 + 9);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 2 + 9);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 3 + 9);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 4 + 9);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 5 + 9);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 6 + 9);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 7 + 9);
		printf("└─────────┘└─────────┘");

		Gotoxy(l_com - 9, y_com + 1 + 9);
		printf("Computer1");
		Gotoxy(l_com - 8, y_com + 3 + 9);
		printf("소지금");
	}
	if (player_num > 2) {

		Gotoxy(l_com, y_com);
		printf("┌─────────┐┌─────────┐");
		Gotoxy(l_com, y_com + 1);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 2);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 3);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 4);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 5);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 6);
		printf("│         ││         │");
		Gotoxy(l_com, y_com + 7);
		printf("└─────────┘└─────────┘");

		Gotoxy(l_com - 9, y_com + 1);
		printf("Computer%d", 2);
		Gotoxy(l_com - 8, y_com + 3);
		printf("소지금");
	}
	if (player_num > 3) {
		Gotoxy(r_com, y_com);
		printf("┌─────────┐┌─────────┐");
		Gotoxy(r_com, y_com + 1);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 2);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 3);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 4);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 5);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 6);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 7);
		printf("└─────────┘└─────────┘");

		Gotoxy(r_com + 23, y_com + 1);
		printf("Computer%d", 3);
		Gotoxy(r_com + 24, y_com + 3);
		printf("소지금");
	}
	if (player_num > 4) {
		Gotoxy(r_com, y_com + 9);
		printf("┌─────────┐┌─────────┐");
		Gotoxy(r_com, y_com + 1 + 9);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 2 + 9);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 3 + 9);
		printf("│         ││         │");
		//printf("│   4월   ││         │");
		Gotoxy(r_com, y_com + 4 + 9);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 5 + 9);
		printf("│         ││         │");
		Gotoxy(r_com, y_com + 6 + 9);
		printf("│         ││         │");
		//printf("│       光││         │");
		Gotoxy(r_com, y_com + 7 + 9);
		printf("└─────────┘└─────────┘");

		Gotoxy(r_com + 23, y_com + 1 + 9);
		printf("Computer%d", 4);
		Gotoxy(r_com + 24, y_com + 3 + 9);
		printf("소지금");
		
	}
}

void Interface(int race_money) {

	
}

void Draw_race_money(int * money,int * r_money) { //MAX 99,999,999

	int x = 56;
	int y = 8;

	

	Gotoxy(x, y);
	printf("판 돈");

	if (*money > 999999) {
		
		Color(RED);

		Gotoxy(x - 4, y + 2);
		printf("%d", *money / 1000000);
		printf(",");
		if ((*money % 1000000) / 1000 < 100 && (*money % 1000000) / 1000 >= 10) printf("0");
		else if ((*money % 1000000) / 1000 < 10) printf("00");
		printf("%d", (*money % 1000000) / 1000);
		printf(",");
		if (*money % 1000 < 100 && *money % 1000 >= 10) printf("0");
		else if (*money % 1000 < 10) printf("00");
		printf("%d", *money % 1000);
	}
	else if (*money > 999) {

		Color(BLUE);

		Gotoxy(x - 2, y + 2);
		printf("%d", (*money % 1000000) / 1000);
		printf(",");
		if (*money % 1000 < 100 && *money % 1000 >= 10) printf("0");
		else if (*money % 1000 < 10) printf("00");
		printf("%d", *money  % 1000);
	}
	else {

		Color(YELLOW);

		Gotoxy(x , y + 2);
		printf("%d", *money);
	}
	Color(WHITE);
	printf(" 원");

	y += 4;
	Gotoxy(x-1,y);

	Color(GREAN);
	printf("콜비용");
	Color(WHITE);
	


	if (*r_money > 999999) {

		Gotoxy(x - 4, y + 2);
		printf("%d", *r_money / 1000000);
		printf(",");
		if ((*money % 1000000) / 1000 < 100 && (*r_money % 1000000) / 1000 >= 10) printf("0");
		else if ((*r_money % 1000000) / 1000 < 10) printf("00");
		printf("%d", (*r_money % 1000000) / 1000);
		printf(",");
		if (*money % 1000 < 100 && *r_money % 1000 >= 10) printf("0");
		else if (*r_money % 1000 < 10) printf("00");
		printf("%d", *r_money % 1000);
	}
	else if (*r_money > 999) {

		Gotoxy(x - 2, y + 2);
		printf("%d", (*r_money % 1000000) / 1000);
		printf(",");
		if (*r_money % 1000 < 100 && *r_money % 1000 >= 10) printf("0");
		else if (*r_money % 1000 < 10) printf("00");
		printf("%d", *r_money % 1000);
	}
	else {


		Gotoxy(x, y + 2);
		printf("%d", *r_money);
	}
	
	printf(" 원");
}

void Draw_image(int * deck, int turn, int player) {

	int x =0;
	int y = 0;

	//몇번째 플레이어인가
	if (player == 0) {
		x = 47;
		y = 23;
	}
	else if(player == 1) {
		x = l_com;
		y = y_com + 9;
	}
	else if (player == 2) {
		x = l_com;
		y = y_com;
	}
	else if (player == 3) {
		x = r_com;
		y = y_com;
	}
	else {
		x = r_com;
		y = y_com+9;
	}

	//첫번째 카드 깡
	if (turn == 1) {

		Gotoxy(x, y + 1);
		
		//열끗 표시
		if (0 < deck[player * 2] && deck[player * 2] < 11) {
			printf("│ [");
			Color(YELLOW);
			printf("열끗");
			Color(WHITE);
			printf("]  │");
		}
		else {
			printf("│         │");
		}

		Gotoxy(x, y + 3);
		// 월 표시
		if (deck[player * 2] % 10 != 0) {
			printf("│   %d월   │", deck[player * 2] % 10);
		}
		else {
			printf("│  10월   │");
		}

		Gotoxy(x, y + 6);
		
		// 광 표시
		if (deck[player * 2] == 1 || deck[player * 2] == 3 || deck[player * 2] == 8) {
			printf("│       ");
			Color(RED);
			printf("光");
			Color(WHITE);
			printf("│");
		}
		else {
			printf("│         │");
		}
	}
	//두번째 카드 깡
	if (turn == 2) {
		
		Gotoxy(x + 11, y + 1);
		//열끗 표시
		if (0 < deck[(player * 2) + 1] && deck[(player * 2) + 1] < 11) {
			printf("│ [");
			Color(YELLOW);
			printf("열끗");
			Color(WHITE);
			printf("]  │");
		}
		else {
			printf("│         │");
		}

		Gotoxy(x + 11, y + 3);
		// 월 표시
		if (deck[(player * 2) + 1] % 10 != 0) {
			printf("│   %d월   │", deck[(player * 2) + 1] % 10);
		}
		else {
			printf("│  10월   │");
		}

		Gotoxy(x + 11, y + 6);
		// 광 표시
		if (deck[(player * 2) + 1] == 1 || deck[(player * 2) + 1] == 3 || deck[(player * 2) + 1] == 8) {
			printf("│       ");
			Color(RED);
			printf("光");
			Color(WHITE);
			printf("│");
		}
		else {
			printf("│         │");
		}
	}
}

void Reset_text() {
	
	Gotoxy(1, 32);
	printf("                                                                                                                  ");
	Gotoxy(1, 33);
	printf("                                                                                                                  ");
	Gotoxy(1, 34);
	printf("                                                                                                                  ");
	Gotoxy(1, 35);
	printf("                                                                                                                  ");
}

int Move_betting(int status) { // status 가 1이면 선빵 선택지 , 2이면 중간 선택지 , 3이면 콜만 가능한거


	int kb_result = 1;
	int kb_sub = 1; // 1 - 다이 , 2 - 따당 , 3- 콜, 4 - 하프 , 5 - 올인

	if (status == 1) {
		Gotoxy(4, 33);
		printf("▶");

		Color(RED);
		Gotoxy(7, 33);
		printf("다이");
		Color(GRAY);
		Gotoxy(32, 33);
		printf("따당");
		Color(GRAY);
		Gotoxy(57, 33);
		printf(" 콜 ");
		Gotoxy(82, 33);
		Color(SKY);
		printf("하프");
		Color(GRAY);
		Gotoxy(107, 33);
		printf("올인");
		Color(WHITE);
		//제일 처음 다이랑 하프만 가능 할 때
		while (kb_result == 1 || kb_result == 2 || kb_result == 3 || kb_result == 4) {
			kb_result = kb(kb_result);

			if (kb_result == 3) { //왼쪽 누를 때 4만 가능

				if (kb_sub == 4) {
					Gotoxy(4, 33);
					printf("▶");
					Gotoxy(79, 33);
					printf("  ");
					kb_sub -= 3;
				}
			}
			else if (kb_result == 4) { // 오른쪽 누를 때 1만 가능함
				if (kb_sub == 1) {
					Gotoxy(4, 33);
					printf("  ");
					Gotoxy(79, 33);
					printf("▶");
					kb_sub += 3;
				}
			}
		}
	}
	else if (status == 2) {
		//중간 모든것이 될 때
		Gotoxy(4, 33);
		printf("▶");

		Color(RED);
		Gotoxy(7, 33);
		printf("다이");
		Color(YELLOW);
		Gotoxy(32, 33);
		printf("따당");
		Color(GREAN);
		Gotoxy(57, 33);
		printf(" 콜 ");
		Gotoxy(82, 33);
		Color(SKY);
		printf("하프");
		Color(PURPLE);
		Gotoxy(107, 33);
		printf("올인");
		Color(WHITE);
		while (kb_result == 1 || kb_result == 2 || kb_result == 3 || kb_result == 4) {
			kb_result = kb(kb_result);

			if (kb_result == 3) { //왼쪽 누를 때 (2, 3, 4, 5) 가능

				if (kb_sub == 2) {
					Gotoxy(4, 33);
					printf("▶");
					Gotoxy(29, 33);
					printf("  ");
					kb_sub--;
				}
				else if (kb_sub == 3) {
					Gotoxy(4, 33);
					printf("  ");
					Gotoxy(29, 33);
					printf("▶");
					Gotoxy(54, 33);
					printf("  ");
					kb_sub--;
				}
				else if (kb_sub == 4) {
					Gotoxy(29, 33);
					printf("  ");
					Gotoxy(54, 33);
					printf("▶");
					Gotoxy(79, 33);
					printf("  ");
					kb_sub--;
				}
				else if (kb_sub == 5) {
					Gotoxy(54, 33);
					printf("  ");
					Gotoxy(79, 33);
					printf("▶");
					Gotoxy(104, 33);
					printf("  ");
					kb_sub--;
				}

			}
			else if (kb_result == 4) { // 오른쪽 누를 때
				if (kb_sub == 1) {
					Gotoxy(4, 33);
					printf("  ");
					Gotoxy(29, 33);
					printf("▶");
					Gotoxy(54, 33);
					printf("  ");
					kb_sub++;
				}
				else if (kb_sub == 2) {
					Gotoxy(29, 33);
					printf("  ");
					Gotoxy(54, 33);
					printf("▶");
					Gotoxy(79, 33);
					printf("  ");
					kb_sub++;
				}
				else if (kb_sub == 3) {
					Gotoxy(54, 33);
					printf("  ");
					Gotoxy(79, 33);
					printf("▶");
					Gotoxy(104, 33);
					printf("  ");
					kb_sub++;
				}
				else if (kb_sub == 4) {
					Gotoxy(79, 33);
					printf("  ");
					Gotoxy(104, 33);
					printf("▶");
					kb_sub++;
				}
			}
		}
	}
	else {
		Gotoxy(4, 33);
		printf("▶");

		Color(RED);
		Gotoxy(7, 33);
		printf("다이");
		Color(GRAY);
		Gotoxy(32, 33);
		printf("따당");
		Color(GREAN);
		Gotoxy(57, 33);
		printf(" 콜 ");
		Gotoxy(82, 33);
		Color(GRAY);
		printf("하프");
		Color(GRAY);
		Gotoxy(107, 33);
		printf("올인");
		Color(WHITE);
		//마지막 콜이나 다이만 가능할 때
		while (kb_result == 1 || kb_result == 2 || kb_result == 3 || kb_result == 4) {
			kb_result = kb(kb_result);

			if (kb_result == 3) { //왼쪽 누를 때 3만 가능

				if (kb_sub == 3) {
					Gotoxy(4, 33);
					printf("▶");
					Gotoxy(54, 33);
					printf("  ");
					kb_sub -= 2;
				}
			}
			else if (kb_result == 4) { // 오른쪽 누를 때 1만 가능함
				if (kb_sub == 1) {
					Gotoxy(4, 33);
					printf("  ");
					Gotoxy(54, 33);
					printf("▶");
					kb_sub += 2;
				}
			}
		}
	}
	return kb_sub;
}

void Draw_money(int * players_money) {

	int x = 47;
	int y = 23;
	Gotoxy(x - 13, y + 5);
	printf("%d 원", players_money[0]);

	Gotoxy(l_com - 13, y_com + 5 + 9);
	printf("%d 원", players_money[1]);

	if (player_num > 2) {
		Gotoxy(l_com - 13, y_com + 5);
		printf("%d 원", players_money[2]);
	}
	if (player_num > 3) {
		Gotoxy(r_com + 23, y_com + 5);
		printf("%d 원", players_money[3]);
	}
	if (player_num > 4) {
		Gotoxy(r_com + 23, y_com + 5 + 9);
		printf("%d 원", players_money[4]);
	}
}

void Select_betting(struct play_status *pp, int * players_money, int * total_money, int * race_money) {

	int player_select = 0;
	int player = turn % player_num;


	if (player == 0) {
		if (pp -> die == true) {

		}
		else {

			//상황에따른 플레이어 메뉴 선택
			if (first_turn == true) {
				player_select = Move_betting(1);
				first_turn = false;
			}
			else if (pp -> call == true) {
				player_select = Move_betting(3);
			}
			else {
				player_select = Move_betting(2);
			}

			//선택지에 따른 결과
			//다이
			if (player_select == 1) {

				Reset_text();
				Gotoxy(56, 33);
				printf("사용자 : 다이");	Sleep(2000);

				pp->die = true;
				call_people--;
				die_cnt++;
			}
			//따당
			else if (player_select == 2) {
				Reset_text();
				Gotoxy(56, 33);
				printf("사용자 : 따당");	Sleep(2000);
				players_money[0] -= 2 * (*race_money);
				total_money[0] += 2 * (*race_money);
				race_money[0] *= 2;
				call_cnt = 0;
			}
			//콜
			else if (player_select == 3) {
				Reset_text();
				Gotoxy(56, 33);
				printf("사용자 :  콜");	Sleep(2000);
				players_money[0] -= race_money[0];
				total_money[0]+= race_money[0];

				pp->call = true;
				call_cnt++;
			}
			//하프
			else if (player_select == 4) {
				Reset_text();
				Gotoxy(56, 33);
				printf("사용자 : 하프");	Sleep(2000);
				players_money[0] -= (total_money[0] + race_money[0]) / 2;
				total_money[0] += (total_money[0] + race_money[0]) / 2 + race_money[0];
				race_money[0] += (total_money[0] + race_money[0]) / 2;

				call_cnt = 0;
			}
			//올인
			else if (player_select == 5) {

			}
		}
	}
	else {

		if ((pp+player)->die == true) {
			player_select = 0;
		}
		else {

			//상황에따른 플레이어 메뉴 선택
			if (first_turn == true) {

				int r = rand()%2;
				if(r == 0) player_select = 1;
				else player_select = 4;
			}
			else if ((pp + player)->call == true) {
				int r = rand() % 2;

				if (r == 0) player_select = 1;
				else player_select = 3;
			}
			else {
				int r = rand() % 4;

				if (r == 0) player_select = 1;
				else if (r == 1) player_select = 2;
				else if (r == 2) player_select = 3;
				else if (r == 3) player_select = 4;
			}

			//콜만나오게 테스트--------------------------------------------
			//player_select = 3;

			//선택지에 따른 결과
			//다이
			if (player_select == 1) {

				Reset_text();
				Gotoxy(56, 33);
				printf("Computer%d : 다이", turn%player_num);
				Sleep(2000);
				(pp + player)->die = true;
				//if ((pp + player)->call = true && ) call_cnt--;
				die_cnt++;
				call_people--;
			}
			//따당
			else if (player_select == 2) {

				Reset_text();
				Gotoxy(56, 33);
				printf("Computer%d : 따당", turn%player_num);

				Sleep(2000);
				players_money[player] -= 2 * (*race_money);
				total_money[0] += 2 * (*race_money);
				race_money[0] *= 2;
				call_cnt = 0;
			}
			//콜
			else if (player_select == 3) {

				Reset_text();
				Gotoxy(56, 33);
				printf("Computer%d :  콜", turn%player_num);
				Sleep(2000);
				players_money[player] -= race_money[0];
				total_money[0] += race_money[0];

				call_cnt ++;
				(pp + player)->call = true;
			}
			//하프
			else if (player_select == 4) {

				Reset_text();
				Gotoxy(56, 33);
				printf("Computer%d : 하프", turn%player_num);
				Sleep(2000);

				if (first_turn == true) {
					first_turn = false;
				}

				players_money[player] -= (total_money[0] + race_money[0]) / 2;
				total_money[0] += (total_money[0] + race_money[0]) / 2 + race_money[0];
				race_money[0] += (total_money[0] + race_money[0]) / 2;

				call_cnt = 0;
			}
			//올인
			else if (player_select == 5) {

			}

		}
	}
	turn++;

}


void Show_tier(int player,int * tier) {

	int x = 47;
	int y = 23;

	Gotoxy(x, y);
	if (player == 0) Gotoxy(56,21);
	else if (player == 1) Gotoxy(l_com + 24, y_com + 3 + 9);
	else if (player == 2) Gotoxy(l_com + 24, y_com + 3);
	else if (player == 3) Gotoxy(r_com -7, y_com + 3);
	else if (player == 4) Gotoxy(r_com - 7, y_com + 3 + 9);

	if (tier[player] == 1) printf("삼팔광땡");
	else if (tier[player] == 2) printf("암행어사");
	else if (tier[player] == 3) printf("일팔광땡");
	else if (tier[player] == 4) printf("일삼광땡");
	else if (tier[player] == 5) printf("장땡");
	else if (tier[player] == 6) printf("땡잡이");
	else if (tier[player] == 7) printf("구땡");
	else if (tier[player] == 8) printf("팔땡");
	else if (tier[player] == 9) printf("칠땡");
	else if (tier[player] == 10) printf("육땡");
	else if (tier[player] == 11) printf("오땡");
	else if (tier[player] == 12) printf("사땡");
	else if (tier[player] == 13) printf("삼땡");
	else if (tier[player] == 14) printf("이땡");
	else if (tier[player] == 15) printf("일땡");
	else if (tier[player] == 16) printf("알리");
	else if (tier[player] == 17) printf("독사");
	else if (tier[player] == 18) printf("구삥");
	else if (tier[player] == 19) printf("장삥");
	else if (tier[player] == 20) printf("장사");
	else if (tier[player] == 21) printf("세륙");
	else if (tier[player] == 22) printf("9끗");
	else if (tier[player] == 23) printf("8끗");
	else if (tier[player] == 24) printf("7끗");
	else if (tier[player] == 25) printf("6끗");
	else if (tier[player] == 26) printf("5끗");
	else if (tier[player] == 27) printf("4끗");
	else if (tier[player] == 28) printf("3끗");
	else if (tier[player] == 29) printf("2끗");
	else if (tier[player] == 30) printf("1끗");
	else if (tier[player] == 31) printf("망통");
	else if (tier[player] == 100) printf("멍텅구리");
	else if (tier[player] == 101) printf("구사");

}

void Win(int winner) {

	if (winner == 0) {
		Gotoxy(5, 33);
		Reset_text();
		Gotoxy(5, 33);
		printf("사용자의 승리");
	}
	else if (winner == 1) {
		Gotoxy(5, 33);
		Reset_text();
		Gotoxy(5, 33);
		printf("컴퓨터1의 승리");
	}
	else if (winner == 2) {
		Gotoxy(5, 33);
		Reset_text();
		Gotoxy(5, 33);
		printf("컴퓨터2의 승리");
	}
	else if (winner == 3) {
		Gotoxy(5, 33);
		Reset_text();
		Gotoxy(5, 33);
		printf("컴퓨터3의 승리");
	}
	else if (winner == 4) {
		Gotoxy(5, 33);
		Reset_text();
		Gotoxy(5, 33);
		printf("컴퓨터4의 승리");
	}
}