#define SWAP( a, b ) { const char *c=a; a=b; b=c; }


const int NUM_WORDS = 82176;
const int COLOR_GREEN = 10;
const int COLOR_WHITE = 7;
const int COLOR_GRAY = 8;
const int COLOR_YELLOW = 6;
const int COLOR_ORANGE = 12;

const int KEY_ENTER=13;
const int KEY_BACKSPACE=8;

const int UP = 72;
const int DOWN = 80;

FILE *DB = fopen( "kkutu.txt" , "rt" );

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;


typedef struct _UsedString{
	char data[256];
	_UsedString* nextNode;
} UsedString;


struct {
	
	int timeRound=0;
	int timeTurn=0;
	int chain=0;
	int numRound=0;
	int score[2]={0,0};
	char mission=NULL;
	char *word_DB[NUM_WORDS] = {NULL};
	//char **word_Used;
	int timeList[5] = { 10, 30, 60, 90, 150 };
	
} GameInfo;



bool Add_UsedString(UsedString** root, char *data);			// 
void Print_UsedString(UsedString* node);
void Delete_UsedString(UsedString* node);

void SetGameInfo();

void Sort( const char ** );
void DB_Write( char *[NUM_WORDS] );
bool IsInvalid( const char* );
int CheckMission( const char*, char alp );
bool IsAttack( char );
char charLast( char* );
char *FindStr_mission( char start, char mission );
void printm( const char* );
int whereX();
int whereY();
void gotoxy( int x, int y );
void setcursortype( CURSOR_TYPE );
bool IsExist();
bool IsUsed( UsedString **root, char* );


void SetGameInfo() {
	
	char ch=NULL;
	int x,y;
	
	gotoxy(35,10);
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
	printf( "Round : %d", GameInfo.numRound );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GRAY );
	gotoxy(35,12);
	printf( "Ti me : %d", GameInfo.timeRound );
	gotoxy(35,14);
	printf( "Default" );
	
	gotoxy(35,10);
	
	for( ; GameInfo.numRound==0 || GameInfo.timeRound==0 ; ) {
	
		ch=getch();
	
		switch( ch ) {
			
			case KEY_ENTER :
				
				switch( whereY() ) {
				
					case 10 :
						
						gotoxy(35,11);
						printf("Round: ");
						scanf("%d",&GameInfo.numRound);
						
						if( GameInfo.numRound<1 || GameInfo.numRound>10 ) {
							
							GameInfo.numRound=0;
							gotoxy(15,11);
							printf("1 <= Round <= 10");
							getch();
							gotoxy(15,11);
						}
						
						gotoxy(35,10);
						
						break;
						
					case 12 :
						
						gotoxy(35,13);
						
						do {
							
						y = whereY();
						
							for( int i=0 ; i<( sizeof(GameInfo.timeList)/sizeof(int) ) ; i++ ) {
								
								if( y==(13+i) ) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
								gotoxy( 35, 13+i ); printf( "   %d  ", GameInfo.timeList[i] );
								SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GRAY );
								
							}
							gotoxy(35,y);
								
						 	ch=getch();
						 	if(ch==(-32)){
						 		ch=getch();
						 		if(ch==UP && y>13) gotoxy(35, y-1);
						 		else if(ch==DOWN && ( y < 13+sizeof(GameInfo.timeList)/sizeof(int)-1 ) ) gotoxy( 35, y+1 );
							 }
							//if( ch==-32 && getch()==UP && y>13 ) gotoxy( 35, y-1 );
							//else if( getch()==DOWN && ( y < 13+sizeof(GameInfo.timeList)/sizeof(int)-1 ) ) gotoxy( 35, y+1 );	
							else if( ch==KEY_ENTER ) GameInfo.timeRound = GameInfo.timeList[ y-13 ]*1000;
							
						} while( ch!=KEY_ENTER );
						
						
						for( int i=0 ; i<( sizeof(GameInfo.timeList)/sizeof(int) ) ; i++ ) {
							
							gotoxy(35,13+i); printf("       ");	
							}
						gotoxy(35,12);
						
						break;
						
					case 14 :
						
						GameInfo.timeRound = 60*1000; 
						GameInfo.numRound = 5;
						GameInfo.timeTurn = GameInfo.timeRound/5;
						
				}
				
			break;
			
			case -32:
				switch(getch()){
					case UP :
						if( whereY()>10 ) gotoxy( 35, whereY()-2 );
						break;
			
					case DOWN : 
						if( whereY()<14 ) gotoxy( 35, whereY()+2 );
						break;
						
				}
				break;
			case 'w':
				if( whereY()>10 ) gotoxy( 35, whereY()-2 );
				printf("\a");
				break;
			case 's':
				if( whereY()<14 ) gotoxy( 35, whereY()+2 );
				break;
				
		}
		
		x=whereX(), y=whereY();
		
		system("cls");
		
		gotoxy(x,y);
		
		
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GRAY );
		if( y==10 ) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
		gotoxy(35,10);
		printf( "Round : %d", GameInfo.numRound );
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GRAY );
		if( y==12 ) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
		gotoxy(35,12);
		printf( "Ti me : %d", GameInfo.timeRound/1000 );
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GRAY );
		if( y==14 ) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
		gotoxy(35,14);
		printf( "Default" );
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
		gotoxy(x,y);
			
			
	}
	
	system("cls");
}


void Sort( const char **str_bundle ) {
   
   int min;
   
   for( int i=0 ; str_bundle[i+1]!=NULL ; i++ ) {
         
         min=i;
         
      for( int j=i ; str_bundle[j]!=NULL ; j++ ) if( strcmp( str_bundle[j] , str_bundle[min] ) < 0 ) min=j;
      
      if( str_bundle[min]!=str_bundle[i] ) SWAP( str_bundle[min], str_bundle[i] );
  }
}


void DB_Write() {


	char buf[256] = {NULL};

	if( DB!=NULL ) {
		
			for( int i=0 ; i<NUM_WORDS ; i++ ) {
				
				fgets( buf , 256 , DB );
			
				buf[strlen(buf)-1] = NULL;
				
				GameInfo.word_DB[i] = (char*) malloc( strlen(buf) * sizeof(char) +1 );
				strcpy( GameInfo.word_DB[i] , buf );
	
			}
			
		fclose(DB);
		

	}
	
	else {
		
		printf("File Not Found.");
		Sleep(2000);
		exit(0);
	}
}


/*	bool IsUsed( const char* str ) {
	
	static const char *word_USED[NUM_WORDS];
	Sort(word_USED);
	
	for( int i=0 ; word_USED!=NULL ; i++ ) if( strcmp(str,word_USED[i])==0 ) return 1;
	
	return 0;
}	*/


int CheckMission( const char *str , char alpha ) {
	
	int cnt=0;
	for( int i=0 ; i<strlen(str) ; i++ ) if( str[i]==alpha ) cnt++;

	return cnt;
	}


bool IsAttack( char ch ) {
	
	if( ch=='j' || ch=='q' || ch=='v' || ch=='x' || ch=='z' ) return 1;
	else return 0;
}


char charLast( char *str ) {
	
	return str[strlen(str)-1];
}


char* FindStr_mission( UsedString **root, char alpStart, char alpMission ) {
	
	char *strBest, *str;
	strBest = str = NULL;
	
	int i;
	for( i=0 ; i<NUM_WORDS ; i++ ) if( GameInfo.word_DB[i][0]==alpStart ) break;
	
	for( int nMax=0 ; GameInfo.word_DB[i]!=NULL && GameInfo.word_DB[i][0]==alpStart ; i++ ) {
		
		if( IsUsed( root, GameInfo.word_DB[i] )==true ) continue;					// 해당 단어가 중복된 단어인지 검사 
		
		if( strBest==NULL ) {			// 아직까지의 최선의 단어가 없을 경우, 일단 현재 선택된 문자열을 최선의 단어로 선정 
			
		strBest=GameInfo.word_DB[i];	continue;
		}
		
		int nMission = CheckMission( GameInfo.word_DB[i], alpMission );
		
		if( nMission > nMax ) {			// 미션 알파벳의 수가 더 많을 경우, 그 단어를 최선의 단어로 선정 
			
			nMax = nMission;
			strBest = GameInfo.word_DB[i];
		}
		
		else if( nMission == nMax ) {		// 여기부턴 아래 적힌 우선순위 3,4,5 순서대로 쓰까놓음 
			
			if( strlen(GameInfo.word_DB[i])>strlen(strBest)+2 ) strBest=GameInfo.word_DB[i];
			else if( strlen(GameInfo.word_DB[i])>strlen(strBest) && IsAttack(charLast(strBest))==false ) strBest=GameInfo.word_DB[i];
			else if( strlen(GameInfo.word_DB[i])==strlen(strBest) && charLast(GameInfo.word_DB[i])=='y' ) strBest=GameInfo.word_DB[i];
			else if( strlen(GameInfo.word_DB[i])+2>=strlen(strBest) && IsAttack(charLast(strBest))==false && IsAttack(charLast(GameInfo.word_DB[i]))==true ) strBest=GameInfo.word_DB[i];
		}
	}
	
		return strBest;
		
		
		/* 우선순위 :
	1. 썼던 단어가 아님 
	2. 미션을 많이먹음
	3. 길이가 긺
	4. j,q,v,x,z 로 끝남 (공격단어) 
	5. y로 끝남  (준공격단어) 
	3,4는 길이가 비슷하면 4가 우선
	 
	예 ) HJ : hajj ( hajji가 더 길지만 한 글자 차이이다) */ 
}


void printm( const char *str , char alpha ) {
	
	if( strlen(str)<9 ) {
		
		gotoxy( whereX()-strlen(str)+1,whereY() );
		
		for( int i=0 ; i<strlen(str) ; i++ ) {
			
		if( str[i]==alpha ) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GREEN );

		
		if( i>0 ) putch(' ');
		putch(str[i]);
		if( GameInfo.timeRound/strlen(str)/70 ) Sleep( GameInfo.timeRound/strlen(str)/70 );
			
			SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
	}
	}
	
	else {
		
		for( int i=0 ; i<strlen(str) ; i++ ) {
			
			gotoxy( whereX()-i, whereY() );
			
			if( i%2==1 ) {
				
				for( int k=0 ; k<i ; k++ ) putch(' ');
				gotoxy( whereX()-(i+1) , whereY() );
			}
			
			
			for( int j=0 ; j<=i ; j++ ) {
				
				if( str[j]==alpha ) SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GREEN );
				putch(str[j]);
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_WHITE );
			}	
			
			if( GameInfo.timeRound/strlen(str)/100 ) Sleep( GameInfo.timeRound/strlen(str)/100 );

		}
	}
			
}


int whereX() {

     CONSOLE_SCREEN_BUFFER_INFO BufInfo;

     GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE) , &BufInfo );

     return BufInfo.dwCursorPosition.X;
}


int whereY() {

     CONSOLE_SCREEN_BUFFER_INFO BufInfo;

     GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE) , &BufInfo );

     return BufInfo.dwCursorPosition.Y;
}


void gotoxy( int x, int y ){
	
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE) , Cur );
}


void kkutu_display(){
	
	gotoxy(2,7);
	printf("MISSION");
	gotoxy( 2+strlen("MISSION")+50/2 ,7 );
}


void setcursortype( CURSOR_TYPE c ) {

     CONSOLE_CURSOR_INFO CurInfo;
     
     switch (c) {

     case NOCURSOR:

          CurInfo.dwSize=1;
          CurInfo.bVisible=FALSE;
          break;

     case SOLIDCURSOR:

          CurInfo.dwSize=100;
          CurInfo.bVisible=TRUE;
          break;

     case NORMALCURSOR:

          CurInfo.dwSize=20;
          CurInfo.bVisible=TRUE;
          break;

     }

     SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}


char* Round_Str( int numRound ) {
	
	char *str = (char*)malloc( 256*sizeof(char) );
	
	do {
		
		str = GameInfo.word_DB[rand()%NUM_WORDS];
	} while( strlen(str)!=numRound );
	
	return str;
}


int CalcScore( int L, int c, int d, int t, int m ) {
	
	if( -1*d+t>0 )
	return 2 * ( pow( 5+7*L, 0.74 ) + 0.88*c ) * ( 0.5 + 0.5*( 1-d/t ) ) * ( 1 + 0.5*m );
	
	else return -1 * ( 10+2.1*c + 0.15*GameInfo.score[c%2] );
}





bool Add_UsedString(UsedString** root, char *data);			// 
void Print_UsedString(UsedString* node);
void Delete_UsedString(UsedString* node);


/* ------------------------------------------------------------- 

int main(){
	UsedString* root = NULL;
	char inputStr[50];
	
	while(1){
		if(root != NULL){
			printf("--------------------------------현재까지 입력된 단어 list--------------------------------\n\n");
			Print_UsedString(root);
			sleep(2);
			system("cls");
		}
		
		printf("문자열 입력 : ");
		scanf("%s", inputStr);
		
		if(!Add_UsedString(&root, inputStr)) break;
		sleep(1);
		system("cls");
	}
	
	printf("저장된 데이터를 모두 삭제합니다.\n");
	Delete_UsedString(root);
	
	return 0;
}

 ------------------------------------------------------------- */


bool IsUsed( UsedString **root, char *data ) {
	
	UsedString *searchNode = *root;
	
	// printf("[%s], ", data);	// 디버깅용 1 
	
	while( searchNode ) {
		// printf("%s, ", searchNode->data); 	// 디버깅용2 
		if( strcmp( searchNode->data, data ) == 0 ) {
			// printf("\n");	// 디버깅용3 
			return true;
		}
		searchNode = searchNode->nextNode;
	}
	//for( int i=0 ; GameInfo.word_Used[i] ; i++ ) {
	//	if( strcmp( GameInfo.word_Used[i], data ) == 0 ) {
	//		return true;
	//	}
	//}
	// printf("\n");	// 디버깅용4 
	return false;
	
}
	



bool Add_UsedString(UsedString** root, char *data){
	UsedString* 	addNode = (UsedString*)malloc(sizeof(UsedString));
	UsedString* 	searchNode = *root;
	UsedString* 	followNode = NULL;
	
	strcpy(addNode->data, data);
	addNode->nextNode = NULL;
	
	if(*root == NULL){
		*root = addNode;
		return false;
	}
	
	while(searchNode != NULL){
	//	printf("%d\n", strcmp(data, searchNode->data));
		
		switch(strcmp(data, searchNode->data)){
			case 0:		// 단어가 중복되었을 경우 -> addNode를 해제시키고 true를 리턴 
				free(addNode);
				return true;
			case 1:		// 사전순으로 저장될 위치를 찾은 경우 -> 적절한 위치에 저장하고 false를 리턴 
				addNode->nextNode = searchNode;
				if(followNode != NULL) followNode->nextNode = addNode;
				if(searchNode == *root) *root = addNode;
		//		printf("%s 추가 완료!\n", addNode->data); 
				return false;
			case -1:		// 적절한 위치를 찾지 못한 경우 -> 루프를 계속 돔 
				followNode = searchNode;
				searchNode = searchNode->nextNode;
		}
	}
	followNode = addNode;		// 마지막까지 적절한 위치를 찾지 못한 경우, 마지막 위치에 저장 
	return false;
}

void Print_UsedString(UsedString* node){
	while(node != NULL){
		printf("%s\n", node->data);
		node = node->nextNode;
	}
}

void Delete_UsedString(UsedString* root){
	UsedString*	searchNode = root;
	UsedString* followNode = NULL;
	
	while(searchNode != NULL){
	//	printf("\n\n%s 삭제 완료\n", searchNode->data);
		followNode = searchNode;
		searchNode = searchNode->nextNode;
		free(followNode);
	}
}


bool IsExist( char* str ) {
	
	for( int i=0 ; i<NUM_WORDS ; i++ ) {
		
		if( strcmp( GameInfo.word_DB[i], str )==0 ) return true;
	}
	return false;
}



#if 0

void DB_Scan(char ***GameInfo.word_DB) {
	
	FILE *DB;
	DB = fopen("Kkutu.txt","rt");
	
	char buf[256] = {NULL};
	int cntRealloc = 0;


	if( DB!=NULL ) {
		
		printf("Scanning DB, Please Wait.");
		
	GameInfo.word_DB = (char***)malloc( 26*sizeof(char**) );
	for( int i=0 ; i<26 ; i++ ) GameInfo.word_DB[i] = (char**)malloc( 500*sizeof(char*) );
		
				for( int i=0, j=0 ; feof(DB)==false ; j++ ) {
					
					fgets( buf , 256 , DB );
					
					buf[strlen(buf)-1] = NULL;
					
					if( islower(buf[0])==0 ) break;
					
					if( buf[0] > 'a'+i ){
						
						i += ( buf[0] - 'a'+i );
						j=0;
					}
					
					if(j==0) cntRealloc=0;
					
					if( j >= 500*(cntRealloc+1) ) {
		
						cntRealloc++;
						for( int k=0 ; k<26 ; k++ ) GameInfo.word_DB[i] = (char**)realloc( GameInfo.word_DB[i] , (j+500)*sizeof(char*) );
				}
				
				
					GameInfo.word_DB[i][j] = (char*)malloc( strlen(buf)*sizeof(char)+1);
					GameInfo.word_DB[i][j] = buf;
					
			}
			
		fclose(DB);
	}
	
	else printf("File Not Found.");
}

#endif



