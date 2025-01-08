#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NEXT 1          //往后面寻找匹配的坐标
#define PRE -1          //往前面寻找匹配的坐标
#define FIRST_FIRST 1   //当前直线，的第一个坐标，覆盖，比较直线，的第一个坐标
#define FIRST_SECOND 2  //当前直线，的第一个坐标，覆盖，比较直线，的第二个坐标
#define SECOND_FIRST 3  //当前直线，的第二个坐标，覆盖，比较直线，的第一个坐标
#define SECOND_SECOND 4 //当前直线，的第二个坐标，覆盖，比较直线，的第二个坐标
#define X_VERTICAl 1     //当前直线的x被覆盖 
#define Y_VERTICAL 2     //当前直线的y被覆盖
#define MAX_CLOSE_LINE_NUM 10 //闭合直线的数量
#define DETERMINATION_NUM 15
int cumulative_error=0;
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
typedef struct MatchMode
{
    int coordinate_match=0;
    int self_data_vertical=0;
    int compare_data_vertical=0;
    int match_error_num=1000;
}MatchMode;
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
void Func1(int* array,MatchMode* mode)
{
    int abs_array[8]={0};
    abs_array[0]=abs(array[0]-array[4])+abs(array[1]-array[5]); 
    abs_array[1]=abs(array[0]-array[6])+abs(array[1]-array[7]); 
    abs_array[2]=abs(array[2]-array[4])+abs(array[3]-array[5]);
    abs_array[3]=abs(array[2]-array[6])+abs(array[3]-array[7]);

    abs_array[4]=abs(array[4]-array[6]);
    abs_array[5]=abs(array[5]-array[7]);
    abs_array[6]=abs(array[0]-array[2]);
    abs_array[7]=abs(array[1]-array[3]);
    for(int i=0;i<4 && mode->coordinate_match==0;i++)
    {
        mode->coordinate_match = (abs_array[i] < DETERMINATION_NUM) ? i+1 : 0;
        if(mode->coordinate_match > 0){cumulative_error+=abs_array[i];mode->match_error_num=abs_array[i];break;}
    }
    mode->compare_data_vertical= (abs_array[4] < abs_array[5]) ? X_VERTICAl : Y_VERTICAL;
    mode->self_data_vertical= (abs_array[6] < abs_array[7]) ? X_VERTICAl : Y_VERTICAL;
    if(mode->self_data_vertical==mode->compare_data_vertical)
    {
        mode->coordinate_match=0;
    }
}
Node* Func3(Node* current_node,int direction,Node* T)
{
    if(current_node==NULL)return NULL;
    Node* compare_node=current_node;
    Node* temp_node;
    int error_num=DETERMINATION_NUM;
    MatchMode mode_select;
    MatchMode temp_mode={0};
    while(1)
    {
        direction==1 ? compare_node=compare_node->next:compare_node=compare_node->pre;
        if(compare_node==NULL)
        {
            compare_node=temp_node;
            mode_select=temp_mode;
            switch(mode_select.coordinate_match)
            {
                case FIRST_FIRST:if(mode_select.compare_data_vertical==X_VERTICAl){current_node->x1=compare_node->x1;compare_node->y1=current_node->y1;}
                                    else{current_node->y1=compare_node->y1;compare_node->x1=current_node->x1;};break;
                case FIRST_SECOND:if(mode_select.compare_data_vertical==X_VERTICAl){current_node->x1=compare_node->x2;compare_node->y2=current_node->y1;}
                                    else{current_node->y1=compare_node->y2;compare_node->x2=current_node->x1;};break;
                case SECOND_FIRST:if(mode_select.compare_data_vertical==X_VERTICAl){current_node->x2=compare_node->x1;compare_node->y1=current_node->y2;}
                                    else{current_node->y2=compare_node->y1;compare_node->x1=current_node->x2;};break;
                case SECOND_SECOND:if(mode_select.compare_data_vertical==X_VERTICAl){current_node->x2=compare_node->x2;compare_node->y2=current_node->y2;}
                                    else{current_node->y2=compare_node->y2;compare_node->x2=current_node->x2;};break;
                default : break;
            }
            if(mode_select.coordinate_match!=0)
            {
                T->traversal_flag++;
                current_node->traversal_flag=T->traversal_flag;
                return compare_node;
            }
            return NULL;
        }
        else
        {
            if(compare_node->traversal_flag>0)continue;
            int array[]={
                current_node->x1,current_node->y1,
                current_node->x2,current_node->y2,
                compare_node->x1,compare_node->y1,
                compare_node->x2,compare_node->y2};
            MatchMode mode;
            Func1(array,&mode);//191,914,335,914 200,973,200,916
            if(mode.match_error_num<error_num)
            {
                error_num=mode.match_error_num;
                temp_mode=mode;
                temp_node=compare_node;
            }
        }
    }
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
        MatchMode mode={0};
        Func1(array,&mode);
        switch(mode.coordinate_match)
        {
            case 1:if(mode.compare_data_vertical==X_VERTICAl){head_node->x1=node->x1;node->y1=head_node->y1;}else{head_node->y1=node->y1;node->x1=head_node->x1;}break;
            case 2:if(mode.compare_data_vertical==X_VERTICAl){head_node->x1=node->x2;node->y2=head_node->y1;}else{head_node->y1=node->y2;node->x2=head_node->x1;}break;
            case 3:if(mode.compare_data_vertical==X_VERTICAl){head_node->x2=node->x1;node->y1=head_node->y2;}else{head_node->y2=node->y1;node->x1=head_node->x2;}break;
            case 4:if(mode.compare_data_vertical==X_VERTICAl){head_node->x2=node->x2;node->y2=head_node->y2;}else{head_node->y2=node->y2;node->x2=head_node->x2;}break;
            default : break;
        }
        line++;
        
    }
}
Node* Func5(Node*T)
{
    Node* node=T->next;
    int index=1;
    Node* current_node=NULL;
    Node* line_list=InitNode();
    if(T->traversal_flag==1)
    {
        T->traversal_flag=0;
        return NULL;
    }
    else
    {
        while(node->traversal_flag!=1)
        {
            node=node->next;
        }   
    }
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
            node->traversal_flag=0;
            node=node->next;
        }
    }
    Func4(p_line);
    return p_line;
}
void IndependentGraphProcessing(Node** line,Node* T)
{
    Node* node=T->next;
    while(node)
    {
        if(node->x1==node->x2)
        {
            
        }
        else if(node->y1==node->y2)
        {

        }
        node=node->next;
    }
}
void StraightLine(int* array,int len)
{
    MatchMode mode={0};
    int temp_array[8]={0};
    for(int i=0;i<len;i=i+4)
    {
        temp_array[4]=array[i+0];
        temp_array[5]=array[i+1];
        temp_array[6]=array[i+2];
        temp_array[7]=array[i+3];
        Func1(temp_array,&mode);
        if(mode.compare_data_vertical==X_VERTICAl)
        {
            array[i+0]=(array[i+0]+array[i+2])/2;
            array[i+2]=array[i+0];
        }
        else if(mode.compare_data_vertical==Y_VERTICAL)
        {
            array[i+1]=(array[i+1]+array[i+3])/2;
            array[i+3]=array[i+1];
        }
        mode.compare_data_vertical=0;
    }
}
void PrintfLine(Node** line,const char* tip)
{
    printf("--------------------------------------------------------");
    printf("\n%s\n",tip);
    printf("\n");
    while(*line)
    {
        Node* node=(*line)->next;
        printf("\n");
        while(node)
        {
            //printf("\n%d\n%d\n%d\n%d",node->x1,node->y1,node->x2,node->y2);
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

    191, 975, 194, 710,
    193, 713, 343, 708,
    341, 972, 346, 711,
    183, 977, 351, 974,
    198, 973, 202, 916,
    334, 976, 337, 914,
    191, 916, 345, 912
// 592, 972, 595, 914,
// 627, 970, 630, 915,
// 435, 493, 672, 489,
// 1032, 869, 1222, 865,
// 435, 682, 673, 678,
// 477, 988, 517, 969,
// 440, 720, 676, 715,
// 506, 518, 607, 513,
// 1219, 872, 1224, 824,
// 439, 1020, 680, 1016,
// 502, 650, 606, 645,
// 447, 869, 667, 865,
// 507, 623, 606, 619,
// 433, 830, 673, 827,
// 439, 712, 443, 493,
// 439, 1026, 443, 819,
// 504, 545, 604, 539,
// 979, 831, 1242, 826,
// 666, 717, 670, 493,
// 666, 1025, 670, 822,
// 993, 1016, 998, 829,
// 1030, 1019, 1035, 862,
// 993, 1021, 1037, 1015

// 688, 974, 691, 915,
// 334, 976, 337, 914,
// 198, 973, 202, 916,
// 183, 977, 351, 974,
// 341, 972, 346, 711,
// 462, 720, 502, 716,
// 463, 712, 500, 709,
// 495, 916, 697, 913,
// 460, 975, 465, 712,
// 680, 977, 683, 914,
// 191, 975, 194, 710,
// 191, 916, 345, 912,
// 456, 977, 696, 973,
// 498, 917, 503, 707,
// 193, 713, 343, 708

// 614, 575, 840, 571,
// 609, 644, 1017, 639,
// 831, 640, 835, 574,
// 830, 1088, 836, 776,
// 625, 785, 991, 778,
// 622, 1088, 994, 1082,
// 832, 583, 1002, 579,
// 618, 643, 625, 572,
// 997, 643, 1003, 580,
// 996, 1090, 1003, 776,
// 618, 1088, 626, 778



// 507, 887, 511, 749,
// 405, 892, 506, 888,
// 404, 752, 508, 749,
// 405, 877, 509, 873,
// 404, 894, 407, 746,
// 402, 767, 509, 763

// 375, 1033, 382, 968,
// 374, 974, 477, 968,
// 474, 1032, 480, 967,
// 456, 808, 460, 729,
// 404, 970, 410, 805,
// 395, 805, 401, 728,
// 446, 976, 451, 800,
// 395, 733, 458, 727,
// 394, 807, 460, 801,
// 378, 1035, 478, 1028

// 960, 1023, 966, 901,
// 1181, 945, 1207, 940,
// 629, 331, 672, 325,
// 665, 712, 672, 333,
// 1007, 1023, 1013, 901,
// 951, 1023, 1353, 1017,
// 964, 910, 1347, 904,
// 299, 714, 664, 706,
// 1180, 986, 1205, 981,
// 293, 912, 680, 902,
// 625, 661, 632, 330,
// 626, 1022, 633, 901,
// 1342, 1020, 1349, 905,
// 292, 668, 632, 661,
// 297, 1025, 675, 1017,
// 289, 1024, 300, 904,
// 292, 710, 297, 662,
// 665, 1023, 673, 900
};
    int len=(sizeof(array)/sizeof(int));
    StraightLine(array,len);
    
    Node* T=InitNode();
    
    AddNode(T,array,len);

    MyPrintf[0](&T,"init data");

    Node** line=LineHandle(T);


    printf("\n%d\n",cumulative_error);

    MyPrintf[0](line,"3 line data");

    MyPrintf[1](&T,"rest data");

}
// use Two line links; source data cumulative error is 133      func4 error 34
// use Two line links; x/y vertical data cumulative error 108   func4 error 33
// use Two point link  cumulative 104                           func4 cumulative error 31
