#include<stdio.h>
#include<stdlib.h>
int getMove (int **, int, int, int);
int K = 10000, Q = 79, R = 50, B = 33, N = 31, P = 10, Depth = 5;
int
getmin (int a, int b)
{
  if (a < b)
    return a;
  return b;
}

int
getmax (int a, int b)
{
  if (a > b)
    return a;
  return b;
}

int
mod (int a)
{
  if (a > 0)
    return a;
  return -a;
}

void play (int **);
void PrintBoard (int **);

int
main ()
{
  printf ("Initializations...\n");
  int **board = (int **) malloc (sizeof (int *) * 8);
  int i = 0, j;
  for (; i < 8; i++)
    {
      board[i] = (int *) malloc (sizeof (int) * 8);
      for (j = 0; j < 8; j++)
	board[i][j] = 0;
    }
  board[0][0] = R;
  board[0][1] = N;
  board[0][2] = B;
  board[0][3] = Q;
  board[0][4] = K;
  board[0][5] = B;
  board[0][6] = N;
  board[0][7] = R;
  board[7][0] = -R, board[7][1] = -N, board[7][2] = -B, board[7][3] =
    -Q, board[7][4] = -K, board[7][5] = -B, board[7][6] = -N, board[7][7] =
    -R;
  for (i = 0; i < 8; i++)
    {
      board[1][i] = P;
      board[6][i] = -P;
    }
  PrintBoard (board);
  printf ("\n\n\t****Let's Play****\n");
  play (board);
  free (board);
  return 0;
}

int
King (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (mod (x1 - x2) < 2 && mod (y1 - y2) < 2)
    {
      if (a[x1][y1] * a[x2][y2] > 0)
	{
	  return 0;
	}
    }
  else
    return 0;
  //  if(inCheck())
  //return 0;
  return 1;
}

int
Rook (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  if (x1 == x2 && mod (y1 - y2) > 0 || y1 == y2 && mod (x1 - x2) > 0);
  else
    return 0;
  if (x1 - x2 != 0)
    {
      int i, maX = getmax (x1, x2);
      for (i = getmin (x1, x2) + 1;; i++)
	{
	  if (i == maX)
	    return 1;
	  if (a[i][y1] != 0)
	    return 0;
	}
    }
  else
    {
      int i = getmin (y1, y2) + 1, maX = getmax (y1, y2);
      for (;; i++)
	{
	  if (i == maX)
	    return 1;
	  if (a[x1][i] != 0)
	    {
	      return 0;
	    }
	}
    }
  //if(inCheck())
  //return 0;
  return 1;
}

int
Knight (int move, int **a)
{
  // printf("Entred Knight()\n");
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  if (mod (x1 - x2) + mod (y1 - y2) == 3)
    if (mod (x1 - x2) == 1 || mod (x1 - x2) == 2)
      return 1;
  //printf("Ended Knight:n");
  return 0;
}

int
Bishop (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (mod (x1 - x2) != mod (y1 - y2))
    return 0;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  int i = 1, j = mod (x1 - x2);
  if (y1 < y2 && x1 < x2)
    {
      for (; i < j; i++)
	if (a[x1 + i][y1 + i] != 0)
	  return 0;
    }
  else if (y1 < y2 && x1 > x2)
    {
      for (; i < j; i++)
	if (a[x1 - i][y1 + i] != 0)
	  return 0;
    }
  else if (y1 > y2 && x1 > x2)
    {
      for (; i < j; i++)
	if (a[x1 - i][y1 - i] != 0)
	  return 0;
    }
  else
    {
      for (; i < j; i++)
	if (a[x1 + i][y1 - i] != 0)
	  return 0;
    }
  return 1;
}

int
Pawn (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] == -P)
    {
      if (x1 == 6 && x2 == 4 && a[x2][y2] == 0)
	return 1;
      if (!(x1 - x2 == 1))
	return 0;
      if (mod (y1 - y2) == 1)
	{
	  if (a[x1][y1] * a[x2][y2] >= 0)
	    return 0;
	  else
	    return 1;
	}
      if (y1 == y2)
	{
	  if (a[x1][y1] * a[x2][y2] == 0)
	    return 1;
	  else
	    return 0;
	}
    }
  else
    {
      if (x1 == 1 && x2 == 3 && a[x2][y2] == 0)
	return 1;
      if (!(x2 - x1 == 1))
	return 0;
      if (mod (y1 - y2) == 1)
	{
	  if (a[x1][y1] * a[x2][y2] >= 0)
	    return 0;
	  else
	    return 1;
	}
      if (y1 == y2)
	{
	  if (a[x1][y1] * a[x2][y2] == 0)
	    return 1;
	  else
	    return 0;
	}
    }
  return 0;
}

int
isValidMove (int move, int **a)	//return 0 if invalid   if move=1234 it means move piece from 1,2 to 3,4
{
  int x1, y1, x2, y2, tmpmove = move;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  printf ("  %d  %d %d %d\n%d  %d\n", x1 + 1, y1 + 1, x2 + 1, y2 + 1,
	  a[x1][y1], a[x2][y2]);
  if (mod (a[x1][y1]) == Q)
    return Bishop (tmpmove, a) || Rook (tmpmove, a);
  if (mod (a[x1][y1]) == K)
    return King (tmpmove, a);
  if (mod (a[x1][y1]) == P)
    return Pawn (tmpmove, a);
  if (mod (a[x1][y1]) == R)
    return Rook (tmpmove, a);
  if (mod (a[x1][y1]) == N)
    return Knight (tmpmove, a);
  if (mod (a[x1][y1]) == B)
    return Bishop (tmpmove, a);
  return 0;
}

void
moveIt (int move, int **a)
{
  printf ("Move Done\n");
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  a[x2][y2] = a[x1][y1];
  a[x1][y1] = 0;
  PrintBoard (a);
  //  printf("\n***Your Move is Done***\n");
}

void
PrintBoard (int **a)
{
  int i, j;
  for (i = 1; i < 9; i++)
    printf ("   %d  ", i);
  printf ("\n");
  for (i = 0; i < 8; i++)
    {
      printf (" ");
      if (i & 1)
	for (j = 0; j < 4; j++)
	  printf ("******      ");
      else
	for (j = 0; j < 4; j++)
	  printf ("      ******");
      printf ("\n%d", i + 1);
      for (j = 0; j < 8; j++)
	{
	  if ((i + j) & 1)
	    printf ("**");
	  else
	    printf ("  ");
	  if (mod (a[i][j]) == K)
	    {
	      if (a[i][j] > 0)
		printf ("WK");
	      else
		printf ("BK");
	    }
	  else if (mod (a[i][j]) == Q)
	    {
	      if (a[i][j] > 0)
		printf ("WQ");
	      else
		printf ("BQ");
	    }
	  else if (mod (a[i][j]) == R)
	    {
	      if (a[i][j] > 0)
		printf ("WR");
	      else
		printf ("BR");
	    }
	  else if (mod (a[i][j]) == B)
	    {
	      if (a[i][j] > 0)
		printf ("WB");
	      else
		printf ("BB");
	    }
	  else if (mod (a[i][j]) == N)
	    {
	      if (a[i][j] > 0)
		printf ("WN");
	      else
		printf ("BN");
	    }
	  else if (mod (a[i][j]) == P)
	    {
	      if (a[i][j] > 0)
		printf ("WP");
	      else
		printf ("BP");
	    }
	  else if (a[i][j] == 0)
	    {
	      if ((j + i) & 1)
		printf ("**");
	      else
		printf ("  ");
	    }
	  if ((i + j) & 1)
	    printf ("**");
	  else
	    printf ("  ");
	}
      printf ("\n ");
      if (i & 1)
	for (j = 0; j < 4; j++)
	  printf ("******      ");
      else
	for (j = 0; j < 4; j++)
	  printf ("      ******");
      printf ("\n");
    }
}

void
update (int **a, int **b)
{
  int i = 0, j = 0, count;
  for (; i < 8; i++)
    {
      for (j = 0; j < 8; j++)
	if (a[i][j] != 0)
	  {
	    b[count][0] = a[i][j];
	    b[count][1] = i * 10 + j;
	  }
    }
}

void
play (int **a)
{
  int Move = 1;
  while (Move != -1111)
    {
      scanf ("%d", &Move);
      Move -= 1111;
      if (Move == -1111)
	break;
      if (Move < 0 || Move > 7777)
	{
	  printf ("***Invalid Move***\n");
	  continue;
	}
      if(isValidMove(Move,a))
      	moveIt (Move, a);
      else
	{
	  printf ("***Invalid   Move***\n");
	  continue;
	}
      Move = getMove (a, -1, 0, 1);
      if (Move == 0)
	{
	  PrintBoard (a);
	  printf ("\ncontinue\n");
	  continue;
	}
      moveIt (Move, a);
      printf ("My Move  %d\n", Move + 1111);
    }
  return;
}

typedef struct XMoves
{
  struct XMoves *next;
  int x;
} moveS;
moveS *
pop (moveS * start, int x)
{
  if (start == NULL)
    {
      moveS *tmp = (moveS *) malloc (sizeof (moveS));
      tmp->x = x;
      tmp->next = NULL;
      return tmp;
    }
  moveS *tmp = (moveS *) malloc (sizeof (moveS));
  tmp->next = start;
  tmp->x = x;
  start = tmp;
  return start;
}

moveS *WhereToMoveKing (int **, int, int, moveS *);
moveS *WhereToMovePawn (int **, int, int, moveS *);
moveS *WhereToMoveRook (int **, int, int, moveS *);
moveS *WhereToMoveKnight (int **, int, int, moveS *);
moveS *WhereToMoveBishop (int **, int, int, moveS *);
int
getMove (int **a, int turn, int score, int depth)
{
//   PrintBoard(a);
  int tmpscore, bestScore = 30000 * turn;
  int count = 0, bestT, best = 0;
  int x2, y2, tmp, i, j, x1, y1, tmp1, tmp2;
  moveS *move = NULL;		//(moveS*)malloc(sizeof(moveS));
  if (depth == 1)
    {
      for (i = 0; i < 8; i++)
	{
	  for (j = 0; j < 8; j++)
	    {
	      if (a[i][j] == 0)
		continue;
	      if (a[i][j] * turn > 0)
		{
		  if (mod (a[i][j]) == Q)
		    {
		      a[i][j] = turn * B;
		      move = WhereToMoveBishop (a, i, j, move);
		      a[i][j] = turn * R;
		      move = WhereToMoveRook (a, i, j, move);
		      a[i][j] = turn * Q;
		      continue;
		    }
		  if (mod (a[i][j]) == R)
		    {
		      move = WhereToMoveRook (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == P)
		    {
		      move = WhereToMovePawn (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == B)
		    {
		      move = WhereToMoveBishop (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == N)
		    {
		      move = WhereToMoveKnight (a, i, j, move);
		    }
		}
	    }
	}
      moveS *start = move;
      if (turn == 1)
	{
	  int flag=0;
	  while (move != NULL)
	    {
	      tmp = move->x;
	      y2 = move->x % 10;
	      move->x /= 10;
	      x2 = move->x % 10;
	      move->x /= 10;
	      y1 = move->x % 10;
	      move->x /= 10;
	      x1 = move->x % 10;
	      tmp1 = a[x1][y1];
	      tmp2 = a[x2][y2];
	      a[x2][y2] = a[x1][y1];
	      a[x1][y1] = 0;
	      bestT = bestScore;
	      bestScore =
		getmin (bestScore, tmp2 +
			getMove (a, -turn, bestScore, depth + 1));
	      if (bestScore < bestT || flag==0)
		{
		  best = tmp;
		}
	      a[x1][y1] = tmp1;
	      a[x2][y2] = tmp2;
	      start = move;
	      move = move->next;
	      free (start);
	      flag++;
	    }
	}
      else
	{
	  int flag=0;
	  while (move != NULL)
	    {
	      tmp = move->x;
	      y2 = move->x % 10;
	      move->x /= 10;
	      x2 = move->x % 10;
	      move->x /= 10;
	      y1 = move->x % 10;
	      move->x /= 10;
	      x1 = move->x % 10;
	      tmp1 = a[x1][y1];
	      tmp2 = a[x2][y2];
	      a[x2][y2] = a[x1][y1];
	      a[x1][y1] = 0;
	      bestT = bestScore;
	      int sdhvasjdb=tmp2 + getMove (a, -turn, bestScore, depth + 1);
	      //	      printf("1\t%d\t%d\n",bestScore,tmp2);
	      bestScore =
		getmax (bestScore,sdhvasjdb
			);
	      //printf("2\t%d\t%d\t%d\t\t%d\n",bestScore,tmp2,sdhvasjdb,tmp+1111);
	      if (bestScore > bestT||flag==0)
		{
		  //  printf("%d>%d\n",bestScore,bestT);
		  best = tmp;
		}
	      a[x1][y1] = tmp1;
	      a[x2][y2] = tmp2;
	      start = move;
	      move = move->next;
	      free (start);
	      flag++;
	    }
	}
      return best;
    }
    
    
  if (depth > 1 && depth < Depth)
    {
      for (i = 0; i < 8; i++)
	{
	  for (j = 0; j < 8; j++)
	    {
	      if (a[i][j] == 0)
		continue;
	      if (a[i][j] * turn > 0)
		{
		  if (mod (a[i][j]) == R)
		    {
		      move = WhereToMoveRook (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == N)
		    {
//                        printf ("Checking Knight %d %d:\n", i + 1, j + 1);
		      move = WhereToMoveKnight (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == B)
		    {
//                        printf ("Checking Bishop %d %d :\n", i + 1, j + 1);
		      move = WhereToMoveBishop (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == Q)
		    {
//                        printf ("Checking Queen %d %d :\n", i + 1, j + 1);
		      a[i][j] = turn * B;
		      move = WhereToMoveBishop (a, i, j, move);
		      a[i][j] = turn * R;
		      move = WhereToMoveRook (a, i, j, move);
		      a[i][j] = turn * Q;
		      continue;
		    }
		  if (mod (a[i][j]) == P)
		    {
//                        printf ("Checking Pawn %d %d :\n", i + 1, j + 1);
		      move = WhereToMovePawn (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == K)
		    {
//                        printf ("Checking King %d %d: \n", i + 1, j + 1);
		      move = WhereToMoveKing (a, i, j, move);
		    }
		}
	    }
	}
      moveS *start = move;
      if (turn == 1)
	{
	  //printf("MayBe ");
	  while (move != NULL)
	    {
	      //  printf("%d ",move->x+1111);
	      tmp = move->x;
	      y2 = move->x % 10;
	      move->x /= 10;
	      x2 = move->x % 10;
	      move->x /= 10;
	      y1 = move->x % 10;
	      move->x /= 10;
	      x1 = move->x % 10;
	      tmp1 = a[x1][y1];
	      tmp2 = a[x2][y2];
	      a[x2][y2] = a[x1][y1];
	      a[x1][y1] = 0;
	      bestScore =
		getmin (bestScore,tmp2+ getMove (a, -turn, bestScore, depth + 1));
	      a[x1][y1] = tmp1;
	      a[x2][y2] = tmp2;
	      start = move;
	      move = move->next;
	      free (start);
	    }
	  // printf("\n");
	}
      else
	{
	  while (move != NULL)
	    {
	      tmp = move->x;
	      y2 = move->x % 10;
	      move->x /= 10;
	      x2 = move->x % 10;
	      move->x /= 10;
	      y1 = move->x % 10;
	      move->x /= 10;
	      x1 = move->x % 10;
	      tmp1 = a[x1][y1];
	      tmp2 = a[x2][y2];
	      a[x2][y2] = a[x1][y1];
	      a[x1][y1] = 0;
	      bestScore =
		getmax (bestScore, tmp2+getMove (a, -turn, bestScore, depth + 1));
	      a[x1][y1] = tmp1;
	      a[x2][y2] = tmp2;
	      start = move;
	      move = move->next;
	      free (start);
	    }
	}
//        free (start);
      return bestScore;
    }
  if (depth == Depth)
    {
      for (i = 0; i < 8; i++)
	{
	  for (j = 0; j < 8; j++)
	    {
	      if (a[i][j] == 0)
		continue;
	      if (a[i][j] * turn > 0)
		{
		  if (mod (a[i][j]) == Q)
		    {
		      a[i][j] = turn * R;
		      move = WhereToMoveRook (a, i, j, move);
		      a[i][j] = turn * B;
		      move = WhereToMoveBishop (a, i, j, move);
		      a[i][j] = turn * Q;
		      continue;
		    }
		  if (mod (a[i][j]) == R)
		    {
		      move = WhereToMoveRook (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == P)
		    {
		      move = WhereToMovePawn (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == B)
		    {
		      move = WhereToMoveBishop (a, i, j, move);
		      continue;
		    }
		  if (mod (a[i][j]) == N)
		    {
		      move = WhereToMoveKnight (a, i, j, move);
		    }
		}
	    }
	}
      moveS *start = move;
      if (turn == 1)
	{
	  //	  printf("MayBe\t");
	  while (move != NULL)
	    {
	      //  printf("%d ",move->x+1111);
	      y2 = move->x % 10;
	      move->x = move->x / 10;
	      x2 = move->x % 10;
	      bestScore = getmin (bestScore, a[x2][y2]);
	      start = move;
	      move = move->next;
	      free (start);
	    }
	  //	  printf("\n");
	}
      else
	{
	  //  printf("MatBe ");
	  while (move != NULL)
	    {
	      // printf("%d ",move->x+1111);
	      y2 = move->x % 10;
	      move->x = move->x / 10;
	      x2 = move->x % 10;
	      bestScore = getmax (bestScore, a[x2][y2]);
	      start = move;
	      move = move->next;
	      free (start);
	    }
	  //printf("\n");
	}
      return bestScore;
    }
  return 0;
}

int
Encode (int i, int j, int k, int l)
{
  return 1000 * i + 100 * j + 10 * k + l;
}

moveS *
WhereToMoveRook (int **a, int i, int j, moveS * move)
{
  int x = 1, count = 0;
  while (i - x > -1)
    {
      if (Rook (Encode (i, j, i - x, j), a))
	{
	  move = pop (move, Encode (i, j, i - x, j));
	  count++;
	}
      x++;
    }
  x = 1;
  while (i + x < 8)
    {
      if (Rook (Encode (i, j, i + x, j), a))
	{
	  move = pop (move, Encode (i, j, i + x, j));
	  count++;
	}
      x++;
    }
  x = 1;
  while (j - x > -1)
    {
      if (Rook (Encode (i, j, i, j - x), a))
	{
	  move = pop (move, Encode (i, j, i, j - x));
	  count++;
	}
      x++;
    }
  x = 1;
  while (j + x < 8)
    {
      if (Rook (Encode (i, j, i, j + x), a))
	{
	  move = pop (move, Encode (i, j, i, j + x));
	  count++;
	}
      x++;
    }
  /*moveS *tp=move;
     while(tp!=NULL)
     {
     printf("Possible Move %d\n",tp->x+1111);
     tp=tp->next;
     }
     printf("%d\n",count); */
  return move;
}

moveS *
WhereToMoveKnight (int **a, int i, int j, moveS * move)
{
  int x, y;
  if (i - 2 > -1)
    {
      if (j - 1 > -1)
	{
	  if (Knight (Encode (i, j, i - 2, j - 1), a))
	    {
	      move = pop (move, Encode (i, j, i - 2, j - 1));
	    }
	}
      if (j + 1 < 8)
	{
	  if (Knight (Encode (i, j, i - 2, j + 1), a))
	    {
	      move = pop (move, Encode (i, j, i - 2, j + 1));
	    }
	}
    }
  if (i + 2 < 8)
    {
      if (j - 1 > -1)
	{
	  if (Knight (Encode (i, j, i + 2, j - 1), a))
	    {
	      move = pop (move, Encode (i, j, i + 2, j - 1));
	    }
	}
      if (j + 1 < 8)
	{
	  if (Knight (Encode (i, j, i + 2, j + 1), a))
	    {
	      move = pop (move, Encode (i, j, i + 2, j + 1));
	    }
	}
    }
  if (i - 1 > -1)
    {
      if (j - 2 > -1)
	{
	  if (Knight (Encode (i, j, i - 1, j - 2), a))
	    {
	      move = pop (move, Encode (i, j, i - 1, j - 2));
	    }
	}
      if (j + 2 < 8)
	{
	  if (Knight (Encode (i, j, i - 1, j + 2), a))
	    {
	      move = pop (move, Encode (i, j, i - 1, j + 2));
	    }
	}
    }
  if (i + 1 < 8)
    {
      if (j - 2 > -1)
	{
	  if (Knight (Encode (i, j, i + 1, j - 2), a))
	    {
	      move = pop (move, Encode (i, j, i + 1, j - 2));
	    }
	}
      if (j + 2 < 8)
	{
	  if (Knight (Encode (i, j, i + 1, j + 2), a))
	    {
	      move = pop (move, Encode (i, j, i + 1, j + 2));
	    }
	}
    }
  return move;
}

moveS *
WhereToMoveBishop (int **a, int i, int j, moveS * move)
{
  int x;
  for (x = 1; x < 8; x++)
    {
      if (i - x > -1 && j - x > -1)
	{
	  if (Bishop (Encode (i, j, i - x, j - x), a))
	    {
	      move = pop (move, Encode (i, j, i - x, j - x));
	    }
	}
      if (i - x > -1 && j + x < 8)
	{
	  if (Bishop (Encode (i, j, i - x, j + x), a))
	    {
	      move = pop (move, Encode (i, j, i - x, j + x));
	    }
	}
      if (i + x < 8 && j - x > -1)
	{
	  if (Bishop (Encode (i, j, i + x, j - x), a))
	    {
	      move = pop (move, Encode (i, j, i + x, j - x));
	    }
	}
      if (i + x < 8 && j + x < 8)
	{
	  if (Bishop (Encode (i, j, i + x, j + x), a))
	    {
	      move = pop (move, Encode (i, j, i + x, j + x));
	    }
	}
    }
  return move;
}

moveS *
WhereToMovePawn (int **a, int i, int j, moveS * move)
{
  if (a[i][j] < 0)
    {
      if (j > 0 && i > 0)
	{
	  if (Pawn (Encode (i, j, i - 1, j - 1), a))
	    {
	      move = pop (move, Encode (i, j, i - 1, j - 1));
	    }
	}
      if (i > 0)
	{
	  if (Pawn (Encode (i, j, i - 1, j), a))
	    {
	      move = pop (move, Encode (i, j, i - 1, j));
	    }
	}
      if (i == 6)
	{
	  if (Pawn (Encode (i, j, i - 2, j), a))
	    {
	      move = pop (move, Encode (i, j, i - 2, j));
	    }
	}
      if (j < 7 && i > 0)
	{
	  if (Pawn (Encode (i, j, i - 1, j + 1), a))
	    {
	      move = pop (move, Encode (i, j, i - 1, j + 1));
	    }
	}
    }
  if (a[i][j] > 0)
    {
      if (j < 7 && i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j + 1), a))
	    {
	      move = pop (move, Encode (i, j, i + 1, j + 1));
	    }
	}
      if (i == 1)
	{
	  if (Pawn (Encode (i, j, i + 2, j), a))
	    move = pop (move, Encode (i, j, i + 2, j));
	}
      if (i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j), a))
	    move = pop (move, Encode (i, j, i + 1, j));
	}
      if (j > 0 && i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j - 1), a))
	    move = pop (move, Encode (i, j, i + 1, j - 1));
	}
    }
  return move;
}

moveS *
WhereToMoveKing (int **a, int i, int j, moveS * move)
{
  int x, y;
  for (x = -1; x < 2; x++)
    {
      for (y = -1; y < 2; y++)
	{
	  if (x + i > -1 && x + i < 8 && j + y > -1 && j + y < 8)
	    {
	      if (King (Encode (i, j, i + x, j + y), a))
		{
		  move = pop (move, Encode (i, j, i + x, j + y));
		}
	    }
	}
    }
  return move;
}
