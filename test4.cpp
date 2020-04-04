//왼쪽 아래 (381,120) 한칸 149

//남은 시간 표기는 showMessage로 할 수 없어서 클리어시 타이머에 표기되게했습니다!

#include <Bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma comment(lib, "Bangtal.lib")

SceneID scene1, scene2, scene3;
ObjectID startButton, resetButton, clearButton, newgameButton, endButton, restartButton;
ObjectID puzzle[9];
TimerID timer;
int p, num;
float second;

int tube[9]; // tube[n] = t : n번칸에는 t번 튜브 그림이 들어가 있음
int reset[9];

int X[9] = { 381, 530, 679, 381, 530, 679, 381, 530, 679 };
int Y[9] = { 418, 418, 418, 269, 269, 269, 120, 120, 120 };

ObjectID createObject(const char* name, const char* image, SceneID scene, int x, int y, bool shown, float size) {

	ObjectID object = createObject(name, image);
	locateObject(object, scene, x, y);
	scaleObject(object, size);

	if (shown == true)
		showObject(object);

	return object;
}

//n번칸 그림을 t번 튜브 그림으로 바꿔주기
void setPuzzleImage(ObjectID nPuzzle, int t) {

	if (t == 0)
		setObjectImage(nPuzzle, "튜브_0.png");
	else if (t == 1)
		setObjectImage(nPuzzle, "튜브_1.png");
	else if (t == 2)
		setObjectImage(nPuzzle, "튜브_2.png");
	else if (t == 3)
		setObjectImage(nPuzzle, "튜브_3.png");
	else if (t == 4)
		setObjectImage(nPuzzle, "튜브_4.png");
	else if (t == 5)
		setObjectImage(nPuzzle, "튜브_5.png");
	else if (t == 6)
		setObjectImage(nPuzzle, "튜브_6.png");
	else if (t == 7)
		setObjectImage(nPuzzle, "튜브_7.png");
	else if (t == 8)
		setObjectImage(nPuzzle, "빈칸.png");
}

void random() {

	//tube[9]에 무작위 배열 넣기
	for (int i = 0; i < 9; i++) {
		tube[i] = rand() % 9;

		for (int j = 0; j < i; j++) {
			if (tube[i] == tube[j])
				i--;
		}
	}

	//리셋 배열에 저장해두기
	for (int i = 0; i < 9; i++)
		reset[i] = tube[i];

}

//a칸부터 b칸까지 tube값에 맞게 이미지 재설정
void setPuzzle(int a, int b) {

	for (int i = a; i < b + 1; i++) {
		setPuzzleImage(puzzle[i], tube[i]);
	}
}

bool clearCheck() {
	
	bool clear = true;

	for (int i = 0; i < 9; i++) {
		
		if (i != tube[i]) 
			clear = false;
	}

	return clear;
}

void clear() {
	
	stopTimer(timer);
	second = 600 - getTimer(timer);
	setTimer(timer, second);

	enterScene(scene3);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	//시작버튼 누르면 퍼즐 화면 넘어가고 퍼즐 섞어주기
	if (object == startButton || object == newgameButton || object == restartButton) {
		enterScene(scene2);
		random();
		setPuzzle(0, 8);
		setTimer(timer, 600);
		startTimer(timer);
	}

	else if (object == resetButton) {

		for (int i = 0; i < 9; i++)
			tube[i] = reset[i];

		setPuzzle(0, 8);
	}

	else if (object == clearButton) {

		if (clearCheck() == false)
			showMessage("퍼즐이 완성되지 않았습니다");

		else 
			clear();

	}

	else if (object == endButton)
		endGame();

	//퍼즐 조각 누를 경우
	else
	{
		//object값 - 5 == puzzle[p]의 p값
		p = object - 5;

		//왼쪽 드래그 : 0,3,6칸 아닐 경우 + 드래그한 칸이 빈칸일 경우
		if (action == MOUSE_DRAG_LEFT && p % 3 != 0 && tube[p - 1] == 8) {
			num = tube[p];
			tube[p] = tube[p - 1];
			tube[p - 1] = num;
			setPuzzle(p - 1, p);
		}

		else if (action == MOUSE_DRAG_RIGHT && p % 3 != 2 && tube[p + 1] == 8) {
			num = tube[p];
			tube[p] = tube[p + 1];
			tube[p + 1] = num;
			setPuzzle(p, p + 1);
		}

		else if (action == MOUSE_DRAG_UP && p > 2 && tube[p - 3] == 8) {
			num = tube[p];
			tube[p] = tube[p - 3];
			tube[p - 3] = num;
			setPuzzle(p - 3, p);
		}

		else if (action == MOUSE_DRAG_DOWN && p < 6 && tube[p + 3] == 8) {
			num = tube[p];
			tube[p] = tube[p + 3];
			tube[p + 3] = num;
			setPuzzle(p, p + 3);
		}
	}
}


int main() {
	
	srand(time(NULL));	//랜덤값 초기화
	setMouseCallback(mouseCallback);

	scene1 = createScene("귀요미 튜브 퍼즐", "튜브 원본.png");
	///////////처음장면/////////////////////////////////
	startButton = createObject("시작버튼", "start.png", scene1, 950, 300, true, 2.0f);

	
	scene2 = createScene("귀요미 튜브 퍼즐", "튜브 배경.png");
	///////////퍼즐장면/////////////////////////////////
	resetButton = createObject("리셋 버튼", "reset.png", scene2, 950, 420, true, 2.0f);
	newgameButton = createObject("뉴게임 버튼", "new game.png", scene2, 950, 320, true, 2.0f);
	clearButton = createObject("클리어 버튼", "clear.png", scene2, 950, 220, true, 2.0f);
	

	//튜브 그림 조각 생성
	for (int i = 0; i < 9; i++)
		puzzle[i] = createObject("퍼즐", "튜브.png", scene2, X[i], Y[i], true, 1.0f);


	scene3 = createScene("걸린 시간은 옆에!", "튜브 원본.png");
	////////클리어장면//////////////////////////////////////////
	endButton = createObject("종료 버튼", "end.png", scene3, 950, 250, true, 2.0f);
	restartButton = createObject("재시작 버튼", "restart.png", scene3, 950, 350, true, 2.0f);


	timer = createTimer(600);
	showTimer(timer);
	

	startGame(scene1);
}