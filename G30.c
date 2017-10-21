int orderG(){
    return 30;
}

int sizeG(){
    return 15*29;
}

int are_adjacent(int u, int v){
    if(0<=u && 0<=v && u<orderG() && v<orderG()){
        if (u>v)
            return (u-v)%30;
        else
            return (v-u)%30;
    }
    return 0;
}





