#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Tabellare_lib.h" 

int main (int argc, char *argv[])
{	
	if(argc<2){
		fprintf(stderr, "Errore: numero variabili e/o mintermini non inseriti\n");
          return 1;
	}
	first=-1, ctr1=-1;
	_Bool primi=0;
	int n_var = atoi(argv[1]), n_min=argc-2, arg[n_min-1], arg_cpy[n_min-1], n_min_tmp=n_min, n_min_f=n_min;
	for(int x=0; x<n_min; x++)
		arg[x] = arg_cpy[x] = atoi(argv[x+2]);	//min decimali
	if(Controlla_Valori(n_var, n_min, arg))
		return 1;
	d_Emin = mtrxcpy(n_min, 64, argv, 1);
	b_Emin = Bin(n_var, n_min, arg);	//Alloco mintermini binari
	Raggruppa_min (d_Emin, n_min, n_var, b_Emin);
	puts("\n##### RICERCA IMPLICANTI PRIMI #####\n");
	Stampa_var(n_var);
	Stampa_min (d_Emin, n_min, n_var, b_Emin);
	while(!Confronta_min (&n_min, n_var))
		if(n_min)
			n_min_f=n_min;	//per free
	int len = ctr1+1;
	puts("\nImplicanti Primi");			//stampo implicanti primi.
	for(int x=0; x<len; x++)
		printf("P(%d) = %s\n", x, implicanti[x]);
	puts("\n##### TABELLA DI COPERTURA #####\nE = Implicanti Primi Essenziali\nP = Implicanti Primi\nx = Implicanti coperti\n");
	char** tabella_copertura = mtrxcpy(len, n_min_tmp, Tab_Copertura(len, implicanti, n_min_tmp, arg_cpy), 0);
	int coperto[len-1];
	int cop = Implicanti_tab(len, n_min_tmp, tabella_copertura, coperto);
	if (Implicanti_Primi(len, n_min_tmp, tabella_copertura, arg_cpy))
		if(s+1<r+1) {
			puts("##### DOMINANZA PER COLONNE #####\nE = Implicanti Primi Essenziali\nx = Implicanti coperti\n");
			Colonne();
			++primi;
		}
		else {
			puts("##### DOMINANZA PER RIGHE #####\nE = Implicanti Primi Essenziali\nx = Implicanti coperti\n");
			Righe();
			++primi;
		}
	puts("\n##### IMPLICANTI ESSENZIALI #####\n");
	Stampa_Fine(primi, len, n_var, coperto, 0, cop);
	puts("\n##### ESPRESSIONE MINIMA #####\nvariabile minuscola = variabile negata\n");
	Stampa_Fine(primi, len, n_var, coperto, 1, cop);
	puts("\n");		

	Free_mtrx(n_min_f, d_Emin);
	Free_mtrx(n_min_f, b_Emin);
	Free_mtrx(len, tabella_copertura);
	Free_mtrx(len, implicanti);
	Free_mtrx(s, impl_primi);
	Free_mtrx(len, bin_impl);
	return 0;
}