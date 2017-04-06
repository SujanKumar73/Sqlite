#include<stdio.h>
#include<sqlite3.h>
#include<stdlib.h>
#include<unistd.h>

//function to hold control in loops
void waitforEOL( void )
{
	char ch;
	ch = getchar();
	while( ch != '\n' )
		ch = getchar();
}

//function to deal with callback for creating table api at sqlite_exec() 
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main()
{
	int choice;
	sqlite3 *db;
	char *zErrMsg=0;
	char *sql;
	int rc;
	rc=sqlite3_open("map.db",&db);	//open database
	const char* data = "Callback function called";
	if(rc)
	{
		fprintf(stderr,"unable to open DB:%s\n",sqlite3_errmsg(db));
		return(0);
	}
	else
	{
		fprintf(stderr,"opened DB successfully\n");
	}
	//sqlite3_close(db);
	////////////////Check table already exist/////////////
	char *check="SELECT name FROM sqlite_master WHERE type='table' AND name='MAP';";
	////////////////Create table QUERY/////////////////////
	sql= "CREATE TABLE MAP(" \
		 "NAME	TEXT  PRIMARY KEY NOT NULL,"\
		 "TYPE TEXT NOT NULL,"\
		 "LATITUDE TEXT NOT NULL,"\
		 "LONGITUDE TEXT NOT NULL,"\
		 "DISTANCE TEXT NOT NULL);";
		 	////////////////passing create table query to exec() api///////////
	rc=sqlite3_exec(db,sql,callback,0,&zErrMsg);
	//printf("%d\n",rc);
	if(rc!=SQLITE_OK)
	{
		fprintf(stderr,"%s\n", zErrMsg);
      	sqlite3_free(zErrMsg);
    }
    else
    {
    	fprintf(stdout,"table created\n");
    }
	sleep(1);
  //  sqlite3_close(db);
	while(1)
	{
		sleep(1);
		system("clear");
		printf("----------MAP------------\n");
		printf("1.Add new Record \n2.Display existing Record\n3.Sort by Type\n4.Quit\n----->");
		scanf("%d",&choice);
		waitforEOL();
			if(choice==1)
			{
				char name[30],type[30],latitude[10],longitude[10],distance[10];
				char statement[300];
				system("clear");
				printf("--------------------------------Give Details--------------------------------\nname-->");
				scanf("%[^\n]s",name);
				waitforEOL();
				printf("type-->");
				scanf("%s",type);
				printf("latitude-->");
				scanf("%s",latitude);
				printf("longitude-->");
				scanf("%s",longitude);
				printf("distance-->");
				scanf("%s",distance);
				
				sprintf(statement," INSERT INTO MAP (NAME,TYPE,LATITUDE,LONGITUDE,DISTANCE) VALUES ('%s','%s','%s','%s','%s')",name,type,latitude,longitude,distance);
				sql = statement; 
				rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
				 if( rc != SQLITE_OK )
				 {
				 	 fprintf(stderr, "SQL error: %s\n", zErrMsg);
  					 sqlite3_free(zErrMsg);
				 }
				 else
				 {
				 	 printf("%s",name);
  					 fprintf(stdout, " Record created successfully\n");
  					// sleep(2);
				 }	
			}
			//sqlite3_close(db);
			else if(choice==2)
			{
				system("clear");
				 sql = "SELECT * from MAP";
				 
				 /* Execute SQL statement */
			   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
			   if( rc != SQLITE_OK )
			   {
				  fprintf(stderr, "SQL error: %s\n", zErrMsg);
				  sqlite3_free(zErrMsg);
			   }
			   else
			   {
				  fprintf(stdout, "All records displayed\n");
				  while(1)
				  {
				  	printf("Press Enter to go back!!\n");
				  	choice=0;
				  	waitforEOL();
				  	break;
				  }
			   }	
			}
			else if(choice==3)
			{
				char statement1[200],sort[20];
				system("clear");
				printf("Enter TYPE you want to sort of [atm,resturant,hospital,etc.....]\n----->");
				scanf("%s",sort);
				sprintf(statement1,"SELECT * from MAP WHERE TYPE = '%s'",sort);
				sql=statement1;
				 rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
				 if( rc != SQLITE_OK )
			   	 {
					  fprintf(stderr, "SQL error: %s\n", zErrMsg);
					  sqlite3_free(zErrMsg);
			  	 } 
			   else
			   	{
					  fprintf(stdout, "--> %s <-- based record listed successfully!!\n",sort);
					  while(1)
					  {
				  		printf("\nPress Enter to go back!!\n");
				  		choice=0;
				  		waitforEOL();
				  		waitforEOL();
				  		break;
					  }
			   	}	
				
			}
			else if(choice==4)
			{
				break;
			}
			else
			{
				system("clear");
				printf("invalid entry!!----------------------------------------\n");
			}
	}					

sqlite3_close(db);	
}
