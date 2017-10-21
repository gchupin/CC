int orderG(){
    return 60;
}

int sizeG(){
    return 58*30;
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





