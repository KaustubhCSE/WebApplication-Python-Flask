/*
 * Name : Kaustubh Mohgaonkar
 * ID : 1001101770
 * Lab : Assignment 3
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define PID 0
#define ARRIVAL 1
#define RUNTIME 2
#define PRIORITY 3

int number_of_jobs = 0;

int job_array[1024][4];

enum
{
  UNKNOWN=0,
  FCFS,           // First Come First Served
  SJN,            // Shortest Job Next
  SJNPRE,         // Shortest Job Next with Preemption
  PRI,            // Priority
  PRIPRE          // Priority with Preemption
};


int main( int argc, char *argv[])

{
   int i;
//   int frame;
   char *line, *file_name, *sch_type, *time_q;
   FILE *fr;
//   bool done;
   line = (char*)malloc(255*sizeof(char));

   if( argc > 4 )
   {
     printf("Too many command-line parameters.\n");
     return -1;
   }

   for (i=0; i< argc;i++)
   {
	   if (strcmp(argv[i],"-a") == 0)
	   {
		   sch_type = argv[i+1];
	   }
	   if (strcmp(argv[i],"-q") == 0)
	   {
		   time_q = argv[i+1];
	   }
	   else if( strstr(argv[i],".txt"))
		   file_name = argv[i];

   }

   fr = fopen (file_name, "rt");

   while(fgets ( line, 255, fr ))
   {
	   int pid      = -1;
	   int priority = -1;
	   int arrival  = -1;
	   int runtime  = -1;

	   if (strstr(line,"#")== NULL)
	   {
		   sscanf( line, "%d, %d, %d, %d", &pid, &arrival, &runtime, &priority );
		   job_array[number_of_jobs][PID]     = pid;
		   job_array[number_of_jobs][ARRIVAL] = arrival;
		   job_array[number_of_jobs][RUNTIME] = runtime;
		   job_array[number_of_jobs][PRIORITY]= priority;
		   number_of_jobs ++;
	   }
   }

//   for( int i = 0; i < number_of_jobs; i++ )
//   {
//	   printf("%d, %d, %d, %d\n",
//	               job_array[i][PID],
//	               job_array[i][ARRIVAL],
//	               job_array[i][RUNTIME],
//	               job_array[i][PRIORITY]
//	               );
//   }

   printf("\n");
   fclose(fr);

   if(strcmp(sch_type, "FCFS") ==0)
   {
	   int tmpPID;
	   int tmpArrival;
	   int tmpRuntime;
	   int tmptime = 0;
	   int tmpStartTime = 0;
	   int track = 0;
	   int i,j,n = 0;

	   while(track < number_of_jobs )
	   {
		   for( i= 0; i < number_of_jobs ; i++)
		   {
			   if ( job_array[i][RUNTIME] != 0)
			   {
				      tmpPID      = job_array[i][PID];
				      tmpArrival  = job_array[i][ARRIVAL];
				      tmpRuntime = job_array[i][RUNTIME];

				      if( job_array[i][ARRIVAL] < tmptime || job_array[i][ARRIVAL] == tmptime )
				      {
				    	  for( j= 0; j < number_of_jobs ; j++ )
				    	  {
				    		  if (job_array[j][RUNTIME] != 0)
				    		  {
				    			  if ( job_array[j][RUNTIME] < tmptime || job_array[j][RUNTIME] == tmptime)
				    			  {
				    				  tmpPID      = job_array[j][PID];
				    				  tmpArrival  = job_array[j][ARRIVAL];
				    				  tmpRuntime = job_array[j][RUNTIME];

				    			  }
				    			  }
				    		  }
				    	  }

				    	  for( n = 0 ; n < number_of_jobs ; n++ )
				    	  {
				    			  tmpStartTime = tmptime;
				    			  tmptime = tmptime + job_array[n][RUNTIME];
				    			  job_array[n][RUNTIME] = 0;
				    			  printf("%d, %d, %d\n",
				    			              job_array[n][PID],
				    			              tmpStartTime,
				    			              tmptime
				    			              );
				    			  track++;
				    			  i = 0;
				    	  }
				      }
			   }
		   }
	   }

// Code for Priority without preemption :

   else if (strcmp(sch_type, "PRI") ==0)
   {
	   int tmpPID;
	   int tmpArrival;
	   int tmpPriority;
	   int tmptime = 0;
	   int tmpStartTime = 0;
	   int track = 0;
	   int i,j,n = 0;

	   while(track < number_of_jobs )
	   {
		   for( i= 0; i < number_of_jobs ; i++)
		   {
			   if ( job_array[i][RUNTIME] != 0)
			   {
				      tmpPID      = job_array[i][PID];
				      tmpArrival  = job_array[i][ARRIVAL];
				      tmpPriority = job_array[i][PRIORITY];

				      if( job_array[i][RUNTIME] < tmptime || job_array[i][ARRIVAL] == tmptime )
				      {
				    	  for( j= 0; j < number_of_jobs ; j++ )
				    	  {
				    		  if (job_array[j][RUNTIME] != 0)
				    		  {
				    			  if ( job_array[j][ARRIVAL] < tmptime || job_array[j][ARRIVAL] == tmptime)
				    			  {
				    				  if ( tmpPriority > job_array[j][PRIORITY] )
				    				  {
				    					  tmpPID      = job_array[j][PID];
				    					  tmpArrival  = job_array[j][ARRIVAL];
				    					  tmpPriority = job_array[j][PRIORITY];
				    				  }
				    				  else if( tmpPriority == job_array[j][PRIORITY] )
				    				  {
				    					  if ( tmpArrival > job_array[j][ARRIVAL] )
				    					  {
				    						  tmpPID      = job_array[j][PID];
				    						  tmpArrival  = job_array[j][ARRIVAL];
				    						  tmpPriority = job_array[j][PRIORITY];
				    					  }
				    					  else if (tmpArrival  == job_array[j][ARRIVAL])
				    					  {
				    						  tmpPID      = job_array[j][PID];
				    					  }
				    				  }
				    			  }
				    		  }
				    	  }

				    	  for( n = 0 ; n < number_of_jobs ; n++ )
				    	  {
				    		  if (tmpPID == job_array[n][PID])
				    		  {
				    			  tmpStartTime = tmptime;
				    			  tmptime = tmptime + job_array[n][RUNTIME];
				    			  job_array[n][RUNTIME] = 0;
				    			  printf("%d, %d, %d\n",
				    			              job_array[n][PID],
				    			              tmpStartTime,
				    			              tmptime
				    			              );
				    			  track++;
				    			  i = 0;
				    		  }
				    	  }
				      }
			   }
		   }
	   }
   }

// Code for Shortest Job next without preemption :

   else if (strcmp(sch_type, "SJN") ==0)
   {
	   int tmpPID;
	   int tmpArrival;
	   int tmpRuntime;
//	   int tmpPriority;
	   int tmptime = 0;
	   int tmpStartTime = 0;
	   int track = 0;
	   int i,j,n = 0;

	   while(track < number_of_jobs )
	   {
		   for( i= 0; i < number_of_jobs ; i++)
		   {
			   if ( job_array[i][RUNTIME] != 0)
			   {
				      tmpPID      = job_array[i][PID];
				      tmpArrival  = job_array[i][ARRIVAL];
				      tmpRuntime = 	job_array[i][RUNTIME];

				      if( job_array[i][RUNTIME] < tmptime || job_array[i][ARRIVAL] == tmptime )
				      {
				    	  for( j= 0; j < number_of_jobs ; j++ )
				    	  {
				    		  if (job_array[j][RUNTIME] != 0)
				    		  {
				    			  if ( job_array[j][ARRIVAL] < tmptime || job_array[j][ARRIVAL] == tmptime)
				    			  {
				    				  if ( tmpRuntime > job_array[j][RUNTIME]  )
				    				  {
				    					  tmpPID      = job_array[j][PID];
				    					  tmpArrival  = job_array[j][ARRIVAL];
				    					  tmpRuntime = job_array[j][RUNTIME];
				    				  }
				    				  else if( tmpRuntime == job_array[j][RUNTIME] )
				    				  {
				    					  if ( tmpArrival > job_array[j][ARRIVAL] )
				    					  {
				    						  tmpPID      = job_array[j][PID];
				    						  tmpArrival  = job_array[j][ARRIVAL];
				    						  tmpRuntime = job_array[j][RUNTIME];
				    					  }
				    					  else if (tmpArrival  == job_array[j][ARRIVAL])
				    					  {
				    						  tmpPID      = job_array[j][PID];
				    					  }
				    				  }
				    			  }
				    		  }
				    	  }

				    	  for( n = 0 ; n < number_of_jobs ; n++ )
				    	  {
				    		  if (tmpPID == job_array[n][PID])
				    		  {
				    			  tmpStartTime = tmptime;
				    			  tmptime = tmptime + job_array[n][RUNTIME];
				    			  job_array[n][RUNTIME] = 0;
				    			  printf("%d, %d, %d\n",
				    			              job_array[n][PID],
				    			              tmpStartTime,
				    			              tmptime
				    			              );
				    			  track++;
				    			  i = 0;
				    		  }
				    	  }
				      }
			   }
		   }
	   }
   }
   else
	   printf("Invalid scheduler");

   return 0;
}


