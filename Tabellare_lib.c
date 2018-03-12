#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Tabellare_lib.h"
_Bool Controlla_Valori(int n_var, int n_min, int* arg)
{	
	int counter=0;
	for (int x=0; x<n_min; x++)	{	
		if(arg[x]<0)
			fprintf(stderr, "Errore: mintermine negativo (%d) non ammesso\n", arg[x]), ++counter;
		if(arg[x]>=power(n_var))
			fprintf(stderr, "Errore: mintermine %d non ammesso (%d variabili)\n\tMintermine massimo = %g\n", arg[x], n_var, power(n_var)-1), ++counter;
		for (int y=x+1; y<n_min; y++)
			if(arg[y] == arg[x])
				fprintf(stderr, "Errore: mintermine %d già inserito\n", arg[y]), ++counter;    	
	}
	if (!counter)
		return 0; 
	else
		return 1;
}

double power(unsigned int y)
{
  if(!y)
    return 1;
  else
    return 2*power(--y);
}

char** mtrxcpy(int m, int n, char** mtrx1, _Bool argv)
{	int a = argv ? 2 : 0;
	char** mtrx2 = calloc (m, sizeof(char*));
	for (int x=0; x<m; x++) 
	{
		mtrx2[x] = calloc (n, sizeof(char));
		for(int y=0; y<n; y++)
			mtrx2[x][y] = mtrx1[x+a][y];
	}
	return mtrx2;
}

char** Bin(int var, int min, int* n)	//completa magia
{	
	char** bin = calloc (min, sizeof(char*));
	for(int y=0; y<min; y++) {	
		bin[y] = calloc (var+1, sizeof(char));
		for(int x=var-1; x>=0; x--, n[y]/=2)
			bin[y][x] = n[y]%2 +'0';
		bin[y][var]='\0';
	}
	return bin;
}

void Stampa_var (int var)
{
	char lit = 'A';
	for(int x=0; x<var; x++)
		printf("%c", lit+x);
	puts("");
}

void Scambia(char* arr1, char* arr2, int n_var)
{
  int x= -1;
  char tmp;
  while(++x <= n_var || (arr1[x]!='\0' || arr2[x]!='\0'))
    {
      tmp= arr2[x];
      arr2[x]= arr1[x];
      arr1[x]= tmp;
    }
}

int N_uni (int x, int n_var, char** Emin)
{	
	int counter=0;
	for (int y=0; y<n_var; y++)
		if(Emin[x][y]=='1')
			counter++;
	return counter;
}

void Raggruppa_min (char** min, int n_min, int n_var, char** Emin)
{
	for (int x=0; x<n_min-1; x++)
		for (int y=0; y<n_min-x-1; y++)
			if(N_uni(y, n_var, Emin) > N_uni(y+1, n_var, Emin)) 
			{
				Scambia(Emin[y], Emin[y+1], n_var);		
				Scambia(min[y], min[y+1], 64);
			}

}

int N_gruppi (int n_min, int n_var, char** Emin)
{
	int counter=1;
	for (int x=1; x<n_min-1; x++)
		if(N_uni(0, n_var, Emin) != N_uni(x, n_var, Emin)) 
			if(N_uni(x, n_var, Emin) != N_uni(x+1, n_var, Emin))
				counter++;
	return counter+1;	

}

void Stampa_min (char** min, int n_min, int n_var, char** Emin)
{	
	puts("");
	for (int x=0; x<n_min; x++)
		printf("%s  |  %s\n", Emin[x], min[x]);
	puts("");
	for (int x=0; x<n_var+5+strlen(min[n_min-1])+1; x++)
		printf("-");
	puts("");
}

_Bool int_cmp(int x, int a, int* v)
{
	for (int y=0; y<=a; y++)
		if(x==v[y])
			return 1;
	return 0;
}

void Crea_impl(int implicante, int x)
{
	if(!implicante) { 
		if(!++ctr1 && !++first)	
			implicanti = calloc(1, sizeof(char*));
		else
			implicanti = realloc(implicanti, (ctr1+1)*sizeof(char*));
		implicanti[ctr1] = calloc(64, sizeof(char));
		strcpy(implicanti[ctr1], d_Emin[x]);
	}
}
void Crea_impl_bin(int implicante, int x, int n_var)
{	 
	if(!implicante) { 
		int tmp = ctr1-1;
		if(!++tmp && !++first)	
			bin_impl = calloc(1, sizeof(char*));
		else
			bin_impl = realloc(bin_impl, (tmp+1)*sizeof(char*));
		bin_impl[tmp] = calloc(n_var, sizeof(char));
		strcpy(bin_impl[tmp], b_Emin[x]);
	}
}

_Bool Confronta_min(int *n_min, int n_var)
{	printf(" ");
	int gruppo, implicante, counter=0, a=-1, v[1024], tmp;
	ctr=-1;
	for (int x=0; x<*n_min; x++) {
		implicante=0, gruppo = 1; //gruppo uguale
		for (int y=x+1; y<*n_min; y++)
			if(N_uni(x, n_var, b_Emin) != N_uni(y, n_var, b_Emin) && gruppo) {
				tmp=implicante;
				implicante+=Semplifica_bit(x, y, n_var);
				if (implicante!=tmp && implicante)
					v[++a] = y;
				if(y<*n_min-1)
					if(N_uni(y, n_var, b_Emin) != N_uni(y+1, n_var, b_Emin))
						gruppo = 0;  //gruppo successivo diverso
			}
		if(a!=-1 && int_cmp(x, a, v))
			continue;
		Crea_impl(implicante, x);
		Crea_impl_bin(implicante, x, n_var);
	}
	*n_min = ctr+1;
	if(!*n_min)
		return 1;
	b_Emin = mtrxcpy(*n_min, n_var, Emin_new, 0);
	d_Emin = mtrxcpy(*n_min, 64, min_new, 0);
	Free_mtrx(*n_min, Emin_new);
	Free_mtrx(*n_min, min_new);
	Stampa_min (d_Emin, *n_min, n_var, b_Emin);
	return 0;
}

_Bool strcomp(int m, int n, char** mtrx)
{
	int counter=0, z;
	for (int x=0; x<m; x++) {
			for(z=0; z<n; z++)
				if(mtrx[m][z]!=mtrx[x][z])
					break;
			if (z==n)
				return 1;
		}
	return 0;
}

_Bool Semplifica_bit(int x, int y, int n_var)
{	
	int counter=0, sign=0;
	for (int a=0; a<n_var; a++)
		if(b_Emin[x][a]!=b_Emin[y][a])
			counter++, sign=a;
	if (counter==1)	{	
		++ctr;
		if(ctr) 
			Emin_new = realloc(Emin_new, (ctr+1)*sizeof(char*)), min_new = realloc(min_new, (ctr+1)*sizeof(char*));
		else 
			Emin_new = calloc(1, sizeof(char*)), min_new = calloc(1, sizeof(char*));
		Emin_new[ctr] = calloc(n_var+1, sizeof(char));
		min_new[ctr] = calloc(64, sizeof(char));
		strcpy(Emin_new[ctr], b_Emin[x]); 
		Emin_new[ctr][sign]='-';
		strcpy(min_new[ctr], d_Emin[x]);
		strcat(min_new[ctr], ",");
		strcat(min_new[ctr], d_Emin[y]);
		if(ctr)
			if (strcomp(ctr, n_var, Emin_new)) { 
				free(min_new[ctr]);
				free(Emin_new[ctr]);
				ctr--;
				}
		return 1;	
	}
	else return 0;	
}
int impl_len (int x, char** tmp)
{
	int imp=0; 
	for (int y=0; tmp[x][y]!='\0'; y++)
		if(tmp[x][y]!=',') {
			imp++;
			if(y!=strlen(tmp[x])-1 && tmp[x][y+1]!=',')
				imp--;
		}
	return imp;
}

int N_impl (int len, char** arr)
{	
	int imp=0;
	for (int x=0; x<len; x++)
		imp +=impl_len (x, arr);
	return imp;
}

int* atoiz(char** arr1, int len)
{	
	int z=0;
	int n_impl = N_impl(len, arr1);
	int* arr2 = calloc(n_impl, sizeof(int));
	for (int x=0, a=0; x<len; x++, a++){
			arr2[a]=atoi(arr1[x]);
			for (int y=1; y<strlen(arr1[x]);y++)	
				if(arr1[x][y]==',')
					arr2[++a]=atoi(arr1[x]+(y+1));		//*3*
	}
	return arr2;
}

void Copri_tab (char** tabella, int len, int n_min)
{
	int counter=0, var=0;
	for (int x=0; x<n_min; x++)
	{	
		counter=0;
			for (int y=0; y<len; y++)
				if(tabella[y][x]=='P')
					for (int z=0; z<len; z++)
						if (tabella[y][x]!=tabella[z][x])
							counter++, var=y;
		if (counter==len-1)
			tabella[var][x]='E';
	}
	
	for (int x=0; x<n_min; x++)			
		for (int y=0; y<len; y++)
			if (tabella[y][x]=='E')
				for(int z=0; z<n_min; z++)
					if(tabella[y][z]=='P') {
						tabella[y][z]='x';
						for (int w=0; w<len; w++)	//°3°
							if(tabella[w][z]=='P')
								tabella[w][z]='x';	
			 }
}

void Stampa_tab(char** tabella , int len, int n_min, int* arg)
{
	len-1>9 ? printf("     ") : printf("    ");
	for (int x=0; x<n_min; x++)
		printf("  %d", arg[x]);
	for (int x=0; x<len; x++) {
		x>9 ? printf("\nP(%d)", x) : printf("\nP(%d) ", x);
		for (int y=0; y< n_min; y++)
			if(tabella[x][y]=='\0')
				arg[y]>9 ? printf("    ") : printf("   ");
			else
				arg[y]>9 ? printf("   %c", tabella[x][y]) : printf("  %c", tabella[x][y]);
	}
	puts("\n");
}


char** Tab_Copertura(int len, char** implicanti, int n_min, int*arg)
{	
	int n_impl = N_impl(len, implicanti);
	int* impl = atoiz(implicanti, len);
	char** tabella = calloc (len, sizeof(char*));
	for (int x=0, a=0, y, z; x<len; a+=impl_len(x, implicanti), x++) {
		tabella[x] = calloc(n_min, sizeof(char));
		for (z=a; z<a+impl_len(x, implicanti); z++) { 
			for (y=0; y<n_min; y++)
				if(impl[z] == arg[y])
					tabella[x][y]='P';
		}
	}
 	Copri_tab (tabella, len, n_min);
 	Stampa_tab (tabella , len, n_min, arg);
 	return tabella;
}

int int_cmp2(int x, int a, int* v)
{
	for (int y=0; y<=a; y++)
		if(x==v[y])
			return y;
	return -1;
}

_Bool Implicanti_Primi (int len, int n_min, char** tabella, int* arg) //il più magico
{
	int counter, counter2, a=-1, b=-1, x_tmp=0, incr=0, incr2=0;
	impl_primi = mtrxcpy(len, n_min, tabella, 0);
	impl_primi_ReC = calloc(2, sizeof(int*));
	for (int x=0; x<len; ++x){	//per righe
		counter2=0;
		for (int y=0; y<n_min; y++)
			if(impl_primi[x][y]!='P')
				counter2++;
		if(counter2==n_min){
			--len;
			for(int y=x; y<len; y++)
				for (int z=0; z<n_min;z++)
					impl_primi[y][z] = impl_primi[y+1][z];
			--x, ++incr;
		}
		else{
			if(!++b)
				impl_primi_ReC[0] = calloc(1, sizeof(int));
			else
				impl_primi_ReC[0] = realloc(impl_primi_ReC[0], (b+1)*sizeof(int));
			impl_primi_ReC[0][b] = x+incr;
		}
	}
	for (int x=0; x<n_min; x++){	//per colonne
		counter=0;
		for (int y=0; y<len; y++)
			if(impl_primi[y][x]!='P')
				counter++;
		if(counter==len){
			--n_min;
			for(int z=x; z<n_min; z++)
				for (int w=0; w<len; w++)
					impl_primi[w][z] = impl_primi[w][z+1];
			++incr2, --x;
		}
		else{
			if(!++a)
				impl_primi_ReC[1] = calloc(1, sizeof(int));
			else
				impl_primi_ReC[1] = realloc(impl_primi_ReC[1], (a+1)*sizeof(int));
			impl_primi_ReC[1][a] = arg[x+incr2];
		}
	}
	s=len-1, r=n_min-1;
	if(s<=0&&r<=0)
		return 0;
	else return 1;
}

void Stampa_tab_primi()
{
	impl_primi_ReC[0][s]>9 ? printf("     ") : printf("    ");
	for (int x=0; x<=r; x++)
		printf("  %d", impl_primi_ReC[1][x]);
	for (int x=0; x<=s; x++) {
		impl_primi_ReC[0][x]>9 ? printf("\nP(%d)", impl_primi_ReC[0][x]) : printf("\nP(%d) ", impl_primi_ReC[0][x]);
		for (int y=0; y<=r; y++) 		
			if(impl_primi[x][y]=='\0')
				impl_primi_ReC[1][y]>9 ? printf("    ") : printf("   ");
			else
				impl_primi_ReC[1][y]>9 ? printf("   %c", impl_primi[x][y]) : printf("  %c", impl_primi[x][y]);		
	}
	puts("");
}

void Colonne()
{
	Stampa_tab_primi();
}

void Righe()	//insane
{
	int max=0, max_tmp=0;
	for (int x=0; x<=s; x++){
		max=0;
		for (int y=0; y<=r; y++)
			if(impl_primi[x][y]=='P')
				max++;
		if(x && max<max_tmp)
			max=max_tmp;
		max_tmp=max;//****************
		}
	int couter, couter1, counter=0, counter_tmp=0, arr_col[1024], b=-1;
	for (int x=0; x<=s; x++)
	{
		counter=0, couter=0, couter1=0;
		for (int y=0; y<=r; y++) 			
			if(impl_primi[x][y] == 'P')
				++counter;
		if(counter_tmp<counter&&x){
			for (int y=0; y<=r; y++)
				if(impl_primi[x][y] == 'P')
					impl_primi[x][y]='E', arr_col[++b]=y;	
			for (int y=0; y<=r; y++)
				if(impl_primi[x-1][y] == 'E')
					impl_primi[x-1][y]='x';
		}
		else if (counter_tmp>counter&&x)
			for (int y=0; y<=r; y++){
				if(impl_primi[x-1][y] == 'P' ^ impl_primi[x-1][y] == 'x')
					impl_primi[x-1][y]='E', arr_col[++b]=y;	
			}
		else if (counter_tmp==counter&&x){
				for (int y=0; y<=r; y++){
					if(impl_primi[x-1][y] != '\0')
						if(int_cmp(y, b, arr_col))
							couter++;
					if(impl_primi[x][y] != '\0')
						if(int_cmp(y, b, arr_col))
							couter1++;
				}
				if(couter > couter1){
					for (int y=0; y<=r; y++)
						if(impl_primi[x][y] == 'P' ^ impl_primi[x][y] == 'x')
							impl_primi[x][y]='E', arr_col[++b]=y;

					}
				else if(couter<=couter1)
					for (int y=0; y<=r; y++)
						if(impl_primi[x-1][y] == 'P' ^ impl_primi[x-1][y] == 'x')
							impl_primi[x-1][y]='E', arr_col[++b]=y;	
			 }
		counter_tmp=counter;
	}
	int arr_col1[1024], c=-1;
	for (int x=0; x<=s; x++){
		counter=0, counter_tmp=0;
		for (int y=0; y<=r; y++)
			if(impl_primi[x][y]=='E'){
				counter++, arr_col1[++c]=y;
				if(c && int_cmp(y, c-1, arr_col1))
					counter_tmp++;
			}
			else if(impl_primi[x][y] == 'P')
					impl_primi[x][y]='x';
		if(x && counter==counter_tmp)
			for(int y=0; y<=r; y++)
				if(impl_primi[x][y]=='E')
					impl_primi[x][y]='x';
	}
	int arr_rig[1024], d=-1, r_count,  n_x, n_x_tmp;
	for (int x=0; x<=r; x++){
		counter=0, d=-1;
		for (int y=0; y<=s; y++){
			if(impl_primi[y][x]=='E')
				counter++, arr_rig[++d]=y;
		}
		if(counter>1)
			for (int y=0; y<=d; y++){
				r_count=0;
				for (int z=0; z<=r; z++)
					if(impl_primi[arr_rig[y]][z]=='E')
						++r_count;
				if(r_count<max)
					for (int z=0; z<=r; z++)
						if(impl_primi[arr_rig[y]][z]=='E')
							impl_primi[arr_rig[y]][z]='x';
			}	
	}
	for (int x=0; x<=r; x++){
		counter=0;
		for (int y=0; y<=s; y++)
			if(impl_primi[y][x]=='E')
				counter++;
		if(!counter)
			for (int y=0; y<=s; y++){
				n_x=0;
				if(impl_primi[y][x]=='x'){
					for (int z=0; z<=r; z++)
						if(impl_primi[y][z]=='x')
							n_x++;
				}
				else 
					continue;
				if(y && n_x>=n_x_tmp)
					for (int z=0; z<=r; z++)
						if(impl_primi[y][z]=='x')
							impl_primi[y][z]='E';
				n_x_tmp=n_x;
			}
	}
	Stampa_tab_primi();
}

int Implicanti_tab(int len, int n_min, char** tabella_copertura, int* coperto)
{
	int counter, j=-1;
	for (int x=0; x<len; x++) {
		counter=0;
		for (int y=0; y<n_min; y++){
			if(tabella_copertura[x][y]=='x' ^ tabella_copertura[x][y]=='\0')
				counter++;
		}
		if(counter==n_min){printf("%d\n", x);
			coperto[++j]=x;
		}
	}
	return j;
}

void Stampa_Fine (_Bool primi, int len, int n_var, int* coperto, _Bool minima, int cop)
{	
	int counter=-1;
	for (int x=0; x<=s; x++)
		for(int y=0; y<=r; y++)
			if(impl_primi[x][y]=='E'){
				impl_primi_ReC[0][x]=-1;
				break;
			}
	for(int x=0, y=0; x<len; x++, y++)
		if(primi && int_cmp(x, y, impl_primi_ReC[0]))
			continue;	
		else{
			if(cop>=0)
				if(int_cmp(x, cop, coperto))
					continue;
			if (!minima)
					printf("P(%d) = %s | %s\n", x, bin_impl[x], implicanti[x]);
			else {
				++counter;
				if(counter) 
					printf(" + ");
				for (int z=0; z<n_var; z++)
					if(bin_impl[x][z]=='0')
						printf("%c", 'a'+z);
					else if (bin_impl[x][z]=='1')
						printf("%c", 'A'+z);
			}
		}

}

void Free_mtrx(int n, char** mtrx)
{
	for (int x=0; x<n; x++)
		free(mtrx[x]);
	free(mtrx);
}