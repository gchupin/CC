int orderG(){
    return 33;}

    int sizeG(){
        return 20*9+11*3;
    }

int adj(int u, int v, int n){
    int x,y;
    if (u==v)
        return 0;
    if(u>v){
        x=u;
        y=v;
    }
    else{
        x=v;
        y=u;
    }
    if(x==1)
        return 1;
    if(x<n/2)
        return adj(x,y,n/2);
    if(x==n-1){
        if(y>=n/2) 
            return 1;
        else return 0;
    }
    if(x>=n/2 && y<n/2)
        return adj(x-n/2,y,n/2);
    return 0;
}


int are_adjacent(int u, int v){
    if(0<=u && 0<=v && u<orderG() && v<orderG()){
        if (u/3!=v/3){
            return adj(u/3,v/3,11);
        }
        else return 1;
    }

    return 0;
}





