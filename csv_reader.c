#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>
#define QUOTE '"'
#define NEW_LINE '\n'
#define MAX_CELL_SIZE 500
#define COLUMN_SIZE 3
#define MAX_BUFFER 1000000

int main(){
    FILE *fp;
    char c;
    char quote = '"';
    int quote_start     = 0;
    int quote_end       = 0;
    int new_row         = 0;
    int row_counter     = 0;
    int cell_counter    = 0;
    int cell_size       = 0;
    char cell[MAX_CELL_SIZE] ; 
    char sql[MAX_BUFFER];
    /**********************************/
     MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW row;
     char *server = "localhost";
     char *user = "root";
     char *password = ""; 
     char *database = "blink_dnd";
     char *pre_sql  = "INSERT INTO `c_test`(`msisdn`) VALUES ";
     
    conn = mysql_init(NULL);
                         
    if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    memset(sql,0,MAX_BUFFER);
    strcat(sql,pre_sql);  
    /*************************************/
    
    fp=fopen ("big_csv.csv","rb");
    
    do{
        c = fgetc(fp);
        if(c==EOF)
            break;
        
        switch(c){
            case QUOTE:
                new_row = 0;
                if(quote_start){
                    quote_end  = 1;
                    quote_start= 0;
                }else{
                   quote_end  = 0;
                   quote_start= 1;
                }
                
            break;
            // case NEW_LINE:
                // new_row = 1;
                // row_counter++;
            // break;
            default:
                if(quote_start){
                    if(cell_size<MAX_CELL_SIZE){
                        cell[cell_size] = c;
                        cell_size++;
                    }
                    //continue;
                }else if(quote_end){
                    cell_counter++;
                    cell[cell_size+1] = '\0';
                    
                    
                    if(cell_counter==1){
                        // printf("MSISDN %s %d \n",cell,cell_counter);
                        row_counter++;
                        
                        
/***********************************************************************************/                        
                        
                        strcat(sql,"('");
                        strcat(sql,cell);
                        strcat(sql,"'),\0");
                        
                        //printf("%s \n",sql);
                        
                        if((row_counter%10000)==0){
                            
                            
                            sql[strlen(sql)-1] = 0;
                            
                            if (mysql_query(conn,sql)) {
                                fprintf(stderr, "%s\n", mysql_error(conn));
                                exit(1);
                            }
                            
                            
                            printf("%d\n",row_counter);
                            // exit(0);
                            memset(sql,0,MAX_BUFFER);
                            strcat(sql,pre_sql);  
                        }
                        
                        
                        
                        
/**********************************************************************************/                        
                        
                        
                        
                        
                    }
                   
                    memset(cell,0,500);
                    cell_size=0;
                    //continue;
                    if(cell_counter >= COLUMN_SIZE){
                        cell_counter = 0;
                    }
                        
                }
                
                
        }
        
        
    }while(1);
    
    sql[strlen(sql)-1] = 0;
    if (mysql_query(conn,sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    
    printf("Total rows %d",row_counter);
    return 0;
}