#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Kkutu.h"


bool enter = false;
const int FRAMES_PER_SECOND = 240;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
int sleep_time = 0;


int main() {
	
	setcursortype( NOCURSOR );
	srand( (unsigned)time(NULL) );
	
	SetGameInfo();
	DB_Write();
	
	int timeTurn, timeRound;
	int timeTurn_Init;
	
	char ch = NULL;
	char chStart = NULL;
	char str_[256]={NULL};
	char *strRound = Round_Str( GameInfo.numRound );
	

	
	
	for( int Round=1 ; Round<=GameInfo.numRound ; Round++ ) {
		
		//GameInfo.word_Used = (char**)calloc( GameInfo.timeRound*10*2/1000, sizeof(UsedString*) );
		//for( int i=0 ; i<GameInfo.timeRound*10*2/1000 ; i++ ) GameInfo.word_Used[i] = (char*)calloc( 60, sizeof(char) );
		UsedString *root = NULL;
			
		timeTurn = GameInfo.timeTurn;
		timeRound = GameInfo.timeRound;
		GameInfo.chain = 0;
		chStart = strRound[Round-1];
		GameInfo.mission = rand()%26+97;													//initial random lowercase mission
			
		gotoxy(40-strlen( strRound )+1, 5 );
				
		for( int i=0 ; i<strlen(strRound) ; i++ ) {
					
			if( Round==i+1 ) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_YELLOW );
			else SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
			printf( "%c ", strRound[i] );
		}
		int cnt=0;
		do {
			 
				
			SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
				
				
			gotoxy( 8, 7 );  printf("¦£"); for( int n = 60 ; n>=0 ; n-- ) printf("¦¡"); printf("¦¤");
			gotoxy( 8, 8 );  printf("¦¢"); for( int n = 60 ; n>=0 ; n-- ) printf(" ");  printf("¦¢");
			gotoxy( 8, 9 );  printf("¦§"); for( int n = 60 ; n>=0 ; n-- ) printf("¦¡"); printf("¦©");
			gotoxy( 8, 10 ); printf("¦¢"); for( int n = 60 ; n>=0 ; n-- ) printf(" ");  printf("¦¢");
			gotoxy( 8, 11 ); printf("¦¦"); for( int n = 60 ; n>=0 ; n-- ) printf("¦¡"); printf("¦¥");
				
			timeTurn_Init = timeTurn = timeRound/5;
				
			gotoxy( 40, 8 ); printm( str_, GameInfo.mission );
			Sleep(timeTurn/10);
				
			if( strchr( str_, GameInfo.mission )!=NULL ) GameInfo.mission = rand()%26+97;		//update random lowercase mission
				
				
			SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
				
			gotoxy( 80, 5 ); printf( "MISSION" );
			gotoxy( 80, 7 ); printf( "%c", GameInfo.mission );
			gotoxy( 80, 9 ); printf( "CHAIN" );
			gotoxy( 80, 11 ); printf( "%d", GameInfo.chain );
			gotoxy( 40, 20 ); printf( "PLAYER : %d      ", GameInfo.score[0] );
			gotoxy( 40, 22 ); printf( "   BOT : %d      ", GameInfo.score[1] );
								
			gotoxy( 40-60/2, 8 ); for( int i=0 ; i<60 ; i++ ) printf(" ");					// erase
			gotoxy( 40-60/2, 10 ); for( int i=0 ; i<60 ; i++ ) printf(" ");
				
			if( str_[0]!=NULL ) chStart = charLast( str_);
			gotoxy(40,8); putch(chStart);
			if( str_[0]==NULL ) Sleep(2000);												//breaktime when round starts
			else strset( str_, NULL );
				
				
			DWORD next_game_tick = GetTickCount();
		
			for( int i=0 ; enter==false ;  ) {
				
				if( GameInfo.chain%2==1 ) {														//BOT Turn
					
					timeTurn -= 100;
					Sleep(100);
					strcpy( str_, FindStr_mission( &root, chStart, GameInfo.mission ) );
					enter = true;
				}
				
				if( timeTurn<=0 ) {
					
					strset( str_, NULL ); break;
				}

				if(kbhit()) {
					
					ch = getch();
					
					
					gotoxy(10,12); printf("                   "); for( int j=0 ; j<60 ; j++ ) putch(' ');
						
					if( ch==KEY_ENTER ) {
							
						enter = true; printf("                   ");
					}
					else if( ch==KEY_BACKSPACE ) {
							
						if(i>0) i--; str_[i] = NULL; i--; printf("                   ");
					}
					else {
							
						if( i>=60 ) {
					
							printf("Reached max length.");
							continue;
						}
						
						( islower(ch) ) ? str_[i] = ch : i--;
					}
					
					i++;
				}
				
				gotoxy(40-60/2,10);
				printf("%s ", str_ );
				
				if( enter==true ) {
					
					
					if( str_[0]!=chStart ) {
					
						i=0; gotoxy(40-60/2, 10 );
						for( int j=0 ; j<strlen( str_ ) ; j++ ) putch(' ');
						strset( str_, NULL ); enter = false;
					
					}
					
					else if( IsExist(str_)==false ) {
						
						i=0; gotoxy(40-60/2, 10 );
						for( int j=0 ; j<strlen( str_ ) ; j++ ) putch(' ');
						SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_ORANGE );
						gotoxy(10,12); printf("Nonexistent word. : %s",str_);
						strset( str_, NULL ); enter = false;
						SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
					
					}
					
					else if(( Add_UsedString( &root, str_ )==true )) {
						
						i=0; gotoxy(40-60/2, 10 );
						for( int j=0 ; j<strlen( str_ ) ; j++ ) putch(' ');
						SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_ORANGE );
						gotoxy(10,12); printf("Already used word. : %s",str_);
						strset( str_, NULL ); enter = false;
						SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
					
					}
					
				
				}
				
				
				next_game_tick += SKIP_TICKS;
	   			sleep_time = next_game_tick - GetTickCount();				// calculate time
				timeRound -= SKIP_TICKS;
				timeTurn -= SKIP_TICKS;
				
				if(sleep_time>=0) Sleep(sleep_time);
			
				gotoxy(80,15);
				printf( "%-5.1f" , timeTurn/1000.0);						// print times
				gotoxy(80,16);
				printf( "%-5.1f" , timeRound/1000.0);
			
			}
			
			GameInfo.score[ GameInfo.chain%2 ] += CalcScore( strlen(str_), GameInfo.chain, timeTurn_Init-timeTurn, timeTurn_Init, CheckMission( str_, GameInfo.mission ) );
			if( GameInfo.score[GameInfo.chain%2]<0 ) GameInfo.score[GameInfo.chain%2]=0;
			GameInfo.chain++;
			enter = false;
				
		} while( timeTurn>0 );

		Delete_UsedString( root );
	}
	
}
			










#if 0

int main() {
	
	char *word_DB[NUM_WORDS];
	char str[256];
	
	system("mode con cols=150 lines=40"); 
	
	DB_Write( word_DB );
	
	
	
	puts(FindStr_mission(word_DB, 'n','z'));
	
	
	for( int i=0 ; i<NUM_WORDS ; i++ ) free(word_DB[i]);

	printf("ASD");
	
	getch();
}

#endif



#if 0

int main(){
	
	char ***word_DB;
	DB_Scan(word_DB);
	puts(word_DB[1][1]);
}

#endif
