#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include <string.h>
#include "compute.h"

float ComputeImageAuto(guchar *pucImaOrig, int NbLine, int NbCol, guchar *pucImaRes, int nbClasse, int threshold)
{
  int iNbPixelsTotal, iNumPix;
  int iNbChannels = 3;
  iNbPixelsTotal = NbCol * NbLine;
  float count = 0.0;
  // Vs = (Pixel S, Haut, Droite, Bas, Gauche, Moyenne, Classe)
  greyScale(pucImaOrig, pucImaRes, iNbPixelsTotal, iNbChannels);
  int** Vs = malloc(sizeof(*Vs) * NbLine * NbCol);
  kMeans(Vs, iNbPixelsTotal, pucImaRes, iNbChannels, nbClasse, NbCol, NbLine);
  for (iNumPix = 0; iNumPix < iNbPixelsTotal * iNbChannels; iNumPix = iNumPix + iNbChannels)
  {
    Vs[iNumPix / 3][6] >= (threshold - 1) ? count++ : count;
  }
  return count*100/iNbPixelsTotal;
}

float calculPercentage(char* name, int nbClasse, int threshold)
{
  GdkPixbuf *pixbuf;
  GError *error = NULL;

  pixbuf = gdk_pixbuf_new_from_file (name, &error);
  if (!pixbuf)
  {
  	printf("%s\n", error->message);
  	g_clear_error (&error);
    exit(1);
  }
  guchar *pucImaOrig = gdk_pixbuf_get_pixels(pixbuf);
  guchar *pucImaRes =gdk_pixbuf_get_pixels(pixbuf);
  int NbCol, NbLine;
  NbCol = gdk_pixbuf_get_width(pixbuf);
  NbLine = gdk_pixbuf_get_height(pixbuf);

  return ComputeImageAuto(pucImaOrig, NbLine, NbCol, pucImaRes, nbClasse, threshold);
}

void printAllPercentage(int nbClasse, int threshold)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      char *pch = strrchr(dir->d_name, '.');
      if (pch && (strcmp(pch, ".bmp") == 0 || strcmp(pch, ".jpg") == 0 || strcmp(pch, ".png") == 0))
        printf("%s => %f%%\n", dir->d_name, calculPercentage(dir->d_name, nbClasse, threshold));
    }
    closedir(d);
  }
}

int main(int argc, char* argv[])
{
  if (argc == 3)
    printAllPercentage(atoi(argv[1]), atoi(argv[2]));
  else
    printAllPercentage(8, 7);
  return(0);
}
