int orderG(){
    return 70;
}

int sizeG(){
    return  2275;
}

int are_adjacent(int u, int v){
    if(0<=u && 0<=v && u<orderG() && v<orderG()){
        if (u>v)
            return (u-v)%14;
        else
            return (v-u)%14;
    }
    return 0;
}





