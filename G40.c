int orderG(){
    return 40;
}

int sizeG(){
    return  700;
}

int are_adjacent(int u, int v){
    if(0<=u && 0<=v && u<orderG() && v<orderG()){
        if (u>v)
            return (u-v)%8;
        else
            return (v-u)%8;
    }
    return 0;
}





