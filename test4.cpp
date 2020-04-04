//���� �Ʒ� (381,120) ��ĭ 149

//���� �ð� ǥ��� showMessage�� �� �� ��� Ŭ����� Ÿ�̸ӿ� ǥ��ǰ��߽��ϴ�!

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

int tube[9]; // tube[n] = t : n��ĭ���� t�� Ʃ�� �׸��� �� ����
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

//n��ĭ �׸��� t�� Ʃ�� �׸����� �ٲ��ֱ�
void setPuzzleImage(ObjectID nPuzzle, int t) {

	if (t == 0)
		setObjectImage(nPuzzle, "Ʃ��_0.png");
	else if (t == 1)
		setObjectImage(nPuzzle, "Ʃ��_1.png");
	else if (t == 2)
		setObjectImage(nPuzzle, "Ʃ��_2.png");
	else if (t == 3)
		setObjectImage(nPuzzle, "Ʃ��_3.png");
	else if (t == 4)
		setObjectImage(nPuzzle, "Ʃ��_4.png");
	else if (t == 5)
		setObjectImage(nPuzzle, "Ʃ��_5.png");
	else if (t == 6)
		setObjectImage(nPuzzle, "Ʃ��_6.png");
	else if (t == 7)
		setObjectImage(nPuzzle, "Ʃ��_7.png");
	else if (t == 8)
		setObjectImage(nPuzzle, "��ĭ.png");
}

void random() {

	//tube[9]�� ������ �迭 �ֱ�
	for (int i = 0; i < 9; i++) {
		tube[i] = rand() % 9;

		for (int j = 0; j < i; j++) {
			if (tube[i] == tube[j])
				i--;
		}
	}

	//���� �迭�� �����صα�
	for (int i = 0; i < 9; i++)
		reset[i] = tube[i];

}

//aĭ���� bĭ���� tube���� �°� �̹��� �缳��
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

	//���۹�ư ������ ���� ȭ�� �Ѿ�� ���� �����ֱ�
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
			showMessage("������ �ϼ����� �ʾҽ��ϴ�");

		else 
			clear();

	}

	else if (object == endButton)
		endGame();

	//���� ���� ���� ���
	else
	{
		//object�� - 5 == puzzle[p]�� p��
		p = object - 5;

		//���� �巡�� : 0,3,6ĭ �ƴ� ��� + �巡���� ĭ�� ��ĭ�� ���
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
	
	srand(time(NULL));	//������ �ʱ�ȭ
	setMouseCallback(mouseCallback);

	scene1 = createScene("�Ϳ�� Ʃ�� ����", "Ʃ�� ����.png");
	///////////ó�����/////////////////////////////////
	startButton = createObject("���۹�ư", "start.png", scene1, 950, 300, true, 2.0f);

	
	scene2 = createScene("�Ϳ�� Ʃ�� ����", "Ʃ�� ���.png");
	///////////�������/////////////////////////////////
	resetButton = createObject("���� ��ư", "reset.png", scene2, 950, 420, true, 2.0f);
	newgameButton = createObject("������ ��ư", "new game.png", scene2, 950, 320, true, 2.0f);
	clearButton = createObject("Ŭ���� ��ư", "clear.png", scene2, 950, 220, true, 2.0f);
	

	//Ʃ�� �׸� ���� ����
	for (int i = 0; i < 9; i++)
		puzzle[i] = createObject("����", "Ʃ��.png", scene2, X[i], Y[i], true, 1.0f);


	scene3 = createScene("�ɸ� �ð��� ����!", "Ʃ�� ����.png");
	////////Ŭ�������//////////////////////////////////////////
	endButton = createObject("���� ��ư", "end.png", scene3, 950, 250, true, 2.0f);
	restartButton = createObject("����� ��ư", "restart.png", scene3, 950, 350, true, 2.0f);


	timer = createTimer(600);
	showTimer(timer);
	

	startGame(scene1);
}