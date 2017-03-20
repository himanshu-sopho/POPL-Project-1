#include <stdio.h>
#include <string.h>                                        // header files
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

struct nod                                                 //structure for different registers and the values stored
{
int val;
char reg[10];
};



struct loops                                               //structure storing loop name and line no corresponding to the loop
{
char label[32];
int line_no;
};



struct mem                                                 //creating array for memory for name of the variable and value in data section
{
char var_name[15];
int val;
};



int pc=0;                                                  //initialising programme counter to zero
int loop_counter=0;					   //setting number of loops encountered and no of var in data section to 0
int data_counter=0;



void arithmetic (struct nod node[],char string[], int i, char op[]);         //function definitions
void logical (struct nod node[],char string[], int i, char op[]);
void comparison (struct nod node[],char string[], int i, char op[]);
void load_store (struct nod node[],char string[], int i, char op[], int memory[],struct loops loop[],struct mem data[]);
int control_flow (struct nod node[],char string[], int i, char op[], int memory[], char array[][32],struct loops loop[]);
int search_registers (struct nod node[],char reg_name[],int* ret,int type);
int get_number (char string[],int *i,int *ret);
int check_dependency(struct nod node[],char string[], int i, char op[],int wset[], int rset[]);
int check_dependency_ls(struct nod node[],char string[], int i, char op[],int wset[], int rset[]);
int check_loop_lables (struct nod node[],char string[], int i, char op[],char check_loop[], int memory[], char array[][32],struct loops loop[]);

mutex mutex1,mutex2,mutex3;

int main ()
{

struct nod node[32];                                       //creating 32 registers of type nod
struct mem data[20];                                        //array of struct to store the variables in .data section
struct loops loop[15];                                      //assuming the number of different loops dont exceed 5

int wset[32];
int rset[32];

node[0].val=0;                                             //initialising all registers
strcpy (node[0].reg,"zero");

node[1].val=0;
strcpy (node[1].reg,"at");

node[2].val=0;
strcpy (node[2].reg,"v0");

node[3].val=0;
strcpy (node[3].reg,"v1");

node[4].val=0;
strcpy (node[4].reg,"a0");

node[5].val=0;
strcpy (node[5].reg,"a1");

node[6].val=0;
strcpy (node[6].reg,"a2");

node[7].val=0;
strcpy (node[7].reg,"a3");

node[8].val=0;
strcpy (node[8].reg,"t0");

node[9].val=0;
strcpy (node[9].reg,"t1");

node[10].val=0;
strcpy (node[10].reg,"t2");

node[11].val=0;
strcpy (node[11].reg,"t3");

node[12].val=0;
strcpy (node[12].reg,"t4");

node[13].val=0;
strcpy (node[13].reg,"t5");

node[14].val=0;
strcpy (node[14].reg,"t6");

node[15].val=0;
strcpy (node[15].reg,"t7");

node[16].val=0;
strcpy (node[16].reg,"s0");

node[17].val=0;
strcpy (node[17].reg,"s1");

node[18].val=0;
strcpy (node[18].reg,"s2");

node[19].val=0;
strcpy (node[19].reg,"s3");

node[20].val=0;
strcpy (node[20].reg,"s4");

node[21].val=0;
strcpy (node[21].reg,"s5");

node[22].val=0;
strcpy (node[22].reg,"s6");

node[23].val=0;
strcpy (node[23].reg,"s7");

node[24].val=0;
strcpy (node[24].reg,"t8");

node[25].val=0;
strcpy (node[25].reg,"t9");

node[26].val=0;
strcpy (node[26].reg,"k0");

node[27].val=0;
strcpy (node[27].reg,"k0");

node[28].val=0;
strcpy (node[28].reg,"gp");

node[29].val=0;
strcpy (node[29].reg,"sp");

node[30].val=0;
strcpy (node[30].reg,"fp");

node[31].val=0;
strcpy (node[31].reg,"ra");




int memory[64];                                                     //creating a memory to be used duirng runtime
int n;
	for (n=0;n<64;n++)
	{
	memory[n]=0;
	}

	for (n=0;n<20;n++)
	{
	data[n].val=0;
	}

	for (n=0;n<32;n++)
	{
	wset[n]=0;
	rset[n]=0;
	}






FILE *fp;                                                          //pointer to file having the assembly code
char read[10];

cout << "Enter the file name" << endl;
cin >> read;

fp=fopen (read,"r");                                               //opening the file n read mode

n=0;
char ch;


	while ((ch=fgetc(fp))!=EOF)                                //calculating the number of lines in file                               
	if (ch=='\n')
	{
	n++;
	}


rewind (fp);


char array[n][32];                                                 //array which stores the strings in file
int i=0;
int j=0;



	while ((ch=fgetc (fp))!=EOF)                               //taking each instruction(string) from the file                       
	{
		if (ch!='\n')	
		{
		array[j][i]=ch;
		i++;
		}
		else if (ch=='\n')
		{
		array[j][i]='\0';
		j++;
		i=0;                                        
		}
	}

j=0;
int z1=0;
char string[32];                                                   //copy of instruction in arrray(used for passing the instruction to function)




	for (pc=z1;pc<n;pc++)                                              //start of execution of instructions
	{
	i=0;
	strcpy (string,array[pc]);                                         //copy instruction to string[]
		while (string[i]==' ')                                             //skip until spaces for the string
		{
		i++;
		} 

		char s[32];                                                        //array s for first word in instruction
		j=0;
		while (string[i]!=' ' && string[i]!='\0' && string[i]!='\n')       
		{

		s[j]=string[i];
		i++;
		j++;
		}
		s[j]='\0';


		if (!strcmp(s,".data"))                  //if s==.data go in data section else skip the line(blank lines before .data)
		{
		z1++;
		break;
		}
		else
		{
		z1++;
		}


	}

cout << endl;

	for (pc=z1;pc<n;pc++)
	{
	i=0;
	strcpy(string,array[pc]);                                           //copy the instruction after .data


	while (string[i]==' ')
	{
	i++;
	} 




	char var[15];                                                       //taking the first word in instruction in .data section
	int j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
	{

	var[j]=string[i];
	i++;
	j++;
	}
	var[j]='\0';

	if (!strcmp(var,".text"))                                         //checking if ==.text then break and execute instruction  in .text
	{
		z1++;
		break;
	}
	else
	{

		if (string[i]==':')
		{
			i++;
			while (string[i]==' ')
			{
			i++;
			}

			char type[5];               //if not .text then read the type of variable (.word or .asciiz)(.word only implemented)
			int j=0;
			while (string[i]!=' ')
			{
			type[j]=string[i];
			i++;
			j++;
			}
			type[j]='\0';

			if (strcmp(type,".word"))
			{
			cout << "Error in line %d type of the variable not found\n"<< pc+1;
			exit(1);
			}

			int no=0;                                             // for reading the value in the variable
			while (string[i]==' ')
			{
			i++;
			}

			while (string[i]!=' ' && string[i]!='\0')
			{
				if (!((string[i]-'0')<10) && !((string[i]-'0')<10))   //printing error if not a number
				{
				cout << "Error in line no %d .The variable is not an integer\n",pc+1;
				exit(1);
				}
				no=no*10+(string[i]-'0');
				i++;
			}

			strcpy(data[data_counter].var_name,var);      //if no syntax error store the variable and data in structure mem array
			data[data_counter].val=no;
			data_counter++;                               //incrementing global counter for # variables in .data section
			z1++;
			}
		}
	}
cout << endl;




								     //in case .text is executed the above for loop breaks and enters .text

int m=0;

	for (pc=z1;pc<n;pc++)                                	    //running though the entire code and if the intruction is label 
	{
	i=0;
	strcpy(string,array[pc]);

		while (string[i]==' ' || string[i]==',' )
		{
		i++;
		} 


		char lb[5];                                                 //stores label
		int j=0;
		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':')
		{
		lb[j]=string[i];
		i++;
		j++;
		}
		lb[j]='\0';


		if (string[i]==':')                                          //checks if label 
		{
		int c=0;
		int flag=0;
			for (c=0;c<loop_counter;c++)
			{
				if (!strcmp(loop[c].label,lb))       // if true it is compred with all previous labels to check for duplicate
				{
				flag=1;                                                      //if duplicate set flag=1;
				break;
				}
			}
			if (!flag)                                                   // if not duplicate store in the structure array
			{
			strcpy(loop[loop_counter].label,lb);
			loop[loop_counter].line_no=pc;
			loop_counter++;
			}
		}
	
	}

thread t[4];
pc=z1;
int thread_counter=0;
int g_thread_counter=0;
int ret=0,pc1;

while(pc<n)                                        //starts executing insructions in .text                                    
{
int see_loop=0;
ret=0;
char string1[5][32];
char check_loop[15];
char op1[4][7];
int save=pc;
int check;


	pc1=pc;
	check=pc;
	thread_counter=0;
   	 	while (pc1< check+4 && pc1< n)
   		{ 
		i=0;
		strcpy(string1[thread_counter],array[pc1]);
		strcpy(string,array[pc1]);

			while (string[i]==' ')
			{
			i++;
			} 




		char op[5];                                                  //extracting op code
		int j=0;
		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
		{

		op1[thread_counter][j]=string[i];
		i++;
		j++;
		}
		op1[thread_counter][j]='\0';



		if (string[i]==':')                                            //in case loop (already considered) so skip
		{
		strcpy(check_loop,op1[thread_counter]);
		see_loop=1;
		break;
		}
		else                                                           //based on the op code execute the function
		{
		
			if (!strcmp (op1[thread_counter],"add") || !strcmp (op1[thread_counter],"sub") || !strcmp (op1[thread_counter],"mul") 				|| !strcmp (op1[thread_counter],"addi"))
			{
			ret=check_dependency(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"and") || !strcmp (op1[thread_counter],"andi") 
			|| !strcmp (op1[thread_counter],"or") || !strcmp (op1[thread_counter],"ori") || !strcmp(op1[thread_counter],"nor"))
			{
			ret=check_dependency(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"slt") || !strcmp (op1[thread_counter],"slti"))
			{
			ret=check_dependency(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else  if (!strcmp (op1[thread_counter],"lw") || !strcmp (op1[thread_counter],"sw"))
			{
			ret=check_dependency_ls(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"beq") || !strcmp (op1[thread_counter],"bne") || 
			!strcmp (op1[thread_counter],"j"))
			{
			control_flow(node,string1[thread_counter],i,op1[thread_counter],memory,array,loop);
			} 
			else if (!strcmp (op1[thread_counter],"halt"));
			else if (strcmp (op1[thread_counter],".text") && strlen (op1[thread_counter])!=0)
			{
			cout << "Non instruction in line no " << pc+1;
			exit (1);
			}
			} 
	
		pc1++;
		pc=pc1;
        
    		}



	if (ret==-1)
	{
	pc=save;
	pc1=pc;
	check=pc;
	thread_counter=0;
   	 	while (pc1< check+4 && pc1< n)
   		{ 
		i=0;
		strcpy(string1[thread_counter],array[pc1]);
		strcpy(string,array[pc1]);

			while (string[i]==' ')
			{
			i++;
			} 




		char op[5];                                                  //extracting op code
		int j=0;
		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
		{

		op1[thread_counter][j]=string[i];
		i++;
		j++;
		}
		op1[thread_counter][j]='\0';



		if (string[i]==':')                                            //in case loop (already considered) so skip
		{
		break;
		}
		else                                                           //based on the op code execute the function
		{
		
			if (!strcmp (op1[thread_counter],"add") || !strcmp (op1[thread_counter],"sub") || !strcmp (op1[thread_counter],"mul") 				|| !strcmp (op1[thread_counter],"addi"))
			{
			cout << "not in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(arithmetic,node,string1[thread_counter],i,op1[thread_counter]);
			t[thread_counter].join();
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"and") || !strcmp (op1[thread_counter],"andi") 
			|| !strcmp (op1[thread_counter],"or") || !strcmp (op1[thread_counter],"ori") || !strcmp(op1[thread_counter],"nor"))
			{
			cout << "not in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(logical,node,string1[thread_counter],i,op1[thread_counter]);
			t[thread_counter].join();
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"slt") || !strcmp (op1[thread_counter],"slti"))
			{
			cout << "not in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(comparison,node,string1[thread_counter],i,op1[thread_counter]);
			t[thread_counter].join();		
			thread_counter++;
			}
			else  if (!strcmp (op1[thread_counter],"lw") || !strcmp (op1[thread_counter],"sw"))
			{
			cout << "not in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(load_store,node,string1[thread_counter],i,op1[thread_counter],memory,loop,data);	
			t[thread_counter].join();
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"beq") || !strcmp (op1[thread_counter],"bne") || 
			!strcmp (op1[thread_counter],"j"))
			{
			cout << "not in thread string is-->"<< string1[thread_counter] << endl;
			control_flow(node,string1[thread_counter],i,op1[thread_counter],memory,array,loop);
			}
			else if (!strcmp (op1[thread_counter],"halt"))
			{
			cout << endl << "Programme succesfully terminated" << endl;
			}

        	
			} 
	
		pc1++;
		pc=pc1;
        
    		}
	
	}
	else
	{
	thread_counter=0;
	pc=save;
	pc1=pc;
	check=pc;

   		while (pc1< check+4 && pc1< n)
   		{ 
		i=0;
		strcpy(string1[thread_counter],array[pc1]);
		strcpy(string,array[pc1]);

		while (string[i]==' ')
		{
		i++;
		} 
	



		char op[5];                                                  //extracting op code
		int j=0;
		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
		{

		op1[thread_counter][j]=string[i];
		i++;
		j++;
		}
		op1[thread_counter][j]='\0';



		if (string[i]==':')                                            //in case loop (already considered) so skip
		{
		
		break;
		}
		else                                                           //based on the op code execute the function
		{
		
			if (!strcmp (op1[thread_counter],"add") || !strcmp (op1[thread_counter],"sub") || !strcmp (op1[thread_counter],"mul") 				|| !strcmp (op1[thread_counter],"addi"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(arithmetic,node,string1[thread_counter],i,op1[thread_counter]);
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"and") || !strcmp (op1[thread_counter],"andi") 
			|| !strcmp (op1[thread_counter],"or")|| !strcmp (op1[thread_counter],"ori") || !strcmp(op1[thread_counter],"nor"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(logical,node,string1[thread_counter],i,op1[thread_counter]);
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"slt") || !strcmp (op1[thread_counter],"slti"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(comparison,node,string1[thread_counter],i,op1[thread_counter]);		
			thread_counter++;
			}
			else  if (!strcmp (op1[thread_counter],"lw") || !strcmp (op1[thread_counter],"sw"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(load_store,node,string1[thread_counter],i,op1[thread_counter],memory,loop,data);
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"beq") || !strcmp (op1[thread_counter],"bne") 
			|| !strcmp (op1[thread_counter],"j"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			control_flow(node,string1[thread_counter],i,op1[thread_counter],memory,array,loop);
			}
			else if (!strcmp (op1[thread_counter],"halt"))
			{
			cout << endl << "Programme succesfully terminated" << endl;
			}

        	
			} 
	
		pc1++;
		pc=pc1;
        
  	 	 }
	
     		for (int x=0;x<thread_counter;x++)
		{
		t[x].join();
		}



	thread_counter=0;
	}

	for (int m=0;m<32;m++)
	{
	wset[m]=0;
	rset[m]=0;
	}
	g_thread_counter=0;
	
if (see_loop==1)
{
save=pc;
pc++;

int pc2=pc;

while (pc2<n)
{
strcpy(string,array[pc2]);
i=0;
		while (string[i]==' ')
		{
		i++;
		} 
	



		char op[5];                                                  
		int j=0;
		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
		{

		op[j]=string[i];
		i++;
		j++;
		}
		op[j]='\0';

		
		if (!strcmp (op,"beq") || !strcmp (op,"bne") || !strcmp (op,"j"))
		{
			if (!strcmp (op,"beq") || !strcmp (op,"bne"))
			{
			int yes=check_loop_lables (node,string,i,op,check_loop,memory,array,loop);
				if (yes==1)
				{
				break;
				}			
			}
			else if (!strcmp (op,"j"))
			{
			int yes=check_loop_lables (node,string,i,op,check_loop,memory,array,loop);
				if (yes==1)
				{
				break;
				}
			}
		
		}
	
pc2++;
}

if (pc2==n)
{
pc2=pc;
}

pc1=pc;
check=pc;

		
   	 	while (pc1<=pc2)
   		{ 
			ret=0;
			check=pc1;
			save=pc1;
			while (pc1< check+4 && pc1<=pc2)
   			{ 
		
			i=0;
			strcpy(string1[thread_counter],array[pc1]);
			strcpy(string,array[pc1]);

				while (string[i]==' ')
				{
				i++;
				} 




			char op[5];                                                  //extracting op code
			int j=0;
			while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
			{

			op1[thread_counter][j]=string[i];
			i++;
			j++;
			}
			op1[thread_counter][j]='\0';



			if (string[i]==':')                                            //in case loop (already considered) so skip
			{
			strcpy(check_loop,op1[thread_counter]);
			see_loop=1;
			break;
			}
			else                                                           //based on the op code execute the function
			{
		
			if (!strcmp (op1[thread_counter],"add") || !strcmp (op1[thread_counter],"sub") || !strcmp (op1[thread_counter],"mul") 				|| !strcmp (op1[thread_counter],"addi"))
			{
			ret=check_dependency(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"and") || !strcmp (op1[thread_counter],"andi") 
			|| !strcmp (op1[thread_counter],"or") || !strcmp (op1[thread_counter],"ori") || !strcmp(op1[thread_counter],"nor"))
			{
			ret=check_dependency(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"slt") || !strcmp (op1[thread_counter],"slti"))
			{
			ret=check_dependency(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else  if (!strcmp (op1[thread_counter],"lw") || !strcmp (op1[thread_counter],"sw"))
			{
			ret=check_dependency_ls(node,string1[thread_counter],i,op1[thread_counter],wset,rset);
				if (ret==-1)
				{
				break;
				}
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"beq") || !strcmp (op1[thread_counter],"bne") || 
			!strcmp (op1[thread_counter],"j"))
			{
			ret=-1;
			break;
			control_flow(node,string1[thread_counter],i,op1[thread_counter],memory,array,loop);
			}
			else if (strcmp (op1[thread_counter],".text") && strlen (op1[thread_counter])!=0)
			{
			cout << "Non instruction in line no " << pc+1;
			exit (1);
			}
			else if (!strcmp (op1[thread_counter],"halt"));
			else 
			{
			ret=-1;
			}
			} 
	
		pc1++;
		}

		pc1=save;

		if (ret==-1)
		{

			while (pc1< check+4 && pc1<=pc2)
	   		{
			i=0;
			strcpy(string,array[pc1]);
			strcpy(string,array[pc1]);
			thread tr;

				while (string[i]==' ')
				{
				i++;
				} 




			char op[5];                                                  //extracting op code
			int j=0;
			while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
			{

			op[j]=string[i];
			i++;
			j++;
			}
			op[j]='\0';



			if (string[i]==':')                                            //in case loop (already considered) so skip
			{
			;
			}
			else                                                           //based on the op code execute the function
			{
			
		
			if (!strcmp (op,"add") || !strcmp (op,"sub") || !strcmp (op,"mul") || !strcmp (op,"addi"))
			{
			cout << "not in thread string is-->"<< string << endl;
			tr=thread(arithmetic,node,string,i,op);
			tr.join();
			}
			else if (!strcmp (op,"and") || !strcmp (op,"andi") 
			|| !strcmp (op,"or") || !strcmp (op,"ori") || !strcmp(op,"nor"))
			{
			cout << "not in thread string is-->"<< string << endl;
			tr=thread(logical,node,string,i,op);
			tr.join();
			}
			else if (!strcmp (op,"slt") || !strcmp (op,"slti"))
			{
			cout << "not in thread string is-->"<< string<< endl;
			tr=thread(comparison,node,string,i,op);
			tr.join();
			}
			else  if (!strcmp (op,"lw") || !strcmp (op,"sw"))
			{
			cout << "not in thread string is-->"<< string << endl;
			tr=thread(load_store,node,string,i,op,memory,loop,data);	
			tr.join();
			}
			else if (!strcmp (op,"beq") || !strcmp (op,"bne") || !strcmp (op,"j"))
			{
			cout << "not in thread string is-->"<< string << endl;
			int yes1=control_flow(node,string,i,op,memory,array,loop);
				if (yes1==1)
				{			
				pc1=pc;
				}
				else
				{
				pc1++;
				pc=pc1;
				}
			}
			else if (!strcmp (op,"halt"))
			{
			cout << endl << "Programme succesfully terminated" << endl;
			}
			} 
	
			if (!(!strcmp (op,"beq") || !strcmp (op,"bne") || !strcmp (op,"j")))
			{
			pc1++;
			pc=pc1;
			}
    		   }
	 
		}

		else
		{
		thread_counter=0;
		pc1=save;
		check=pc1;

	   	while (pc1< check+4 && pc1<=pc2)
	   	{ 
			i=0;
			strcpy(string1[thread_counter],array[pc1]);
			strcpy(string,array[pc1]);

				while (string[i]==' ')
				{
				i++;
				} 
	



			char op[5];                                                  //extracting op code
			int j=0;
			while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!=':' && string[i]!='\0')
			{

			op1[thread_counter][j]=string[i];
			i++;
			j++;
			}
			op1[thread_counter][j]='\0';



		  if (string[i]==':')                                            //in case loop (already considered) so skip
		  {
		
		  break;
		  }
		  else                                                           //based on the op code execute the function
		  {
		
			if (!strcmp (op1[thread_counter],"add") || !strcmp (op1[thread_counter],"sub") || !strcmp (op1[thread_counter],"mul") 				|| !strcmp (op1[thread_counter],"addi"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(arithmetic,node,string1[thread_counter],i,op1[thread_counter]);
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"and") || !strcmp (op1[thread_counter],"andi") 
			|| !strcmp (op1[thread_counter],"or")|| !strcmp (op1[thread_counter],"ori") || !strcmp(op1[thread_counter],"nor"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(logical,node,string1[thread_counter],i,op1[thread_counter]);
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"slt") || !strcmp (op1[thread_counter],"slti"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(comparison,node,string1[thread_counter],i,op1[thread_counter]);	
			thread_counter++;
			}
			else  if (!strcmp (op1[thread_counter],"lw") || !strcmp (op1[thread_counter],"sw"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			t[thread_counter]=thread(load_store,node,string1[thread_counter],i,op1[thread_counter],memory,loop,data);
			thread_counter++;
			}
			else if (!strcmp (op1[thread_counter],"beq") || !strcmp (op1[thread_counter],"bne") 
			|| !strcmp (op1[thread_counter],"j"))
			{
			cout << "in thread string is-->"<< string1[thread_counter] << endl;
			control_flow(node,string1[thread_counter],i,op1[thread_counter],memory,array,loop);
			}
			else if (!strcmp (op1[thread_counter],"halt"))
			{
			cout << endl << "Programme succesfully terminated" << endl;
			}

        	
		  } 
	
		pc1++;
		pc=pc1;
        
  	 	}
	
     			for (int x=0;x<thread_counter;x++)
			{
			t[x].join();
			}
		
		}

	for (int m=0;m<32;m++)
	{
	wset[m]=0;
	rset[m]=0;
	}

	}
}
}

	cout << endl << "*STATE OF RGISTERS*" << endl;
	for (int x=0;x<32;x++)
	{
		if (node[x].val!=0)
		{
		cout << "$" << node[x].reg << "-->" << node[x].val << endl;
		}
	}

	cout << endl << "*STATE OF DATA MEMORY*" << endl;

	for (int x=0;x<5;x++)
	{
		if (data[x].val!=0)
		{
		cout << "index-> " << x << "-->" << data[x].val << endl;
		}
	}

	cout << endl << "*STATE OF MEMORY*" << endl;

	for (int x=0;x<64;x++)
	{
		if (memory[x]!=0)
		{
		cout << "index->" << x << " -->" << memory[x] << endl;
		}
	}

	/*cout << "wset" << endl;
	for (n=0;n<32;n++)
	{
	cout << n << " " << wset[n] << endl;;
	}

	cout << "rset" << endl;
	for (n=0;n<32;n++)
	{
	cout << n << " " << rset[n] << endl;;
	}*/

}



void arithmetic (struct nod node[],char string[], int i, char op[])
{
thread t1,t2,t3,t4;

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (string[i-1]!='$') 
	{
	cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
	exit(1);
	}

	char r1[5];                                                         
	int j=0;

	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}

	r1[j]='\0';


while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (string[i-1]!='$') 
	{
	cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
	exit(1);
	}

	char r2[5];                                                          //taking source the register in r2
	j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r2[j]=string[i];
	i++;
	j++;
	}
	r2[j]='\0';

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	int no=0;
	char r3[5];

	if (!strcmp(op,"add") || !strcmp(op,"sub") || !strcmp(op,"mul")) 
	{
		if (string[i-1]!='$')
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}
		j=0;


		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!='\0')
		{
		r3[j]=string[i];
		i++;
		j++;
		}
		r3[j]='\0';                                                          //taking second source register  in r3

	}

	else if (! strcmp(op,"addi"))                                        //if immidiate instruction calculate the number
	{
	t4=thread(get_number,string,&i,&no);
	}

	int r11,r22,r33;
 
	t1=thread(search_registers,node,r1,&r11,1);                            //assign the index of register to r11

	t2=thread(search_registers,node,r2,&r22,2); 

	if (!strcmp(op,"add") || !strcmp(op,"sub") || !strcmp(op,"mul"))
	{ 
	t3=thread(search_registers,node,r3,&r33,3);
	}


t1.join(); t2.join();

if (!strcmp(op,"add"))                                         //executing the corresponding arithmetic instruction
{
t3.join();
node[r11].val=node[r22].val+node[r33].val;
}

else if (!strcmp(op,"sub"))
{
t3.join();
node[r11].val=node[r22].val-node[r33].val;
}

else if (!strcmp(op,"mul"))
{
t3.join();
node[r11].val=node[r22].val*node[r33].val;
}

else if (!strcmp(op,"addi"))
{
t4.join();
node[r11].val=node[r22].val+no;
}


}





void logical (struct nod node[],char string[], int i, char op[])
{
thread t1,t2,t3,t4;

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	char r1[5];                                                        
	int j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}
	r1[j]='\0';


while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}


	if (string[i-1]!='$') 
	{
	cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
	exit(1);
	}


	char r2[5];                                                         	
	j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r2[j]=string[i];
	i++;
	j++;
	}
	r2[j]='\0';


	int no=0;
	char r3[5];


while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}



	if (!strcmp(op,"and") || !strcmp(op,"or") || !strcmp(op,"nor") )              //getting r3 if not immidiate instruction
	{

		if (string[i-1]!='$') 
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}

		j=0;

		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!='\0')
		{
		r3[j]=string[i];
		i++;
		j++;
		}
		r3[j]='\0';

	}
	else if (!strcmp(op,"andi") || !strcmp(op,"ori") )                             //if immidiate getting the value
	{
	t4=thread(get_number,string,&i,&no);
	}
 
 	int r11, r22,r33;
	
	t1=thread(search_registers,node,r1,&r11,1);                            //assign the index of register to r11

	t2=thread(search_registers,node,r2,&r22,2);

	if (!strcmp(op,"and") || !strcmp(op,"or") || !strcmp(op,"nor") )
	{ 
	t3=thread(search_registers,node,r3,&r33,3);
	}

t1.join();t2.join();

if (!strcmp(op,"and"))                                  			//executing desired logical instruction
{
t3.join();
node[r11].val=node[r22].val & node[r33].val;
}

else if (!strcmp(op,"or"))
{
t3.join();
node[r11].val=node[r22].val | node[r33].val;
}

else if (!strcmp(op,"nor"))
{
t3.join();
node[r11].val=~(node[r22].val | node[r33].val);
}

else if (!strcmp(op,"andi"))
{
t4.join();
node[r11].val=node[r22].val & no;
}

else if (!strcmp(op,"ori"))
{
t4.join();;
node[r11].val=node[r22].val | no;
}


}






void comparison (struct nod node[],char string[], int i, char op[])
{
thread t1,t2,t3,t4;

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

if (string[i-1]!='$') 
{
cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
exit(1);
}

	char r1[5];
	int j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}
	r1[j]='\0';



while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (string[i-1]!='$') 
	{
	cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
	exit(1);
	}

	char r2[15];
	j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r2[j]=string[i];
	i++;
	j++;
	}
	r2[j]='\0';

	int no=0;
	char r3[5];


while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (!strcmp(op,"slt"))
	{

		if (string[i-1]!='$') 
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}
		j=0;

		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!='\0')
		{
		r3[j]=string[i];
		i++;
		j++;
		}
		r3[j]='\0';

	}
	else
	{
	t4=thread(get_number,string,&i,&no);
	}

 	int r11, r22;
	
	t1=thread(search_registers,node,r1,&r11,1);                            //assign the index of register to r11

	t2=thread(search_registers,node,r2,&r22,2);

	int r33;

	if (!strcmp(op,"slt"))
	{ 
	t3=thread (search_registers,node,r3,&r33,3);
	}
	
	t1.join();t2.join();

if (!strcmp(op,"slt"))
{
	t3.join();
	if (node[r22].val<node[r33].val)
	{
	node[r11].val=1;
	}
	else
	{
	node[r11].val=0;
	}

}

else if (!strcmp(op,"slti"))
{
	t4.join();
	if (node[r22].val<no)
	{
	node[r11].val=1;
	}
	else
	{
	node[r11].val=0;
	}
}
}







void load_store (struct nod node[],char string[], int i, char op[],int memory[],struct loops loop[],struct mem data[])
{
thread t1,t2,t3;

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	char r1[5];
	int j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}
	r1[j]='\0';



while (string[i]==' ' || string[i]==',')
{
i++;
}


int no=0;
int g_flag=0;

	if (string[i]-'0'>=0 && string[i]-'0'<=9)
	{
		g_flag=1;

		t3=thread(get_number,string,&i,&no);
		t3.join();

		while (string[i]==' ')
		{
		i++;
		}

		if (string[i]!='(')
		{
		cout << endl << "Syntax error in line no " << pc+1 <<endl;
		exit (1);
		}

		while (string[i]=='(' || string[i]==' '|| string[i]=='$')
		{
		i++;
		}
		
		if (string[i-1]!='$')
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit (1);
		}
		
		
	}



	char r2[15];
	j=0;
	while (string[i]!=' ' && string[i]!=','  && string[i]!=')' && string[i]!='\0')
	{
	r2[j]=string[i];
	i++;
	j++;
	}
	r2[j]='\0';


int r11,r22;

	if (!strcmp(op,"sw"))
	{
	t1=thread(search_registers,node,r1,&r11,2);
	}
	else if (!strcmp(op,"lw"))
	{
	t1=thread(search_registers,node,r1,&r11,1);
	} 


	if (g_flag==1)
	{
		if (!strcmp(op,"sw"))
		{
		t2=thread(search_registers,node,r2,&r22,1);
		}
		else if (!strcmp(op,"lw"))
		{
		t2=thread(search_registers,node,r2,&r22,2);
		}
	}

int flag=0;

	if (g_flag==0)
	{
		t1.join();
		int c;
		int flag=0;
		for (c=0;c<data_counter;c++)
		{
			if (!strcmp(data[c].var_name,r2))
			{
			flag=1;
			break;
			}
		}



		if (flag==1)
		{
			if (!strcmp(op,"lw"))
			{
			node[r11].val=data[c].val;
			}
	
			else if (!strcmp(op,"sw"))
			{
			data[c].val=node[r11].val;
			}
		}
		else
		{
			cout << endl << "Error in line no " << pc+1 <<endl;
			exit (1);
		}
	}


	if (g_flag==1)
	{
		t1.join();t2.join();
		int offset=no;
		int base=node[r22].val;


		if (!strcmp(op,"lw"))
		{
		int address=base+offset;
		node[r11].val=memory[address];
		}
		else if (!strcmp(op,"sw"))
		{
		int address=base+offset;
		memory[address]=node[r11].val;
		}

	}


}


int control_flow (struct nod node[],char string[], int i, char op[],int memory[], char array[][32],struct loops loop[])
{

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (!strcmp(op,"beq") || !strcmp (op,"bne"))
	{
		if (string[i-1]!='$') 
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}
	}

	char r1[5];
	int j=0;

	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}
	r1[j]='\0';



while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}


	char r2[5];

	if (!strcmp(op,"beq") || !strcmp(op,"bne"))
	{
		if (string[i-1]!='$') 
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}

		j=0;
		
		while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
		{
		r2[j]=string[i];
		i++;
		j++;
		}
		r2[j]='\0';

	}

int no=0;
	char r3[10];

	if (!strcmp(op,"beq") || !strcmp(op,"bne"))
	{
		while (string[i]==' ' || string[i]==',' || string[i]=='$')
		{
		i++;
		}


		j=0;

		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!='\0')
		{
		r3[j]=string[i];
		i++;
		j++;
		}
		r3[j]='\0';


	}


i=0;
int flag=0;
int r11;

if (!strcmp(op,"beq") || !strcmp(op,"bne"))
{
search_registers (node,r1,&r11,2);
}

i=0;
flag=0;
int r22;

if (!strcmp(op,"beq") || !strcmp(op,"bne"))
{
search_registers (node,r2,&r22,2);
}


if (!strcmp(op,"beq"))
{
	int c=0;
	int flag=0;
	if ((node[r11].val-node[r22].val)==0)
	{

		for (c=0;c<loop_counter;c++)
		{
			if (!strcmp(loop[c].label,r3))
			{
			flag=1;
			break;
			}


		}		
		if (flag=1)
		{
		pc=loop[c].line_no;
		}
		else
		{
		cout << endl << "Invalid label in line no " << pc+1 <<endl;
		exit (1);
		}
		cout << endl << endl;
	}
return flag;

}
else if (!strcmp(op,"bne"))
{
	int c=0;
	int flag=0;
	if ((node[r11].val != node[r22].val))
	{

		for (c=0;c<loop_counter;c++)
		{
			if (!strcmp(loop[c].label,r3))
			{
			flag=1;
			break;
			}

		}
		if (flag==1)
		{
		pc=loop[c].line_no;
		}
		else
		{
		cout << endl << "Invalid label in line no " << pc+1 <<endl;
		exit (1);
		}
		cout << endl << endl;
	}
	
return flag;
}
else if (!strcmp(op,"j"))
{
	int c=0;
	int flag=0;
	for (c=0;c<loop_counter;c++)
	{
		if (!strcmp(loop[c].label,r1))
		{
		flag=1;
		break;
		}

	}
	if (flag==1)
	{
	pc=loop[c].line_no;
	}
	else
	{
	cout << endl << "Invalid label in line no " << pc+1 <<endl;
	exit (0);
	}
	cout << endl;

return flag;
}


}


int search_registers (struct nod node[],char reg_name[],int *ret,int type)
{
int flag=0;
int i;

	if (type==2 || type==3)
	{
		for (i=0;i<32;i++)
		{
			if (!strcmp(node[i].reg,reg_name))
			{
			flag=1;
			break;
			}

		}
	}
	else if (type==1)
	{
		for (i=0;i<32;i++)
		{
			if (!strcmp(node[i].reg,reg_name) && strcmp(reg_name,"zero"))      
			{
			flag=1;                                                
			break;
			}
			if (!strcmp(reg_name,"zero"))                               
			{
			cout << endl << "Error.$zero cannot be modified" << endl;
			exit (1);
			}
		}
	}

if (flag==0)
{
cout << endl << "Error. register not found" << endl;
exit (1);
}

*ret=i;
}


int get_number (char string[],int *i,int *ret)
{
int no=0;

	while (string[*i]!=' ' && string[*i]!=',' && string[*i]!='$' && string[*i]!='\0' && string[*i]!='(')
	{
		if (!((string[*i]-'0')<10) && !((string[*i]-'0')<10))
		{
		cout << endl << "Error.The immidiate value is not a number" << endl;
		exit(1);
		}

	no=no*10+(string[*i]-'0');
	*i=*i+1;
	}

*ret= no;
}

int check_dependency(struct nod node[],char string[], int i, char op[],int wset[], int rset[])
{
thread t1,t2,t3,t4;

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (string[i-1]!='$') 
	{
	cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
	exit(1);
	}

	char r1[5];                                                         
	int j=0;

	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}

	r1[j]='\0';


while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (string[i-1]!='$') 
	{
	cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
	exit(1);
	}

	char r2[5];                                                          //taking source the register in r2
	j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r2[j]=string[i];
	i++;
	j++;
	}
	r2[j]='\0';

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	int no=0;
	char r3[5];

	if (!strcmp(op,"add") || !strcmp(op,"sub") ||!strcmp(op,"mul") || !strcmp(op,"and") || !strcmp(op,"or") 
	|| !strcmp(op,"nor") || !strcmp(op,"slt"))  
	{
		if (string[i-1]!='$')
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}
		j=0;


		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!='\0')
		{
		r3[j]=string[i];
		i++;
		j++;
		}
		r3[j]='\0';                                                          //taking second source register  in r3

	}

	else if (!strcmp(op,"addi") || !strcmp(op,"andi") || !strcmp(op,"slti"))     //if immidiate instruction calculate the number
	{
	t4=thread(get_number,string,&i,&no);
	t4.join();
	}

	int r11,r22,r33,flag;
 
	t1=thread(search_registers,node,r1,&r11,1);                            //assign the index of register to r11
	t1.join();
	
	mutex1.lock();
	if (wset[r11]==1 || rset[r11]==1)
	{
	mutex1.unlock();
	return -1;
	}
	wset[r11]=1;
	mutex1.unlock();

	t2=thread(search_registers,node,r2,&r22,2); 
 	t2.join();

	mutex2.lock();
	if (wset[r22]==1)
	{
	mutex2.unlock();
	return -1;
	}	
	rset[r22]=1;
	mutex2.unlock();
	
	if (!strcmp(op,"add") || !strcmp(op,"sub") ||!strcmp(op,"mul") || !strcmp(op,"and") || !strcmp(op,"or") 
	|| !strcmp(op,"nor") || !strcmp(op,"slt")) 
	{ 
		t3=thread(search_registers,node,r3,&r33,3);
		t3.join();

		mutex3.lock();
		if (wset[r33]==1)
		{
		mutex3.unlock();
		return -1;
		}
		rset[r33]=1;
		mutex3.unlock();
	}




}

int check_dependency_ls(struct nod node[],char string[], int i, char op[],int wset[], int rset[])
{
thread t1,t2,t3;

while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	char r1[5];
	int j=0;
	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}
	r1[j]='\0';



while (string[i]==' ' || string[i]==',')
{
i++;
}


int no=0;
int g_flag=0;

	if (string[i]-'0'>=0 && string[i]-'0'<=9)
	{
		g_flag=1;

		t3=thread(get_number,string,&i,&no);
		t3.join();

		while (string[i]==' ')
		{
		i++;
		}

		if (string[i]!='(')
		{
		cout << endl << "Syntax error in line no " << pc+1 <<endl;
		exit (1);
		}

		while (string[i]=='(' || string[i]==' '|| string[i]=='$')
		{
		i++;
		}
		
		if (string[i-1]!='$')
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit (1);
		}
		
		
	}



	char r2[15];
	j=0;
	while (string[i]!=' ' && string[i]!=','  && string[i]!=')')
	{
	r2[j]=string[i];
	i++;
	j++;
	}
	r2[j]='\0';


int r11,r22;

	if (!strcmp(op,"sw"))
	{
	t1=thread(search_registers,node,r1,&r11,2);
	t1.join();
	
	mutex1.lock();
		if (wset[r11]==1)
		{
		mutex1.unlock();
		return -1;
		}
		rset[r11]=1;
	mutex1.unlock();
	}
	else if (!strcmp(op,"lw"))
	{
	t1=thread(search_registers,node,r1,&r11,1);
	t1.join();
	mutex1.lock();
		if (wset[r11]==1 || rset[r11]==1)
		{
		mutex1.unlock();
		return -1;
		}
		wset[r11]=1;
	mutex1.unlock();
	} 


	if (g_flag==1)
	{
		if (!strcmp(op,"sw"))
		{
		t2=thread(search_registers,node,r2,&r22,1);
		t2.join();

		mutex1.lock();
			if (wset[r22]==1 || rset[r22]==1)
			{
			mutex1.unlock();
			return -1;
			}
		wset[r22]=1;
		mutex1.unlock();
		}
		else if (!strcmp(op,"lw"))
		{
		t2=thread(search_registers,node,r2,&r22,2);
		t2.join();
		mutex1.lock();
			if (wset[r22]==1)
			{
			mutex1.unlock();
			return -1;
			}
			rset[r22]=1;
		mutex1.unlock();
		}
	}
	if (g_flag==0)
	{
	return -1;
	}
}

int check_loop_lables (struct nod node[],char string[], int i, char op[],char check_loop[], int memory[], char array[][32],struct loops loop[])
{
while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}

	if (!strcmp(op,"beq") || !strcmp (op,"bne"))
	{
		if (string[i-1]!='$') 
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}
	}

	char r1[5];
	int j=0;

	while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
	{
	r1[j]=string[i];
	i++;
	j++;
	}
	r1[j]='\0';



while (string[i]==' ' || string[i]==',' || string[i]=='$')
{
i++;
}


	char r2[5];

	if (!strcmp(op,"beq") || !strcmp(op,"bne"))
	{
		if (string[i-1]!='$') 
		{
		cout << endl << "Error missing $ sign in line no " << pc+1 <<endl;
		exit(1);
		}

		j=0;
		
		while (string[i]!=' ' && string[i]!=',' && string[i]!='$')
		{
		r2[j]=string[i];
		i++;
		j++;
		}
		r2[j]='\0';

	}

int no=0;
	char r3[10];

	if (!strcmp(op,"beq") || !strcmp(op,"bne"))
	{
		while (string[i]==' ' || string[i]==',' || string[i]=='$')
		{
		i++;
		}


		j=0;

		while (string[i]!=' ' && string[i]!=',' && string[i]!='$' && string[i]!='\0')
		{
		r3[j]=string[i];
		i++;
		j++;
		}
		r3[j]='\0';


	}


i=0;
int flag=0;
int r11;

if (!strcmp(op,"beq") || !strcmp(op,"bne"))
{
search_registers (node,r1,&r11,2);
}

i=0;
flag=0;
int r22;

if (!strcmp(op,"beq") || !strcmp(op,"bne"))
{
search_registers (node,r2,&r22,2);
}


if (!strcmp(op,"beq")||!strcmp(op,"bne"))
	{
	if (!strcmp(r3,check_loop))
	{
	return 1;
	}
	else 
	{
	return 0;
	}
		
}
else if (!strcmp(op,"j"))
{
	if (!strcmp(r1,check_loop))
	{
	return 1;
	}
	else 
	{
	return 0;
	}
}

return -1;

}
