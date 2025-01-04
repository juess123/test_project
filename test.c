#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NEXT 1          //往后面寻找匹配的坐标
#define PRE -1          //往前面寻找匹配的坐标
#define FIRST_FIRST 1   //当前直线，的第一个坐标，覆盖，比较直线，的第一个坐标
#define FIRST_SECOND 2  //当前直线，的第一个坐标，覆盖，比较直线，的第二个坐标
#define SECOND_FIRST 3  //当前直线，的第二个坐标，覆盖，比较直线，的第一个坐标
#define SECOND_SECOND 4 //当前直线，的第二个坐标，覆盖，比较直线，的第二个坐标
#define X_COVER_X 1     //当前直线的x被覆盖 
#define Y_COVER_y 2     //当前直线的y被覆盖
#define MAX_CLOSE_LINE_NUM 10 //闭合直线的数量
typedef struct Node
{
    int x1;
    int y1;
    int x2;
    int y2;
    int traversal_flag=0;
    struct Node* next;
    struct Node* pre;
}Node;
struct Data
{
    char name[15];
    int x1;
    int y1;
    int x2;
    int y2;
};
Node* InitNode(void)
{
    Node* T=(Node*)malloc(sizeof(Node));
    if(!T)printf("error");
    T->next=NULL;
    T->pre=NULL;
    T->x1=0;
    T->x2=0;
    T->y1=0;
    T->y2=0;
    T->traversal_flag=0;
    return T;
}
void AddNode(Node* T,int* array,int len)
{
    for(int i=0;i<len;)
    {
        Node* node=(Node*)malloc(sizeof(Node));
        if(!node)printf("error");
        node->x1=array[i++];
        node->y1=array[i++];
        node->x2=array[i++];
        node->y2=array[i++];
        node->traversal_flag=0;
        if(T->next!=NULL)T->next->pre=node;
        node->next=T->next;
        T->next=node;
        node->pre=T;
        
    }
}
void Func1(int* array,int* mode)
{
    int abs_array[6];
    abs_array[0]=abs(array[0]-array[4])+abs(array[1]-array[5]); 
    abs_array[1]=abs(array[0]-array[6])+abs(array[1]-array[7]); 
    abs_array[2]=abs(array[2]-array[4])+abs(array[3]-array[5]);
    abs_array[3]=abs(array[2]-array[6])+abs(array[3]-array[7]);
    abs_array[4]=abs(array[4]-array[6]);
    abs_array[5]=abs(array[5]-array[7]);
    if(abs_array[4] < 20 || abs_array[5]< 20)
    {
        mode[1]= (abs_array[4] < abs_array[5]) ? 1 : 2;
    }
    for(int i=0;i<4 && mode[0]==0;i++)
    {
        mode[0] = (abs_array[i] < 30) ? i+1 : 0;
    }
}
Node* Func3(Node* current_node,int direction,Node* T)
{
    if(current_node==NULL)return NULL;
    Node* compare_node=current_node;
    while(1)
    {
        direction==1 ? compare_node=compare_node->next:compare_node=compare_node->pre;
        if(compare_node==NULL)return NULL;
        if(compare_node->traversal_flag>0)continue;
        int array[]={
            current_node->x1,current_node->y1,
            current_node->x2,current_node->y2,
            compare_node->x1,compare_node->y1,
            compare_node->x2,compare_node->y2};
        int mode[2]={0};
        Func1(array,mode);
        switch(mode[0])
        {
            case 0: break;
            case FIRST_FIRST: compare_node->x1=current_node->x1,compare_node->y1=current_node->y1;break;
            case FIRST_SECOND: compare_node->x2=current_node->x1,compare_node->y2=current_node->y1;break;
            case SECOND_FIRST: compare_node->x1=current_node->x2,compare_node->y1=current_node->y2;break;
            case SECOND_SECOND: compare_node->x2=current_node->x2,compare_node->y2=current_node->y2;break;
            default : break;
        }
        switch(mode[1])
        {
            case X_COVER_X: if(mode[0]==1||mode[0]==3)compare_node->x2=compare_node->x1;if(mode[0]==2||mode[0]==4)compare_node->x1=compare_node->x2;break;
            case Y_COVER_y: if(mode[0]==1||mode[0]==3)compare_node->y2=compare_node->y1;if(mode[0]==2||mode[0]==4)compare_node->y1=compare_node->y2;break;
            default : break;
        }
        if(mode[0]!=0)
        {
            T->traversal_flag++;
            current_node->traversal_flag=T->traversal_flag;
            return compare_node;
        }
    }
    return NULL;
}
void Func4(Node** line)
{
    Node* node=NULL;
    Node* head_node=NULL;
    while((*line)!=NULL)
    {
        node=(*line)->next;
        head_node=node;
        while(node->next)
        {
            node=node->next;
        }
        int array[8]=
        {
            head_node->x1,head_node->y1,
            head_node->x2,head_node->y2,
            node->x1,node->y1,
            node->x2,node->y2
        };
        int mode[2]={0};
        Func1(array,mode);
        switch(mode[0])
        {
            case 1:if(mode[1]==1)head_node->x1=node->x1;node->y1=head_node->y1;if(mode[1]==2)head_node->y1=node->y1;node->x1=head_node->x1;break;
            case 2:if(mode[1]==1)head_node->x1=node->x2;node->y2=head_node->y1;if(mode[1]==2)head_node->y1=node->y2;node->x2=head_node->x1;break;
            case 3:if(mode[1]==1)head_node->x2=node->x1;node->y1=head_node->y2;if(mode[1]==2)head_node->y2=node->y1;node->x1=head_node->x2;break;
            case 4:if(mode[1]==1)head_node->x2=node->x2;node->y2=head_node->y2;if(mode[1]==2)head_node->y2=node->y2;node->x2=head_node->x2;break;
            default : break;
        }
        line++;
        
    }
}
Node* Func5(Node*T)
{
    if(T->traversal_flag==1)
    {
        T->traversal_flag=0;
        return NULL;
    }
    Node* line_list=InitNode();
    Node* node=T->next;
    Node* current_node=NULL;
    int index=1;
    while(node && index<=T->traversal_flag)
    {
        current_node=node->next;
        if(node->traversal_flag==index)
        {
            if(node->next==NULL)
            {
                node->pre->next=NULL;
            }
            else
            {
                node->pre->next=node->next;
                node->next->pre=node->pre;
            }
            node->pre=NULL;
            node->next=line_list->next;
            line_list->next=node;
            index++;
            current_node=T->next;
        }
        node=current_node;  
    }
    T->traversal_flag=0;
    return line_list;
}
Node** LineHandle(Node *T)
{
    Node* node=T->next;
    Node* node_pre=NULL;
    Node* node_next=NULL;
    Node* temp_node=NULL;

    Node** p_line=(Node**)calloc(MAX_CLOSE_LINE_NUM,sizeof(Node*));
    int find_flag=0;
    int index=0;
    while(node)
    {
        while(find_flag<2)
        {
            find_flag=0;
            node_next=Func3(node,NEXT,T);
            if(node_next==NULL)
            {
                find_flag++;
            }
            else
            {
                node=node_next;
            }
            node_pre=Func3(node,PRE,T);
            if(node_pre==NULL)
            {
                find_flag++;
            }
            else
            {
                node=node_pre;
            }
        }
        find_flag=0;
        T->traversal_flag++;
        node->traversal_flag=T->traversal_flag;
        temp_node=Func5(T);
        if(temp_node)
        {
            if(index>MAX_CLOSE_LINE_NUM){printf("p_line is full");return NULL;}
            p_line[index]=temp_node;
            index++;
            node=T->next;
        }
        else
        {
            node=node->next;
        }
    }
    Func4(p_line);
    return p_line;
}
void DataDeduplication(struct Data* array,int len)
{
    int abs_1,abs_2,abs_sum;
    for(int i=0;i<len;i++)
    {
        for(int j=i+1;j<len;j++)
        {
            if(strcmp(array[i].name,array[j].name)==0 && array[i].name[0]!='0')
            {
                
                abs_1=abs(array[i].x1-array[j].x1)+(array[i].y1-array[j].y1);
                abs_2=abs(array[i].x2-array[j].x2)+(array[i].y2-array[j].y2);
                abs_sum=abs_1+abs_2;
                if(abs_sum<40)
                {
                    array[j].name[0]='0';
                }
            }
        }
    }
}
struct Data* Round(struct Data* array,int len)
{
    int i,capacity;
    while(i<len)
    {
        if(strcmp(array[i].name,"round")==0)capacity++;
        i++;
    }
    struct Data* round=(struct Data*)(malloc(sizeof(struct Data)*capacity));
    for(i=0;i<len;i++)
    {
        if(strcmp(array[i].name,"round")==0)
        {
            
        }
    }
    return NULL;
}
void StraightLine(int* array,int len)
{
    int mode[2]={0};
    int temp_array[8]={0};
    for(int i=0;i<len;i=i+4)
    {
        temp_array[4]=array[i+0];
        temp_array[5]=array[i+1];
        temp_array[6]=array[i+2];
        temp_array[7]=array[i+3];
        Func1(temp_array,mode);
        if(mode[1]==1)
        {
            array[i+0]=(array[i+0]+array[i+2])/2;
            array[i+2]=array[i+0];
        }
        else if(mode[1]==2)
        {
            array[i+1]=(array[i+1]+array[i+3])/2;
            array[i+3]=array[i+1];
        }
        mode[1]=0;
    }
}
void PrintfLine(Node** line,const char* tip)
{
    printf("--------------------------------------------------------");
    printf("\n%s\n",tip);
    printf("\n");
    while(*line)
    {
        Node* node=*line;
        printf("\n");
        while(node)
        {
            printf("%d,%d,%d,%d\n",node->x1,node->y1,node->x2,node->y2);
            node=node->next;
        }
        line++;
    }
    printf("\n");
    printf("--------------------------------------------------------");
}
void PrintfSoureCAD(Node** T,const char* tip)
{
    printf("\n");
    printf("\n%s\n",tip);
    Node* node=(*T)->next;
    while(node)
    {
        printf("%d\n%d\n%d\n%d\n",node->x1,node->y1,node->x2,node->y2);
        node=node->next;
    }
    printf("\n");
}
void PrintfSource(Node** T,const char* tip)
{
    printf("--------------------------------------------------------");
    printf("\n%s\n",tip);
    Node* node=(*T)->next;
    printf("\n");
    while(node)
    {
        printf("%d,%d,%d,%d\n",node->x1,node->y1,node->x2,node->y2);
        node=node->next;
    }
    printf("\n");
    printf("--------------------------------------------------------");
}
int main()
{
    void (*MyPrintf[3])(Node** T,const char* tip);
    MyPrintf[0]=PrintfLine; 
    MyPrintf[1]=PrintfSource;
    MyPrintf[2]=PrintfSoureCAD;
    int array[]=
    {
        960, 1023, 966, 901, 
        1181, 945, 1207, 940, 
        629, 331, 672, 325, 
        665, 712, 672, 333, 
        1007, 1023, 1013, 901, 
        951, 1023, 1353, 1017, 
        964, 910, 1347, 904, 
        299, 714, 664, 706, 
        1180, 986, 1205, 981, 
        293, 912, 680, 902, 
        625, 661, 632, 330, 
        626, 1022, 633, 901, 
        1342, 1020, 1349, 905, 
        292, 668, 632, 661, 
        297, 1025, 675, 1017,
        289, 1024, 300, 904,
        292, 710, 297, 662,
        665, 1023, 673, 900,
    };
    int len=(sizeof(array)/sizeof(int));
    //DataDeduplication(array,len);
    StraightLine(array,len);
    
    Node* T=InitNode();


    AddNode(T,array,len);

    MyPrintf[1](&T,"init data");

    Node** line=LineHandle(T);

    MyPrintf[0](line,"3 line data");

    MyPrintf[1](&T,"rest data");

}