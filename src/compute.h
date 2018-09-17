
#include <gtk/gtk.h>

#define LargeurImage 360

void ComputeImage(guchar *pucImaOrig,
		  guint uiNbCol,
		  guint uiNbLine,
		  guchar *pucImaRes,
			int nbClasse,
			int threshold);

/* NOS FONCTIONS */
int** kMeans(int**, int, guchar*, int, int, int, int);

int** ComputeNeighbours(int**, guchar *, int, int, int);
int** ComputeMass(int**, int);
int** Update(int**, int**, int , int);
int Classify(int*, int**, int);

void greyScale(guchar*, guchar*, int, int);
int compare (void const*, void const*);
int mean(int*);
int equal(int **, int *, int);
