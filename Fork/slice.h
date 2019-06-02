void slice(char vector[], int length, int niveles, int inicio, int nc){
}

int removeSpace(char * vector, int length){

    // To keep track of non-space character count 
    int count = 0; 

    // Traverse the given string. If current character 
    // is not space, then place it at index 'count++' 
    for (int i = 0; vector[i]; i++) 
        if (vector[i] != ' ') 
            vector[count++] = vector[i]; // here count is 
                                   // incremented 
    vector[count] = '\0'; 
    return strlen(vector);
 
}

void children(char vector[], int length, int niveles, int inicio, int op){
    /*Inicio indica donde va a iniciar cada hijo al cifrar el texto*/
    pid_t child_pid[100], wpid;
    int status = 0;
    pid_t grandchild_pid[100];

    int nc = length/niveles;

    printf("A cada proceso hijo le corresponde %d caracteres\n", nc);

    int nietos = nc/niveles;

    printf("A cada proceso nieto le corresponde %d caracteres\n",  nietos);

    int inicionietos = inicio;

    length = removeSpace(vector,length);
     printf("Text after removing blanks\n%s\n", vector);
     printf("New length %d\n", length);

//Father code (before child processes start)
    for (int progress=0; progress<niveles; progress++) {
    for (int id=0; id<1; id++) {
        if ((child_pid[progress] = fork()) == 0) {
            inicionietos = inicio;
            sleep(progress);
            printf("Hijo %d empieza en %d\n", progress, inicio);
            printf("child pid %d   parent pid %d\n",getpid(),getppid());fflush(stdout);
            
                if (progress+1 == niveles){
                    /*Añadir lo que sobra en caso de division inexacta, si no sobra nada añade 0*/
                    nc = (length/niveles) + (length%niveles);
                    printf("Division inexacta, a este ultimo hijo le corresponde %d caracteres\n\n\n\n", nc);
                }

                if (op == 1){
                    /*Encriptar en murcielago*/
                    //encriptar(vector,inicio,nc);
                }

                else if (op == 2){
                    /*Desencriptar*/
                }
            slice(vector,length,niveles,inicio,nc);
            for (int g = 0; g<niveles; g++){
                //Nietos
                if ((grandchild_pid[g] = fork()) == 0) {

                printf("Nieto %d empieza en %d\n", g,inicionietos);
                printf("child pid %d   parent pid %d\n",getpid(),getppid());fflush(stdout);

                    if (g+1 == niveles){
                    /*Añadir lo que sobra en caso de division inexacta, si no sobra nada añade 0*/
                    nietos = (nc/niveles) + (nc%niveles);
                    printf("Division inexacta, a este ultimo nieto le corresponde %d caracteres\n", nietos);
                    }

                if (op == 1){
                    /*Desencriptar en cesar*/
                }

                else if (op == 2){
                    /*Encriptar en cesar*/
                    encriptar(vector,inicionietos,nietos);
                    grandChildFIle(vector,getpid(),inicionietos,nietos);
                }

                //Each grandchildren takes a different beginning, so first has to wait before creating a second one and so on
                waitpid(grandchild_pid[g],&status,WNOHANG);
                exit(0);
                }
                inicionietos =  inicionietos + nietos;
            }
            while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes 
            //All grandchildren pids, saves the one I created last in first position
            for (int q=niveles-1; q>=0; q--) {
                printf("Saved grandchild %d pid %d\n", q,grandchild_pid[q]);
                concatGrandChildren(vector, getpid(), grandchild_pid, niveles);
            } 
            exit(0);
        }
    }
    inicio = inicio + nc;
    }

while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes 
//Father code (After all child processes end)    

/*All children pids, saves and exes in order
    for (int p=0; p<niveles; p++) {
        printf("Saved child %d pid %d\n", p,child_pid[p]);
    } */


}