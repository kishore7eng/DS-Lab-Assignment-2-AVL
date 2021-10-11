#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class AVL_Node
{
public:
   int key;
   int bf; // balance factor bf = height(LChild subtree) – height(RChild subtree)
   AVL_Node *LChild, *RChild;
   AVL_Node(){
    LChild=NULL;
    RChild=NULL;}
   AVL_Node(int k)
   {
       LChild=NULL;
       key=k;
       RChild=NULL;
       bf=0;
   }
   ~AVL_Node()
   {
       delete LChild;
       delete RChild;
   }
   friend class AVL_Tree;
};
class AVL_Tree
{
private:
    AVL_Node *h;
public:
    AVL_Tree()
    {
        h=new AVL_Node(0);
    }
    AVL_Tree(const AVL_Tree &T);
    AVL_Tree &operator=(const AVL_Tree &T);
    void AVL_Insert(int k);
    void AVL_Delete(int k);
    bool AVL_Search(int k);
    void AVL_Print(ofstream &fp);
    void AVL_Printtree(AVL_Node *,ofstream&);
    void write_to_file(AVL_Node *root, ofstream &myfile);
    ~AVL_Tree(){delete h;}
};
void AVL_Tree::AVL_Insert(int k)
{
    AVL_Node *pbp=h;
    //pbp refers to parent of balance pointer that might affect the balancing property
    AVL_Node *iterate;
    iterate=h->RChild;
    //bp refers to balance pointer
    AVL_Node *bp=h->RChild;
    //pos refers to positon of the node to be inserted
    AVL_Node *pos,*random;
    AVL_Node *node= new AVL_Node(k);
    //if the node to be inserted is the first node
    if(iterate==NULL)
    {
        h->RChild=node;
        return;
    }
    //iterate to find the position of new node and position of node where imbalance could occur
    while(1)
    {
        if(k<iterate->key)
        {

            pos=iterate->LChild;
            if(pos==NULL)
            {
                iterate->LChild=node;
                pos=iterate->LChild;

                break;
            }
            else
            {
                if(pos->bf!=0)
                {
                    pbp=iterate;
                    bp=pos;
                }
                iterate=pos;
            }
        }
        else if(k>iterate->key)
        {
            pos=iterate->RChild;
            if(pos==NULL)
            {
                iterate->RChild=node;
                pos=iterate->RChild;
                break;
            }
            else
            {
                if(pos->bf!=0)
                {
                    pbp=iterate;
                    bp=pos;
                }
                iterate=pos;
            }
        }
        else
            throw "ELEMENT EXISTS";

    }

    int b;
    //finding whether new node is inserted to left or right of parent
    if (k<bp->key)
        b=1;
    else
        b=-1;

    if(b==1)
    {
        random=bp->LChild;
        iterate=bp->LChild;
    }
    else
    {
        random=bp->RChild;
        iterate=bp->RChild;
    }

    while(iterate!=pos)
    {
        if(k<iterate->key)
        {
            iterate->bf=1;
            iterate=iterate->LChild;
        }
        else
        {
            iterate->bf=-1;
            iterate=iterate->RChild;
        }
    }

    if(bp->bf==0)
    {
        bp->bf=b;
        return;
    }
    else if(bp->bf==-1*b)
    {
        bp->bf=0;
        return;
    }
    else
    {//Single rotation
       if(random->bf==1 && b==1)
       {
           iterate=random;
           bp->LChild=iterate->RChild;
           iterate->RChild=bp;
           iterate->bf=0;
           bp->bf=0;
       }
       else if(random->bf==-1 and b==-1)
       {
           iterate=random;
           bp->RChild=iterate->LChild;
           iterate->LChild=bp;
           iterate->bf=0;
           bp->bf=0;
       }
       //Double rotation
       else if(random->bf==-1*b && b==1)
       {
           iterate=random->RChild;
           random->RChild=iterate->LChild;
           iterate->LChild=random;
           bp->LChild=iterate->RChild;
           iterate->RChild=bp;
           if(iterate->bf==0)
           {
               bp->bf=0;
               random->bf=0;
           }
           else if(iterate->bf==1)
           {
               bp->bf=-1;
               random->bf=0;
           }
           else
           {
               bp->bf=0;
               random->bf=1;
           }
           iterate->bf=0;
       }
       else if(random->bf==-1*b && b==-1)
       {
           iterate=random->RChild;
           random->LChild=iterate->RChild;
           iterate->LChild=bp;
           bp->RChild=iterate->LChild;
           iterate->RChild=random;
           if(iterate->bf==0)
           {
               bp->bf=0;
               random->bf=0;
           }
           else if(iterate->bf==1)
           {
               bp->bf=0;
               random->bf=-1;
           }
           else
           {
               bp->bf=1;
               random->bf=0;
           }
           iterate->bf=0;
       }

    }

    if(bp==pbp->LChild)
        pbp->LChild=iterate;
    else
        pbp->RChild=iterate;

}
bool AVL_Tree::AVL_Search(int k)
{
    AVL_Node *temp=h->RChild;
    if(temp==NULL)
        throw "Tree is empty";

    while(temp!=NULL)
    {
        if(temp->key==k)
            return true;
        else if(temp->key>k)
            temp=temp->LChild;
        else
            temp=temp->RChild;

    }

    return false;
}
void AVL_Tree::AVL_Delete(int k)
{
    AVL_Node *iterate=h->RChild;
    int b;
    stack<AVL_Node*> s;
    s.push(h);
    AVL_Node *bp,*pbp,*random;
    bool flag=AVL_Search(k);

    if(flag==false)
        throw "Element does not exists";
    //finding the path and pushing it to the stack
    while(iterate!=NULL)
    {
        if(iterate->key<k)
        {
            s.push(iterate);
            iterate=iterate->RChild;
        }
        else if(iterate->key>k)
        {
            s.push(iterate);
            iterate=iterate->LChild;
        }
        else
            break;
    }
    //node to be deleted is leaf node
    if(iterate->LChild==NULL && iterate->RChild==NULL)
    {
        AVL_Node *temp,*parent;
        temp=iterate;
        parent=s.top();
        if(iterate==parent->LChild)
            parent->LChild=NULL;
        else
            parent->RChild=NULL;
        iterate=NULL;
        delete temp;

    }
    // node ro be deleted has one child
    else if(iterate->LChild==NULL || iterate->RChild==NULL)
    {
        AVL_Node *temp;
        if(iterate->LChild)
            temp=iterate->LChild;
        else
            temp=iterate->RChild;
        iterate->key=temp->key;
        iterate->LChild=temp->LChild;
        iterate->RChild=temp->RChild;
        iterate->bf=temp->bf;

        delete temp;
    }
    //node to be deleted has two children
    else
    {
         s.push(iterate);
         AVL_Node *InSuccessor=iterate->RChild;
         //finding inorder successor
         while(InSuccessor->LChild!=NULL)
         {
             s.push(InSuccessor);
             InSuccessor=InSuccessor->LChild;
         }
         iterate->key=InSuccessor->key;
         AVL_Node *temp;
         if(InSuccessor->LChild)
            temp=InSuccessor->LChild;
         else
            temp=InSuccessor->RChild;
         AVL_Node*parent=s.top();
         k=InSuccessor->key;

         if(temp==NULL)
         {
             temp=InSuccessor;
             if(InSuccessor==parent->LChild)
                  parent->LChild=NULL;
             else
                parent->RChild=NULL;

         }
         else
         {
             InSuccessor->key=temp->key;
             InSuccessor->LChild=temp->LChild;
             InSuccessor->RChild=temp->RChild;
             InSuccessor->bf=temp->bf;
         }
         delete temp;
    }
    //updation of balance fsctor

    while(s.top()!=h)//looking at path of node deleted
    {
        bp=s.top();
        if(k<bp->key)
            b=1;
        else
            b=-1;
        s.pop();
        pbp=s.top();

        if(bp->bf==b)
        {
            bp->bf=0;
            continue;
        }
        else if(bp->bf==0)
        {
            if( b==1)
              bp->bf=-1;
            else
                bp->bf=1;
            return;
        }
        else
        {
            if(bp->bf==1)
                random=bp->LChild;
            else
                random=bp->RChild;
                //single rotation
            if(random->bf==(-1*b) && b==-1)
            {
                iterate=random;
                bp->LChild=random->RChild;
                random->RChild=bp;
                bp->bf=0;
                random->bf=0;
            }
            else if(random->bf==-1*b && b==1)
            {
                iterate=random;
                bp->RChild=random->LChild;
                random->LChild=bp;
                random->bf=0;
                bp->bf=0;
            }
            else if(random->bf==0 && b==-1)
            {
                iterate=random;
                bp->LChild=random->RChild;
                random->RChild=bp;
                random->bf=b;
            }
            else if(random->bf==0 && b==1)
            {
                iterate=random;
                bp->RChild=random->LChild;
                random->LChild=bp;
                random->bf=b;
            }
            //double rotation
            else if(random->bf==b && b==-1)
            {
                iterate=random->RChild;
                random->RChild=iterate->LChild;
                iterate->LChild=random;
                bp->LChild=iterate->RChild;
                iterate->RChild=bp;
                if(iterate->bf==0)
                {
                    bp->bf=0;
                    random->bf=0;
                }
                else if(iterate->bf==1)
                {
                   bp->bf=-1;
                   random->bf=0;
                }
                else
                {
                  bp->bf=0;
                  random->bf=1;
                }
                iterate->bf=0;
           }
           else if(random->bf==b && b==1)
           {
               iterate=random->RChild;
               random->LChild=iterate->RChild;
               iterate->LChild=bp;
               bp->RChild=iterate->LChild;
               iterate->RChild=random;
               if(iterate->bf==0)
               {
                    bp->bf=0;
                    random->bf=0;
               }
               else if(iterate->bf==1)
               {
                   bp->bf=0;
                   random->bf=-1;
               }
               else
              {
                  bp->bf=1;
                  random->bf=0;
              }
              iterate->bf=0;
           }
        }
        if(bp==pbp->LChild)
            pbp->LChild=iterate;
        else
            pbp->RChild=iterate;
    }
}
/*void AVL_Tree:: AVL_Printtree(AVL_Node*ptr,ofstream &fp)
{
        if(ptr==NULL)
            return;

        if(ptr->LChild!=NULL){
                fp<<ptr->key<<"[xlabel="<<ptr->bf<<", color=\"#ff0000\"]"<<"\n"<<ptr->key<<"->"<<ptr->LChild->key<<"\n";
            AVL_Printtree(ptr->LChild,fp);

            }

        if(ptr->RChild!=NULL)
        {

            fp<<ptr->key<<"[xlabel="<<ptr->bf<<", color=\"#ff0000\"]"<<"\n"<<ptr->key<<"->"<<ptr->RChild->key<<"\n";
            AVL_Printtree(ptr->RChild,fp);
        }
        if(ptr->LChild==NULL && ptr->RChild==NULL)
        {
            fp<<ptr->key<<"[xlabel="<<ptr->bf<<", color=\"#ff0000\"]\n";
        }


        return;
}

void AVL_Tree::AVL_Print(ofstream &fp)
{
    AVL_Node*ptr = h->RChild;
    AVL_Printtree(ptr, fp);
}
*/
int main()
{
    cout<<"a";
    AVL_Tree *obj=new AVL_Tree();
    int a,x,flag;
    while(1){
    cout<<" 1-Insert\n 2-Search\n 3-Delete\n 4-Print Tree\n 5-Exit\n";
    cout<<"Enter Choice:";
    cin>>a;
    switch(a)
    {
        case 1:cout<<"Enter the element to be inserted: ";
               cin>>x;
               try{obj->AVL_Insert(x);}
               catch(const char *e)
               {
                   cout<<e<<endl;
               }
               break;
        case 2:cout<<"Enter the element to be searched: ";
               cin>>x;
               flag=obj->AVL_Search(x);
               if(flag==1)
               cout<<"Element found "<<endl;
               else
               {
                   cout<<"Element not found"<<endl;
               }
               break;
        case 3:cout<<"Enter the element to be deleted: ";
               cin>>x;
               try{obj->AVL_Delete(x);}
               catch(const char *e)
               {
                   cout<<e<<endl;
               }
               break;
        case 4:{cout<<"Printing tree using graphviz\n";
               ofstream fp;
               fp.open("C:\\Users\\rmkis\\avl.graphviz.txt");
               fp<<"digraph AVL {\n";
               obj->AVL_Print(fp);
               fp << "}\n";
               fp.close();
               break;}

        case 5:exit(0);
    }
}
    return 0;
}
