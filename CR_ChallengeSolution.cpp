#include <stdio.h>
#include <string.h>

static int verbose_flag;

#define V 4 //Matrix  size

int path[V * V] = { 0 };//final path - assuming response requires all the positions
int length_path = 0; //length_path is zero on the beginning.

void search_path(int graph[V][V], int indX, int indY); // Funtion that implements kind of Dijkstra algorithm.
void printSolution(int dist[V * V]); // Function to visualize the solution path.


int recursive_search_path(int graph[V][V], bool Visit[V * V], int tempath[V * V], int Stepx, int indX, int indY) {

	int thisPosX = indX;
	int thisPosY = indY;
	int rowNum = 0;
	int colNum = 0;
	int notpath = 1; //not solution path yet

	//finding neighbords...
	// mapping initial positions
	int startPosX = (thisPosX - 1 < 0) ? thisPosX : thisPosX - 1; 
	int startPosY = (thisPosY - 1 < 0) ? thisPosY : thisPosY - 1;

	// mapping final positions
	int endPosX = (thisPosX + 1 > V - 1) ? thisPosX : thisPosX + 1; 
	int endPosY = (thisPosY + 1 > V - 1) ? thisPosY : thisPosY + 1;

	for (int rowNum = startPosX; rowNum <= endPosX; rowNum++) {
		for (int colNum = startPosY; colNum <= endPosY; colNum++) {

			//ignoring diagonals **rule #1
			if ((rowNum == thisPosX - 1 && colNum == thisPosY - 1) ||
				(rowNum == thisPosX - 1 && colNum == thisPosY + 1) ||
				(rowNum == thisPosX + 1 && colNum == thisPosY - 1) ||
				(rowNum == thisPosX + 1 && colNum == thisPosY + 1)
				) {				
				continue;
			}

			if (Visit[rowNum * V + colNum] == false && // vertex not visited
				graph[indX][indY] > graph[rowNum][colNum] // **rule #2 this must be lest than.
				) {
				notpath = 0; // path found; (camino encontrado)
				Visit[rowNum * V + colNum] = true;
				tempath[Stepx] = graph[rowNum][colNum];
				//printf(" index:%d next: %d ", Stepx, tempath[Stepx]);
				 //printf("next: %d ", graph[rowNum][colNum]);
				Stepx += 1;
				int ret = recursive_search_path(graph, Visit, tempath, Stepx, rowNum, colNum);
				if (ret == -1) {
					//dead point or not return point
					Stepx = Stepx - 1;
				}
				else {

					if (ret >= length_path) {
						//NEW largest solution
						int currentLength = path[0] - path[length_path]; // diferencia path q existe
						int newLength = tempath[0] - tempath[ret]; // diferencia path encontrado

						//printf("%d-%d %d-%d\n", path[0], path[length_path], tempath[0], tempath[ret])
						//printf("currentLength %d, newLength %d \n", currentLength, newLength);

						if (newLength > currentLength)
						{
							memcpy(path, tempath, (ret + 1) * sizeof(int)); //copying the values to end solution.
							length_path = ret;
						}
					}
					if (ret >= 1)
						Stepx = Stepx - 1;
				}
			}
		}
	}

	if (!notpath) {
		return -1;
	}
	else {
		//I found a path.
		return Stepx - 1;
	}
}


// Funtion that implements like of Dijkstra single source largest path algorithm

void search_path(int graph[V][V], int indX, int indY) {

	int distance[V * V] = { 0 }; // initial distance is 0
	bool usedRegister[V * V]; // usedRegister[i] will true if vertex i is included in largest distance from src to i is finalized

	// Initialize all distances as 0 and stpSet[] as false
	for (int i = 0; i < V * V; i++)
		usedRegister[i] = false;
	// Distance of source vertex from itself is always 0 and explore all the nehibors
	usedRegister[indX * V + indY] = true;
	distance[0] = graph[indX][indY];
	//compute path.
	recursive_search_path(graph, usedRegister, distance, 1, indX, indY);
}

// A utility function to print the constructed distance array
void printSolution(int dist[V * V])
{
	int i = 0;
	printf("path: %i", dist[i]);
	for (i = 1; i <= V; i++) {
		if (dist[i] == 0) break;
		printf("-%i", dist[i]);
	}
	printf("\n\n");	
}



int main(int argc, char* argv[]) {

	int Stepx = -1;
	int Stepy = -1;
	bool all_solutions = false;

	// 4X4 Matrix Definition
	int graph[V][V] = {{ 4,8,7,3 },
					   { 2,5,9,3 },
					   { 6,3,2,5 },
					   { 4,4,1,6 }
	};

	if ((Stepx < 0 || Stepx > V - 1) || (Stepy < 0 || Stepy > V - 1)) {
		printf("*Solution for a 4X4 Matrix:\n");
		all_solutions = true;
	}


	if (all_solutions) {
		int best_path = 0;
		int b_path[V * V] = { 0 }; //Final best path

		for (int x = 0; x < V; x++)
			for (int y = 0; y < V; y++) {
				search_path(graph, x, y);
				if (length_path >= best_path) {
					memcpy(b_path, path, (length_path + 1) * sizeof(int)); //Copying the values to final solution.
					best_path = length_path;
				}
			}
		printSolution(b_path);

	}
	else {
		search_path(graph, Stepx, Stepy);
		printSolution(path);
	}

	return 0;
}
