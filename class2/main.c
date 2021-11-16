#define NULL 0
#include <stdlib.h>
#include <stdio.h>

typedef struct bnode
{
    int data;
    struct bnode* lc, * rc;
}bnode;

bnode* creat(bnode* proot)
{
    bnode* p, * q;
    int data;
    int i, n;
    proot = NULL;
    printf("input n:");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        p = (bnode*)malloc(sizeof(bnode));
        p->lc = NULL;
        p->rc = NULL;
        printf("intput data:");
        scanf("%d", &data);
        p->data = data;
        if (proot == NULL)
            proot = p;
        else
        {
            q = proot;
            while (q != NULL)
            {
                if (q->data > data)
                    if (q->lc != NULL)
                        q = q->lc;
                    else
                    {
                        q->lc = p;
                        q = NULL;
                    }
                else if (q->rc != NULL)
                    q = q->rc;
                else
                {
                    q->rc = p;
                    q = NULL;
                }
            }
        }
    }
    return(proot);
}

void preorder(bnode* proot)
{
    if (proot == NULL)
        return;
    else
    {
        printf("%d\t", proot->data);
        if (proot->lc != NULL) preorder(proot->lc);
        if (proot->rc != NULL) preorder(proot->rc);
    }
}

void inorder(bnode* proot)
{
    if (proot == NULL)
        return;
    else
    {
        if (proot->lc != NULL) inorder(proot->lc);
        printf("%d\t", proot->data);
        if (proot->rc != NULL) inorder(proot->rc);
    }
}

void postorder(bnode* proot)
{
    if (proot == NULL)
        return;
    else
    {
        if (proot->lc != NULL) postorder(proot->lc);
        if (proot->rc != NULL) postorder(proot->rc);
        printf("%d\t", proot->data);
    }
}
void insert(bnode* proot, int c)
{
    if (proot == NULL)
    {
        proot = (bnode*)malloc(sizeof(bnode));
        proot->data = c;
        proot->lc = NULL;
        proot->rc = NULL;
    }
    else if (c < proot->data) insert(proot->lc, c);
    else if (c > proot->data) insert(proot->rc, c);
}
int main()
{
    bnode tr1;
    bnode* proot=NULL, twotree;
    int data = 0;
    /*
    proot = creat(&twotree);
    printf("\n preorder are: ");
    preorder(proot);
    printf("\n inorder are:  ");
    inorder(proot);
    printf("\n postorder are:");
    postorder(proot);
    printf("\n");*/

    //proot = NULL;
    printf("intput data:(输入0结束)");
    scanf("%d", &data);
    while (data != 0)
    {
        insert(proot, data);
        printf("intput data:(输入0结束)");
        scanf("%d", &data);
    }
    inorder(proot);
}
