#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define veryBigNumber 10000000;

typedef struct coordinate {
	int x;
	int y;
}coordinate;
typedef struct neighbor {
	struct cell* parent;

	struct neighbor* next;
}neighbor;
typedef struct cell {
	coordinate coor;
	int done;
	int addedToSomeone;

	struct neighbor* neighbors;
	struct neighbor* realNeighbors;

	struct cell* next;
}cell;
typedef struct availableCell {
	struct cell* cell;
	struct availableCell* next;
}availableCell;

//CREATE CELLS
cell* createCells(int size) {
	cell* head = NULL, * curr = NULL;
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cell* node = (cell*)malloc(sizeof(cell));
			node->coor.x = i;
			node->coor.y = j;
			node->done = 0;
			node->addedToSomeone = 0;
			node->neighbors = NULL;
			node->realNeighbors = NULL;
			node->next = NULL;

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
		neighbor* nHead = temp->neighbors, * nTemp;
		neighbor* rnHead = temp->realNeighbors, * rnTemp;
		while (nHead != NULL)
		{
			nTemp = nHead;
			nHead = nHead->next;
			free(nTemp);
		}
		while (rnHead != NULL)
		{
			rnTemp = rnHead;
			rnHead = rnHead->next;
			free(rnTemp);
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

	do {
		neighbor* neighHead = NULL, * neighCurr = NULL;

		for (struct cell* c = headCells; c; c = c->next) {
			distX = c->coor.x - currCell->coor.x;
			distY = c->coor.y - currCell->coor.y;

			if ((abs(distX) == 1 && distY == 0) || (distX == 0 && abs(distY) == 1))
			{
				struct neighbor* node = (neighbor*)malloc(sizeof(neighbor));
				node->parent = c;
				node->next = NULL;

				if (neighHead == NULL) neighHead = neighCurr = node;
				else neighCurr = neighCurr->next = node;
			}
		}

		currCell->neighbors = neighHead;
		currCell = currCell->next;
	} while (currCell != NULL);
}
void addRealNeighbor(struct cell* cell) {
	struct neighbor* rNH = NULL;
	struct neighbor* rNC = NULL;
	for (struct neighbor* curr = cell->neighbors; curr; curr = curr->next) {
		if (curr->parent->addedToSomeone == 0) {
			struct neighbor* node = (neighbor*)malloc(sizeof(neighbor));
			node->next = NULL;
			node->parent = curr->parent;

			if (rNH == NULL) rNH = rNC = node;
			else rNC = rNC->next = node;

			curr->parent->addedToSomeone = 1;
		}
	}
	cell->realNeighbors = rNH;
}
int availableNeighbors(struct cell* cell) {
	int a = 0;
	for (struct neighbor* curr = cell->neighbors; curr; curr = curr->next)
		if (curr->parent->done == 0) a++;
	return a;
}
int availableCellsNumber(struct availableCell* CurrAvailableHead) {
	int a = 0;
	for (struct availableCell* curr = CurrAvailableHead; curr; curr = curr->next) {
		if (curr->cell->done == 0) a++;
	}
	return a;
}
availableCell* updateAvailableCells(struct availableCell* CurrAvailableHead, struct cell* addableCells) {
	//add the newones
	addableCells->done = 1;
	addableCells->addedToSomeone = 1;
	addRealNeighbor(addableCells);

	//remove, what not available anymore
	struct availableCell* prev = NULL;
	struct availableCell* curr = NULL;
	struct availableCell* temp = NULL;
	for (curr = CurrAvailableHead; curr != NULL; curr = curr->next) {
		if (curr->cell->done == 1) {
			if (curr == CurrAvailableHead) {
				temp = curr;
				CurrAvailableHead = CurrAvailableHead->next;
				curr = curr->next;
				free(temp);
			}
			else {
				for (prev = CurrAvailableHead; prev->next != curr; prev = prev->next);
				temp = curr;
				prev->next = curr->next;
				curr = curr->next;
				free(temp);
				
			}
		}
		if (curr == NULL) break;
	}
	

	if (CurrAvailableHead != NULL) {
		for (curr = CurrAvailableHead; curr->next != NULL; curr = curr->next);
	}

	for (struct neighbor* currN = addableCells->realNeighbors; currN; currN = currN->next) {
		struct availableCell* node = (availableCell*)malloc(sizeof(availableCell));
		node->next = NULL;
		node->cell = currN->parent;

		if (CurrAvailableHead == NULL) CurrAvailableHead = curr = node;
		else curr = curr->next = node;
		
	}
	return CurrAvailableHead;
}
// BINARY TREE
void createBinaryTree(struct cell* cellHead) {
	srand(time(NULL));
	//0, 0-val kezdunk
	struct availableCell* availableCell = NULL;
	struct availableCell* curr = NULL;
	struct cell* lastOne = NULL;
	availableCell = updateAvailableCells(availableCell, cellHead);
	lastOne = cellHead;
	while (availableCell != NULL) {
		int r = availableCellsNumber(availableCell);

		if (r != 0) {
			int b = rand() % r; //hanyadik

			curr = availableCell;
			for (int i = 0; i < b; i++) {
				curr = curr->next;
			}
			availableCell = updateAvailableCells(availableCell, curr->cell);
		}
		else break;

		/*int r = availableCellsNumber(availableCell);

		if (r != 0) {
			//30-70 : 25 - legutolso, 75 - random
			int type = rand() % 100;
			int b = rand() % r; //hanyadik
			if (type < 25) {
				int a = availableNeighbors(lastOne);
				if (a != 0) {
					int e = rand() % a;
				}
				
				for (curr = availableCell; curr->next; curr = curr->next);

			}
			else {
				curr = availableCell;
				int b = rand() % r; //hanyadik
				for (int i = 0; i < b; i++) {
					curr = curr->next;
				}
			}
			availableCell = updateAvailableCells(availableCell, curr->cell);
			lastOne = curr->cell;
		}
		else break;*/
	}
}
/*
int findANeighborDistance(struct cell* cellHead, coordinate who, coordinate whom) {
	struct cell* cellCurr = NULL;
	struct neighbor* neibr = NULL;
	for (cellCurr = cellHead; (cellCurr->coor.x != whom.x) || (cellCurr->coor.y != whom.y); cellCurr = cellCurr->next);
	if (cellCurr->neighbors == NULL) return -1;

	for (neibr = cellCurr->neighbors;
		((neibr->whereIAm.x != who.x || neibr->whereIAm.y != who.y)) && neibr->next != NULL;
		neibr = neibr->next);
	if (neibr == NULL || !(neibr->whereIAm.x == who.x && neibr->whereIAm.y == who.y)) return -1;
	return neibr->distance;
}
void Dijkstra(cell* cellHead) {
	int done = 0;
	while (!done) {
		int minTime = veryBigNumber;
		cell* cellMin = NULL;
		for (cell* cellCurr = cellHead; cellCurr; cellCurr = cellCurr->next) {
			if (cellCurr->processed == 0 && cellCurr->minTime < minTime) {
				minTime = cellCurr->minTime;
				cellMin = cellCurr;
			}
		}
		if (cellMin == NULL) {
			done = 1;
			break;
		}
	}
}
*/

void heatMap(cell* cellHead) {
	FILE* file = fopen("heatmap.svg", "w");
	fprintf(file, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n\n", 880, 880);

	for (cell* current = cellHead; current; current = current->next)
	{
		int a = (current->coor.x + 1) * 80;
		int b = (current->coor.y + 1) * 80;

		fprintf(file, "<circle cx = \"%d\" cy = \"%d\" r = \"10\" stroke =\"black\" fill = \"black\" />\n", a, b);

		for (neighbor* c = current->realNeighbors; c; c = c->next) {
			int e = (c->parent->coor.x + 1) * 80;
			int f = (c->parent->coor.y + 1) * 80;
			fprintf(file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", a, b, e, f);
		}
	}
	fprintf(file, "\n</svg>");
}

int main() {
	cell* cells = createCells(7);
	createNeighbors(cells, 7);
	
	createBinaryTree(cells);
	heatMap(cells);
	freeCells(cells);

	return 0;
}
