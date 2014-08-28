/*
Project: Chess Playing Robot
Sponsored by: Robotics Club IIT Kanpur
*/
/*

CPRv1.3 version is the first code with simplest artificial intelligence.
Got completed on 11:59 pm 14th June 2014.

CPRv1.4 version has implemented search algorithms, i.e., minimax but 
still alpha beta pruning has not been implemented yet.
The fault in pawn moves [4x6xW and 3x1xB move checked] improved.
Now, as per my knowledge everything is correct. Feeling happy after getting defeated!! Really
Special thanks to SAUMYA, AMIT, SHILPI

**CPRv1.5**  20th June 2014, 10:15pm
This version is new becuase of using alpha beta algorithms which makes the searching
faster. It is really very smart.
Still there is space for improvements:-
1. Flagging the event of loss of game
2. Using historical moves initially instead of searching
3. Improving GUI
*/
#include<stdio.h>
#include<iostream.h>
#include<process.h>
#include<math.h>
#include<conio.h>

const int pval=10;
const int nval=31;
const int bval=33;
const int rval=50;
const int qval=79;
const int kval=10000;
int process=0;
struct square
{char piece;
char piece_color;
int value;
};
typedef square square;
//This is a structure which will be used in telling the possible moves. A 2D array
//8x8 would show 1 for possible moves.
struct contourPlot
{int array[8][8];
int piece_val[8][8];
};
typedef struct contourPlot contourPlot;

//Here is the master matrix
struct square arr[8][8];
//int trackchance=1; //1 for white and 0 for black -- for peiodic turn.[don't make it public]

struct movePath
{ int ir,ic,fr,fc;
int val=0;
};

void clearscreen();
void initialize(square [][8]);
void display(square [][8]);

//void getMove(char turntracker); //getMove() function merged with main()
int isValidMove(int, int, int, int, square [][8], char turntracker);
void makeMove(int, int, int, int, square [][8], char turntracker);

//Here are the functions assuring proper implementation of chess rules
int knightrules(int,int, int,int, square[][8]);
int pawnrules(int,int, int,int, square[][8]);
int kingrules(int,int, int,int, square[][8]);
int rookrules(int,int, int,int, square[][8]);
int bishoprules(int,int, int,int, square[][8]);
int queenrules(int,int, int,int, square[][8]);

//Function to define score at any instance of chess game
//int tellScore(square virtualbox [][8], char chance_piece); //will use master matrix in its evaluation
//no longer in use

//functions to tell possible moves of chess pieces
contourPlot tellMove(int, int, square [][8], char turntracker);

//
int allocate_val(char);
//
void makeBestMove(square arr[][8],char chance_piece, int depth_level);
//Function to tell best move; depth level will state the degree of recursion
movePath tellBestMove(square arr[][8], char chance_piece, int depth_level);
//movePath firstNodeVal(int, int, square[][8],char chance_piece);
//no longer in use
//Arbitrary Movement
int getArbitMove(square array[][8],char turntracker);
//copy function
//endscore
int endScore(square arr[][8]);
movePath getNewMove(square [][8]);
void doMove(int,int,int,int, square [][8]);
int getNodeScore(square virtualbox[][8], int alpha,int beta, int level);
//maximum and minimum function
int max(int,int);
int min(int,int);

void copy_square(square a [][8], square b [][8])
{ int i,j;
for(i=0;i<8;i++)
{ for(j=0;j<8;j++)
{ a[i][j].piece=b[i][j].piece;
 a[i][j].piece_color=b[i][j].piece_color;
 a[i][j].value=b[i][j].value;
}
}
}
//main function
int main()
{
char turntracker='W';
initialize(arr); //a function called in beginning to initialize the chess pieces their required positions

while(1)
{
	process=0;

printf("\n        CHESS PLAYING ROBOT CONSOLE\n");
printf("        Indian Institute of Technology Kanpur\n\n");
display(arr);
printf("\n");

//getmove merged here
printf("\nMake your move [For ending game, type -1 and press enter]\t");
int initrow, initcol, finrow, fincol,temp;
if(turntracker=='B')
{ process=0;
	getch();
printf("\n\t\tBlack's Turn.  Move is played automatically:\n");
makeBestMove(arr,'B',1);
turntracker='W';
}
else if(turntracker=='W')
{
printf("\n\t\tWhite's Turn.:");
printf("\nFormat: InitialRow InitialColumn FinalRow FinalColumn\t" );
cin>>initrow;
if(initrow==-1)
exit(0);
cin>>initcol>>finrow>>fincol;
temp=isValidMove(initrow,initcol,finrow,fincol,arr,turntracker);
if (temp==1)
{makeMove(initrow,initcol,finrow,fincol,arr,turntracker);
turntracker='B';
}
else if(temp==0)
printf("\n** INVALID MOVE ** Please try again.\n"); //loop iteration
}  

}
}

void initialize(square arr[][8])
{
int i,j;
for(i=0; i<8;i++) //to give default values to all squares
for(j=0;j<8;j++)
arr[i][j]={' ',' ', 0};

/*Now, we'll fill each square with its specification at t=0s.
For a white piece the value of piece_color is W and vice versa.
For piecs their shorthand notations have been choosed.
King: K; Pawn P; Queen Q; Bishop:B; Rook R and Knight N
*/
arr[0][0]={'R','W',1};
arr[0][1]={'N','W',1};
arr[0][2]={'B','W',1};
arr[0][3]={'Q','W',1};
arr[0][4]={'K','W',1};
arr[0][5]={'B','W',1};
arr[0][6]={'N','W',1};
arr[0][7]={'R','W',1};
arr[1][0]={'P','W',1};
arr[1][1]={'P','W',1};
arr[1][2]={'P','W',1};
arr[1][3]={'P','W',1};
arr[1][4]={'P','W',1};
arr[1][5]={'P','W',1};
arr[1][6]={'P','W',1};
arr[1][7]={'P','W',1};
arr[6][0]={'P','B',1};
arr[6][1]={'P','B',1};
arr[6][02]={'P','B',1};
arr[6][03]={'P','B',1};
arr[6][04]={'P','B',1};
arr[06][05]={'P','B',1};
arr[06][06]={'P','B',1};
arr[06][07]={'P','B',1};
arr[07][0]={'R','B',1};
arr[07][01]={'N','B',1};
arr[07][02]={'B','B',1};
arr[07][03]={'Q','B',1};
arr[07][04]={'K','B',1};
arr[07][05]={'B','B',1};
arr[07][06]={'N','B',1};
arr[07][07]={'R','B',1};

}

void display(square arr[][8])
{


char starboard[40][48];
int i,j,k=1; //job of k is to decide if square is black or white
for(i=0;i<40;i++)
{
for(j=0;j<48;j++)
{if(k>=1&&k<=6)
{ k++;
starboard[i][j]='.';
}
else if(k>6&&k<=12)
{
starboard[i][j]=' ';
k++;
if(k==13)
k=1;
}
}

if(i==4&&j==48)
k=7;
if(i==9&&j==48)
k=1;
if(i==14&&j==48)
k=7;
if(i==19&&j==48)
k=1;
if(i==24&&j==48)
k=7;
if(i==29&&j==48)
k=1;
if(i==34&&j==48)
k=7;

}

/*Now having made the basic display, I'm going to add the chess pieces position in the display board
Basically this will be an overwriting process at the required position.
*/
//starboard[2][2]=arr[0][0].piece;
//starboard[2][3]=arr[0][0].piece_color;

for(i=0;i<8;i++)
for(j=0;j<8;j++)
{
starboard[i*5+2][j*6+2]=arr[i][j].piece;
starboard[i*5+2][j*6+3]=arr[i][j].piece_color;
}
//Now having made required alterations, show it all
for(i=0;i<40;i++)
{
for(j=0;j<48;j++)
printf("%c",starboard[i][j]);
printf("\n");
}
}

void clearscreen()
{int i;
for(i=0;i<20;i++)
cout<<"\n\n\n";
}



/*void getMove(char turntracker)
{ 

}*/

int isValidMove(int ir,int ic,int fr,int fc, square arr[][8], char turntracker)
{ //unexpected number input
if (ir<0||ic<0||fr<0||fc<0||ir>7||ic>7||fr>7||fc>7)
return 0;
if(ir==fr && fc==ic)
return 0;

//to see if the move is in agreement with the color
if(arr[ir][ic].piece_color!=turntracker)
return 0;

//same piece type at initial and final position
if(arr[ir][ic].piece_color==arr[fr][fc].piece_color)
return 0;

//passing function to each chess rule validator function
if(arr[ir][ic].piece=='P')
{ return pawnrules(ir,ic,fr,fc, arr);
}
else
if(arr[ir][ic].piece=='N')
{ return knightrules(ir,ic,fr,fc,arr);
}
else
if(arr[ir][ic].piece=='R')
{ return rookrules(ir,ic,fr,fc,arr);
}
else
if(arr[ir][ic].piece=='Q')
{ return queenrules(ir,ic,fr,fc,arr);
}
else
if(arr[ir][ic].piece=='K')
{ return kingrules(ir,ic,fr,fc,arr);
}
else
if(arr[ir][ic].piece=='B')
{ return bishoprules(ir,ic,fr,fc,arr);
}
else
return 0;
}

/*Values of rows and colummn run from 0 to 7.
This is because all testing things will be done by the Chess Playing Robot Team only.
So, no need to make it more comfortable chess engine*/

void makeMove(int ir,int ic, int fr, int fc, square arr[][8], char turntracker)
{//Just showing the possible moves . If need arises, can be deleted.
int i,j;

if(!isValidMove(ir,ic,fr,fc,arr,turntracker))
{
getArbitMove(arr,turntracker); //always the arbitrary move is for black piece
printf("\nwas here\n");
}
//loop iteration
struct contourPlot temp=tellMove(ir,ic,arr,turntracker);
printf("\nProcessed moves: %d %d %d %d\n", ir,ic,fr,fc);
for(i=0;i<8;i++)
{
for(j=0;j<8;j++)
{
printf("%d\t",temp.array[i][j]);
}
printf("\n");
}

if(arr[fr][fc].value==1&&temp.array[fr][fc]==2)
{
printf("\n      $$ Great!! CAPTURED $$");
}
printf("\n      $$ Move Done!!      $$");
//test
printf("\n      process =%d \n", process);
arr[fr][fc]=arr[ir][ic];
arr[ir][ic]={' ',' ',0};

}

//chess rule validity functions


//1.Pawn

int pawnrules(int ir, int ic, int fr, int fc, square arr[][8])
{
	if(abs(ir-fr)>2||abs(fc-ic)>=2)
	return 0;
		//single piece chess movement
	if(arr[ir][ic].piece_color=='W')
	{
	if(fr<=ir)  //unidirectional movement for white
	return 0;
	if(ir==4&&fr==6) //now happy---last trouble got cleared
	return 0;
	if(ir==5&&fr==7)
	return 0;
	} //for white
	else
	if(arr[ir][ic].piece_color=='B')
	{
	if(fr>=ir)  //unidirectional movement for black
	return 0;
	if(ir==3&&fr==1)
	return 0;
	if(ir==5&&fr==7)
	return 0;
	}
	
	//single piece movement
	if(abs(ir-fr)==1 && ic-fc==0&& arr[fr][fc].value==0)
	{	
	return 1;
	}
	
	//capture case
	if(abs(ir-fr)==1 && abs(ic-fc)==1 )
	{
	if(arr[ir][ic].piece_color=='W')
	{
		if(arr[fr][fc].piece_color=='B')
		return 1;
		else
		return 0;
	}
	if(arr[ir][ic].piece_color=='B')
	{
		if(arr[fr][fc].piece_color=='W')
		return 1;
		else
		return 0;
	}
	return 0;
	}
	
	//double piece movement
	if(abs(ir-fr)==2 && abs(ic-fc)==0)
	{
	if(arr[ir][ic].piece_color=='W')
	{
	if(!ir==1)
	return 0;
	if(arr[2][ic].value==1 || arr[3][ic].value==1)
	return 0;
	return 1;
	}
	if(arr[ir][ic].piece_color=='B')
	{
	if(!ir==6)
	return 0;
	if(arr[5][ic].value==1 || arr[4][ic].value==1)
	return 0;
	
	return 1;
	}
	}
return 0;
}




//2. For Knight
int knightrules(int ir,int ic, int fr,int fc, square arr[][8])
{ int flag=0;
if(abs(ir-fr)==1 && abs(ic-fc)==2)
flag=1;
else
if(abs(ir-fr)==2&&abs(ic-fc)==1)
flag=1;

return flag;

}


//3. For Queen
int queenrules(int ir,int ic, int fr,int fc, square arr[][8])
{ int flag=0;
int i,j;

//condition : slope should be equal to 1 or vertical or horizontal movement
if(!((abs(ic-fc)==abs(ir-fr))||abs(ic-fc)==0||abs(ir-fr)==0))
return 0;


//Now checking the diagonal condition
if(abs(ic-fc)==abs(ir-fr))
{
//there are 4 possibilities
//1. ir>fr and ic>fc
if(ir>fr && ic>fc)
{
for(i=fr+1, j=fc+1;i<ir;i++,j++)
{if(arr[i][j].value==1)
return 0;}
return 1;
}
//2. ir>fr and ic<fc
if(ir>fr && ic<fc)
{
for(i=fr+1, j=fc-1; i<ir;i++,j--)
{if(arr[i][j].value==1)
return 0;}
return 1;
}

//3. ir<fr and ic>fc
if(ir<fr && ic>fc)
{
for(i=ir+1, j=ic-1; i<fr;i++,j--)
{if(arr[i][j].value==1)
return 0;}
return 1;
}
//4. ir<fr and ic<fc
if(ir<fr && ic<fc)
{ for(i=ir+1, j=ic+1; i<fr;i++,j++)
{if(arr[i][j].value==1)
return 0;}
return 1;
}
}

//Now I have to take into a/c the vertical and horizontal movement

else if((ir-fr)==0) //horizontal case
{ if(ic>fc)  
{
for(i=fc+1;i<ic;i++)
{
if(arr[ir][i].value==1)
return 0;
}
flag=1;}

else if(ic<fc)
{for(i=ic+1;i<fc;i++)
{
if(arr[ir][i].value==1)
return 0;
}
flag=1;}
}

//vertical

else if((ic-fc)==0) //vertical case
{ 
if(ir>fr)
{
for(i=fr+1;i<ir;i++)
{
if(arr[i][ic].value==1)
return 0;
}
flag=1;}

else if(ir<fr)
{ 
for(i=ir+1;i<fr;i++)
{
if(arr[i][ic].value==1)
return 0;
}
flag=1;}

}
return flag;

}

//4. For Bishop
int bishoprules(int ir,int ic, int fr,int fc, square arr[][8])
{
int flag=0;
int jumpvalue=0,i,j;

//condition is that slope should be equal to 1.
if(abs(ic-fc)!=abs(ir-fr))
return 0;

//we are now left with valid moves
jumpvalue=abs(fc-ic);

//now checking if there is something that is blocking my path.
//there are 4 possibilities
//1. ir>fr and ic>fc
if(ir>fr && ic>fc)
{
for(i=fr+1, j=fc+1;i<ir;i++,j++)
{if(arr[i][j].value==1)
return 0;}
return 1;
}
//2. ir>fr and ic<fc
if(ir>fr && ic<fc)
{
for(i=fr+1, j=fc-1; i<ir;i++,j--)
{if(arr[i][j].value==1)
return 0;}
return 1;
}

//3. ir<fr and ic>fc
if(ir<fr && ic>fc)
{
for(i=ir+1, j=ic-1; i<fr;i++,j--)
{if(arr[i][j].value==1)
return 0;}
return 1;
}
//4. ir<fr and ic<fc
if(ir<fr && ic<fc)
{ for(i=ir+1, j=ic+1; i<fr;i++,j++)
{if(arr[i][j].value==1)
return 0;}
return 1;
}

}
//5. For rook
int rookrules(int ir,int ic, int fr,int fc, square arr[][8])
{ int i,j;
int flag=0;

if((ir-fr)==0) //horizontal case
{ if(ic>fc)  
{
for(i=fc+1;i<ic;i++)
{
if(arr[ir][i].value==1)
return 0;
}
flag=1;}

else if(ic<fc)
{for(i=ic+1;i<fc;i++)
{
if(arr[ir][i].value==1)
return 0;
}
flag=1;}
}

//vertical

else if((ic-fc)==0) //vertical case
{ //printf("Im at ic=fc");
if(ir>fr)
{
for(i=fr+1;i<ir;i++)
{
if(arr[i][ic].value==1)
return 0;
}
flag=1;}

else if(ir<fr)
{ //printf("i'm at ir<fr");
for(i=ir+1;i<fr;i++)
{ //cout<<"I'm being iterated";
if(arr[i][ic].value==1)
return 0;
}
flag=1;}

}
return flag;
}

//6. For King
int kingrules(int ir,int ic, int fr,int fc, square arr[][8])
{
int flag=0;
if(abs(ir-fr)==1 && abs(ic-fc)==0)
flag=1;
else if(abs(ic-fc)==1&&abs(ir-fr)==0)
flag=1;
else if(abs(ic-fc)==1&&abs(ir-fr)==1)
flag=1;

return flag;
}

//rules defined


/*Now I'm planning to implement AI in my code.
There are many faults still. For eg.
1. castelling not implemented
2. checkmate cases
3. enpassant
4. chess piece promotion

I'll try to develop AI in further code and will try to remove errors*/

/*All these will be achieved as follows:
1. make a function which can tell the possible moves
2. remove checkmate case
3. see the ai part throught the alpha beta pruning
*/
//there is no problem with layer.
contourPlot tellMove(int row,int col, square arr[][8], char turntracker)  //CHECKED
{ int i,j;
struct contourPlot layer;
for(i=0;i<8;i++)
{for(j=0;j<8;j++)
{if(isValidMove(row,col,i,j,arr,turntracker))
{
if(arr[i][j].value==1)
{
	if(arr[row][col].piece!='P')
	{
	layer.array[i][j]=2; 
	layer.piece_val[i][j]=allocate_val(arr[i][j].piece);
	} //2 denotes capturing case
	else
	{
		if(abs(i-row)==1 && abs(j-col)==1)
		{
		layer.array[i][j]=2;
		layer.piece_val[i][j]=allocate_val(arr[i][j].piece);
		}
		else
		{
		layer.array[i][j]=0;
		layer.piece_val[i][j]=0;
		}
	}

}
else
{
layer.array[i][j]=1;
layer.piece_val[i][j]=0;
}
}
else
{
layer.array[i][j]=0;
layer.piece_val[i][j]=0;
}
}
}
return layer;
}  
//I'm currently working on depth level 1 -- 12.6.2014

/*Convention:
For Black (Robot): +ve
For White (human): -ve
*/

int allocate_val(char c)  //CHECKED
{
 switch(c)
 {case 'P': return pval;
 case 'N': return nval;
 case 'K': return kval;
 case 'Q': return qval;
 case 'B': return bval;
 case 'R': return rval;
 };
}    

/*Planning to implement AI for pieces for depth level 1
What I want to do is sth like making a VIRTUAL MOVEMENT in a virtual space.
And then evaluating the further possibilities.
let me make a function for virtually traversing the node depths.
Now start....*/

void makeBestMove(square arr[][8],char chance_piece='B', int depth_level=1)
{
//struct movePath bestPath=tellBestMove(arr, chance_piece, depth_level);
struct movePath bestPath=getNewMove(arr);
//makeMove(bestPath.ir,bestPath.ic, bestPath.fr, bestPath.fc, arr,'B');
makeMove(bestPath.ir,bestPath.ic, bestPath.fr, bestPath.fc, arr,'B');
}



/* 13.06.2014 time 6:32am
Tasks to do after sleep
1. debug all the errors[compile and logical]
2. max evaluation should be more better. make a structure to store init and final posn so that changes can be executed later on
3. try to run the code.
I think depth level 1 is complete and after compiling and debugging all errors, use recursion and with little modification, do for depth level=2
and after that use alpha beta pruning
*/

//Function to make arbitrary movement in case the program is unable to decide its movement based upon its searches.

int getArbitMove(square array[][8], char turntracker='B')
{
	//struct contourPlot layer;
	//turntracker='B';
int i,j,k,l;
for(i=0;i<8;i++)
{
for(j=7;j>=0;j--)
{if(array[i][j].piece_color==turntracker)
//layer=tellMove(i,j,array, turntracker);
for(k=0;k<8;k++)
{
for(l=0;l<8;l++)
{ //if(layer.array[k][l]==2||layer.array[k][l]==1)
	if(isValidMove(i,j,k,l,array,'B'))
	{
	makeMove(i,j,k,l,array,turntracker);
	break;}

}
}
}
}
return 0;
}

int depth=4;

movePath getNewMove(square arr[][8]) //recursively
{	int alpha=-100000, beta=+100000;//default values
	int countTime=0;
	movePath bestmove, tempmove;
	square virtualbox1[8][8];
	int i,j,k,l;
	for(i=0;i<8;i++)
	for(j=0;j<8;j++)
	for(k=0;k<8;k++)
	for(l=0;l<8;l++)
	{
		if(isValidMove(i,j,k,l,arr,'B'))
		{	
			copy_square(virtualbox1,arr);
			doMove(i,j,k,l,virtualbox1);
			tempmove.val=getNodeScore(virtualbox1, alpha, beta, 2 );  //level=2
			//Here, the code is level 1
			//2 is level in the next function
			//it also denotes that if value is 2/even then it's white's turn and vice versa 
			if(countTime==0||tempmove.val>bestmove.val)
			{
				bestmove.ir=i;
				bestmove.ic=j;
				bestmove.fr=k;
				bestmove.fc=l;
				bestmove.val=tempmove.val;//{i,j,k,l,tempmove.val};
			}
			countTime++;
		}
}
	return bestmove;
}

int getNodeScore(square virtualbox[][8], int alpha,int beta, int level)
{
	int alpha1=alpha,beta1=beta; 
	process++;
	int i,j,k,l;
	square virtualbox1[8][8];
	int countTime=0;
	int temp, goodScore;
	//turn allocation a/q level
	char c;
	if(level%2==0)
	c='W';
	else
	c='B';
	//my motive is to send maximized score(level is even)/minimizer (level is odd except 1) at the top
	
	//for the basecase, i.e., when required number of depth is equal to the acquired level
	if(level==depth+1) //basecase
		return endScore(virtualbox);
	
	//if white's turn in this level, get scores that will be maximized in itself
	//and it should recieve scores from its node
	if(level%2==0)
	{
		for(i=0;i<8;i++)
		for(j=0;j<8;j++)
		for(k=0;k<8;k++)
		for(l=0;l<8;l++)
		{
			if(isValidMove(i,j,k,l,virtualbox,'W'))
			{
			copy_square(virtualbox1, virtualbox);
			doMove(i,j,k,l,virtualbox1);
			temp=getNodeScore(virtualbox1, alpha1, beta1, level+1 ); 
			if(countTime==0||temp>goodScore)  //maximize
			{
				goodScore=temp;
			}
			if(countTime!=0)  
			{
				if(countTime!=0&&level>2)
				{
					if(goodScore>beta)
					return goodScore;
				}
			}
			alpha1 = max(alpha1,goodScore);
			countTime++;
			}
		}
	}
	else
	{
				for(i=0;i<8;i++)
		for(j=0;j<8;j++)
		for(k=0;k<8;k++)
		for(l=0;l<8;l++)
		{
			if(isValidMove(i,j,k,l,virtualbox,'B'))
			{
			copy_square(virtualbox1, virtualbox);
			doMove(i,j,k,l,virtualbox1);
			temp=getNodeScore(virtualbox1, alpha1, beta1, level+1 ); 
			if(countTime==0||temp<goodScore)  //minimize
			{
				goodScore=temp;
				
			}
			if(countTime!=0&&level>2)
			{
				if(goodScore<alpha)
				return goodScore;
			}
			beta1=min(beta1,goodScore);
			countTime++;
			}
		}
		
	}
	
	return goodScore;
	
}
int endScore(square arr[][8])
{
	int i,j, score=0;
	for(i=0;i<8;i++)
	for(j=0;j<8;j++)
	{
		if(arr[i][j].piece_color=='W')
		score+=-allocate_val(arr[i][j].piece);
		else if(arr[i][j].piece_color=='B')
		score+=allocate_val(arr[i][j].piece);
		
	}
	return score;
}

void doMove(int i,int j, int k, int l, square arr[][8])
{
	arr[k][l].piece_color=arr[i][j].piece_color;
	arr[k][l].piece=arr[i][j].piece;
	arr[k][l].value=arr[i][j].value;
	arr[i][j]={' ', ' ', 0};
}
int max(int a, int b)
{
	if(a<b)
	return b;
	else
	return a;
}
int min(int a,int b)
{
	if(a<b)
	return a;
	else
	return b;
}
