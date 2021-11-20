#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//max elérési idő n*n*max random()
#define maxTime 1000;
#define veryBigNumber 10000000;

typedef struct coordinate {
	int x;
	int y;
}coordinate;
typedef struct neighbor {
	coordinate whereIAm;
	int distance;
	struct neighbor* next;
}neighbor;
typedef struct cell {
	coordinate coor;
	int minTime;

	struct neighbor* neightbors;

	struct cell* fromWhere;
	struct cell* next;
}cell;


//CREATE CELLS
cell* createCells(int size) {
	cell* head = NULL, * curr = NULL;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cell* node = (cell*)malloc(sizeof(cell));
			node->coor.x = i;
			node->coor.y = j;
			node->fromWhere = NULL;
			node->neightbors = NULL;
			node->next = NULL;
			node->minTime = veryBigNumber;

			if (head == NULL) head = curr = node;
			else curr = curr->next = node;
		}
	}
	return head;
}
//FREE CELLS
void freeCells(cell* head)
{
	cell* temp;
	while (head != 0)
	{
		temp = head;

		//Cellákhoz szomszédok tartoznak, először mindig azt szabadítom fel
		neighbor* nHead = temp->neightbors, * nTemp;
		while (nHead != NULL)
		{
			nTemp = nHead;
			nHead = nHead->next;
			free(nTemp);
		}

		head = head->next;
		free(temp);
	}
}
//CREATE NEIGHBORS
void createNeighbors(struct cell* headCells, int size) {
	struct cell* currCell = headCells;
	struct neighbor* neighHead = NULL, * neighCurr = NULL;
	int distX, distY;

	srand(time(NULL));

	do {
		neighbor* neighHead = NULL, * neighCurr = NULL;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				distX = i - currCell->coor.x;
				distY = j - currCell->coor.y;


				if((abs(distX) == 1 && distY == 0) || (distX == 0 && abs(distY) == 1))
				{
					struct neighbor* node = (neighbor*)malloc(sizeof(neighbor));;
					coordinate c;
					c.x = i;
					c.y = j;

					int yesOrNo = rand() % 9;
					int distance = -1;

					if (distX >= 0 && distY >= 0 && yesOrNo >= 4) {
						node->distance = rand() % 9 + 1;
					}
					else if(distX < 0 || distY < 0){
						int distance = findANeighborDistance(headCells, currCell->coor, c);
						node->distance = distance;
					}

					if ((distX >= 0 && distY >= 0 && yesOrNo >= 4) || ((distX < 0 || distY < 0) && node->distance != -1)) {
						node->next = NULL;
						node->whereIAm = c;

						if (neighHead == NULL) neighHead = neighCurr = node;
						else neighCurr = neighCurr->next = node;
					}
					else {
						free(node);
					}
				}
			}
		}
		currCell->neightbors = neighHead;
		currCell = currCell->next;
	} while (currCell != NULL);
}

int findANeighborDistance(struct cell* cellHead, coordinate who, coordinate whom) {
	struct cell* cellCurr = NULL;
	struct neighbor* neibr = NULL;
	for (cellCurr = cellHead; (cellCurr->coor.x != whom.x) || (cellCurr->coor.y != whom.y); cellCurr = cellCurr->next);
	if (cellCurr->neightbors == NULL) return -1;
	
	for (neibr = cellCurr->neightbors; 
		((neibr->whereIAm.x != who.x || neibr->whereIAm.y != who.y)) && neibr->next != NULL;
		neibr = neibr->next);
	if (neibr == NULL || !(neibr->whereIAm.x == who.x && neibr->whereIAm.y == who.y)) return -1;
	return neibr->distance;
}

struct cell array[10][10];

int main() {
	//srand(time(NULL));
	cell* cells = createCells(10);
	createNeighbors(cells, 10);
	freeCells(cells);
	

	return 0;
}
