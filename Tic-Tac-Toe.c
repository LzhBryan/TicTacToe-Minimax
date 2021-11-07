#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Windows.h>

#define SIZE 9
#define AI_player 'X'
#define AI_computer 'O'

void instruction();
void generate_map(char *game_mark);
void computer_move(char *game_mark, char computer_mark, char player_mark, int cells, int AI);
void player_move(char *game_mark, char computer_mark, char player_mark, int cells, int AI);
void check_cells(int cells, char *game_mark, int *is_occupied);
int detect_win(char *game_mark, char computer_mark, char player_mark);
int is_win(int *computer_score, int *player_score);
int minimax(char *game_mark, int cells, int *c_move, int is_Min, int alpha, int beta);
int best_value(int arr[], int size, int is_Max);


int main(void){
	printf("-----------Tic-Tac-Toe Program----------\n\n");
	instruction();

	// randomize the first player
	srand(time(0));
	int initial_move = rand()%2;
	char game_mark[9], player_mark, computer_mark;
	
	// initialize array to all space characters
	for(int i=0; i<SIZE; i++)
		game_mark[i] = ' ';
	
	int AI;
	printf("Type 0 for easy mode and type 1 for difficult mode\n");
	scanf("%d", &AI);

	if (AI){
		printf("The player will make the first move in difficult mode.\n");
		generate_map(game_mark);
		player_move(game_mark, AI_computer, AI_player, SIZE, AI);
	}

	else{
		printf("The first player to start will be randomly decided in easy mode.\n\n");
		if (initial_move){
			printf("The player starts first\n");
			player_mark = 'X';
			computer_mark = 'O';
			generate_map(game_mark);
			player_move(game_mark, computer_mark, player_mark, SIZE, AI);
		}
		
		else{
			printf("The computer starts first\n");
			computer_mark = 'X';
			player_mark = 'O';
			computer_move(game_mark, computer_mark, player_mark, SIZE, AI);		
		}
	}

	return 0;					
}

void instruction(){
	char instruction_mark[9];

	printf("*****INSTRUCTION*****");
	printf("This program has two difficulty mode. ( Easy mode and diffcult mode )\n");
	Sleep(1);
	printf("When playing the game, enter the cell number to place your mark.\n");
	Sleep(2);

	for (int i=0; i<SIZE; i++)
		instruction_mark[i] = i;

	printf("Below shows the cell numbers.\n");
	generate_map(instruction_mark);

	Sleep(5);
	printf("After entering the number, press ENTER to proceed.\n");
	printf("*****NOTE*****\n");
	Sleep(1);
	printf("First to play is randomized in easy mode.\n");
	Sleep(1);
	printf("But user will play first in difficult mode.\n");
}

void generate_map(char *game_mark){
	
	printf("\n");
	printf("\t  %c  |  %c  |  %c  \n",*(game_mark+0),*(game_mark+1),*(game_mark+2));
	printf("\t-----|-----|-----\n");
	printf("\t  %c  |  %c  |  %c \n",*(game_mark+3),*(game_mark+4),*(game_mark+5));
	printf("\t-----|-----|-----\n");
	printf("\t  %c  |  %c  |  %c \n",*(game_mark+6),*(game_mark+7),*(game_mark+8));
	printf("\n");
}

void computer_move(char *game_mark, char computer_mark, char player_mark, int cells, int AI){
	
	if(cells == 0)
		exit(0);

	printf("It is the computer's turn now.\n");

	if(AI && cells == 4){
		char *copy_mark = (char *)malloc(SIZE);
		memcpy(copy_mark,game_mark,SIZE);
		int c_move = -1;
		minimax(copy_mark, cells, &c_move, 0, -1000, 1000);
		*(game_mark + c_move) = computer_mark;
	}

	else{

		int is_occupied[cells];
		check_cells(cells, game_mark, is_occupied);

		// randomize the move
		srand(time(0));
		
		//assign the mark to corresponding cell
		*(game_mark + is_occupied[rand()%cells]) = computer_mark;
	}

	generate_map(game_mark);
	if(detect_win(game_mark, computer_mark, player_mark) == -1){
		exit(0);
	}

	else
		player_move(game_mark, computer_mark, player_mark, cells-1, AI);
}

void player_move(char *game_mark, char computer_mark, char player_mark, int cells, int AI){

	if (cells == 0)
		exit(0);

	int is_occupied[cells], player_cell;
	check_cells(cells, game_mark, is_occupied);
	
	printf("It is the player's turn now.\n");
	printf("Choose a cell to place your mark: ");
	scanf("%d",&player_cell);

	// exception handling of scanf **

	--player_cell;

	// validation of user input
	for(int i=0; i<cells; i++){
		if(player_cell == is_occupied[i])
			*(game_mark + player_cell) = player_mark;

		// else statement **
	}	
	
	generate_map(game_mark);
	if(detect_win(game_mark, computer_mark, player_mark))
		exit(0);
	else
		computer_move(game_mark, computer_mark, player_mark, cells-1, AI);
}

void check_cells(int cells, char *game_mark, int *is_occupied){

	// obtain cells that are available
	for(int i=0;i<SIZE;i++){
		if ( *(game_mark+i) == ' ' ){
			*(is_occupied) = i;
			is_occupied++;
		}
	}
}

int detect_win(char *game_mark, char computer_mark, char player_mark){

	int computer_score=0, player_score=0;

    // detect row
    for(int i=0; i<SIZE; i+=3){
        for(int j=i; j<i+3; j++){
            if (*(game_mark+j) == computer_mark)
                computer_score++;
            
            else if (*(game_mark+j) == player_mark)
                player_score++;
        }
		if (is_win(&computer_score, &player_score) == 2)
			return 1;

		else if	(is_win(&computer_score, &player_score))
			return -1;
		
	}

	// detect column
    for(int j=0; j<3; j++){
        for(int i=j; i<SIZE; i+=3){
            if (*(game_mark+i) == computer_mark)
                computer_score++;
            
            else if (*(game_mark+i) == player_mark)
                player_score++;
        }

        if (is_win(&computer_score, &player_score) == 2)
			return 1;

		else if	(is_win(&computer_score, &player_score))
			return -1;


    }

	// detect right diagonal
    for(int i=0; i<SIZE; i+=4){
        if (*(game_mark+i) == computer_mark)
                computer_score++;
            
        else if (*(game_mark+i) == player_mark)
                player_score++;
    }

	if (is_win(&computer_score, &player_score) == 2)
		return 1;

	else if	(is_win(&computer_score, &player_score))
		return -1;
	


	//detect left diagonal
    for(int i=2; i<SIZE-2; i+=2){
        if (*(game_mark+i) == computer_mark)
                computer_score++;
            
        else if (*(game_mark+i) == player_mark)
                player_score++;
    }

	if (is_win(&computer_score, &player_score) == 2)
		return 1;

	else if	(is_win(&computer_score, &player_score))
		return -1;

	return 0;
}

int is_win(int *computer_score, int *player_score){

    if (*computer_score == 3)
        return 1;
        
    else if (*player_score == 3)
		return 2;

    else {
        *computer_score = 0;
        *player_score = 0;
		return 0;
    }
}

int minimax(char *game_mark, int cells, int *c_move, int is_Max, int alpha, int beta){

	int is_occupied[cells];
	check_cells(cells, game_mark, is_occupied);
	int is_win = detect_win(game_mark, AI_computer, AI_player);

	// return scores if reach terminal nodes or win condition
	if (sizeof(is_occupied) == 0 || is_win != 0)
		return is_win;

	int value[cells];

	// (breath-first search)
	for(int i=0; i<cells; i++){

		// make a copy of the states for each horizontal nodes (breath-first search)
		char *ptr = (char *)malloc(sizeof(cells));
		memcpy(ptr, game_mark, SIZE);

		// choose first possible move and update game and vertically extend the nodes (depth-first search)
		if(is_Max){
			*(ptr + is_occupied[i]) = AI_player;
			printf("Max game %d at depth %d\n", i, cells);
            generate_map(ptr);
			value[i] = minimax(ptr, cells-1, c_move, 0, alpha, beta);
			if(beta <= alpha)
				break;
		}

		else{
			*(ptr + is_occupied[i]) = AI_computer;
			printf("Min game %d at depth %d\n", i, cells);
            generate_map(ptr);
			value[i] = minimax(ptr, cells-1, c_move, 1, alpha, beta);
			if (beta <= alpha)
				break;
		}
	}

	if(is_Max){
		int index = best_value(value, cells, 0);
		alpha = value[index];
		printf("alpha %d in MAX game at depth %d\n", alpha, cells);
		return value[index];
	}

	else{
		int index = best_value(value, cells, 1);
		*c_move = is_occupied[index];
		beta = value[index];
		printf("beta %d in Min game at depth %d\n", beta, cells);
		return value[index];
	}	
}

int best_value(int arr[], int size, int is_Min){

	int index;

	if(is_Min){
		int min = 1000;
		for(int i=0; i<size; i++){
			if(arr[i] < min){
				min = arr[i];
				index = i;
			}
		}
	}
	
	else{
		int max = -1000;
		for(int i=0; i<size; i++){
			if(arr[i] > max){
				max = arr[i];
				index = i;
			}
		}
	}
	
	return index;
}
