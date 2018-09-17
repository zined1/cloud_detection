#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto:


  But:

  Entrees:
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes � la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes � la suite des autres)


  Sortie:

  Rem:

  Voir aussi:

  ---------------------------------------*/

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


void ComputeImage(guchar *pucImaOrig, int NbLine, int NbCol, guchar *pucImaRes, int nbClasse, int threshold)
{
  int iNbPixelsTotal, iNumPix;
  int iNumChannel, iNbChannels = 3;
  iNbPixelsTotal=NbCol*NbLine;
  // Vs = (Pixel S, Haut, Droite, Bas, Gauche, Moyenne, Classe)

  greyScale(pucImaOrig, pucImaRes, iNbPixelsTotal, iNbChannels);
  int** Vs = malloc(sizeof(*Vs) * NbLine * NbCol);

  printf("Segmentation de l'image....\n");
  kMeans(Vs, iNbPixelsTotal, pucImaRes, iNbChannels, nbClasse, NbCol, NbLine);
  for (iNumPix = 0; iNumPix < iNbPixelsTotal * iNbChannels; iNumPix = iNumPix + iNbChannels)
    for (iNumChannel=0; iNumChannel<iNbChannels; iNumChannel++)
      pucImaRes[iNumPix+iNumChannel] = Vs[iNumPix / 3][6] >= (threshold - 1) ? 255 : 0;
  /* FREE */
  for (int i = 0; i < iNbPixelsTotal; i++)
    free(Vs[i]);
  free(Vs);
}

int** kMeans(int **Vs, int iNbPixelsTotal, guchar* pucImaRes, int iNbChannels, int nbClasse, int NbCol, int NbLine)
{
  int **Vc = malloc(sizeof(*Vc) * nbClasse);

  ComputeNeighbours(Vs, pucImaRes, NbLine, NbCol, iNbChannels); // Creation de Vs
  ComputeMass(Vc, nbClasse); // Initialisation de la "Mass"
  for (int i = 0; i < iNbPixelsTotal; i++)
  {
    qsort (Vs[i], 5, sizeof(*Vs[i]), compare);
    Vs[i][6] = Classify(Vs[i], Vc, nbClasse); // Classifie Vs
  }
  Vs = Update(Vs, Vc, iNbPixelsTotal, nbClasse); // Algo K-Mean modifi�
  for (int i = 0; i < nbClasse; i++)
    free(Vc[i]);
  free(Vc);
  return Vs;
}

void greyScale(guchar *pucImaOrig, guchar *pucImaRes, int iNbPixelsTotal, int iNbChannels)
{
  for (int iNumPix=0; iNumPix < iNbPixelsTotal * iNbChannels; iNumPix = iNumPix + iNbChannels)
  {
    guchar ucMeanPix = (unsigned char)((*(pucImaOrig + iNumPix) + *(pucImaOrig + iNumPix + 1) + *(pucImaOrig + iNumPix + 2)) / 3);
    for (int iNumChannel = 0; iNumChannel < iNbChannels; iNumChannel++)
      pucImaRes[iNumPix + iNumChannel] = ucMeanPix;
  }
}

int** ComputeMass(int** Vc, int nbClasse) // Fonction qui cr�e les intervalles (Vc)
{
  for (int i = 0; i < nbClasse; i++)
  {
    Vc[i] = malloc(sizeof(int) * 5);
    for (int j = 0; j < 5; j ++)
      Vc[i][j] = (256 * i) / nbClasse + 256 / (2 * nbClasse);
  }
  return Vc;
}

int** Update(int** Vs, int** Vc, int iNbPixelsTotal, int nbClasse) // Algo principal k-means
{
  while (1)
  {
    //Evite de parcourir 8 fois l'image
    int *nombre = calloc(nbClasse, sizeof(int));
    int *somme = calloc(nbClasse, sizeof(int));
    int *new_Vc = calloc(nbClasse, sizeof(int));

    for (int i = 0; i < iNbPixelsTotal; i++)
    {
      nombre[Vs[i][6]]++; // nombre[Numero de la classe] += 1
      somme[Vs[i][6]] += Vs[i][5]; // somme[Numero de la classe] += Moyenne des pixels de Vs
    }
    for (int i = 0; i < nbClasse; i++)
      if (nombre[i] == 0)
	      new_Vc[i] = 0;
      else
	      new_Vc[i] = somme[i] / nombre[i]; //Nouvelle moyenne
    if (equal(Vc, new_Vc, nbClasse)) // Si les moyennes sont les m�mes => stabilit�
    {
      free(nombre);
      free(somme);
      free(new_Vc);
      break;
    }
    else
    {
      for (int i = 0; i < nbClasse; i++)
      {
        for (int j = 0; j < 5; j++)
          Vc[i][j] = new_Vc[i]; //On update Vc
      }
      for (int i = 0; i < iNbPixelsTotal; i++)
	Vs[i][6] = Classify(Vs[i], Vc, nbClasse); //Update des nouvelles classes
      free(nombre);
      free(somme);
      free(new_Vc);
    }
  }
  return Vs;
}


int Classify(int* pixel, int** Vc, int nbClasse) //Donne la classe du pixel
{
  int diff = abs(Vc[0][0] - pixel[5]); //on d�finit une "base"
  int j = 0;
  for (int i = 1; i < nbClasse; i++)
  {
    int value = abs(Vc[i][0] - pixel[5]); //simple algo de recherche de min
    if (value < diff)
    {
      diff = value;
      j = i;
    }
  }
  return j;
}

int** ComputeNeighbours(int** Vs, guchar *pucImaRes, int NbLine, int NbCol, int iNbChannels) // Construit Vs pour les voisins
{
  if (Vs)
  {
    for (int i = 0; i < NbLine * NbCol; i++)
    {
      Vs[i] = malloc(sizeof(int)* 7); // Ordre : Millieu, Haut, Droite, Bas, Gauche (sens horloge)
      for (int j = 0; j < 7; j++)
	Vs[i][j] = -1;
    }
  }
  int z = 0;
  for (int i = 0; i < NbLine; i++)
  {
    for (int j = 0; j < NbCol; j++)
    {
      Vs[z][0] = pucImaRes[z * iNbChannels];
      if (i != 0)
      {
        //printf("z: %d | i: %d | j: %d | Nbline: %d | iNbChannels: %d\n", z, i, j, NbLine, iNbChannels);
        Vs[z][1] = pucImaRes[(z - NbCol) * iNbChannels];
      }
      if (i != NbLine - 1)
        Vs[z][3] = pucImaRes[(z + NbCol) * iNbChannels];
      if (j != 0)
	Vs[z][4] = pucImaRes[(z - 1) * iNbChannels];
      if (j != NbCol - 1)
	Vs[z][2] = pucImaRes[(z + 1) * iNbChannels];
      z++;
    }
  }
  for (int i = 0; i < NbLine * NbCol; i++)
    Vs[i][5] = mean(Vs[i]);
  return Vs;
}

int compare (void const *a, void const *b) // Compare pour qsort
{
  int const *pa = a;
  int const *pb = b;
  return *pb - *pa;
}

int mean(int* V) //Calcul la moyenne des pixels
{
  int count = 0;
  for (int i = 0; i < 5; i++)
    if (V[i] >= 0)
      count += V[i];
  return count / 5;
}

int equal(int **a, int *b, int nbClasse) // Permet de comparer le nouveau Vc et l'ancien
{
  for (int i = 0; i < nbClasse; i++)
    if (a[i][0] != b[i])
      return 0;
  return 1;
}
