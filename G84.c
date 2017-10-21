int orderG(){
    return 84;}

    int sizeG(){
        return 840;  
    }

int are_adjacent(int u, int v){
    if(0<= u && 0<=v && u<orderG() && v<orderG()){
        int i,j,k,index=0;
        int T[orderG()];
        for(i=0;i<9;i++)
            for(j=i+1;j<9;j++)
                for(k=j+1;k<9;k++)
                {
                    T[index]= (1 << i) + (1 << j)+(1<<k);
                    index++;
                }
        return (!(T[u] & T[v]));
    }	
    else return 0;
}


