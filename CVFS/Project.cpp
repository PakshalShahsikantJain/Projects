#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define MAXFILES 50
#define FILESIZE 1024

#define READ 4
#define WRITE 2

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

struct SuperBlock
{
    int TotalInodes;
    int FreeInode;
}Obj_Super;

struct inode
{
    char File_name[50];
    int Inode_number;
    int File_Size;
    int File_Type;      // If it is 0 mens file is deleted
    int ActualFileSize;
    int Link_Count;
    int Reference_Count;
    char * Data;
    int permission;
    struct inode *next;
};

typedef struct inode INODE;
typedef struct inode * PINODE;
typedef struct inode ** PPINODE;

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Count;
    PINODE iptr;
    int Mode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

struct UFDT
{
    PFILETABLE ufdt[MAXFILES];
}UFTDObj;


PINODE Head = NULL; // Global pointer of inode

// It is used to check whether the given file name is already present or not
bool ChekFile(char *name)
{
    PINODE temp = Head;
    
    while(temp != NULL)
    {
        if(temp->File_Type != 0)
        {
            if(strcmp(temp->File_name, name) == 0)
            {
                break;
            }
        }
        temp = temp -> next;
    }
    
    if(temp == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void CreateUFDT()
{
    int i = 0;
    
    for(i = 0; i < MAXFILES; i++)
    {
        UFTDObj.ufdt[i] = NULL;
    }
}

void CreateDILB()   // Create linked list of iniodes
{
    // Create linked list of inodes
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = Head;
    
    while(i <= MAXFILES)   // Loop iterates 100 times
    {
        newn = (PINODE)malloc(sizeof(INODE));
        
        newn->Inode_number = i;
        newn->File_Size = FILESIZE;
        newn->File_Type = 0;
        newn->ActualFileSize = 0;
        newn->Link_Count = 0;
        newn->Reference_Count = 0;
        newn->Data = NULL;
        newn->next = NULL;
        
        if(Head == NULL)    // First inode
        {
            Head = newn;
            temp = Head;
        }
        else    // iNode second onwords
        {
            temp->next = newn;
            temp = temp ->next;
        }
        
        i++;
    }
    
    printf("DILB created succesfully!!\n");
}

void CreateSuperBlock()
{
    Obj_Super.TotalInodes = MAXFILES;
    Obj_Super.FreeInode = MAXFILES;
    
    printf("Super block created Succesfully\n");
}

void InitializeSuperBlock()
{
	int i = 0;
	for(i = 0;i < 50 ; i++)
	{
		UFTDObj.ufdt[MAXFILES] = NULL;	
	}
}

void SetEnvoirnment()
{
    CreateDILB();
    CreateSuperBlock();
    CreateUFDT();
    printf("Envoirnment for the Virtual file system is set...\n");
}

void DeleteFile(char *name)
{
    bool bret = false;
    
    if(name == NULL)
    {
        return;
    }
    
    bret = ChekFile(name);
    if(bret == false)
    {
        printf("There is no such file\n");
        return;
    }
    
    // Search UFDT entry
    int i = 0;
    for(i = 0; i < MAXFILES; i++)
    {
        if(strcmp(UFTDObj.ufdt[i]->iptr->File_name, name) == 0)
        {
            break;
        }
    }
    
    strcpy(UFTDObj.ufdt[i]->iptr->File_name,"");
    UFTDObj.ufdt[i]->iptr->File_Type = 0;      // If it is 0 mens file is deleted
    UFTDObj.ufdt[i]->iptr->ActualFileSize = 0;
    UFTDObj.ufdt[i]->iptr->Link_Count = 0;
    UFTDObj.ufdt[i]->iptr->Reference_Count = 0;
    
    //Free the memory of file
    free( UFTDObj.ufdt[i]->iptr->Data);
    
    free(UFTDObj.ufdt[i]);
    
    UFTDObj.ufdt[i] = NULL;
    
    Obj_Super.FreeInode++;
}

int CreateFile(char *name, int Permission)
{
    bool bret = false;
    
    if((name == NULL) || (Permission > READ+WRITE) || (Permission < WRITE))
    {
        return -1;
    }
    
    bret = ChekFile(name);
    if(bret == true)
    {
        printf("File is already present\n");
        return -1;
    }
    
    if(Obj_Super.FreeInode == 0)
    {
        printf("There is no inode to create the file\n");
        return -1;
    }
    
    // Search for empty entry from UFDT
    int i = 0;
    for(i = 0; i < MAXFILES; i++)
    {
        if(UFTDObj.ufdt[i] == NULL)
        {
            break;
        }
    }
    
    if(i == MAXFILES)
    {
        printf("Unable to get entry in UFDT\n");
        return -1;
    }
    
    // Allicate memory for file table
    UFTDObj.ufdt[i] = (PFILETABLE)malloc(sizeof(FILETABLE));
    
    // Iniotialiose the file table
    UFTDObj.ufdt[i]->ReadOffset = 0;
    UFTDObj.ufdt[i]->WriteOffset = 0;
    UFTDObj.ufdt[i]->Mode = Permission;
    UFTDObj.ufdt[i]->Count = 1;
    
    // Search empty inode
    PINODE temp = Head;
    
    while(temp != NULL)
    {
        if(temp-> File_Type == 0)
        {
            break;
        }
        temp = temp->next;
    }
    
    UFTDObj.ufdt[i]->iptr = temp;
    strcpy(UFTDObj.ufdt[i]->iptr->File_name,name);
    UFTDObj.ufdt[i]->iptr->File_Type = REGULAR;      // If it is 0 mens file is deleted
    UFTDObj.ufdt[i]->iptr->ActualFileSize = 0;
    UFTDObj.ufdt[i]->iptr->Link_Count = 1;
    UFTDObj.ufdt[i]->iptr->Reference_Count = 1;
    
    // Allocate memory for files data
    UFTDObj.ufdt[i]->iptr->Data = (char *)malloc(sizeof(FILESIZE));
    
    Obj_Super.FreeInode--;
    
    return i;
}

void LS()
{
    PINODE temp = Head;
    
    while(temp != NULL)
    {
        if(temp -> File_Type != 0)
        {
            printf("%s\n",temp->File_name);
        }
        temp = temp->next;
    }
}

// file descriptor
// Data
// Size of data

int WriteFile(char *name, char *arr, int size)
{
    // Search fd from the name
    
    return 0;
}

int WriteFile(int fd, char *arr, int size)
{
    if(UFTDObj.ufdt[fd] == NULL)
    {
        printf("Invalid file descriptor\n");
        return -1;
    }
    
    if(UFTDObj.ufdt[fd]->Mode == READ)
    {
        printf("There is no write permission\n");
        return -1;
    }
    
    // Data gets copied into the buffer
    strncpy(((UFTDObj.ufdt[fd]->iptr->Data)+(UFTDObj.ufdt[fd]->WriteOffset)),arr,size);
    
    UFTDObj.ufdt[fd]->WriteOffset = UFTDObj.ufdt[fd]->WriteOffset + size;
    
    return size;
}

int ReadFile(int fd, char *arr, int size)
{
	int read_Size = 0;

	if(UFTDObj.ufdt[fd] == NULL)
	{
		return -1;
	}

	if((UFTDObj.ufdt[fd]->Mode != READ)&&(UFTDObj.ufdt[fd]->Mode != READ + WRITE))
	{
		return -2;
	}

	if(UFTDObj.ufdt[fd]->ReadOffset == UFTDObj.ufdt[fd]->iptr->ActualFileSize)
	{
		return -3;
	}

	if(UFTDObj.ufdt[fd]->iptr->File_Type != REGULAR)
	{
		return -4;
	}

	read_Size = (UFTDObj.ufdt[fd]->iptr->ActualFileSize)-(UFTDObj.ufdt[fd]->ReadOffset);

	if(read_Size < size)
	{
		strncpy(arr,(UFTDObj.ufdt[fd]->iptr->Data) + (UFTDObj.ufdt[fd]->ReadOffset),read_Size);
		UFTDObj.ufdt[fd]->ReadOffset = UFTDObj.ufdt[fd]->ReadOffset + read_Size;	
	}

	else
	{
		strncpy(arr,(UFTDObj.ufdt[fd]->iptr->Data) + (UFTDObj.ufdt[fd]->ReadOffset),size);
		UFTDObj.ufdt[fd]->ReadOffset = UFTDObj.ufdt[fd]->ReadOffset + size;	
	}

	return size;
}

int GetFDfromName(char * name)
{
	int i = 0;

	while(i < 50)
	{
		if(UFTDObj.ufdt[MAXFILES] != NULL)
			if(strcmp((UFTDObj.ufdt[MAXFILES]->iptr->File_name),name) == 0)
				break;
		i++;
	}
	if(i == 50)
	{
		return -1;
	}
	else 
	{
		return i;
	}

	return 0;
}

PINODE Get_Inode(char *name)
{
	PINODE temp = Head;
	int i = 0;

	if(name == NULL)
	{
		return NULL;
	}

	while(temp != NULL)
	{
		if(strcmp(name,temp->File_name) == 0)
		{
			break;
		}
		temp = temp->next; 
	}

	return temp;
}

void DisplayHelp()
{
    printf("-----------------------------------------------------\n");
    printf("open : It is used to open the existing file\n");
    printf("close : It is used to close the opened file\n");
    printf("read : It is used to read the contents of file\n");
    printf("write : It is used to write the data into file\n");
    printf("lseek : It is used to change the offset of file\n");
    printf("stat : It is used to odisplay the information of file\n");
    printf("fstat : It is used to display the information of opened file\n");
    printf("creat : It is used to create new regular file\n");
    printf("rm : It is used to delete regular file\n");
    printf("ls : It is used to display all names of files\n");
    printf("-----------------------------------------------------\n");
}

void ManPage(char *str)
{
	if(strcmp(str,"open") == 0)
    {
        printf("Desricption : It is used to open an existing file \n");
        printf("Usage : open File_name Mode\n");
    }
    else if (strcmp(str,"close") == 0)
    {
        printf("Desricption : It is used to close the existing file\n");
        printf("Usage : close File_name\n");
    }
    else if (strcmp(str,"ls") == 0)
    {
        printf("Desricption : It is used to list out all names of the files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp(str,"stat") == 0)
    {
    	printf("Description : It is used to Dispaly file or File System Status/get file Status\n");
    	printf("Usage : stat File_name");
    }
    else if(strcmp(str,"fstat") == 0)
    {
    	printf("Description : It is used to Get Status of File\n");
    	printf("usage : fstat File_name");
    }
    else if(strcmp(str,"truncate") == 0)
    {
    	printf("Description : It is Used to Shrink or Extend the Size of a File To Given Size\n");
    	printf("Usage : truncate -s 0 File_name");
    }
	else if(strcmp(str,"closeall") == 0)
    {	
    	printf("Description : It is used To Close all Running Program At once\n");
    	printf("Usage : closeall");
    }
    else if(strcmp(str,"lseek") == 0)
    {	
    	printf("Description : It is Used To Reposition The File offset of the opened file With the file Descriptor fd to the argument offset");
    	printf("Usage : off_t lseek");
    }
    else if(strcmp(str,"rm") == 0)
    {
    	printf("Description : It is used to Delete File With Any of File Extensions\n");
    	printf("Usage : rm.Extensions(Extension May Be .c,.cpp,.java,.mp3,etc....)\n");
    }
    else
    {
        printf("ERROR : No Manual Entry found\n");
    }
}

int open(char *name,int Mode)
{
	int i = 0;
	PINODE temp = NULL;

	if((name == NULL)||(Mode <= 0))
	{
		return -1;
	}

	temp = Get_Inode(name);
	if(temp == NULL)
	{
		return -2;
	}

	if(temp->permission < Mode)
	{
		return -3;
	}

	while(i < 50)
	{
		if(UFTDObj.ufdt[i] == NULL)
		{
			break;
		}

		i++;
	}

	UFTDObj.ufdt[i] = (PFILETABLE)malloc(sizeof(FILETABLE));
	if(UFTDObj.ufdt[i] == NULL)
	{
		return -1;
	}
	UFTDObj.ufdt[i]->Count = 1;
	UFTDObj.ufdt[i]->Mode = Mode;
	if(Mode == READ + WRITE)
	{
		UFTDObj.ufdt[i]->ReadOffset = 0;
		UFTDObj.ufdt[i]->WriteOffset = 0;
	}
	else if(Mode == READ)
	{
		UFTDObj.ufdt[i]->ReadOffset = 0;
	}
	else if(Mode == WRITE)
	{
		UFTDObj.ufdt[i]->WriteOffset = 0;
	}
	UFTDObj.ufdt[i]->iptr = temp;
	(UFTDObj.ufdt[i]->iptr->Reference_Count)++;

	return i;
}

void CloseFileByfd(int fd)
{
	UFTDObj.ufdt[fd]->ReadOffset = 0;
	UFTDObj.ufdt[fd]->WriteOffset = 0;
	(UFTDObj.ufdt[fd]->iptr->Reference_Count)--;
}

int CloseFileByName(char *name)
{
	int i = 0;
	i = GetFDfromName(name);

	if(i == -1)
	{
		return -1;
	}

	UFTDObj.ufdt[i]->ReadOffset = 0;
	UFTDObj.ufdt[i]->WriteOffset = 0;
	(UFTDObj.ufdt[i]->iptr->Reference_Count)--;

	return 0;
}

void CloseAllFiles()
{
	int i = 0;

	while(i < 50)
	{
		if(UFTDObj.ufdt[i] != NULL)
		{
			UFTDObj.ufdt[i]->ReadOffset = 0;
			UFTDObj.ufdt[i]->WriteOffset = 0;
			(UFTDObj.ufdt[i]->iptr->Reference_Count)--;
			break;
		}
		i++;
	}
}
int fstat_file(int fd)
{
	PINODE temp = Head;
	int i = 0;

	if(fd < 0)
	{
		return -1;
	}

	if(UFTDObj.ufdt[fd] == NULL)
	{
		return -2;
	}

	temp = UFTDObj.ufdt[fd] -> iptr;

	printf("\n-------------------Statistical Information About File---------------------\n");
	printf("File Name : %s\n",temp->File_name);
	printf("Inode Number : %d\n",temp->Inode_number);
	printf("File Size : %d\n",temp->File_Size);
	printf("Actual File Size : %d\n",temp->ActualFileSize);
	printf("Link Count : %d\n",temp->Link_Count);
	printf("Reference Count : %d\n",temp->Reference_Count);

	if(temp->permission == 1)
	{
		printf("File permission : Read Only\n");
	}
	else if(temp->permission == 2)
	{
		printf("File Permission : Write only\n");
	}
	else if(temp->permission == 3)
	{
		printf("File Permission : Read & Write\n");
	}
	printf("------------------------------------------------------\n\n");

	return 0;
}
int stat_file(char *name)
{
	PINODE temp = Head;
	int i = 0;

	if(name == NULL)
	{
		return -1;
	}

	while(temp != NULL)
	{
		if(strcmp(name,temp->File_name) == 0)
		{
			break;
		}

		temp = temp->next;
	}

	if(temp == NULL)
	{
		return -2;
	}
	
	printf("\n-------------------Statistical Information About File---------------------\n");
	printf("File Name : %s\n",temp->File_name);
	printf("Inode Number : %d\n",temp->Inode_number);
	printf("File Size : %d\n",temp->File_Size);
	printf("Actual File Size : %d\n",temp->ActualFileSize);
	printf("Link Count : %d\n",temp->Link_Count);
	printf("Reference Count : %d\n",temp->Reference_Count);

	if(temp->permission == 1)
	{
		printf("File permission : Read Only\n");
	}
	else if(temp->permission == 2)
	{
		printf("File Permission : Write only\n");
	}
	else if(temp->permission == 3)
	{
		printf("File Permission : Read & Write\n");
	}
	printf("------------------------------------------------------\n\n");

	return 0;

}

int lseekFile(int fd,int size,int from)
{
	if((fd < 0)||(from > 2))
	{
		return -1;
	}
	if((UFTDObj.ufdt[fd]->Mode == READ)||(UFTDObj.ufdt[fd]->Mode == READ + WRITE))
	{
		if(from == CURRENT)
		{
			if(((UFTDObj.ufdt[fd]->ReadOffset)+ size) > UFTDObj.ufdt[fd]->iptr->ActualFileSize)
			{
				return -1;
			}
			if(((UFTDObj.ufdt[fd]->ReadOffset) + size) < 0)
			{
				return -1;
			}
			(UFTDObj.ufdt[fd]->ReadOffset) = (UFTDObj.ufdt[fd]->ReadOffset) + size;
		}
		else if(from == START)
		{
			if(size > (UFTDObj.ufdt[fd]->iptr->ActualFileSize))
			{
				return -1;
			}
			if(size < 0)
			{
				return -1;
			}
			(UFTDObj.ufdt[fd]->ReadOffset) = size;
		}
		else if(from == END)
		{
			if((UFTDObj.ufdt[fd]->iptr->ActualFileSize)+ size > MAXFILES)
			{
				return -1;
			}
			if(((UFTDObj.ufdt[fd]->ReadOffset)+ size) < 0)
			{
				return -1;
			}
			(UFTDObj.ufdt[fd]->ReadOffset) = (UFTDObj.ufdt[fd]->iptr->ActualFileSize) + size;
		}
	}

	else if(UFTDObj.ufdt[fd]->Mode == WRITE)
	{
		if(from == CURRENT)
		{
			if(((UFTDObj.ufdt[fd]->WriteOffset) + size) > MAXFILES)
			{
				return -1;
			}
			if(((UFTDObj.ufdt[fd]->WriteOffset)+ size)<0)
			{
				return -1;
			}
			if(((UFTDObj.ufdt[fd]->WriteOffset)+ size) > (UFTDObj.ufdt[fd]->iptr->ActualFileSize))
			{
				(UFTDObj.ufdt[fd]->iptr->ActualFileSize) = (UFTDObj.ufdt[fd]->WriteOffset) + size;
			}

			(UFTDObj.ufdt[fd]->WriteOffset) = (UFTDObj.ufdt[fd]->WriteOffset) + size;
		}

		else if(from == START)
		{
			if(size > MAXFILES)
			{
				return -1;
			}
			if(size < 0)
			{
				return -1;
			}
			if(size > (UFTDObj.ufdt[fd]->iptr->ActualFileSize))
			{
				(UFTDObj.ufdt[fd]->iptr->ActualFileSize) = size;
			}
			(UFTDObj.ufdt[fd]->WriteOffset) = size;
		}
		else if(from == END)
		{
			if((UFTDObj.ufdt[fd]->iptr->ActualFileSize) + size > MAXFILES)
			{
				return -1;
			}
			if(((UFTDObj.ufdt[fd]->WriteOffset) + size) < 0)
			{
				return -1;
			}
			(UFTDObj.ufdt[fd]->WriteOffset) = (UFTDObj.ufdt[fd]->iptr->ActualFileSize) + size;
		}
	}

	return 0;
}

int truncate_File(char *name)
{
	int fd = GetFDfromName(name);
	if(fd == -1)
	{
		return -1;
	}
	memset(UFTDObj.ufdt[fd]->iptr->Data,0,1024);
	UFTDObj.ufdt[fd]->ReadOffset = 0;
	UFTDObj.ufdt[fd]->WriteOffset = 0;
	UFTDObj.ufdt[fd]->iptr->ActualFileSize = 0;

	return 0;
}
int main()
{
    // Variable Declations
    char str[80];
    char command[4][80];
    int count = 0;

    printf("Customised Virtual File System\n");
    
    SetEnvoirnment();
    
    while(1)
    {
        printf("Marvellous VFS :> ");
        fgets(str,80,stdin); // Accept the coomand
        fflush(stdin);
        
        count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);  // Break that command ito tokens
        
        if(count == 1)
        {
		    if(strcmp(command[0], "help") == 0)
		    {
		        DisplayHelp();
		    }
		    else if(strcmp(command[0],"exit") == 0)
		    {
		        printf("Thank you for using Marvellous Virtual file system");
		        break;
		    }
		    else if(strcmp(command[0],"cls") == 0)
		    {
		        system("cls");    // cls
		    }
		    else if(strcmp(command[0],"ls") == 0)
		    {
		        LS();
		    }
		    else if(strcmp(command[0],"closeall") == 0)
		    {
		    	CloseAllFiles();
		    	printf("All Files Are Closed Successfully\n");
		    }
			else
		    {
		        printf("Command not found !!\n");
		    }
        }
        else if(count == 2)
        {
            if(strcmp(command[0],"man") == 0)   // man open
            {
                ManPage(command[1]);
            }
			else if(strcmp(command[0],"fstat") == 0)
            {
            	int ret = fstat_file(atoi(command[1]));

            	if(ret == -1)
            	{
            		printf("Error : Incorrect Parameters\n");
            	}
            	else if(ret == -2)
            	{
            		printf("Error : There is No Such File\n");
            	}

            	continue;
            }
            else if(strcmp(command[0],"stat") == 0)
            {
            	int ret = stat_file(command[1]);
            	if(ret == -1)
            	{
            		printf("Error : Incorrect Parameters\n");
            	}
            	else if(ret == -2)
            	{
            		printf("Error : There is No Such File\n");
            	}

            	continue;
            }
            else if(strcmp(command[0],"close") == 0)
            {
            	int ret = CloseFileByName(command[1]);
            	if(ret == -1)
            	{
            		printf("Error : There is No Such File \n");
            	}
            	continue;
            }
            else if(strcmp(command[0],"rm") == 0)
            {
                DeleteFile(command[1]);
            }
            else if(strcmp(command[0],"write") == 0)
            {
                char arr[1024];
                
                printf("Please enter data to write\n");
                fgets(arr,1024,stdin);
                
                fflush(stdin);
                
                int ret = WriteFile(atoi(command[1]),arr,strlen(arr)-1);
                if(ret != -1)
                {
                    printf("%d bytes gets written succesfully in the file\n",ret);
                }
            }
            else if(strcmp(command[0],"truncate") == 0)
            {
            	int ret = truncate_File(command[1]);

            	if(ret == -1)
            	{
            		printf("Error : Incorrect Parameters\n");
            	}

            	continue;
            }
            else
            {
                printf("Command not found !!\n");
            }
        }
        else if (count == 3)
        {
            if(strcmp(command[0],"creat") == 0)
            {
                int fd = 0;
                fd = CreateFile(command[1],atoi(command[2]));
                
                if(fd == -1)
                {
                    printf("Unable to create file\n");
                }
                else
                {
                    printf("File sucussefully opened with FD %d\n",fd);
               	}    
            }
            else if(strcmp(command[0],"open") == 0)
            {
            	int ret = open(command[1],atoi(command[2]));
            	if(ret >= 0)
            	{
            		printf("File is Successfully opened with File descriptor : %d\n",ret);
            	}
            	else if(ret == -1)
            	{
            		printf("Error : Incorrect Paramenters\n");
            	}
            	else if(ret == -2)
            	{
            		printf("Error : File Not present\n");
            	}
            	else if(ret == -3)
            	{
            		printf("Error : Permission Denied\n");
            	}

            	continue;
            }
            else if(strcmp(command[0],"read") == 0)
            {
            	int fd = GetFDfromName(command[1]);
            	if(fd == -1)
            	{
            		printf("Error : Incorrect Parameters\n");
            		continue;
            	}
            	char *ptr = (char *)malloc(sizeof(atoi(command[2])) + 1);

            	if(ptr == NULL)
            	{
            		printf("Error : memory Allocation Failure\n");
            		continue;
            	}

            	int ret = ReadFile(fd,ptr,atoi(command[2]));

            	if(ret > 0)
            	{
            		write(2,ptr,ret);
            	}

            	continue;
            }
         
        }
        else if(count == 4)
        {
        	if(strcmp(command[0],"lseek") == 0)
        	{
        		int fd = GetFDfromName(command[1]);
        		if(fd == -1)
        		{
        			printf("Error : Incorrect Parameters\n");
        			continue;
        		}

        		int ret = lseekFile(fd,atoi(command[2]),atoi(command[3]));
        		if(ret == -1)
        		{
        			printf("Error : Unable to Perform lseek\n");
        		}
        	}
        	else 
        	{
        		printf("\n Error : Command Not found\n");
        		continue;
        	}
        }
        else
        {
            printf("Bad command or file name\n");
        }
        
    }
    
    return 0;
}


void BackupFS()
{
    int fd = 0;
    // Store the address if linkelist head
    PINODE temp = Head;
    
    // Create new file for backup
    fd = creat("Marvellous.txt",0777);
    if(fd == -1)
    {
        printf("Unable to create the file");
        return -1;
    }
    
    // Travel the inodes linkedlist
    while(temp != NULL)
    {
        // Check whether file is existing
        if(temp->File_Type != 0)
        {
            // Write the inode into the backp files
            write(fd,temp,sizeof(INODE));
            // Write the data of file into the backup file
            write(fd,temp->Data, 1024);
        }
        // Incerment the pointer by one
        temp = temp - > next;
    }
}

void RstoreFile()
{
    int fd = 0;
    INODE iobj;
    
    char Data[1024];
    
    fd = open("Marvellous.txt".O_RDONLY);
    if(fd == -1)
    {
        printf("Unable to open tyhe file\n")
        return;
    }
    
    while(ret = (read(fd,iobj,sizeof(iobj))) != 0)
    {
        // Copy the contents from Buffer into the inode of IIT
        // Only copy the information
        // Example
        temp->permission = iobj.permission;
        
        // read the files data
        read(fd,Data,1024);
        // Copy the data of file
        memcpy(temp->Buffer,Data, 1024);
    }
    close(fd);
    printf("BAckup done succesfully")
}