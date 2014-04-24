#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#define GIT_PULL "git pull"
#define GIT_MASTER "git checkout master"
#define GIT_REPO "/var/lib/repo"

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}
int repository(char *zz, char *qq);
int mdir(char *gitcmd, char *dir, char *clone);

int main(int argc, char **argv)
{      
  MYSQL *con = mysql_init(NULL);
  
  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  

//Use argv[1] for mysql password
  if (mysql_real_connect(con, "localhost", "redmine", argv[1], 
          "redmine", 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, "SELECT project_id, url FROM repositories where type='Repository::Git'")) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  
  while ((row = mysql_fetch_row(result))) 
  { 
      //for(int i = 0; i < num_fields; i++) 
       repository(row[0], row[1]);
  }
  
  mysql_free_result(result);
  mysql_close(con);
  
  exit(0);
}

int repository(char *zz, char *qq)
{
    char GIT[200] = "git clone git@git.eltex.loc:";
    char GITEND[] = ".git";
    char CURL[200] = "curl \"http://red.eltex.loc/sys/fetch_changesets?id=";
    char repodir[40] = "";
    char repo[200] = "/var/lib/repo/";
    char repopath[100] = "";
    char repoclone[200] = "";
    int repol = 14;
    int j = strlen(qq);
    int l = 0;
    int zl = 0;
    int sl = 0;
    int cl = 0;
    int rl = repol;
    int len = 0;
    int lclone = 0;
    int countx = 0;
    int count = 0;
    int countr = 0;
    for(int k = repol; k < j; k++)
    {
        if (qq[k] == '/'){
	countx++;
	len = k;
	}
    }
    for(int k = repol; k < len; k++)
    {
       	repopath[l] = qq[k];
       	l++;
    }
    if (countx - 1 == 0)
	strcat(repoclone, repo);
    else
    for(int k = repol; k < j; k++)
    {
        if (qq[k] == '/'){
	countr++;
	if (countr == countx - 1){
	lclone = k;
	printf("%d %d %d\n", lclone, countx, countr);
	}
	}
    }
    for(int k = 0; k < lclone; k++)
    {
       	repoclone[cl] = qq[k];
       	cl++;
    }
    strcat(GIT, repopath);
    strcat(GIT, GITEND);
    printf("|clone %s|\n", repoclone);//row[i] ? row[i] : "NULL"); 
	for(int k = repol; k < j; k++)
    	{
        	if (qq[k] == '/'){
		   count++;
        	   len = k;
		   if (count == countx){
    			for(int k = rl; k < len; k++)
    			{
        			repodir[zl] = qq[k];
        			zl++;
    			}
    			strcat(repo, repodir);
			printf("|if %s|\n", repo);//row[i] ? row[i] : "NULL"); 
			mdir(GIT, repo, repoclone);
		   }else{
    			for(int k = repol; k < len; k++)
    			{
        			repodir[sl] = qq[k];
        			sl++;
    			}
			rl = len;
    			strcat(repo, repodir);
			printf("|else %s|\n", repo);//row[i] ? row[i] : "NULL"); 
			if (mkdir(repo, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0)
				perror("mkdir() error");
		   }
    		}
	}
    memset(&qq[0], 0, sizeof(qq));
    printf("|%s|\n", repo);//row[i] ? row[i] : "NULL"); 
    memset(&repodir[0], 0, sizeof(repodir));
    strcat(zz, "&key=i4KaLbnZfY0EzQyulX8x\"");
    strcat(CURL, zz);
    system(CURL);
    memset(&GIT[0], 0, sizeof(GIT));
    memset(&repo[0], 0, sizeof(repo));
    memset(&CURL[0], 0, sizeof(CURL));
    memset(&zz[0], 0, sizeof(zz));
    return 0;
}


int mdir(char *gitcmd, char *dir, char *clone)
{
    char HEAD[] = "  origin/HEAD -> origin/master\n\0";
    if (mkdir(dir, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0)
    {
        perror("mkdir() error");
        chdir(dir);
        char *cmd = "git branch -r";
        char buf[128];
        FILE *ptr;
        if ((ptr = popen (cmd, "r")) != NULL)
        {
                    char branch[40] = "";
            while (fgets (buf, 128, ptr) != NULL)
            {
                if(strcmp(buf, HEAD) != 0)
                {
                    char GITCHECKOUT[60] = "git checkout ";
                    int origin = 0;
                    int blen = 0;
                    int count = 0;
                    for(int k = 0; k < strlen(buf); k++)
                        if(buf[k] == '/'){
                        count++;
                        if(count == 1)
                        origin = k + 1;
                        }
                        if(count == 1)
                    for(int k = origin; k <= strlen(buf); k++)
                    {
                        branch[blen] = buf[k];
                        blen++;
                    }
                    memset(buf, 0, 128);
                    strcat(GITCHECKOUT, branch);
                    system(GITCHECKOUT);
                    memset(&branch[0], 0, sizeof(branch));
                    memset(&GITCHECKOUT[0], 0, sizeof(GITCHECKOUT));
                    system(GIT_PULL);
                }
            }
        if(system(GIT_MASTER))
            perror("system() error");
        }
	pclose(ptr);
    }else{
    
        perror("mkdir() error");
        chdir(clone);
        system(gitcmd);
        chdir(dir);
        char *cmd = "git branch -r";
        char buf[128];
        FILE *ptr;
        if ((ptr = popen (cmd, "r")) != NULL)
        {
                    char branch[40] = "";
            while (fgets (buf, 128, ptr) != NULL)
            {
                if(strcmp(buf, HEAD) != 0)
                {
                    char GITCHECKOUT[60] = "git checkout ";
                    int origin = 0;
                    int blen = 0;
                    int count = 0;
                    for(int k = 0; k < strlen(buf); k++)
                        if(buf[k] == '/'){
                        count++;
                        if(count == 1)
                        origin = k + 1;
                        }
                        if(count == 1)
                    for(int k = origin; k <= strlen(buf); k++)
                    {
                        branch[blen] = buf[k];
                        blen++;
                    }
                    memset(buf, 0, 128);
                    strcat(GITCHECKOUT, branch);
                    system(GITCHECKOUT);
                    memset(&branch[0], 0, sizeof(branch));
                    memset(&GITCHECKOUT[0], 0, sizeof(GITCHECKOUT));
                    system(GIT_PULL);
                }
            }
        if(system(GIT_MASTER))
            perror("system() error");
        }
    pclose(ptr);
    }
    memset(&HEAD[0], 0, sizeof(HEAD));
    return 0;
}
