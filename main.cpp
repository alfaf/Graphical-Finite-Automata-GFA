#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<math.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
const float DEG2RAD = 3.14159/180;
int r=15,noOfstates;
int dfa=FALSE,nfa=FALSE;

struct
{
    int label;
    int link[100];
    int noOfLinks;
    int final_s;
    char input[100];
    int cx;
    int cy;
}states[100];
void createStates(int noOfStates){
    int i;

    for(i=0;i<noOfStates;i++){
        states[i].label=i;
    }

    printf("\n%d States created successfully!\n\n",noOfStates);
    for(i=0;i<noOfStates;i++){
        printf("q%d\t",states[i].label);
    }

    printf("\n\nInitial state is : q0");
}

void getFinalStates(int noOfStates){
    int i, j, total;

    l1:
    printf("\nEnter the total number of Final States: ");
    scanf("%d",&total);
    if(total<1)
    {
        printf("There must be atleast one final state.");
        goto l1;
    }
    for(i=1;i<=total;i++){
        printf("Final State %d:  q",i);
        scanf("%d",&j);
        states[j].final_s=TRUE;
    }
}

void init()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300,300,-300,300);
    glMatrixMode(GL_MODELVIEW);
    glFlush();
}

void drawCircle(float radius)
{
   int i;
   glColor3f(1.0,0.0,0.0);
   glLineWidth(2);
   glBegin(GL_LINE_LOOP);
   for (i=0; i<360; i++)
   {
      float degInRad = i*DEG2RAD;
      glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
   }
   glEnd();
}
void drawState(int i,int x,int y,char st)
{
    glColor3f(1.0,0.0,0.0);
    glRasterPos2f(x-155,147-y);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'q');
    glFlush();
    glRasterPos2f(x-151,147-y);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,st);
    glFlush();
    glPushMatrix();
    glTranslatef(x-150,150-y,0);
    states[i].cx=x-150;
    states[i].cy=150-y;
    if(states[i].final_s==TRUE)
    {
        drawCircle(r-3);
    }
    drawCircle(r);
    glFlush();
    glPopMatrix();
}
void drawTransition(int x,int y,int xx,int yy,char ip)
{
    int rx,ry;
    char c=ip;
  /* glLineWidth(2);
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_STRIP);
    glVertex2i(x-30,y);
    glVertex2i(x-15,y);
    glVertex2i(x-20,y+5);
    glVertex2i(x-20,y-5);
    glVertex2i(x-15,y);
    glEnd();
    glFlush();*/
    glLineWidth(1);
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_STRIP);
    if(x==xx&&y==yy)
    {
        glVertex2i(x-5,y+15);
        glVertex2i(x-5,y+30);
        glVertex2i(x-25,y+30);
        glVertex2i(x-25,y+5);
        glVertex2i(x-15,y+5);
        glVertex2i(x-20,y+10);
        glVertex2i(x-20,y);
        glVertex2i(x-15,y+5);
        glEnd();
        glFlush();
        rx=x-15;
        ry=y+20;
    }
    else
    if(x<xx&&y==yy)
    {
        if(xx-x<=100)
        {
            glVertex2i(x+15,y);
            glVertex2i(xx-15,yy);
            glVertex2i(xx-20,yy+5);
            glVertex2i(xx-20,yy-5);
            glVertex2i(xx-15,yy);
            glColor3f(0.0,0.0,1.0);
            glEnd();
            glFlush();
            rx=(x+xx)/2;
            ry=(y+yy)/2;
        }
        else
        {
            glVertex2i(x,y+15);
            glVertex2i((x+xx)/2,yy+20);
            glVertex2i(xx,yy+15);
            glVertex2i(xx-5,yy+10);
            glVertex2i(xx,yy+20);
            glVertex2i(xx,yy+15);
            glEnd();
            glFlush();
            rx=(x+xx)/2;
            ry=yy+20;
        }
    }
    else
        if(x>xx&&y==yy)
        {
            if(x-xx<=100)
            {
                glVertex2i(x-10,y-10);
                glVertex2i((x+xx)/2,yy-18);
                glVertex2i(xx+10,yy-10);
                glVertex2i(xx+10,yy-5);
                glVertex2i(xx+15,yy-10);
                glVertex2i(xx+10,yy-10);
                glEnd();
                glFlush();
                rx=(x+xx)/2;
                ry=yy-18;
            }
            else
            {
                glVertex2i(x,y-15);
                glVertex2i((x+xx)/2,yy-25);
                glVertex2i(xx,yy-15);
                glVertex2i(xx+5,yy-10);
                glVertex2i(xx,yy-20);
                glVertex2i(xx,yy-15);
                glEnd();
                glFlush();
                rx=(x+xx)/2;
                ry=yy-25;
            }
        }
    else
        if(x<xx&&y>yy)
        {
            if(y-yy>100||xx-x>100)
            {
                glVertex2i(x+10,y-10);
                glVertex2i((x+xx)/2+15,(y+yy)/2+17);
                glVertex2i(xx-10,yy+10);
                glVertex2i(xx-10,yy+15);
                glVertex2i(xx-15,yy+10);
                glVertex2i(xx-10,yy+10);
                glEnd();
                glFlush();
                rx=(x+xx)/2+15;
                ry=(y+yy)/2+17;
            }
            else
            {
                glVertex2i(x+10,y-10);
                glVertex2i(xx-10,yy+10);
                glVertex2i(xx-10,yy+15);
                glVertex2i(xx-15,yy+10);
                glVertex2i(xx-10,yy+10);
                glEnd();
                glFlush();
                rx=(x+xx)/2;
                ry=(y+yy)/2;
            }
        }
    else
        if(x>xx&&y<yy)
        {
            if(yy-y>100||x-xx>100)
            {
                glVertex2i(x-13,y+6);
                glVertex2i((x+xx)/2-15,(y+yy)/2-17);
                glVertex2i(xx+12,yy-12);
                glVertex2i(xx+12,yy-20);
                glVertex2i(xx+20,yy-13);
                glVertex2i(xx+12,yy-12);
                glEnd();
                glFlush();
                rx=(x+xx)/2-15;
                ry=(y+yy)/2-17;
            }
            else
            {
                glVertex2i(x-13,y+6);
                glVertex2i((x+xx)/2-15,(y+yy)/2-17);
                glVertex2i(xx+6,yy-16);
                glVertex2i(xx+11,yy-15);
                glVertex2i(xx+7,yy-20);
                glVertex2i(xx+6,yy-16);
                glEnd();
                glFlush();
                rx=(x+xx)/2-15;
                ry=(y+yy)/2-17;
            }
        }

    else
        if(x<xx&&y<yy)
        {
            if(yy-y>100||xx-x>100)
            {
                glVertex2i(x+10,y+10);
                glVertex2i((x+xx)/2-15,(y+yy)/2+17);
                glVertex2i(xx-10,yy-10);
                glVertex2i(xx-16,yy-9);
                glVertex2i(xx-12,yy-16);
                glVertex2i(xx-10,yy-10);
                glEnd();
                glFlush();
                rx=(x+xx)/2-15;
                ry=(y+yy)/2+17;
            }
            else
            {
                glVertex2i(x+10,y+10);
                glVertex2i(xx-10,yy-10);
                glVertex2i(xx-15,yy-10);
                glVertex2i(xx-10,yy-15);
                glVertex2i(xx-10,yy-10);
                glEnd();
                glFlush();
                rx=(x+xx)/2;
                ry=(y+yy)/2;
            }
        }
    else
        if(x>xx&&y>yy)
        {
             if(y-yy>100||x-xx>100)
            {
                glVertex2i(x-10,y-10);
                glVertex2i((x+xx)/2+20,(y+yy)/2-17);
                glVertex2i(xx+13,yy+7);
                glVertex2i(xx+16,yy+11);
                glVertex2i(xx+18,yy+3);
                glVertex2i(xx+13,yy+7);
                glEnd();
                glFlush();
                rx=(x+xx)/2+20;
                ry=(y+yy)/2-17;
            }
            else
            {
                glVertex2i(x-10,y-10);
                glVertex2i((x+xx)/2+15,(y+yy)/2-15);
                glVertex2i(xx+10,yy+10);
                glVertex2i(xx+15,yy+10);
                glVertex2i(xx+10,yy+15);
                glVertex2i(xx+10,yy+10);
                glEnd();
                glFlush();
                rx=(x+xx)/2+15;
                ry=(y+yy)/2-15;
            }
        }
    else
        if(x==xx&&y>yy)
        {
            if(y-yy<=100)
            {
                glVertex2i(x-10,y-10);
                glVertex2i((x+xx)/2-15,(y+yy)/2);
                glVertex2i(xx-10,yy+10);
                glVertex2i(xx-15,yy+15);
                glVertex2i(xx-8,yy+18);
                glVertex2i(xx-10,yy+10);
                glEnd();
                glFlush();
                rx=(x+xx)/2-15;
                ry=(y+yy)/2;
            }
            else
            {
                glVertex2i(x-13,y-9);
                glVertex2i((x+xx)/2-20,(y+yy)/2);
                glVertex2i(xx-11,yy+10);
                glVertex2i(xx-16,yy+12);
                glVertex2i(xx-8,yy+17);
                glVertex2i(xx-11,yy+10);
                glEnd();
                glFlush();
                rx=(x+xx)/2-20;
                ry=(y+yy)/2;
            }
        }
    else
        if(x==xx&&y<yy)
        {
            if(yy-y<=100)
            {
                glVertex2i(x,y+15);
                glVertex2i(xx,yy-15);
                glVertex2i(xx-3,yy-23);
                glVertex2i(xx+3,yy-23);
                glVertex2i(xx,yy-15);
                glEnd();
                glFlush();
                rx=(x+xx)/2;
                ry=(y+yy)/2;
            }
            else
            {
                glVertex2i(x+13,y+9);
                glVertex2i((x+xx)/2+15,(y+yy)/2);
                glVertex2i(xx+11,yy-13);
                glVertex2i(xx+15,yy-18);
                glVertex2i(xx+5,yy-19);
                glVertex2i(xx+11,yy-13);
                glEnd();
                glFlush();
                rx=(x+xx)/2+15;
                ry=(y+yy)/2;
            }
        }
        glColor3f(0.0,1.0,0.0);
        glRasterPos2f(rx,ry);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,c);
        glFlush();
}
void getLinks(int noOfStates){
    int i, j, k, l, total, temp;
    char ip;
    for(i=0;i<noOfStates;i++)
    {
        k=0;
        printf("Enter the number of inputs for q%d: ",i);
        scanf("%d",&total);
        states[i].noOfLinks=total;
        if(total>0)
        {
            printf("Enter the inputs : ");
            scanf("%s",&states[i].input[0]);
            for(j=1;j<total;j++)
            {
                if(dfa==TRUE)
                {
                    l2:
                    scanf("%s",&ip);
                    for(l=0;l<j;l++)
                    {
                        if(ip==states[i].input[l])
                        {
                            printf("\nYou cannot enter the same inputs for DFA.\nEnter different inputs : ");
                            goto l2;
                        }
                        else
                            states[i].input[j]=ip;
                    }
                }
                else
                    scanf("%s",&states[i].input[j]);
            }
            for(j=0;j<total;j++)
            {
                printf("(q%d, %c) = q",i,states[i].input[j]);
                scanf("%d",&temp);
                states[i].link[k] = temp;
                k++;
            }
        }
    }
}

void display()
{
    int sz;
    char head1[]="Deterministic Finite Automata";
    char head2[]="Non-deterministic Finite Automata";
    glColor3f(0.0,1.0,0.0);
    glRasterPos2f(-175,225);
    if(dfa==TRUE)
    {
        for(sz=0;sz<strlen(head1);sz++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,head1[sz]);
        }
    }
    else
    {
       for(sz=0;sz<strlen(head2);sz++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,head2[sz]);
        }
    }

    int i,j,k;
    int x=0,y=0;
    char c;


    glColor3f(0.0,0.0,0.0);
    for(i=0;i<noOfstates;i++)
    {
        c='0'+i;
        drawState(i,x,y,c);
        x=x+100;
        if(noOfstates>4)
        {
            if((i+1)%3==0)
            {
                x=0;
                y=y+100;
            }
        }
        else
        {
            if((i+1)%2==0)
            {
                x=0;
                y=y+100;
            }
        }
    }

    for(i=0;i<noOfstates;i++)
    {
        for(j=0;j<states[i].noOfLinks;j++)
        {
            for(k=0;k<noOfstates;k++)
            {
                if(states[i].link[j]==states[k].label)
                {
                    drawTransition(states[i].cx,states[i].cy,states[k].cx,states[k].cy,states[i].input[j]);
                }
            }
        }
    }
}

int main(int argc,char *argv[])
{
    int ch;
    printf("--------------------------------------------------------------------------------\n");
    printf("\t\t\tWELCOME TO GRAPHICAL FINITE AUTOMATA\n");
    printf("\n--------------------------------------------------------------------------------\n");


    printf("Select the type of automata :\n1.DFA\n2.NFA\nEnter your choice : ");
    l:
    scanf("%d",&ch);

    if(ch==1)
        dfa=TRUE;
    else
        if(ch==2)
            nfa=TRUE;
        else
        {
            printf("Please select proper option : ");
            goto l;
        }

    printf("Enter the number of states (1 to 9): ");
    scanf("%d",&noOfstates);
    while(noOfstates>9||noOfstates<1)
    {
        printf("Please enter the number of states between 1 to 9 : ");
        scanf("%d",&noOfstates);
    }

    createStates(noOfstates);
    getFinalStates(noOfstates);
    getLinks(noOfstates);
    glutInit(&argc,argv);
    glutInitWindowSize(1366,768);
    glutInitWindowPosition(0,0);
    glutCreateWindow("GFA");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
