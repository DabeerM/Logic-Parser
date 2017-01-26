#include <stdio.h> 
#include <string.h>   
#include <stdlib.h>  

const int Fsize=50;
int no_edges;
int no_nodes;
int i;
const int cases=10;

/*Functions needed for parsing */
int bin_con_pos(char * g){ //Gets the position of the binary connective
	int bin_count = 0;
	int i, j, bin_con = 0;
	for (i = 0; i < strlen(g); i++){
		if ((bin_count==1)&&(g[i]=='v'||g[i]=='^'||g[i]=='>')){ //Binary connective can only occur when: # of '(' > # of ')'
			bin_con = i;
		}
		
		if (g[i]=='('){bin_count++;}
		else if (g[i]==')'){bin_count--;}
		if (bin_con>0){return bin_con;}
	}
	return 0;
}

int parse(char *g) //Checks if formula is valid recursively by 
{
	if ((g[0]=='X')&&(g[1]=='[')&&(g[2]=='x'||g[2]=='y'||g[2]=='z')&&(g[3]=='x'||g[3]=='y'||g[3]=='z')&&(g[4]==']')){return 1;} //i.e. atomic formula
	else if ((g[0]=='-')&&(parse(g+1)>0)){return 2;} //i.e. negative
	else if ((g[0]=='(')&&(g[strlen(g)-1]==')')){ //i.e. binary connective
		int bin_con = bin_con_pos(g); 
		if (bin_con>0){
			char part1[50];
			char part2[50];
			int x = 0, y = 0;
			while (x<bin_con-1){
				part1[x] = g[x+1];
				x++;
			}
			part1[bin_con-1]='\0';
			while (y<strlen(g)-(bin_con+1)-1){
				part2[y]=g[bin_con+1+y];
				y++;
			}
			part2[strlen(g)-(bin_con+1)-1] = '\0';
			if ((parse(part1)>0)&&(parse(part2)>0)){return 3;}
		}
		
	}
	else if ((g[0]=='E')&&(g[1]=='x'||g[1]=='y'||g[1]=='z')){ //Existential formula
		if (parse(g+2)>0){
			return 4;
		}
	} 
	else if ((g[0]=='A')&&(g[1]=='x'||g[1]=='y'||g[1]=='z')){ //Existential formula
		if (parse(g+2)>0){
			return 5;
		}
	} 
	return 0;
	//Check which type of formula it is by checking the first character. If it starts w/: '(' it's binary connective, 'E' Existential, 'A' Universal, '-' Negative & 'X' Atomic
	//See if it's a valid formula recursively
	/* return 1 if atomic, 2 if  neg, 3 if binary, 4 if exists, 5 if for all, ow 0*/
}

/*Functions needed to evalute formula*/
int var_ass(char *E, int V[3], int edges[no_edges][2]){
    int i;
    int node1,node2;

    // Assign nodes respective values
    switch(E[0]){
        case 'x' : node1 = V[0]; break;
        case 'y' : node1 = V[1]; break;
        case 'z' : node1 = V[2]; break;
        case '0' : node1 = 0; break; //i.e. the variable has been assigned a number via existential or universal
        case '1' : node1 = 1; break;
        case '2' : node1 = 2; break;//i.e. the variable has been assigned a number
    }
 
    switch(E[1]){
        case 'x' : node2 = V[0]; break;
        case 'y' : node2 = V[1]; break;
        case 'z' : node2 = V[2]; break;
        case '0' : node2 = 0;    break; //i.e. the variable has been assigned a number via existential or universal
        case '1' : node2 = 1;    break;
        case '2' : node2 = 2;    break;
    }
	
    for( i = 0; i < no_edges; i++){
        if (node1 == edges[i][0]){
            if(node2 == edges[i][1]){
                return 1;
	    }
	}
    }
    return 0;
}

int atomic(char*nm,int edges[no_edges][2],int V[3]){ //Evaluates any atomic formula //Tested Works
	
	char X[3];
	X[0] = nm[2];
	X[1] = nm[3];
	X[2] = '\0';
	return var_ass(X, V, edges); 
}

int negated(char*nm,int edges[no_edges][2],int V[3]){ 
	return (eval(nm+1,edges,no_edges,V)==0); //This is equivalent to inverting a statement
}

int bin_pos(char*str){
	int left_brac=0,right_brac=0;
	for (int i = 0; i < strlen(str); i++){
		if (left_brac==right_brac+1){
			if ((str[i]=='v')||(str[i]=='^')||(str[i]=='>')){return i;}
		}
		if (str[i]=='('){left_brac++;}
		else if (str[i]==')'){right_brac++;}
	}
	return 0; //i.e. no bin connective
}

char* first_half(char*form){
	int binary_operator = bin_pos(form);
	char new_form[binary_operator];
	char * new_str = (char*)malloc(sizeof(new_form));
	for (int i=1;i<binary_operator;i++){
		new_str[i-1]=form[i];
	}
	return new_str;
}

char* second_half(char*form){
	int binary_operator = bin_pos(form);
	char new_form[strlen(form)-binary_operator];
	char * new_str2 = (char*)malloc(sizeof(new_form));
	for (int i=binary_operator+1; i<strlen(form)-1; i++){
		new_str2[i-(binary_operator+1)]=form[i];
	}
	return new_str2;
}

int bin_con(char*nm,int edges[no_edges][2],int V[3]){ 
	int binary_connective = bin_pos(nm);
	int part1=eval(first_half(nm),edges,no_edges,V);
	int part2=eval(second_half(nm),edges,no_edges,V);
	
	switch (nm[binary_connective]){
		case 'v' : if(part1 == 1 || part2 == 1){
                        return 1;
                    } break;
        	case '^' : if(part1 == 1 && part2 == 1){
                        return 1;
                    } break;
       		case '>' :  if(part1 == 1 && part2 == 1){
                        return 1;
                    }
                    else if(part1 == 0){
                        return 1;
                    }
                    else{
                        return 0;
                    } break;
        default : return 0;
	}
}
 
char* replace_all(char char_to_replace,char*nm,int value){ //Replaces all characters in the string with letter 'n' where En(some formula)
	char new_form[strlen(nm)];
	char * new_str = (char*)malloc(sizeof(new_form));
	for (int i = 0; i < strlen(nm); i++){
		if (nm[i]==char_to_replace){
			new_str[i]=value; 
		}
		else{
			new_str[i]=nm[i];
		}
	}
	return new_str;
}

int existential(char*some_form,int edges[no_edges][2],int V[3]){
	
	int first  = eval(replace_all(some_form[1],some_form+2,'0'),edges,no_edges,V);
	int second = eval(replace_all(some_form[1],some_form+2,'1'),edges,no_edges,V);
	int third  = eval(replace_all(some_form[1],some_form+2,'2'),edges,no_edges,V);
	if (first==1||second==1||third==1){
		return 1;
	}
	return 0;
}

int universal(char*some_form,int edges[no_edges][2],int V[3]){
	
	int first  = eval(replace_all(some_form[1],some_form+2,'0'),edges,no_edges,V);
	int second = eval(replace_all(some_form[1],some_form+2,'1'),edges,no_edges,V);
	int third  = eval(replace_all(some_form[1],some_form+2,'2'),edges,no_edges,V);
	if (first==1&&second==1&&third==1){
		return 1;
	}
	return 0;
}

int eval(char *nm, int edges[no_edges][2], int size, int V[3])
{
	switch(nm[0]){ //Don't care about formula validation as it has already been verified by 'parse'
		case 'X':return atomic(nm,edges,V);
		case '-':return negated(nm,edges,V);
		case '(':return bin_con(nm,edges,V);
		case 'E':return existential(nm,edges,V);
		case 'A':return universal(nm,edges,V);
		default:return 0;
	}/* returns 1 if formula nm evaluates to true in graph with 'size' nodes, no_edges edges, edges stored in 'edges', variable assignment V.  Otherwise returns 0.*/
}



int main()
{
  char *name=malloc(Fsize); /*create space for the formula*/
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
 if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++)
    {
      fscanf(fp, "%s %d %d",name,&no_nodes,&no_edges);/*read number of nodes, number of edges*/
      int edges[no_edges][2];  /*Store edges in 2D array*/
      for(i=0;i<no_edges;i++)	 fscanf(fp, "%d%d", &edges[i][0], &edges[i][1]);/*read all the edges*/

      /*Assign variables x, y, z to nodes */
      int W[3];
      /*Store variable values in array
	value of x in V[0]
	value of y in V[1]
	value of z in V[2] */
      for(i=0;i<3;i++)  fscanf(fp, "%d", &W[i]);
      int p=parse(name); 
      switch(p)
	{case 0:fprintf(fpout,"%s is not a formula.  ", name);break;
	case 1: fprintf(fpout,"%s is an atomic formula.  ",name);break;
	case 2: fprintf(fpout,"%s is a negated formula.  ",name);break;
	case 3: fprintf(fpout,"%s is a binary connective formula.  ", name);break;
	case 4: fprintf(fpout,"%s is an existential formula.  ",name);break;
	case 5: fprintf(fpout,"%s is a universal formula.  ",name);break;
	default: fprintf(fpout,"%s is not a formula.  ",name);break;
	}
  
      /*Now check if formula is true in the graph with given variable assignment. */
      if (parse(name)!=0){
	if (eval(name, edges, no_nodes,  W)==1)	fprintf(fpout,"The formula %s is true in G under W\n", name);
	else fprintf(fpout,"The formula %s is false in G under W\n", name);
      }
    }
 
  fclose(fp);
  fclose(fpout);
  free(name);
  return(0);
}
